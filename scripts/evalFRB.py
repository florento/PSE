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


########################
##                    ##
##  global variables  ##
##                    ##
########################

# path to ASAP dataset
_dataset_root = '../../../Datasets/FakeRealBook'

 # default score file name
_score_suffix = '.musicxml'

# root of evaluation dir
_eval_root = '../../PSeval'

# name of dir for evaluation output
_output_dir = 'augASAP'

timestamp = str(datetime.today().strftime('%Y%m%d-%H%M'))

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

# corpus can be 'leads' or 'piano'
def FRB_corpus(corpus):
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

# df.fillna('NaN').to_csv(file, header=True, index=False)
        
        
###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################

# list of opus names with issues 
skip = ['Autumn in New York']

def eval_FRB(corpus='leads', algo=ps.pse.Algo_PSE, 
             tons=135, kpre=33, kpost=23, 
             output_dir='', filename='',             
             debug=True, mark=True):
    global _eval_root
    assert(corpus == 'leads' or corpus == 'piano')
    timestamp = datetime.today().strftime('%Y%m%d-%H%M')
    # default output dir name
    if output_dir == '':
       output_dir = timestamp
    output_path = Path(_eval_root)/'evalFRB'/output_dir
    if not os.path.isdir(output_path):
        if not os.path.isdir(Path(_eval_root)/'evalFRB'):
            os.mkdir(Path(_eval_root)/'evalFRB')
        os.mkdir(output_path)
    else:
        print('WARNING: dir', output_path, 'exists')
    stat = ps.Stats()   
    dataset = FRB_corpus(corpus)
    names = sorted(list(dataset)) # list of index in dataset   
    print('\n', 'starting evaluation of FRB dataset -', len(names), 'entries\n')
    for name in names:
        if (name in skip):
            print('\n', name, 'SKIP\n')
            continue
        if (dataset.get(name) == None):
            print(name, "not found in dataset", corpus)
            continue
        file = dataset[name]
        print('\n', name, '\n')
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = ps.eval_score(score=s, stat=stat, 
                                  sid=0, title=name, composer='', 
                                  algo=algo,
                                  nbtons=tons,           # for PSE 
                                  kpre=kpre, kpost=kpost,  # for PS13                                  
                                  debug=debug, mark=mark)
        if mark and not ps.empty_difflist(lld):
            write_score(s, output_path, name)
    # display and save evaluation table
    # default table file name
    if filename == '':
       filename =  'FRWeval'+'_'+corpus+str(tons)+'_'+timestamp
    stat.show()    
    df = stat.get_dataframe() # create pands dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(output_path/(filename+'.csv') , header=True, index=False)
    stat.write_datasum(output_path/(filename+'_sum.csv'))    
    
    
def eval_FRBitem(name, corpus='leads', algo=ps.pse.Algo_PSE, 
                 tons=104, kpre=33, kpost=23, dflag=True, mflag=True):
    assert(len(name) > 0)
    assert(corpus == 'leads' or corpus == 'piano')
    dataset = FRB_corpus(corpus)
    if (dataset.get(name) == None):
        print(name, "not found in dataset", corpus)
        return
    file = dataset[name]
    score = m21.converter.parse(file.as_posix())
    stat = ps.Stats()   
    # ground truth ks, estimated ks, nnb of nontes and list of diff notes
    #(k_gt, gt_est, nn, ld) = ps.eval_part(part=part, stat=stat, nbtons=tons, 
    #                                      debug=dflag, mark=mflag)
    (ls, lld) = ps.eval_score(score=score, stat=stat, 
                              sid=0, title=name, composer='', 
                              algo=algo,
                              nbtons=tons,          # for PSE 
                              kpre=kpre, kpost=kpost, # for PS13                                  
                              debug=dflag, mark=mflag)
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
    


