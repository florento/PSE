#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan 27 16:50:52 2023

@author: jacquema
"""

#import sys
#import logging
import os
from pathlib import Path, PosixPath
from datetime import datetime
import re
from operator import itemgetter, attrgetter

import pandas as pd
import music21 as m21
import PSeval as ps


########################
##                    ##
##  global variables  ##
##                    ##
########################

# path to ASAP dataset
_dataset_root = '../../../Datasets/ASAP'

 # default score file name
_generic_score = 'xml_score.musicxml'

# root of evaluation dir
_eval_root = '../../PSeval'

# name of dir for evaluation output
_output_dir = 'augASAP'

timestamp = str(datetime.today().strftime('%Y%m%d-%H%M'))

# or '..' if we assume that we are in dir script/ of the evaluation dir

#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

def get_score(score_path):
    global _dataset_root
    global _generic_score
    complete_score_path = Path(_dataset_root) / score_path / _generic_score
    return m21.converter.parse(str(complete_score_path))

def get_parts(score):
    return score.getElementsByClass(m21.stream.Part)

class Opus:
    def __init__(self, nb, mvt, file):
        self.nb = nb
        self.mvt = mvt
        self.file = file
    def __repr__(self):
        return repr((self.nb, self.mvt, self.file))

def multisort(xs, specs):
    for key, reverse in reversed(specs):
        sorted(xs, key=attrgetter(key), reverse=reverse)
    return xs


###########################################
##                                       ##
## automatic evaluation of ASAP dataset  ##
##                                       ##
###########################################
#
# the evaluation files are organized as follows
# _eval_root/_output_dir/composer

def output_dir(composer='Unknown'):
    global timestamp
    global _eval_root
    global _output_dir
    assert(len(_output_dir) > 0)
    outpath = Path(_eval_root)/_output_dir/composer	/timestamp
    if not os.path.isdir(outpath):
        os.mkdir(outpath)
    return outpath;

def output_xml(title='title', composer='Unknown'):
    return output_dir(composer)/(title+'.musicxml')

def write_score(score, title='title', composer='Unknown'):
    score.write('musicxml', fp=output_dir(composer)/(title+'.musicxml'))
    # pdffile = dirname+'/'+outname+'.pdf'
    # os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)

def eval_asapscore(sid, file, stat, title='title', composer='composer',
                   psalgo=ps.pse.Algo_PSE, tons=26, kpre=33, kpost=23,
                   dflag=False, mflag=False):
    s = m21.converter.parse(file)
    (ls, lld) = ps.eval_score(score=s, stat=stat,
                              sid=sid, title=title, composer=composer,
                              algo=psalgo,
                              nbtons=tons,              # for PSE
                              kpre=kpre, kpost=kpost, # for PS13
                              debug=dflag, mark=mflag)
    if mflag: # and not ps.empty_difflist(lld):
        write_score(s, title, composer)

def str_hands(i):
    if i == 0:
        return 'rh'
    elif i == 1:
        return 'lh'
    else:
        return 'ERR'

def revise_table_asap(df):
    # replace part number (0 and 1) by 'rh' 'lh'
    df['part'].update(df['part'].apply(str_hands))
    #df.insert(4, 'hand', df['part'].apply(str_hands))
    #df.pop('part')

def get_table(file, composer='Unknown'):
    fp = output_dir(composer)/file
    assert(os.path.isfile(fp))
    df = pd.read_csv(fp)
    return df

def eval_Brahms(stat=ps.Stats(), algo=ps.pse.Algo_PSE,
                   nbtons=30,          # for PSE
                   kpre=33, kpost=23, # for PS13
                   dflag=True, mflag=True):
    global _dataset_root
    global _generic_score
    if stat == None:
        stat = ps.Stats()
    filep = Path(_dataset_root)/'Brahms'/'Six_Pieces_op_118'/'2'/_generic_score
    eval_asapscore(sid=2, file=filep, stat=stat,
                   title='Six_Pieces_op_118: 2nd one', composer='Brahms',
                   psalgo=algo, tons=nbtons, kpre=kpre, kpost=kpost,
                   dflag=dflag, mflag=mflag)


#eval_Brahms()

def eval_Chopin(no=1,stat=ps.Stats(), algo=ps.pse.Algo_PSE,
                   nbtons=30,          # for PSE
                   kpre=33, kpost=23, # for PS13
                   dflag=True, mflag=True):
    global _dataset_root
    global _generic_score
    if stat == None:
        stat = ps.Stats()
    filep = Path(_dataset_root)/'Chopin'/'Etudes_op_25'/str(no)/_generic_score
    eval_asapscore(sid=no, file=filep, stat=stat,
                   title='', composer='Chopin',
                   psalgo=algo, tons=nbtons, kpre=kpre, kpost=kpost,
                   dflag=dflag, mflag=mflag)

#no=int(input("numéro d'étude? "))
#eval_Chopin(no=no) #algo=ps.pse.Algo_PS14)

#########################
##                     ##
##  Beethoven Sonatas  ##
##                     ##
#########################

def Beethoven_list():
    """list of Opus (sonata nb, mvt, file) of Beethoven Sonatas in ASAP"""
    global _dataset_root
    global _generic_score
    dir_re = re.compile("(\d+)-(\d)_?\d?$")
    bl = []
    p = Path(_dataset_root)/'Beethoven'/'Piano_Sonatas'
    assert(os.path.isdir(p))
    for d in os.listdir(p):
        dm = dir_re.match(d)
        if dm == None:
            continue
        po = p / d / _generic_score
        assert(os.path.isfile(po))
        bl.append(Opus(int(dm.group(1)), int(dm.group(2)), str(po)))
    bl = sorted(bl, key=attrgetter('nb', 'mvt'))
    return bl

Beethoven_skip = []

def eval_Beethoven(stat=ps.Stats(), algo=ps.pse.Algo_PSE,
                   nbtons=30,          # for PSE
                   kpre=33, kpost=23, # for PS13
                   dflag=True, mflag=True):
    """"evaluation of Beethoven Sonatas in ASAP"""
    print('eval ASAP/Beethoven with algo', algo)
    ludwig = 'Beethoven'
    if stat == None:
        stat = ps.Stats()
    for o in Beethoven_list():
        if (o.nb, o.mvt) in Beethoven_skip:
            print(o.nb, o.mvt, ': SKIP')
            continue
        else:
            print(o.nb, o.mvt, ':', o.file)
            eval_asapscore(sid=o.nb*10+o.mvt, file=o.file, stat=stat,
                           title='Sonata '+str(o.nb)+'_'+str(o.mvt), composer=ludwig,
                           psalgo=algo, tons=nbtons, kpre=kpre, kpost=kpost,
                           dflag=dflag, mflag=mflag)
    # write table and summary to files and display stats
    df = stat.get_dataframe()
    revise_table_asap(df)
    df.to_csv(output_dir(composer=ludwig)/(ludwig+'_sonata.csv'), header=True, index=False)
    stat.write_datasum(output_dir(composer=ludwig)/(ludwig+'_sonata_sum.csv'))
    stat.show()

