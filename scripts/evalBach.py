# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

#
# DEPRECATED
# use evalASAP.py
#

import os
from pathlib import Path, PosixPath
from music21 import *
import PSeval as ps


s = corpus.parse('bach/bwv65.2.xml')
sp = s.getElementsByClass(stream.Part)
print(len(sp), 'parts')
soprano =sp[0]
# mes = soprano.getElementsByClass(stream.Measure)
#s.show('text')
#soprano.show()

# first bar
m0 = soprano.measure(0)
k0 = m0.getElementsByClass(key.Key)
print(len(k0), 'key in measure 0 of part soprano' )

# extraction notes & bar nbs
mes = soprano.getElementsByClass(stream.Measure)
print(len(mes), 'measures in part soprano' )
nl = []
b = 0
for m in mes:
    mn = m.getElementsByClass(note.Note)
    print('mesure', b, ':', len(mn), 'notes')
    for n in mn:
        p = (n, b)
        nl.append(p)
    b  += 1
print(len(nl), 'notes for enum')
print(nl)
print('accidental of first note: ', nl[0][0].pitch.accidental)
nl[0][0].pitch.accidental = pitch.Accidental('sharp')
print('changed accidental of first note to: ', nl[0][0].pitch.accidental)
# has changed original
# soprano.show()

print('Flattening')
sopranof = soprano.flatten()
#sopranof.show('text')
k = sopranof.getElementsByClass(key.Key)
print(len(k), 'key in part soprano' )
print(k[0])
nn = sopranof.getElementsByClass(note.Note)
print(len(nn), 'notes in part soprano' )


def extract_notes(part):
    """given a part, compute a list of pairs made of a note of the part and its measure number"""
    # sequence of measures of the part
    mes = part.getElementsByClass(stream.Measure)
    nl = []
    b = 0
    for m in mes:
        mn = m.getElementsByClass(note.Note)
        for n in mn:
            p = (n, b)
            nl.append(p)
        b  += 1
    return nl


            
# TBR
def test1(i, j):
    bundle = m21.corpus.corpora.CoreCorpus().search('bach', 'composer')
    score = bundle[i].parse()
    lp = score.getElementsByClass(m21.stream.Part)
    part = lp[j]    
    print('Bach bundle', i, '/', len(bundle), end = ' ') 
    print(score.metadata.composer, score.metadata.title, end=' ')
    print('part', j, '/', len(lp), end = ' ') 
    if (spellable(part)):
        print('(spellable)', end = ' ')      
    else:
        print('unspellable')              
        return
    pseval_part(part, False)