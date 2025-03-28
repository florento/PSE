#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  5 14:02:31 2024
@author: jacquema

Evaluation of the score of the Fake Real Book dataset
"""

#import sys
#import logging
import os
import subprocess
from pathlib import Path
import pandas
import music21 as m21
import PSeval as ps
import evalXML


########################
##                    ##
##  global variables  ##
##                    ##
########################

# path to FRB dataset
_dataset_root = '../../../Datasets/FiloSax-xml'

# root of evaluation dir
_eval_root = '../../PSeval'

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

def FiloSax_corpus(transposed=False):
    """build a list of scores in a subdirectory of FiloSax"""
    global _dataset_root
    root = Path(_dataset_root)
    if transposed:
        root = root/'transpose_ms'
    return evalXML.get_corpus(root) # flat

def accids(ks, notes):
    c = 0
    for note in notes:
        if note.pitch.accidental != ks.accidentalByStep(note.name):
            c += 1            
    return c
    
def FiloSax_table():
    table = []
    dataset = FiloSax_corpus()
    names = sorted(list(dataset)) # list of index in dataset   
    for name in names:
        if (dataset.get(name) == None):
            print(name, "not found in dataset")
            continue        
        file = dataset[name]
        score = m21.converter.parse(file.as_posix())
        assert(len(score.parts) > 0)
        part = score.parts[0]
        fpart = part.flatten()
        keys = fpart.getElementsByClass([m21.key.Key, m21.key.KeySignature])
        notes = fpart.getElementsByClass(m21.note.Note)
        row = []
        row.append(name)
        row.append(keys[0].sharps if len(keys) > 0 else None)            
        row.append(len(part.getElementsByClass(m21.stream.Measure)))
        row.append(len(notes))
        row.append(accids(keys[0], notes) if len(keys) > 0 else None)
        row.append(len(score.parts))
        row.append(len(keys))
        table.append(row)
    df = pandas.DataFrame(table)
    df.columns = ['name', 'KS','# bars', '# notes', '# accids', '# parts', '# keys']
    df['KS'] = df['KS'].map('{:n}'.format)
    return df  

#####################################
##                                 ##
## automatic evaluation of dataset ##
##                                 ##
#####################################

# list of opus names with issues 
skip = []
      
# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_FiloSax(output='', tablename='', transposed=False,               
                 kpre=0, kpost=0, tons=0, 
                 costtype1=ps.pse.CTYPE_UNDEF, 
                 tonal1=True, octave1=False, det1=False, 
                 global1=100, grid=ps.pse.Grid_Rank, 
                 costtype2=ps.pse.CTYPE_UNDEF, 
                 tonal2=True, octave2=True, det2=False,
                 dflag=True, mflag=True, csflag=0):
    """eval the whole FRB corpus with given algo and parameters"""
    """output: dir where files will be written"""
    """tablename: filename of csv table in output"""
    """kpre: parameter specific to PS13"""
    """kpost: parameter specific to PS13"""
    """tons: nb of Tons in TonIndex (PSE)"""
    """costtype1: table1, cost type. if set, PSE is used, otherwise, PS13 is used"""
    """tonal1: table1, tonal/modal flag for initial state (PSE)"""
    """octave1: table1, octave flag for state transitions (PSE)"""
    """det1: table1, deterministic/exhaustive flag for transitions (PSE)"""
    """grid: name of algorithm for the computation of the grid"""
    """global1: percentage approx for intermediate list of global candidate"""
    """costtype2: table2, cost type. if unset, skip table2 (PSE)"""
    """tonal2: table2, tonal/modal flag for initial state (PSE)"""
    """octave2: table2, octave flag for state transitions (PSE)"""
    """det2: table2, deterministic/exhaustive flag for transitions (PSE)"""
    """dflag: debug flag: print debug messages on terminal"""
    """mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
    """csflag: see pse.eval_score"""   
    global _eval_root
    global skip
    assert(csflag in [0, 1, 2, 3])
    root = Path(_eval_root)/'evalFiloSax'
    if not os.path.isdir(root):
        os.mkdir(root)
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, 
                    t1_tonal=tonal1, t1_octave=octave1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, 
                    t2_tonal=tonal2, t2_octave=octave2, t2_det=det2,
                    debug=dflag, aux_enum=(csflag == 2))        
    # start evaluating the corpus with the speller   
    evalXML.eval_corpus(speller=sp, mflag=mflag, csflag=csflag, 
                        dataset=FiloSax_corpus(transposed), skip=skip, 
                        eval_root=root, output_dir=output, tablename=tablename)
        
# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_FiloSaxitem(name, output='', transposed=False,            
                     kpre=0, kpost=0, tons=0,          
                     costtype1=ps.pse.CTYPE_UNDEF, 
                     tonal1=True, octave1=False, det1=True,       
                     global1=100, grid=ps.pse.Grid_Rank,
                     costtype2=ps.pse.CTYPE_UNDEF, 
                     tonal2=True, octave2=True, det2=True,      
                     dflag=True, mflag=False, csflag=0):   
    """eval one item of the FRB corpus with given algo and parameters"""
    """name: filename of item (prefix) in the dataset"""
    """output: dir where files will be written"""
    """kpre: parameter specific to PS13"""
    """kpost: parameter specific to PS13"""
    """tons: nb of Tons in TonIndex (PSE)"""
    """costtype1: table1, cost type. if set, PSE is used, otherwise, PS13 is used"""
    """tonal1: table1, tonal/modal flag for initial state (PSE)"""
    """octave1: table1, octave flag for state transitions (PSE)"""
    """det1: table1, deterministic/exhaustive flag for transitions (PSE)"""
    """global1: percentage approx for intermediate list of global candidate"""
    """grid: name of algorithm for the computation of the grid"""
    """costtype2: table2, cost type. if unset, skip table2 (PSE)"""
    """tonal2: table2, tonal/modal flag for initial state (PSE)"""
    """octave2: table2, octave flag for state transitions (PSE)"""
    """det2: table2, deterministic/exhaustive flag for transitions (PSE)"""
    """dflag: debug flag: print debug messages on terminal"""
    """mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
    """csflag: see pse.eval_score"""   
    assert(len(name) > 0)
    assert(csflag in [0, 1, 2, 3])
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, 
                    t1_tonal=tonal1, t1_octave=octave1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, 
                    t2_tonal=tonal2, t2_octave=octave2, t2_det=det2,
                    debug=dflag, aux_enum=(csflag == 2))
    # start evaluating the opus with the speller      
    evalXML.eval_item(speller=sp, mflag=mflag, csflag=csflag,
                      dataset=FiloSax_corpus(transposed), 
                      name=name, output_dir=output)

#####################################
##                                 ##
##  transpose for instrument in C  ##
##                                 ##
#####################################

def transpose_all(odir):
    dataset = FiloSax_corpus()
    names = sorted(list(dataset)) # list of index in dataset   
    for name in names:
        print(name)
        original = dataset[name]
        transpose(name, original, odir)        
                
def transpose(name, original, odir):
    """name: original filename in FiloSax (without suffix)"""
    """odir: output dir"""
    score = m21.converter.parse(original.as_posix())
    # it is M-9 for tenor sax, we just do M-2
    # score2 = score.transpose(lp[0].getInstrument().transposition) 
    score2 = score.transpose(m21.interval.Interval('M-2'))    
    lp = score2.getElementsByClass(m21.stream.Part)
    assert(len(lp) == 1)
    part = lp[0]
    part.replace(part.getInstrument(), m21.instrument.Piano())
    part.partName = 'Piano'
    part.partAbbreviation = 'Pno.'
    # score2.show()
    path = Path(os.path.dirname(original))/odir
    xmlfile = path/(name+'_C.musicxml')
    score2.write('musicxml', fp=xmlfile)
    
# compute C++ add instructions for given score, for debugging with gdb
def debug(name):    
    assert(len(name) > 0)
    dataset = FiloSax_corpus()
    evalXML.debug(dataset, name)

def tons(file=''):
    """list of opus without KS"""
    dataset = FiloSax_corpus(True)
    names = sorted(list(dataset)) # list of index in dataset   
    keys = []
    for name in names:
        if (not dataset.get(name)):
            print('\n', name, "not found in dataset, skip")
            continue
        file = dataset[name]
        score = m21.converter.parse(file.as_posix())
        lp = score.getElementsByClass(m21.stream.Part)
        if (len(lp) > 1):
            print(name, len(lp), 'parts, take only 1st')

        keyl = lp[0].flatten().getElementsByClass([m21.key.Key])
        if (len(keyl) > 1):
            print(len(keyl), 'keys, take only 1st')
        key = None if len(keyl) == 0 else keyl[0]
        keyname = key.name if key is not None else None

        ksl = lp[0].flatten().getElementsByClass([m21.key.KeySignature])
        if (len(ksl) > 1):
            print(len(ksl), 'keys, take only 1st')
        ks = None if len(ksl) == 0 else ksl[0]
        if key is not None:
            sks = key.sharps 
        elif ks is not None:
            sks = ks.sharps 
        else:
            sks = None
        print(name, keyname, sks)
        keys.append({'name': name, 'key': keyname, 'ks':sks})
    if file:
        df_raw = pandas.DataFrame(keys)
        df = df_raw.convert_dtypes() # convert to int even when there are NaN (which is float)
        df.to_csv(file, header=True, index=True)
    return keys

def ms2xml(dir):
    global _mscore
    dataset = FiloSax_corpus()
    names = sorted(list(dataset))
    path = Path(dir)
    for name in names:
        ms_file = path/(name+'_C.mscz')
        xml_file = path/(name+'_C.musicxml')
        if not xml_file.is_file(): # file does not exist
            print(name)
            #print(ms_file.as_posix())
            subprocess.run([_mscore, ms_file.as_posix(), '-o', xml_file.as_posix()], 
                           stderr=subprocess.DEVNULL)
    
    
    