# Waldstein chords in 2 first bars (for C++ debug)
def Waldstein():
    global _dataset_root
    bl = Beethoven_list()
    sonata = bl[33] # 21.1
    score = m21.converter.parse(sonata.file)
    lp = score.getElementsByClass(m21.stream.Part)
    #k0 = ps.get_key(lp[1])
    #ln0 = ps.extract_part(lp[0]) # right hand part
    ln1 = ps.extract_part(lp[1])  # left hand part
    for (n, b, s) in ln1[600:]:
        a = 'sp.add('
        a += str(n.pitch.midi)
        a += ', '
        a += str(b)
        a += ', '
        a += 'true' if s else 'false'
        a += ');'
        print(a)
    #sp = ps.Speller()
    #sp.debug(True)
    #ps.add_tons(0, sp)
    #sp.add_notes(ln1[:61], sp)
    #sp.spell()


######################################
##                                  ##
## Bach Das Wohltemperierte Klavier ##
##                                  ##
######################################

def DWK_sublist(p, sublist):
    global _generic_score
    dir_re = re.compile("bwv_(\d+)$")
    p = p / sublist
    assert(os.path.isdir(p))
    bl = []
    for d in os.listdir(p):
        dm = dir_re.match(d)
        if dm == None:
            continue
        po = p / d / _generic_score
        assert(os.path.isfile(po))
        bl.append(Opus(int(dm.group(1)), sublist, str(po)))
    return bl

