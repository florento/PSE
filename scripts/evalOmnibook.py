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
    return evalXML.get_corpus(Path(_dataset_root)/'musicxml')

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
def eval_Omnibook(output_dir='', tablename='',            
                  kpre=0, kpost=0, tons=0, 
                  costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True, 
                  global1=100, grid=ps.pse.Grid_Rank, 
                  costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,
                  dflag=True, mflag=True, csflag=False):
    """eval the whole FRB corpus with given algo and parameters"""
    """output_dir: where files will be written"""
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
    """csflag: spell also chord symbols"""
    global _eval_root
    global skip
    root = Path(_eval_root)/'evalOmnibook'
    if not os.path.isdir(root):
        os.mkdir(root)
    evalXML.eval_corpus(dataset=omnibook_corpus(), skip=skip, 
                        eval_root=root, output_dir=output_dir, tablename=tablename,
                        kpre=kpre, kpost=kpost, tons=tons, 
                        costtype1=costtype1, tonal1=tonal1, det1=det1, 
                        global1=global1, grid=grid, 
                        costtype2=costtype2, tonal2=tonal2, det2=det2, 
                        dflag=dflag, mflag=mflag, csflag=csflag)
        
# PS13: kpre=33, kpost=23
# PSE: tons = 30 | 135 | 165, 
# PSE: costtype1, costtype2 = CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE: grid = Grid_Best | Grid_Rank | Grid_Exhaustive
# PSE: global1 = 0..100 (%)
def eval_Omnibookitem(name, output_dir='',         
                 kpre=0, kpost=0, tons=0,          
                 costtype1=ps.pse.CTYPE_UNDEF, tonal1=True, det1=True,       
                 global1=100, grid=ps.pse.Grid_Rank,
                 costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,      
                 dflag=True, mflag=False, csflag=False):   
    """eval one item of the FRB corpus with given algo and parameters"""
    """name: filename of item (prefix) in the dataset"""
    """output_dir: where files will be written"""
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
    """csflag: spell also chord symbols"""
    assert(len(name) > 0)
    evalXML.eval_item(dataset=omnibook_corpus(), name=name, 
                      output_dir=output_dir,
                      kpre=kpre, kpost=kpost, tons=tons, 
                      costtype1=costtype1, tonal1=tonal1, det1=det1, 
                      global1=global1, grid=grid, 
                      costtype2=costtype2, tonal2=tonal2, det2=det2, 
                      dflag=dflag, mflag=mflag, csflag=csflag)
        
# compute C++ add instructions for given score, for debugging with gdb
def debug(name, corpus='leads'):    
    assert(len(name) > 0)
    dataset = omnibook_corpus()
    evalXML.debug(dataset, name)
