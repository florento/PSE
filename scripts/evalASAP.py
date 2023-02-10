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
from operator import itemgetter, attrgetter

import music21 as m21
import PSeval as ps


# global variables
_dataset_root = '../../../Datasets/ASAP/'  # path for ASAP
_generic_score = 'xml_score.musicxml'      # default score file name


###########################################
##                                       ##
## automatic evaluation of whole dataset ##
##                                       ##
###########################################


def get_score(score_path):
    global _dataset_root
    global _generic_score
    complete_score_path = Path(_dataset_root) / score_path / _generic_score
    return m21.converter.parse(str(complete_score_path))
    
def get_parts(score):
    return score.getElementsByClass(m21.stream.Part)

def eval_asapscore(stat, i, file):
    s = m21.converter.parse(file)
    stat.eval_score(score=s, sid=i)


class Sonata:
    def __init__(self, nb, mvt, file):
        self.nb = nb
        self.mvt = mvt
        self.file = file
    def __repr__(self):
        return repr((self.nb, self.mvt, self.file))

def Beethoven_list():
    """list of Beethoven Sonata in ASAP, 
       in the form of triplets (sonata nb, mvt  nb, path)"""
    global _dataset_root
    global _generic_score
    dir_re = re.compile("(\d+)-(\d)_?\d?$")
    bl = []
    p = Path(_dataset_root)/'Beethoven'/'Piano_Sonatas'
    assert(os.path.isdir(p))
    for d in os.listdir(p):
        dm = dir_re.match(d)
        if dm == None:
            continue
        po = p / d / _generic_score
        assert(os.path.isfile(po))
        bl.append(Sonata(int(dm.group(1)), int(dm.group(2)), str(po)))
    bl = sorted(bl, key=attrgetter('nb', 'mvt')) 
    return bl

    
def eval_Beethoven(stat):
    for s in Beethoven_list():
        print(s.nb, s.mvt, ':', s.file)
        eval_asapscore(stat, s.nb*10 + s.mvt, s.file)

stat = ps.PSStats() 
stat.debug = True

#beethoven1_1 = get_score('Beethoven/Piano_Sonatas/1-1')
#b = m21.converter.parse('xml_score.musicxml')
#lp = get_parts(b)
#lm0 = lp[0].getElementsByClass(m21.stream.Measure)
#lm1 = lp[1].getElementsByClass(m21.stream.Measure)



#import pse

#stat.eval_score(b, 0)  # 0 : standard list of 26 tons
#stat.show()

# Waldstein chords in 2 first bars
def Waldstein():
    stat = ps.PSStats()
    root='../../../Datasets/ASAP/'
    bl = Beethoven_list(root)
    (i, file) = bl[4] # 21.1
    score = m21.converter.parse(file)
    lp = score.getElementsByClass(m21.stream.Part)
    k0 = ps.get_key(lp[1])
    ln0 = ps.extract_part(lp[0])
    ln1 = ps.extract_part(lp[1])
    for (n, b, s) in ln1[:61]:   
        a = 'sp.add('
        a += str(n.pitch.midi)
        a += ', '
        a += str(b)
        a += ', '
        a += 'true' if s else 'false'
        a += ');'
        print(a)        
        #print('sp.add(', n.pitch.midi, ',', b, ',', ('true' if s else 'false)'), ')')
    #sp = ps.Speller()
    #sp.debug(True)
    #ps.add_tons(0, sp)
    #sp.add_notes(ln1[:61], sp)
    #sp.respell()



#file = dataset[470]
#print(file)
#score = m21.converter.parse(file.as_posix())
#lp = score.getElementsByClass(m21.stream.Part)
#part = lp[0]
#ln = ps.extract_notes(part)
#sp = ps.Speller()
#ps.add_notes(ln, sp)
#sp.respell()
#
# for p in ln:
#    print('sp.add(', p[0].pitch.midi, ',', p[1],')')
    
