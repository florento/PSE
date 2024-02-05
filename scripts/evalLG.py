#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 28 12:16:20 2022

@author: jacquema
"""

#import sys
#import logging
import os
from pathlib import Path, PosixPath
from datetime import datetime

import pandas as pd
import music21 as m21
import PSeval as ps


########################
##                    ##
##  global variables  ##
##                    ##
########################

# root of evaluation dir
_eval_root = '../../PSeval'
# or '..' if we assume that we are in dir script/ of the evaluation dir

# path to LG dataset
_dataset_root = '../../../Datasets/Lamarque-Goudard/'

# name of dir for evaluation output
#_output_dir = 'LG/230226'   

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

# sys.path.insert(0, "/jacquema/Datasets/Lamarque-Goudard")

def search_xml(directory_path):
    """search for a MusicXML file in a directory path"""
    files = os.listdir(directory_path)
    # 1: file exported by MuseScore
    for file in files:    
        file_ext = os.path.splitext(file)[1]   # get the extension in the file name
        if (file_ext == '.musicxml'):
            return file
    # 2: get file saved by Finale 26
    for file in files:    
        file_ext = os.path.splitext(file)[1]   # get the extension in the file name
        if (file_ext == '.xml'):
            return file
    # no xml file found: return None
    return 

def first_part(score):
    lp = score.getElementsByClass(m21.stream.Part)
    if (len(lp) != 1):
        print('FAIL, nb parts =', len(lp), flush=True)
        return
    return lp[0]

def key_changes():
    """find scores in data set with key changes"""
    dataset = LG_map()
    li = sorted(list(dataset))  # list of index in dataset
    for i in li:
        file = dataset[i]
        score = m21.converter.parse(file.as_posix())
        kl = ps.extract_keys(score)
        kex = []
        if (len(kl) > 1):
            kex.append(id)
            print(len(kl), 'keys in', id,
                  score.metadata.composer, score.metadata.title)
    return kex

def LG_map():
    global _dataset_root
    dataset_path = Path(_dataset_root)
    assert isinstance(dataset_path, PosixPath)
    ld = os.listdir(dataset_path)
    dataset = dict()
    for directory in ld:
        prefix = directory[:3]
        if (not prefix.isdigit()):
            # print(directory, 'excluded')
            continue
        directory_path = dataset_path / directory
        files = os.listdir(directory_path)
        if 'ref' in files:
            #print(directory, 'has ref')
            directory_path = directory_path / 'ref'
            file = search_xml(directory_path)
            if (file == None):
                # print(directory, 'has no XML file')
                continue
            filepath = directory_path / file
            id = int(prefix)        
            dataset[id] = filepath            
        else:
            #print(directory, 'has no ref/')
            continue
        #print(directory, id)
    dataset = dict(sorted(dataset.items()))
    return dataset



###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################


# these opus might cause complexity issues (extreme measures)
skip = [441, 470, 472, 473, 475, 478]

# 441:
# raise MusicXMLExportException('Cannot convert inexpressible durations to MusicXML.')
# MusicXMLExportException: In part (Voice), measure (11): Cannot convert inexpressible durations to MusicXML.

def eval_LG(psalgo=ps.pse.Algo_PSE, nbtons=30, kpre=33, kpost=23,
            output_dir='', filename='', 
            debug=True, mark=True):
    timestamp = datetime.today().strftime('%Y%m%d-%H%M')
    # default output dir name
    if output_dir == '':
       output_dir = timestamp
    output_path = Path(_eval_root)/'augLG'/output_dir
    if not os.path.isdir(output_path):
        if not os.path.isdir(Path(_eval_root)/'augLG'):
            os.mkdir(Path(_eval_root)/'augLG')
        os.mkdir(output_path)
    else:
        print('WARNING: dir', output_path, 'exists')
    stat = ps.Stats()   
    dataset = LG_map()
    li = sorted(list(dataset)) # list of index in dataset   
    print('\n')
    print('starting evaluation of LG dataset -', len(li), 'entries')
    print('\n')    
    for i in li:
        if (i in skip):
            continue
        file = dataset[i]
        filep = file.parts
        t = ''
        if (filep[-2] == 'ref'):
            t = filep[-3]
        else:
            continue
        print('\n')
        print(t)
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = ps.eval_score(score=s, stat=stat, 
                                  sid=i, title=t, composer='', 
                                  algo=psalgo,
                                  nbtons=nbtons,           # for PSE 
                                  kpre=kpre, kpost=kpost,  # for PS13                                  
                                  debug=debug, mark=mark)
        if mark and not ps.empty_difflist(lld):
            write_score(s, output_path, t)
    # display and save evaluation table
    # default table file name
    if filename == '':
       filename =  'LGeval'+str(nbtons)+'_'+timestamp
    stat.show()    
    df = stat.get_dataframe() # create pands dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(output_path/(filename+'.csv') , header=True, index=False)
    stat.write_datasum(output_path/(filename+'_sum.csv'))
      
def eval_LGitem(i, algo=ps.pse.Algo_PSE, nbtons=30, kpre=33, kpost=23, 
                dflag=True, mflag=True):
    stat = ps.Stats()   
    dataset = LG_map()
    file = dataset[i]
    filep = file.parts
    if (filep[-2] == 'ref'):
        t = filep[-3]+'_eval'
    else:
        t = str(i)+'_eval'
    s = m21.converter.parse(file.as_posix())
    # ground truth ks, estimated ks, nnb of nontes and list of diff notes
    #(k_gt, gt_est, nn, ld) = ps.eval_part(part=part, stat=stat, nbtons=tons, 
    #                                      debug=dflag, mark=mflag)
    (ls, lld) = ps.eval_score(score=s, stat=stat, 
                              sid=i, title=t, composer='', 
                              algo=algo,
                              nbtons=nbtons,            # for PSE 
                              kpre=kpre, kpost=kpost, # for PS13                                  
                              debug=dflag, mark=mflag)
    stat.show()   
    assert(len(lld) == 1) # always 1 unique part in LG dataset
    if mflag and len(lld[0]) > 0:
        s.show()
        write_score(s, Path(os.getcwd()), t)
        
def write_score(score, output_path, outname):
    assert(len(outname) > 0)
    if not os.path.isdir(output_path):
        os.mkdir(output_path)
    output_path = output_path/outname
    if not os.path.isdir(output_path):
        os.mkdir(output_path)
    xmlfile = output_path/(outname+'.musicxml')
    score.write('musicxml', fp=xmlfile)
    # pdffile = dirname+'/'+outname+'.pdf'
    # os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)
        
                    
#######################
##                   ## 
##       test        ##
## manual evaluation ##
##                   ##
#######################

def debug(i):    
    dataset = LG_map()
    file = dataset[i]
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
    #sp = ps.Speller()
    #sp.debug(True)
    #ps.add_tons(0, sp)
    #sp.add_notes(ln1[:61], sp)
    #sp.spell()


def find_chords():
    dataset = LG_map()
    li = sorted(list(dataset)) # list of index in dataset   
    for i in li:
        file = dataset[i]
        filep = file.parts
        t = ''
        if (filep[-2] == 'ref'):
            t = filep[-3]
        else:
            continue
        s = m21.converter.parse(file.as_posix())
        lc = first_part(s).flatten().getElementsByClass(m21.chord.Chord)
        if (len(lc) > 0):
            print(t)

# TBR
#_dataset = init(dataset_root)
#li = sorted(list(dataset)) # list of index in dataset    

# old, TBR
#def eval_item(id, tons=0, dflag=False):
#    global _dataset_root
#    dataset = LG_map(_dataset_root)
#    file = dataset[id]
#    score = m21.converter.parse(file.as_posix())
#    print(id, score.metadata.composer, score.metadata.title, end=' ')
#    part = first_part(score)
#    if (not ps.spellable(part)):
#        print('FAIL, cannot spell', flush=True)
#        return
#    # estimated ks and list of diff notes
#    (k, ld) = ps.pseval_part(part=part, nbtons=tons, debug=dflag)
#    if (len(ld) > 0):
#        score.show()
#        tb = m21.text.TextBox('estimated key signature = '+str(k), 30, 30)
#        score.append(tb)
#        fpart = part.flatten()
#        ln = fpart.getElementsByClass(m21.note.Note) 
#        for (i, n, a, o, p) in ld:
#            ln[i].style.color = 'red'
#            ln[i].pitch.step = ps.mk_step(n)
#            if ((a != ps.pse.Accid.Natural) or (p == True)):
#                ln[i].pitch.accidental = ps.mk_accid(a)
#            ln[i].pitch.octave = o                
#        score.show()
                

#TESTI=128
#dataset = init(dataset_root)
#file = dataset[TESTI]
#print(file)
#score = m21.converter.parse(file.as_posix())
#score.show('text')
#score.show()
#part = first_part(score)
#ln = pse.extract_notes(part)
#for (n, b) in ln:
#    print('sp.add(', n.pitch.midi, ',', b, ')')
    
# 470 boucle dans respell
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

#275 (chord)
#dataset = LG_map(_dataset_root)
#file = dataset[275]
#score = m21.converter.parse(file.as_posix())
#score.show('text')
#part = first_part(score)
#ln = ps.extract_part(part)
#for p in ln:
#    print('sp.add(', p[0].pitch.midi, ',', p[1], 'true' if p[2] else 'false',')')

