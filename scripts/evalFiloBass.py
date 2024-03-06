#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  5 14:02:31 2024
@author: jacquema

Evaluation of the score of the Fake Real Book dataset
"""


import sys
import logging

sys.path.append('/Users/xavriley/Projects/pse/lib')
import pse

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
#_dataset_root = '/Users/xavriley/Dropbox/PhD/Datasets/FiloBass ISMIR Publication/musicxml/'
_dataset_root = '../../../Datasets/FiloBass-xml'

 # default score file name
_score_suffix = '.musicxml'

# root of evaluation dir
_eval_root = '../../PSeval'

Path(_eval_root).mkdir(parents=True, exist_ok=True)

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

def FiloBassCorpus():
    """build a list of scores in a subdirectory of FiloBass"""
    global _dataset_root
    global _score_suffix
    dataset_path = Path(_dataset_root)
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
    
def FiloBass_table():
    table = []
    dataset = FiloBassCorpus()
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

# df.fillna('NaN').to_csv(file, header=True, index=False)
        
        
###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################

# list of opus names with issues 
skip = ['All-the-Things-You-Are',
        'Alone-Together',
        'Apple-Jump',]

def eval_FiloBass(algo=ps.pse.Algo_PSE, 
             tons=135, kpre=33, kpost=23, 
             output_dir='', filename='',             
             debug=True, mark=True):
    global _eval_root
    timestamp = datetime.today().strftime('%Y%m%d-%H%M')
    # default output dir name
    if output_dir == '':
       output_dir = timestamp
    output_path = Path(_eval_root)/'evalFiloBass'/output_dir
    if not os.path.isdir(output_path):
        if not os.path.isdir(Path(_eval_root)/'evalFiloBass'):
            os.mkdir(Path(_eval_root)/'evalFiloBass')
        os.mkdir(output_path)
    else:
        print('WARNING: dir', output_path, 'exists')
    stat = ps.Stats()   
    dataset = FiloBassCorpus()
    names = sorted(list(dataset)) # list of index in dataset   
    print('\n', 'starting evaluation of FiloBass dataset -', len(names), 'entries\n')
    for name in names:
        if (name in skip):
            print('\n', name, 'SKIP\n')
            continue
        if (dataset.get(name) == None):
            print(name, "not found in dataset")
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
       filename =  'FRWeval'+'_'+str(tons)+'_'+timestamp
    stat.show()    
    df = stat.get_dataframe() # create pands dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(output_path/(filename+'.csv') , header=True, index=False)
    stat.write_datasum(output_path/(filename+'_sum.csv'))    
    
    
def eval_FiloBassitem(name, algo=ps.pse.Algo_PSE, 
                 tons=135, kpre=33, kpost=23, dflag=True, mflag=True):
    assert(len(name) > 0)
    dataset = FiloBassCorpus()
    if (dataset.get(name) == None):
        print(name, "not found in dataset")
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


def debug(name):    
    assert(len(name) > 0)
    dataset = FiloBassCorpus()
    if (dataset.get(name) == None):
        print(name, "not found in dataset")
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
    
#if __name__=="__main__":
#    eval_FiloBass(tons=135)

