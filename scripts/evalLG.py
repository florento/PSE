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

# import pandas as pd
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
    if ('musescore' in files) and (os.path.isdir(directory_path/'musescore')) and (len(os.listdir(directory_path/'musescore')) > 0):
        directory_path = directory_path/'musescore'
        files = os.listdir(directory_path)        
        for file in files:    
            file_ext = os.path.splitext(file)[1]   # get the extension in the file name
            if (file_ext == '.musicxml'):
                return directory_path/file
    # 2: get file saved by Finale 26
    elif ('finale' in files) and (os.path.isdir(directory_path/'finale')) and (len(os.listdir(directory_path/'finale')) > 0):
        directory_path = directory_path/'finale'
        files = os.listdir(directory_path)        
        for file in files:    
            file_ext = os.path.splitext(file)[1]   # get the extension in the file name
            if (file_ext == '.xml'):
                return directory_path/file
    # no xml file found: return None
    else:
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
            filepath = search_xml(directory_path) # returns path
            if (filepath == None):
                # print(directory, 'has no XML file')
                continue
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

def eval_LG(output_dir:str='', tablename:str='', 
            kpre=33, kpost=23,      # parameters specific to PS13
            nbtons=30,              # PSE: nb of Tons in TonIndex
            ct1=ps.pse.CTYPE_UNDEF, # PSE: table1, cost type. if set, PSE is used, otherwise, PS13 is used
            tonal1=True,            # PSE: table1, tonal/modal flag    
            det1=True,              # PSE: table1, deterministic (PS14) flag
            ct2=ps.pse.CTYPE_UNDEF, # PSE: table2, cost type 
            tonal2=True,            # PSE: table2, tonal/modal flag 
            det2=True,              # PSE: table2, deterministic (PS14) flag
            emask=False,           # restrict candidate globals after 1st table
            eglobal1=0,            # % tolerance for restricted list candidate globals
            eglobal2=0,            # % tolerance for final list globals
            dflag=True,             # debug flag
            mflag=True):            # mark flag
    """eval the whole LG corpus with given algo parameters"""
    # construction of speller PS13
    if (ct1 == ps.pse.CTYPE_UNDEF):
        sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, debug=debug) 
    # construction of speller PSE with 1 or 2 steps
    else:
        sp = ps.Spellew(nbtons=nbtons,
                        t1_costtype=ct1, t1_tonal=tonal1, t1_det=det1, 
                        t2_costtype=ct2, t2_tonal=tonal2, t2_det=det2, 
                        mask=emask, global1=eglobal1, global2=eglobal2,
                        debug=dflag) 
    # output path
    if not output_dir:
       output_dir = sp.new_dir() # default timestamped dir associated to current speller 
    opath = Path(_eval_root)/output_dir
    if not os.path.isdir(opath):
        opath.mkdir(parents=True, exist_ok=True) # Python ≥ 3.5
    # process all LG opus
    stat = ps.Stats() # will be updated by the speller 
    dataset = LG_map()
    li = sorted(list(dataset)) # list of index in dataset   
    print('starting evaluation of LG dataset -', len(li), 'entries', flush=True)
    for i in li:
        if (i in skip): # skip particular score
            print('eval LG: skip', i, flush=True)
            continue 
        file = dataset[i] # should always be present
        filep = file.parts
        t = ''
        if (filep[-3] == 'ref'):
            t = filep[-4]
        else:
            print('eval LG: skip', i, '(ref/ not found)', flush=True)
            continue # skip score
        print('   ')
        print('evaluation of', t, flush=True)
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = sp.eval_score(score=s, stats=stat, 
                                  score_id=i, title=t, composer='', 
                                  output_path=opath if mflag else None)
    # display and save evaluation table
    if not tablename:
       tablename =  'LG_'+output_dir  # default table file name
    stat.show()    
    df = stat.get_dataframe() # create pands dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(opath/(tablename+'.csv') , header=True, index=False)
    stat.write_datasum(opath/(tablename+'_sum.csv'))
      
def eval_LGitem(score_id, 
                kpre=33, kpost=23,      # parameters specific to PS13
                nbtons=30,              # PSE: nb of Tons in TonIndex
                ct1=ps.pse.CTYPE_UNDEF, # PSE: table1, cost type 
                tonal1=True,            # PSE: table1, tonal/modal flag    
                det1=True,              # PSE: table1, deterministic (PS14) flag
                ct2=ps.pse.CTYPE_UNDEF, # PSE: table2, cost type 
                tonal2=True,            # PSE: table2, tonal/modal flag 
                det2=True,              # PSE: table2, deterministic (PS14) flag 
                emask=False,             # restrict candidate globals after 1st table
                eglobal1=0,               # % tolerance for restricted list candidate globals
                eglobal2=0,               # % tolerance for final list globals
                dflag=True, 
                mflag=True):
    # construction of speller PS13
    if (ct1 == ps.pse.CTYPE_UNDEF):
        sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, debug=debug) 
    # construction of speller PSE with 1 or 2 steps
    else:
        sp = ps.Spellew(nbtons=nbtons,
                        t1_costtype=ct1, t1_tonal=tonal1, t1_det=det1, 
                        t2_costtype=ct2, t2_tonal=tonal2, t2_det=det2, 
                        mask=emask, global1=eglobal1, global2=eglobal2,
                        debug=dflag) 
    stat = ps.Stats()   
    opath=Path(os.getcwd())
    # process the LG opus of given id
    dataset = LG_map()
    file = dataset.get(score_id)
    if file is None:
        print('opus', score_id, 'not found in dataset LG, abort')
        return
    filep = file.parts
    t = ''
    if (filep[-2] == 'ref'):
        t = filep[-3]+'_'+sp.new_dir()
    else:
        t = str(score_id)+'_'+sp.new_dir()
    s = m21.converter.parse(file.as_posix())
    print('evaluation of LG', score_id, t, flush=True)

    (ls, lld) = sp.eval_score(score=s, stats=stat, 
                              score_id=score_id, title=t, composer='', 
                              output_path=opath)
    stat.show()   
    assert(len(lld) == 1) # always 1 unique part in LG dataset
    if mflag and len(lld[0]) > 0:
        s.show()
        ps.write_score(s, opath, t)
        

                    
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

