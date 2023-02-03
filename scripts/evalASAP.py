#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan 27 16:50:52 2023

@author: jacquema
"""


#import sys
#import logging
import os
from pathlib import Path, PosixPath
import re

import music21 as m21
import PSeval as ps




###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################




def get_score(score_path):
    _dataset_root = '../../../Datasets/ASAP/'
    _generic_score = 'xml_score.musicxml'
    complete_score_path = Path(_dataset_root) / score_path / _generic_score
    return m21.converter.parse(str(complete_score_path))
    

def get_parts(score):
    return score.getElementsByClass(m21.stream.Part)


def eval_asapscore(stat, i, file):
    s = m21.converter.parse(file)
    stat.eval_score(score=s, sid=i)


beethoven1_1 = get_score('Beethoven/Piano_Sonatas/1-1')


def Beethoven_list(root):
    dir_re = re.compile("(\d+)-(\d)_?\d?$")
    bl = []
    p = Path(root)/'Beethoven'/'Piano_Sonatas'
    assert(os.path.isdir(p))
    for d in os.listdir(p):
        dm = dir_re.match(d)
        if dm == None:
            continue
        po = p / d / 'xml_score.musicxml'
        assert(os.path.isfile(po))
        bl.append((int(dm.group(1)) + 0.1 * int(dm.group(2)), str(po)))
    return bl

    
def eval_Beethoven(root, stat):
    for (i, file) in Beethoven_list(root):
        print(i, ':', file)
        eval_asapscore(stat, i, file)


b = m21.converter.parse('xml_score.musicxml')
lp = get_parts(b)
lm0 = lp[0].getElementsByClass(m21.stream.Measure)
lm1 = lp[1].getElementsByClass(m21.stream.Measure)

stat = ps.PSStats()
root='../../../Datasets/ASAP/'
bl = Beethoven_list(root)
(i, file) = bl[4] # 21.1
score = m21.converter.parse(file)
lp = score.getElementsByClass(m21.stream.Part)
k0 = ps.get_key(lp[1])


import pse
sp = pse.Speller()
sp.debug(True)
ps.add_tons(0, sp)
ln0 = ps.extract_notes(lp[0])
ln1 = ps.extract_notes(lp[1])
ps.add_notes(ln1[:61], sp)
#sp.respell()

#stat.eval_score(b, 0)  # 0 : standard list of 26 tons
#stat.show()