def DWK_list():
    """list Opus (BWV nb, 'prelude' or 'fugue', path) of Bach WK preludes and fugues in ASAP"""
    global _dataset_root
    p = Path(_dataset_root)/'Bach'
    bl = DWK_sublist(p, 'Prelude') + DWK_sublist(p, 'Fugue')
    #bl = multisort(bl, (('mvt', True), ('nb', False)))
    bl = sorted(bl, key=attrgetter('nb'))
    return bl

lBach = [#(846, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_846/xml_score.musicxml'),
         #(846, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_846/xml_score.musicxml'),
         #(848, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_848/xml_score.musicxml'),
         #(848, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_848/xml_score.musicxml'),
         #(854, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_854/xml_score.musicxml'),
         #(854, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_854/xml_score.musicxml'),
         (856, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_856/xml_score.musicxml'),
         #(856, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_856/xml_score.musicxml'),
         #(857, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_857/xml_score.musicxml'),
         (857, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_857/xml_score.musicxml'),
         #(858, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_858/xml_score.musicxml'),
         #(858, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_858/xml_score.musicxml'),
         #(860, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_860/xml_score.musicxml'),
         #(860, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_860/xml_score.musicxml'),
         #(862, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_862/xml_score.musicxml'),
         #(862, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_862/xml_score.musicxml'),
         #(863, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_863/xml_score.musicxml'),
         #(863, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_863/xml_score.musicxml'),
         #(864, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_864/xml_score.musicxml'),
         #(864, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_864/xml_score.musicxml'),
         #(865, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_865/xml_score.musicxml'),
         #(865, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_865/xml_score.musicxml'),
         #(866, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_866/xml_score.musicxml'),
         #(866, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_866/xml_score.musicxml'),
         #(867, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_867/xml_score.musicxml'),
         #(867, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_867/xml_score.musicxml'),
         #(868, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_868/xml_score.musicxml'),
         #(868, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_868/xml_score.musicxml'),
         #(870, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_870/xml_score.musicxml'),
         #(870, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_870/xml_score.musicxml'),
         (873, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_873/xml_score.musicxml'),
         #(873, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_873/xml_score.musicxml'),
         #(874, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_874/xml_score.musicxml'),
         #(874, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_874/xml_score.musicxml'),
         #(875, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_875/xml_score.musicxml'),
         #(875, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_875/xml_score.musicxml'),
         #(876, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_876/xml_score.musicxml'),
         #(876, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_876/xml_score.musicxml'),
         #(880, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_880/xml_score.musicxml'),
         #(880, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_880/xml_score.musicxml'),
         #(883, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_883/xml_score.musicxml'),
         #(883, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_883/xml_score.musicxml'),
         #(884, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_884/xml_score.musicxml'),
         #(884, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_884/xml_score.musicxml'),
         #(885, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_885/xml_score.musicxml'),
         #(885, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_885/xml_score.musicxml'),
         #(887, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_887/xml_score.musicxml'),
         #(887, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_887/xml_score.musicxml'),
         #(888, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_888/xml_score.musicxml'),
         #(888, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_888/xml_score.musicxml'),
         #(889, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_889/xml_score.musicxml'),
         #(889, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_889/xml_score.musicxml'),
         #(891, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_891/xml_score.musicxml'),
         #(891, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_891/xml_score.musicxml'),
         #(892, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_892/xml_score.musicxml'),
         #(892, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_892/xml_score.musicxml'),
         #(893, 'Prelude', '../../../Datasets/ASAP/Bach/Prelude/bwv_893/xml_score.musicxml'),
         #(893, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_893/xml_score.musicxml')
         ]

# the following cause a complexity explosion. TBC.
# (866, 'Prelude') lonnnng
Bach_skip = [(856, 'Prelude'), (873, 'Prelude'), (857, 'Fugue')]

def eval1_Bach(nb, mvt, file, stat,
               algo=ps.pse.Algo_PSE, tons=25, kpre=33, kpost=23,
               dflag=True, mflag=True):
    global _dataset_root
    global _generic_score
    if file == None:
        p = Path(_dataset_root)/'Bach'/mvt
        file = p/('bwv_'+str(nb))/_generic_score
        assert(os.path.isfile(file))
    print('BWV', nb, mvt, ':', file, end='\n', flush=True)
    c = 0 if mvt == 'Prelude' else 1
    eval_asapscore(sid=nb*10+c, file=file, stat=stat,
                   title='BWV'+str(nb)+'_'+str(mvt), composer='Bach',
                   psalgo=algo, tons=tons, kpre=33, kpost=23,
                   dflag=dflag, mflag=mflag)

def eval_Bach(stat=ps.Stats(),
              algo=ps.pse.Algo_PSE, nbtons=30, kpre=33, kpost=23,
              dflag=True, mflag=True):
    print('eval ASAP/Bach with algo', algo)
    global Bach_skip
    for o in DWK_list():
        if (o.nb, o.mvt) in Bach_skip:
            continue
        else:
            print('\n', flush=True)
            eval1_Bach(nb=o.nb, mvt=o.mvt, file=o.file, stat=stat,
                       algo=algo, tons=nbtons, kpre=33, kpost=23,
                       dflag=dflag, mflag=mflag)
    df = stat.get_dataframe()
    revise_table_asap(df)
    # write table and summary to files and display stats
    df.to_csv(output_dir(composer='Bach')/'DWK.csv', header=True, index=False)
    stat.write_datasum(output_dir(composer='Bach')/'DWK_sum.csv')
    stat.show()

#eval_Bach(algo=ps.pse.Algo_PSE)


def DWV_num(bwv, mvt):
    """index in list DWK_list() of the Opus BWV, and mvt Prelude or Fugue"""
    lB=DWK_list()
    i = 0
    for e in lB:
        if e.nb == bwv and e.mvt == mvt:
            return i
        else:
            i = i+1
    return None # not found

def quick_eval_Bach(bwv=854, stat=ps.Stats(),
              algo=ps.pse.Algo_PSE, nbtons=25, kpre=33, kpost=23,
              dflag=True, mflag=True):
    lB=DWK_list()
    lB1=[]
    for e in lB:
        lB1.append(e.nb)
    i=lB1.index(bwv)
    p=lB[i]
    eval1_Bach(nb=p.nb, mvt=p.mvt, file=p.file, stat=stat,algo=algo,
               tons=nbtons, kpre=33, kpost=23, dflag=dflag, mflag=mflag)
    if i<len(lB)-1:
        print()
        f=lB[i+1]
        eval1_Bach(nb=f.nb, mvt=f.mvt, file=f.file, stat=stat,algo=algo,
                   tons=nbtons, kpre=33, kpost=23, dflag=dflag, mflag=mflag)



######################################
##                                  ##
##                                  ##
######################################


def debug(composer="Bach",i=0):

    if composer=="Beethoven":
        dataset=Beethoven_list()

    else:
        dataset = DWK_list()

    file = dataset[i].file
    #print(file)
    score = m21.converter.parse(file)
    lp = score.getElementsByClass(m21.stream.Part)
    ln = ps.extract_part(lp[0]) # first and unique part
    for (n, b, s) in ln:
        a = 'sp.add('
        a += str(n.pitch.midi)
        a += ', '
        a += str(b)
        a += ', '
        a += 'true' if s else 'false'
        a += ');'
        print(a)
##





#########################
##                     ##
##    Chopin Etudes    ##
##                     ##
#########################

# bl = Beethoven_list()
#beethoven1_1 = get_score('Beethoven/Piano_Sonatas/1-1')
#b = m21.converter.parse('xml_score.musicxml')
#lp = get_parts(b)
#lm0 = lp[0].getElementsByClass(m21.stream.Measure)
#lm1 = lp[1].getElementsByClass(m21.stream.Measure)

#import pse


#stat.eval_score(b, 0)  # 0 : standard list of 26 tons
#stat.show()


#file = dataset[470]
#print(file)
#score = m21.converter.parse(file.as_posix())
#lp = score.getElementsByClass(m21.stream.Part)
#part = lp[0]
#ln = ps.extract_notes(part)
#sp = ps.Speller()
#ps.add_notes(ln, sp)
#sp.spell()
#
# for p in ln:
#    print('sp.add(', p[0].pitch.midi, ',', p[1],')')
