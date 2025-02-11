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
import PSeval as ps
import evalXML


########################
##                    ##
##  global variables  ##
##                    ##
########################

# path to FRB dataset
_dataset_root = '../../../Datasets/CharlieParkerOmnibook'

# root of evaluation dir
_eval_root = '../../PSeval'

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'


#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

def omnibook_corpus():
    """build a list of scores in a subdirectory of the Omninook"""
    global _dataset_root
    return evalXML.get_corpus(Path(_dataset_root)/'musicxml', True)  # flat corpus

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
def eval_Omnibook(output='', tablename='',            
                  kpre=0, kpost=0, tons=0, 
                  costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=False, 
                  global1=100, grid=ps.pse.Grid_Rank, 
                  costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=False,
                  dflag=True, mflag=True, csflag=0):
    """eval the whole corpus with given algo and parameters"""
    """output: dir where files will be written"""
    """tablename: filename of csv table"""
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
    """dflag: debug flag"""
    """mflag: mark flag"""
    """csflag: 0 if we do not spell the notes of chord symbols"""
    """        1 if we spell them"""   
    """        2 if we force their names in spelling"""   
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                    debug=dflag)
    global _eval_root
    global skip
    assert(csflag in [0, 1, 2])
    root = Path(_eval_root)/'evalOmnibook'
    if not os.path.isdir(root):
        os.mkdir(root)
    evalXML.eval_corpus(speller=sp, mflag=mflag, csflag=csflag,
                        dataset=omnibook_corpus(), skip=skip, 
                        eval_root=root, output_dir=output, tablename=tablename)
        
# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_Omnibookitem(name, output='',         
                      kpre=0, kpost=0, tons=0,          
                      costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True,       
                      global1=100, grid=ps.pse.Grid_Rank,
                      costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,      
                      dflag=True, mflag=False, csflag=0):   
    """eval one item of the corpus with given algo and parameters"""
    """name: filename of item (prefix) in the dataset"""
    """output: dir where files will be written"""
    """kpre: parameter specific to PS13"""
    """kpost: parameter specific to PS13"""
    """tons: nb of Tons in TonIndex (PSE)"""
    """costtype1: table1, cost type. if set, PSE is used, otherwise, PS13 is used"""
    """tonal1: table1, tonal/modal flag for initial state (PSE)"""
    """det1: table1, deterministic/exhaustive flag for transitions (PSE)"""
    """global1: percentage approx for intermediate list of global candidate"""
    """grid: name of algorithm for the computation of the grid"""
    """costtype2: table2, cost type. if unset, skip table2 (PSE)"""
    """tonal2: table2, tonal/modal flag for initial state (PSE)"""
    """det2: table2, deterministic/exhaustive flag for transitions (PSE)"""
    """dflag: debug flag"""
    """mflag: mark flag"""
    """csflag: 0 if we do not spell the notes of chord symbols"""
    """        1 if we spell them"""   
    """        2 if we force their names in spelling"""   
    assert(len(name) > 0)
    assert(csflag in [0, 1, 2])
    # initialize a speller
    sp = ps.Spellew(ps13_kpre=kpre, ps13_kpost=kpost, 
                    nbtons=tons,
                    t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1, 
                    global1=global1, grid=grid,
                    t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                    debug=dflag)
    #print('force global', -2, ps.pse.Mode.Major, flush=True)
    #sp.get_speller().force_global(-2, ps.pse.Mode.Major)
    #print('nb globals:', sp.get_speller().globals(), flush=True)
    print('spelling', name, flush=True)
    evalXML.eval_item(speller=sp, mflag=mflag, csflag=csflag,
                      dataset=omnibook_corpus(), name=name, 
                      output_dir=output)
        
# compute C++ add instructions for given score, for debugging with gdb
def debug(name):    
    assert(len(name) > 0)
    dataset = omnibook_corpus()
    evalXML.debug(dataset, name)
