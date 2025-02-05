#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  5 14:02:31 2024
@author: jacquema

Framework for the evaluation of a set of XML scores all in the same directory
"""

#import sys
#import logging
import os
from pathlib import Path, PosixPath
from datetime import datetime
import music21 as m21
import PSeval as ps


########################
##                    ##
##  global variables  ##
##                    ##
########################


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
def get_corpus(dataset_path):
    """build a dictionary of XML scores with their embedding directories"""
    # default score file name
    score_suffix = ['.musicxml', '.xml', '.mxml']
    assert isinstance(dataset_path, PosixPath)
    if not os.path.exists(dataset_path):
        print('Error: ', dataset_path, 'not found')
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
        if (os.path.splitext(file)[1] in score_suffix):
            # map score name to file path
            dataset[os.path.splitext(file)[0]] = filepath
    # sort the list alphabetically
    dataset = dict(sorted(dataset.items()))
    return dataset

        
#####################################
##                                 ##
## automatic evaluation of dataset ##
##                                 ##
#####################################

# PS13: kpre=33, kpost=23
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_corpus(speller, dataset, skip=[],
                eval_root='.', output_dir='', tablename='',
                mflag=True, csflag=False):
    """eval a whole corpus with a given speller"""
    """speller: instance of speller for the evaluation"""
    """dataset: a dictionary as produced by XML_corpus"""
    """skip: list of names in corpus to avoid during evaluation"""
    """eval_root: directory where the evaluation directory will lie"""
    """output_dir: name of directory where the evaluation files will be written"""
    """tablename: file name of evaluation table. will be written in the output_dir"""
    """mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
    """csflag: spell also the notes of the chord symbols"""
    # prepare the output dir
    algoname = speller.algoname()    
    timestamp = datetime.today().strftime('%Y%m%d-%H%M')
    output_path = Path(eval_root)
    if not os.path.exists(output_path):
        print('ERROR output dir: ', output_path, 'not found')
        return
    if output_dir == '':
       output_dir = algoname+'_'+timestamp
    output_path = output_path/output_dir
    if not os.path.isdir(output_path):
        os.mkdir(output_path)
    else:
        print('WARNING: output dir', output_path, 'exists')

    # input data and processing   
    stat = ps.Stats()   
    names = sorted(list(dataset)) # list of index in dataset   
    print('\n', 'starting evaluation of FRB dataset -', len(names), 'entries\n')
    i = 0 # score id
    for name in names:
        if (name in skip):
            print('\n', name, 'SKIP\n', flush=True)
            continue
        if (not dataset.get(name)):
            print('\n', name, "not found in dataset, skip")
            continue
        if mflag:
            output_path2 = output_path/name
            if not os.path.isdir(output_path2):  
                os.mkdir(output_path2)
        else:
            output_path2 = None
        file = dataset[name]
        print('\n', name, '\n')
        s = m21.converter.parse(file.as_posix())
        (ls, lld) = speller.eval_score(score=s, stats=stat, score_id=i, 
                                  title=name, composer='', 
                                  output_path=output_path2, 
                                  chord_sym=csflag, 
                                  reset_globals = True)
        i += 1
        #if mflag and not ps.empty_difflist(lld): # done in eval_score
        #    write_score(s, output_path2, name)
    # display and save evaluation table
    # default table file name
    if not tablename:
       tablename =  'eval_'+algoname+'_'+timestamp
    stat.show()    
    df = stat.get_dataframe() # create pandas dataframe
    df.pop('part') # del column part number (always 0)
    df.to_csv(output_path/(tablename+'.csv') , header=True, index=False)
        
# PS13: kpre=33, kpost=23
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_item(speller, dataset, name, output_dir='', 
              kpre=0, kpost=0, tons=0,          
              costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True,       
              global1=100, grid=ps.pse.Grid_Rank,   
              costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,      
              mflag=False, csflag=False):   
    """eval one item of the FRB corpus with given algo and parameters"""
    """speller: speller instance for the evaluation"""
    """dataset: a dictionary as produced by XML_corpus"""
    """name: filename of item (prefix) in the dataset"""
    """output_dir: where files will be written"""
    """mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
    """csflag: spell also the notes of the chord symbols"""
    # input data
    assert(len(name) > 0)
    if (dataset.get(name) == None):
        print(name, "not found in dataset")
        return
    file = dataset[name]
    score = m21.converter.parse(file.as_posix())
    stat = ps.Stats()   
    opath = Path(os.getcwd()) if output_dir=='' else Path(output_dir)
    # ground truth ks, estimated ks, nnb of nontes and list of diff notes
    #(k_gt, gt_est, nn, ld) = ps.eval_part(part=part, stat=stat, nbtons=tons, 
    #                                      debug=dflag, mark=mflag)
    (ls, lld) = speller.eval_score(score=score, stats=stat, score_id=0, 
                                   title=name, composer='', output_path=opath, 
                                   chord_sym = csflag, 
                                   reset_globals = False)    
    stat.show()   
    assert(len(lld) == 1) # always 1 unique part in LG dataset
    if mflag and len(lld[0]) > 0:
        score.show()
        write_score(score, opath, name)
        
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

def debug(dataset, name, part=0):    
    """compute the C++ add instructions for given part, for debugging with gdb"""
    """dataset: a dictionary as produced by XML_corpus"""
    """name: filename of item (prefix) in the dataset"""
    """part: number of part in the score file"""
    assert(len(name) > 0)
    if (dataset.get(name) == None):
        print(name, "not found in dataset")
        return
    file = dataset[name]
    score = m21.converter.parse(file)
    lp = score.getElementsByClass(m21.stream.Part)
    ln = ps.extract_part(lp[part]) 
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
