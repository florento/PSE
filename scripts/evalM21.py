#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  7 10:30:14 2022

@author: jacquema
"""




#import sys
#import logging
#import os
from pathlib import Path, PosixPath
import music21 as m21
import PSeval as pse



def pseval_m21(bundle):
    nb = len(bundle)
    for i in range(nb):
        print('\n ********** Bundle', str, i, '/', nb, '**********') 
        score = bundle[i].parse()
        pse.pseval_score(score)
        input("Press Enter to continue...")


bundleBach = m21.corpus.corpora.CoreCorpus().search('Bach', 'composer')
        
        
        