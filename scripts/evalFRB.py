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
from pathlib import Path, PosixPath
from datetime import datetime
import re
from operator import itemgetter, attrgetter
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

 # default score file name
_score_suffix = '.musicxml'

# root of evaluation dir
_eval_root = '../../PSeval'

# name of dir for evaluation output
# _output_dir = 'augASAP'

timestamp = str(datetime.today().strftime('%Y%m%d-%H%M'))

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
    global _score_suffix
    dataset_path = Path(_dataset_root) / corpus
    assert isinstance(dataset_path, PosixPath)
    if not os.path.exists(dataset_path):
        print(dataset_path, 'not found')
        return
    # map: opus_name -> path
    dataset = dict()
    files = os.listdir(dataset_path)
    for file in files:    
        filepath = dataset_path/file
        # skip directories
        if os.path.isdir(filepath):
            continue 
        # check the extension in the file name
        if (os.path.splitext(file)[1] == _score_suffix):
            # map score name to file path
            dataset[os.path.splitext(file)[0]] = filepath
    # sort the list alphabetically
    dataset = dict(sorted(dataset.items()))
    return dataset

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
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_FRB(corpus='leads', 
             kpre=0, kpost=0, tons=0, 
             costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True, 
             global1=100, grid=ps.pse.Grid_Rank, 
             costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,
             output_dir='', tablename='',
             dflag=True, mflag=True, csflag=False):
    """eval the whole FRB corpus with given algo and parameters"""
    """corpus: leads or piano (obsolete)"""
    """kpre: parameter specific to PS13"""
    """kpost: parameter specific to PS13"""
    """tons: nb of Tons in TonIndex (PSE)"""
    """costtype1: table1, cost type. if set, PSE is used, otherwise, PS13 is used"""
    """tonal1: table1, tonal/modal flag for initial state (PSE)"""
    """det1: table1, deterministic/exhaustive flag for transitions (PSE)"""
    """grid: name of algorithm for the computation of the grid"""
    """global1: percentage approx for intermediate list of global candidate"""
    """costtype2: table2, cost type. if unset, skip table2 (PSE)"""
    """tonal2: table2, tonal/modal flag for initial state (PSE)"""
    """det2: table2, deterministic/exhaustive flag for transitions (PSE)"""
    """output_dir: where files will be written"""
    """tablename: filename of csv table"""
    """dflag: debug flag"""
    """mflag: mark flag"""
    """csflag: spell also chord symbols"""
    global _eval_root
    global skip
    assert(corpus == 'leads' or corpus == 'piano')
    timestamp = datetime.today().strftime('%Y%m%d-%H%M')
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                    debug=dflag)
    algoname = sp.algoname()
    # default output dir name
    if output_dir == '':
       output_dir = algoname+'_'+timestamp
    output_path = Path(_eval_root)/'evalFRB'/output_dir
    if not os.path.isdir(output_path):
        if not os.path.isdir(Path(_eval_root)/'evalFRB'):
            os.mkdir(Path(_eval_root)/'evalFRB')
        os.mkdir(output_path)
    else:
        print('WARNING: dir', output_path, 'exists')
    # input data and processing   
    stat = ps.Stats()   
    dataset = FRB_corpus(corpus)
    names = sorted(list(dataset)) # list of index in dataset   
    print('\n', 'starting evaluation of FRB dataset -', len(names), 'entries\n')
    i = 0 # score id
    for name in names:
        if (name in skip):
            print('\n', name, 'SKIP\n', flush=True)
            continue
        if (not dataset.get(name)):
            print('\n', name, "not found in dataset, skip", corpus)
            continue
        file = dataset[name]
        print('\n', name, '\n')
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = sp.eval_score(score=s, stats=stat, score_id=i, 
                                  title=name, composer='', 
                                  output_path=output_path, chord_sym=csflag)
        i += 1
        if mflag and not ps.empty_difflist(lld):
            write_score(s, output_path, name)
    # display and save evaluation table
    # default table file name
    if not tablename:
       tablename =  'FRBeval'+'_'+corpus+str(tons)+'_'+timestamp
    stat.show()    
    df = stat.get_dataframe() # create pandas dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(output_path/(tablename+'.csv') , header=True, index=False)
        
# PS13: kpre=33, kpost=23
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_FRBitem(name, corpus='leads', 
                 kpre=0, kpost=0, tons=0,          
                 costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True,       
                 global1=100, grid=ps.pse.Grid_Rank,
                 costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,      
                 output_dir='', filename='',             
                 dflag=True, mflag=False, csflag=False):   
    """eval one item of the FRB corpus with given algo and parameters"""
    """name: filename of item (prefix) in the dataset"""
    """corpus: leads or piano (obsolete)"""
    """kpre: parameter specific to PS13"""
    """kpost: parameter specific to PS13"""
    """tons: nb of Tons in TonIndex (PSE)"""
    """costtype1: table1, cost type. if set, PSE is used, otherwise, PS13 is used"""
    """tonal1: table1, tonal/modal flag for initial state (PSE)"""
    """det1: table1, deterministic/exhaustive flag for transitions (PSE)"""
    """grid: name of algorithm for the computation of the grid"""
    """global1: percentage approx for intermediate list of global candidate"""
    """costtype2: table2, cost type. if unset, skip table2 (PSE)"""
    """tonal2: table2, tonal/modal flag for initial state (PSE)"""
    """det2: table2, deterministic/exhaustive flag for transitions (PSE)"""
    """output_dir: where files will be written"""
    """tablename: filename of csv table"""
    """dflag: debug flag"""
    """mflag: mark flag"""
    """csflag: spell also chord symbols"""
    assert(len(name) > 0)
    assert(corpus == 'leads' or corpus == 'piano')
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                    debug=dflag)
    # input data
    dataset = FRB_corpus(corpus)
    if (dataset.get(name) == None):
        print(name, "not found in dataset", corpus)
        return
    file = dataset[name]
    score = m21.converter.parse(file.as_posix())
    stat = ps.Stats()   
    opath = Path(os.getcwd()) if output_dir=='' else Path(output_dir)
    # ground truth ks, estimated ks, nnb of nontes and list of diff notes
    #(k_gt, gt_est, nn, ld) = ps.eval_part(part=part, stat=stat, nbtons=tons, 
    #                                      debug=dflag, mark=mflag)
    (ls, lld) = sp.eval_score(score=score, stats=stat, score_id=0, 
                              title=name, composer='', output_path=opath, 
                              chord_sym = csflag)    
    stat.show()   
    assert(len(lld) == 1) # always 1 unique part in LG dataset
    if mflag and len(lld[0]) > 0:
        score.show()
        write_score(score, Path(os.getcwd()), name)
        
def write_score(score, output_path, outname):
    if not os.path.isdir(output_path):
        os.mkdir(output_path)
    xmlfile = output_path/(outname+'.musicxml')
    score.write('musicxml', fp=xmlfile)

def write_score2(score, output_path, outname):
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

# compute C++ add instructions for given score, for debugging with gdb
def debug(name, corpus='leads'):    
    assert(len(name) > 0)
    dataset = FRB_corpus(corpus)
    if (dataset.get(name) == None):
        print(name, "not found in dataset", corpus)
        return
    file = dataset[name]
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
