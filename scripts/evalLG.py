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
import music21 as m21
import PSeval as ps


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

# sys.path.insert(0, "/jacquema/Datasets/Lamarque-Goudard")

# global variables
# path to LG dataset
_dataset_root = _eval_root+'/../../Datasets/Lamarque-Goudard/'

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

def key_changes(root):
    """find scores in data set with key changes"""
    dataset = LG_map(root)
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

def LG_map(root):
    dataset_path = Path(root)
    assert isinstance(dataset_path, PosixPath)
    ld = os.listdir(dataset_path)
    dataset = dict()
    for directory in ld:
        prefix = directory[:3]
        if (not prefix.isdigit()):
            print(directory, 'excluded')
            continue
        directory_path = dataset_path / directory
        files = os.listdir(directory_path)
        if 'ref' in files:
            #print(directory, 'has ref')
            directory_path = directory_path / 'ref'
            file = search_xml(directory_path)
            if (file == None):
                print(directory, 'has no XML file')
                continue
            filepath = directory_path / file
            id = int(prefix)        
            dataset[id] = filepath            
        else:
            print(directory, 'has no ref/')
            continue
        #print(directory, id)
    dataset = dict(sorted(dataset.items()))
    return dataset



###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################

# global variables

# root of evaluation dir
_eval_root = '../../PSeval'
# or '..' if we assume that we are in dir script/ of the evaluation dir

# name of dir for evaluation output
_output_dir = 'LG/230228_300'   

# MuseScore commandline executable
_mscore = '/Applications/MuseScore\ 4.app/Contents/MacOS/mscore'

# these opus might cause complexity issues (extreme measures)
skip = [441, 470, 472, 473, 475, 478]

# 441:
# raise MusicXMLExportException('Cannot convert inexpressible durations to MusicXML.')
# MusicXMLExportException: In part (Voice), measure (11): Cannot convert inexpressible durations to MusicXML.


def eval_LG(stat, tons=26, debug=False, mark=False):
    global _dataset_root
    # stat.nbtons = tons
    dataset = LG_map(_dataset_root)
    li = sorted(list(dataset)) # list of index in dataset   
    print('\n')
    print('starting evaluation')
    print('\n')    
    for i in li:
        if (i in skip):
            continue
        file = dataset[i]
        print('\n')
        print(file)
        filep = file.parts
        t = ''
        if (filep[-2] == 'ref'):
            t = filep[-3]
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = ps.eval_score(score=s, sid=i, title=t, composer='', 
                                  stat=stat, tons=tons, debug=debug, 
                                  mark=mark)
        if mark and not ps.empty_difflist(lld):
            write_score(s, t)
   
def eval_item(id, tons=26, dflag=False, mflag=False):
    global _dataset_root
    dataset = LG_map(_dataset_root)
    file = dataset[id]
    score = m21.converter.parse(file.as_posix())
    print(id, score.metadata.composer, score.metadata.title, end=' ')
    part = first_part(score)
    if (not ps.spellable(part)):
        print('FAIL, cannot spell', flush=True)
        return
    # grounnd truth ks, estimated ks, nnb of nontes and list of diff notes
    (ks_gt, gs_est, nn, ld) = ps.eval_part(part=part, nbtons=tons, 
                                           debug=dflag, mark=mflag)
    filep = file.parts
    if (filep[-2] == 'ref'):
        t = filep[-3]+'_eval'
    else:
        t = str(id)+'_eval'
    if mflag and len(ld) > 0:
        score.show()
        write_score(score, t)
                    
def write_score(score, outname):
    global _eval_root
    global _output_dir
    #global _mscore
    assert(len(_output_dir) > 0)
    dirname = _eval_root+'/'+_output_dir
    if not os.path.isdir(dirname):
        os.mkdir(dirname)
    dirname = dirname+'/'+outname
    if not os.path.isdir(dirname):
        os.mkdir(dirname)
    xmlfile = dirname+'/'+outname+'.musicxml'
    score.write('musicxml', fp=xmlfile)
    # pdffile = dirname+'/'+outname+'.pdf'
    # os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)
 
def eval_export(filename, tons=26, debug=False, mark=False):
    global _eval_root
    global _output_dir
    filename = _eval_root+'/'+_output_dir+'/'+filename
    stat = ps.Stats()    
    eval_LG(stat, tons, debug, mark)
    stat.show()    
    df = stat.get_dataframe() # create pands dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(filename, header=True, index=False)

def complete_table(df):
    
    return
        


#######################
##                   ## 
##       test        ##
## manual evaluation ##
##                   ##
#######################

# TBR
#_dataset = init(dataset_root)
#li = sorted(list(dataset)) # list of index in dataset    

# old, TBR
def eval_item(id, tons=0, dflag=False):
    global _dataset_root
    dataset = LG_map(_dataset_root)
    file = dataset[id]
    score = m21.converter.parse(file.as_posix())
    print(id, score.metadata.composer, score.metadata.title, end=' ')
    part = first_part(score)
    if (not ps.spellable(part)):
        print('FAIL, cannot spell', flush=True)
        return
    # estimated ks and list of diff notes
    (k, ld) = ps.pseval_part(part=part, nbtons=tons, debug=dflag)
    if (len(ld) > 0):
        score.show()
        tb = m21.text.TextBox('estimated key signature = '+str(k), 30, 30)
        score.append(tb)
        fpart = part.flatten()
        ln = fpart.getElementsByClass(m21.note.Note) 
        for (i, n, a, o, p) in ld:
            ln[i].style.color = 'red'
            ln[i].pitch.step = ps.mk_step(n)
            if ((a != ps.pse.Accid.Natural) or (p == True)):
                ln[i].pitch.accidental = ps.mk_accid(a)
            ln[i].pitch.octave = o                
        score.show()
                

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

