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
import re
from operator import itemgetter, attrgetter

import pandas as pd
import music21 as m21

import PSeval as ps

#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

# global variables
_dataset_root = '../../../Datasets/ASAP/'  # path to ASAP
_generic_score = 'xml_score.musicxml'      # default score file name

def get_score(score_path):
    global _dataset_root
    global _generic_score
    complete_score_path = Path(_dataset_root) / score_path / _generic_score
    return m21.converter.parse(str(complete_score_path))

def get_parts(score):
    return score.getElementsByClass(m21.stream.Part)


###########################################
##                                       ##
## automatic evaluation of ASAP dataset  ##
##                                       ##
###########################################

# root of evaluation dir
_eval_root = '../../PSeval'
# or '..' if we assume that we are in dir script/ of the evaluation dir

# name of dir for evaluation output
_output_dir = 'ASAP'   

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

def write_score(score, title='title', composer='Unknown'):
    global _eval_root
    global _output_dir
    assert(len(_output_dir) > 0)
    dirname = _eval_root+'/'+_output_dir+'/'+composer
    if not os.path.isdir(dirname):
        os.mkdir(dirname)
    xmlfile = dirname+'/'+title+'.musicxml'
    score.write('musicxml', fp=xmlfile)
    # pdffile = dirname+'/'+outname+'.pdf'
    # os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)
    
def eval_asapscore(sid, file, stat, title='title', composer='Unknown',
                   tons=0, dflag=False, mflag=False):
    s = m21.converter.parse(file)
    (ls, lld) = ps.eval_score(score=s, sid=sid, title=title, composer=composer,
                              stat=stat, tons=tons, debug=dflag, mark=mflag)
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




########################
##                    ##
##  Beethoven Sonata  ##
##                    ##
########################

def Beethoven_list():
    """list of Beethoven Sonata in ASAP, 
		in the form of triplets (sonata nb, mvt nb, path)"""
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

def eval_Beethoven(stat, tons=0, dflag=False, mflag=False):
    if stat == None:
        stat = ps.Stats()
    for s in Beethoven_list():
        if (s.nb, s.mvt) in Beethoven_skip:
            print(s.nb, s.mvt, ': SKIP')           
            continue
        else:
            print(s.nb, s.mvt, ':', s.file)
            eval_asapscore(sid=s.nb*10+s.mvt, file=s.file, stat=stat,
                           title='Sonata '+str(s.nb)+'_'+str(s.mvt), composer='Beethoven',
                           tons=tons, debug=dflag, mark=mflag)
    df = stat.get_dataframe()
    revise_table_asap(df)
    # write table and summary to files and display stats
    df.to_csv(_dataset_root+'/Beethoven/Beethoven.csv', header=True, index=False)
    stat.write_datasum(_dataset_root+'/Beethoven/Beethoven_sum.csv')
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
    #sp.respell()



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
    """list of Bach WK preludes and fugues in ASAP, 
    in the form of triplets (BWV nb, prelude or fugue, path)"""
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
         #(857, 'Fugue', '../../../Datasets/ASAP/Bach/Fugue/bwv_857/xml_score.musicxml'),
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

# (866, 'Prelude') lonnnng
Bach_skip = [(856, 'Prelude'), (873, 'Prelude'), ]

def eval1_Bach(nb, mvt, file, stat, tons=25, dflag=False, mflag=False):
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
                   tons=tons, dflag=dflag, mflag=mflag)
    
def eval_Bach(tons=25, dflag=False, mflag=False): 
    global Bach_skip
    global _dataset_root
    stat = ps.Stats()
    for s in DWK_list():
        if (s.nb, s.mvt) in Bach_skip:
            continue
        else:
            eval1_Bach(nb=s.nb, mvt=s.mvt, file=s.file, stat=stat, 
                       tons=tons, dflag=dflag, mflag=mflag)
    df = stat.get_dataframe()
    revise_table_asap(df)
    # write table and summary to files and display stats
    df.to_csv(_dataset_root+'/Bach/DWK.csv', header=True, index=False)
    stat.write_datasum(_dataset_root+'/Bach/DWK_sum.csv')
    stat.show()


# bl = Beethoven_list()
#beethoven1_1 = get_score('Beethoven/Piano_Sonatas/1-1')
#b = m21.converter.parse('xml_score.musicxml')
#lp = get_parts(b)
#lm0 = lp[0].getElementsByClass(m21.stream.Measure)
#lm1 = lp[1].getElementsByClass(m21.stream.Measure)

#import pse

def get_table(subdir,file):
    global _eval_root
    fp = Path(_eval_root)/'ASAP'/subdir/file
    assert(os.path.isfile(fp))
    df = pd.read_csv(fp)
    return df

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
#sp.respell()
#
# for p in ln:
#    print('sp.add(', p[0].pitch.midi, ',', p[1],')')
    

