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
_dataset_root = '../../../Datasets/FakeRealBook'

# root of evaluation dir
_eval_root = '../../PSeval'

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

# corpus can be 'leads' or 'piano'
def FRB_corpus(corpus='leads'):
    """build a list of scores in a subdirectory of the FRB"""
    global _dataset_root
    assert(corpus == 'leads' or corpus == 'piano')
    return evalXML.get_corpus(Path(_dataset_root)/corpus, True) # flat corpus

def accids(ks, notes):
    c = 0
    for note in notes:
        if note.pitch.accidental != ks.accidentalByStep(note.name):
            c += 1            
    return c
    
def FRB_table(corpus='leads'):
    assert(corpus == 'leads' or corpus == 'piano')
    table = []
    dataset = FRB_corpus(corpus)
    names = sorted(list(dataset)) # list of index in dataset   
    for name in names:
        if (dataset.get(name) == None):
            print(name, "not found in dataset", corpus)
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
skip = ['Autumn in New York']
       

# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_FRB(corpus='leads', output='', tablename='',            
             kpre=0, kpost=0, tons=0, 
             costtype1=ps.pse.CTYPE_UNDEF, 
             tonal1=True, octave1=False, det1=False, 
             global1=100, grid=ps.pse.Grid_Rank, 
             costtype2=ps.pse.CTYPE_UNDEF, 
             tonal2=True, octave2=True, det2=False,
             dflag=True, mflag=True, csflag=0):
    """eval the whole corpus with given algo and parameters"""
    """corpus: leads or piano (obsolete)"""
    """output: dir where files will be written"""
    """tablename: filename of csv table"""
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
    """dflag: debug flag"""
    """mflag: mark flag"""
    """csflag: see pse.eval_score"""   
    global _eval_root
    global skip
    assert(corpus == 'leads' or corpus == 'piano')
    assert(csflag in [0, 1, 2])
    root = Path(_eval_root)/'evalFRB'
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
                        dataset=FRB_corpus(corpus), skip=skip, 
                        eval_root=root, output_dir=output, tablename=tablename)
        
# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_FRBitem(name, corpus='leads', output='',         
                 kpre=0, kpost=0, tons=0,          
                 costtype1=ps.pse.CTYPE_UNDEF, 
                 tonal1=True, octave1=False, det1=True,       
                 global1=100, grid=ps.pse.Grid_Rank,
                 costtype2=ps.pse.CTYPE_UNDEF, 
                 tonal2=True, octave2=True, det2=True,      
                 dflag=True, mflag=False, csflag=0):       
    """eval one item of the FRB corpus with given algo and parameters"""
    """name: filename of item (prefix) in the dataset"""
    """corpus: leads or piano (obsolete)"""
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
    """csflag: 0 if we do not spell the notes of chord symbols"""
    """        1 if we spell them"""   
    """        2 if we force their names in spelling"""   
    assert(len(name) > 0)
    assert(corpus == 'leads' or corpus == 'piano')
    assert(csflag in [0, 1, 2])
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                    debug=dflag, aux_enum=(csflag == 2))
    # start evaluating one opus with the speller
    evalXML.eval_item(speller=sp, mflag=mflag, csflag=csflag,
                      dataset=FRB_corpus(corpus), name=name, output_dir=output)
        
# compute C++ add instructions for given score, for debugging with gdb
def debug(name, corpus='leads'):    
    assert(len(name) > 0)
    dataset = FRB_corpus(corpus)
    evalXML.debug(dataset, name)

def atonals(file=''):
    """list of opus of FRB without KS"""
    dataset = FRB_corpus('leads')
    names = sorted(list(dataset)) # list of index in dataset   
    keys = []
    for name in names:
        if (not dataset.get(name)):
            print('\n', name, "not found in dataset, skip")
            continue
        file = dataset[name]
        score = m21.converter.parse(file.as_posix())
        lp = score.getElementsByClass(m21.stream.Part)
        ks = ps.get_key(lp[0])
        if (ks):            
            print(name, ks.sharps)
            keys.append({'name': name, 'key': ks.sharps})
        else:
            print(name, 'NONE')           
            keys.append({'name': name, 'key': None})
    if file:
        df_raw = pandas.DataFrame(keys)
        df = df_raw.convert_dtypes() # convert to int even when there are NaN (which is float)
        df.to_csv(file, header=True, index=True)
    return keys
