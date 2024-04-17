#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 23 13:18:18 2022

@author: jacquema
"""

#import sys
import os
import time
from pathlib import Path  #PosixPath
from datetime import datetime
import collections
#from dataclasses import dataclass
import numpy as np
import pandas as pd
import music21 as m21
import subprocess # run command line
import pse

# import module with full path
# see https://www.geeksforgeeks.org/how-to-import-a-python-module-given-the-full-path/

#sys.path.insert(0, "/Users/jacquema/Code/PSE")
#from pse import *
#import pse

#import importlib.util
#spec = importlib.util.spec_from_file_location("pse","/Users/jacquema/Code/qparse/PSE/pse.cpython-310-darwin.so")
#pse = importlib.util.module_from_spec(spec)
#spec.loader.exec_module(pse)

print(pse.excuseme)


###############
##           ##
## M21 to PS ##
##           ##
###############


# a barred note is a triplet made of 
# - a note and 
# - the number of the  bar it belongs to
# - a flag saying whether the onset of the note is the same as the onset of the next note

# Key object is the more expressive (tonic, mode...)
# KeySignature object is just the number of sharps (> 0) or flats, < 0)
def key_changes(part):
    """return the number of key signature change in the given music21 part"""
    fpart = part.flatten()
    kl = fpart.getElementsByClass([m21.key.Key, m21.key.KeySignature])
    return (len(kl) - 1)

def get_key(part):
    """return the key signature of a music21 part, if unique, otherwise None"""
    #kl = part.flatten().getElementsByClass(m21.key.Key)
    kl = part.flatten().getElementsByClass([m21.key.Key, m21.key.KeySignature])
    if (len(kl) == 1):
        return kl[0]
    else:
        return None

def count_notes(part):
    """return the number of notes in a music21 part"""    
    fpart = part.flatten()
    nn = len(fpart.getElementsByClass(m21.note.Note))
    for c in fpart.getElementsByClass(m21.chord.Chord):
        nn += len(c)
    return nn    

def count_chords(part):
    """return the number of chords in a music21 part"""    
    cl = part.getElementsByClass(m21.chord.Chord)
    return len(cl) 

def count_measures(part):
    """return the number of measures in a music21 part"""    
    ml = part.getElementsByClass(m21.stream.Measure)
    return len(ml) 

def extract_notes(part):
    """extract the list of barred notes occurring in a music21 part"""    
    mes = part.getElementsByClass(m21.stream.Measure)
    b = 0
    ln = []
    #max_notes = 0
    #max_bar = 0
    for m in mes:
        #nn = 0
        for e in m.flatten():                      # merge voices in measure
            if isinstance(e, m21.note.Note):
                ln.append((e, b))                  # append a pair          
                #nn = nn+1
            elif isinstance(e, m21.chord.Chord):
                for n in e:
                    assert(isinstance(n, m21.note.Note))
                    ln.append((n, b))
                    #nn = nn+1
        #if (nn > max_notes):
            #max_notes = nn
            #max_bar = b
        b  += 1
    #print('max notes per bar = ', max_notes, 'in bar: ', max_bar)
    return ln

def insert_note(note, date, next_note, next_date, bar, note_list):
    """insert note as a barred note at the end of note_list"""
    if (note == None):
        return
    else:
        assert(isinstance(note, m21.note.Note))

    if (next_note == None):   
        note_list.append((note, bar, False))  # add a triplet
    else:        
        assert(isinstance(next_note, m21.note.Note))
        # grace notes are spelled separatly
        if note.duration.isGrace:
            note_list.append((note, bar, False)) 
        # other simultaneous notes (notes in chords) are spelled simultaneously
        else:            
            note_list.append((note, bar, (date == next_date))) #(note.offset == next_note.offset)))

def extract_measure(m, b):
    """extract the list of barred notes occurring in a music21 part"""    
    assert(isinstance(m, m21.stream.Measure))
    ln = []
    prev_n = None        
    prev_offset = 0              # onset of prev note in measure       
    for e in m.flatten():      # merge voices in measure
        if isinstance(e, m21.note.Note):
            insert_note(prev_n, prev_offset, e, e.offset, b, ln)
            prev_n = e
            prev_offset = e.offset
        elif isinstance(e, m21.chord.Chord):
            for cn in e:
                assert(isinstance(cn, m21.note.Note))
                insert_note(prev_n, prev_offset, cn, e.offset, b, ln)
                prev_n = cn
                prev_offset = e.offset
    insert_note(prev_n, prev_offset, None, 0, b, ln)      # insert last note
    return ln
    
def extract_part(part):
    """extract the list of barred notes occurring in a music21 part"""    
    mes = part.getElementsByClass(m21.stream.Measure)
    ln = []
    b = 0
    for m in mes:
        ln += extract_measure(m, b)
        b  += 1
    return ln

def extract_onlynotes(part):
    """extract the list of barred notes occurring in a music21 part"""    
    mes = part.getElementsByClass(m21.stream.Measure) # music21.stream.Measure
    b = 0
    ln = []
    for m in mes:
        mn = m.getElementsByClass(m21.note.Note) # music21.note.Note
        for n in mn:
            p = (n, b)
            ln.append(p)
        b  += 1
    return ln
    
def add_notes(ln, sp):
    """feed a speller with a list of (MIDI) notes with bar number"""
    i = 0    
    for (n, b) in ln:
        #print('add', i, ':', n.pitch.midi, b, flush=True)
        sp.add(midi=n.pitch.midi, bar=b)
        i = i+1
        
def add_ton(ks, mode, f_global, sp):
    """add predefined group of tonalities to a speller"""
    print('add_ton', ks, mode, ("(global)" if f_global else "(local)"))
    if (ks < -1 or ks > 7):
        print("unsupported key signature", ks)
        return
    sp.add_ton(ks, mode, f_global)


def add_tons(tons, sp):
    """add predefined group of tonalities to a speller"""
    print('add_tons', tons, flush=True)
    # default tons of module (30)
    if tons == 0: 
        return                            
    # major, minor, KS in [-5 .. 6]
    elif tons == 24:    
        for k in range(-5, 7):            
            sp.add_ton(k, pse.Mode.Major, True) # can be global
            sp.add_ton(k, pse.Mode.Minor, True) 
        sp.close_tons(True)   
    # Bach DWK
    elif tons == 25:    
        # maj key signature in [-4 .. 7]       
        # C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B          
        for k in range(-4, 8):            
            sp.add_ton(k, pse.Mode.Major, True) # can be global
        # min key signature in [-6 .. 6]
        # C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
        for k in range(-6, 7):           
            sp.add_ton(k, pse.Mode.Minor, True) 
        sp.close_tons(True)    
    # major, minor, KS in [-6 .. 6]
    elif tons == 26:
        for k in range(-6, 7):            
            sp.add_ton(k, pse.Mode.Major, True)
            sp.add_ton(k, pse.Mode.Minor, True)
        sp.close_tons(True)    
    # major, minor, KS in [-7 .. 7]
    elif tons == 30:
        for k in range(-7, 8):     
            sp.add_ton(k, pse.Mode.Major, True)
            sp.add_ton(k, pse.Mode.Minor, False)
        sp.close_tons(True)    
    # major, minor, jazz antic modes, KS in [-6 .. 6] 
    elif tons == 104:
        for k in range(-6, 7):            
            sp.add_ton(k, pse.Mode.Ionian,     True)
            sp.add_ton(k, pse.Mode.Dorian,     False)
            sp.add_ton(k, pse.Mode.Phrygian,   False)
            sp.add_ton(k, pse.Mode.Lydian,     False)
            sp.add_ton(k, pse.Mode.Mixolydian, False)
            sp.add_ton(k, pse.Mode.Aeolian,    False)
            sp.add_ton(k, pse.Mode.Locrian,    False)
            sp.add_ton(k, pse.Mode.Minor,      True)
        sp.close_tons(False)    
    # major, minor, jazz antic modes, KS in [-7 .. 7] 
    elif tons == 135:
        for k in range(-7, 8):      
            sp.add_ton(k, pse.Mode.Major,      True)
            sp.add_ton(k, pse.Mode.Minor,      False)
            sp.add_ton(k, pse.Mode.MinorMel,   False)
            sp.add_ton(k, pse.Mode.Dorian,     False)
            sp.add_ton(k, pse.Mode.Phrygian,   False)
            sp.add_ton(k, pse.Mode.Lydian,     False)
            sp.add_ton(k, pse.Mode.Mixolydian, False)
            sp.add_ton(k, pse.Mode.Aeolian,    False)
            sp.add_ton(k, pse.Mode.Locrian,    False)
        sp.close_tons(False)    
    else:
        print('ERROR: unsupported number of tons', tons, flush=True)
        sp.close_tons()    
    
        
###############
##           ##
## PS to M21 ##
##           ##
###############


def get_pitches(sp):
    """extract the list of music21 pitches from a speller"""    
    pl = []
    for i in range(sp.size()):
        p = m21_pitch(name = sp.name(i),  
                     accid = sp.accidental(i),
                     octave =sp.octave(i))
        pl.append(p)
    return pl

def m21_pitch(name, accid, octave):
    """construct a music21.pitch from a PSE NoteName, a PSE Accidental ad an octave number"""    
    return  m21.pitch.Pitch(step=m21_step(name), 
                            accidental=m21_accid(accid), 
                            octave=octave)

def m21_step(nn):
    """cast a PSE NoteName into a music21 character"""
    if (nn == pse.NoteName.A):
        return 'A'
    elif (nn == pse.NoteName.B):
        return 'B'
    elif (nn == pse.NoteName.C):
        return 'C'
    elif (nn == pse.NoteName.D):
        return 'D'
    elif (nn == pse.NoteName.E):
        return 'E'
    elif (nn == pse.NoteName.F):
        return 'F'
    elif (nn == pse.NoteName.G):
        return 'G'
    else:
        print('Invalid argument')
        return 'X'

def m21_accid(a):
    """cast a PSE Accidental into a music21.pitch.Accidental"""
    if (a == pse.Accid.TripleSharp):
        return m21.pitch.Accidental('triple-sharp')
    elif (a == pse.Accid.DoubleSharp):
        return m21.pitch.Accidental('double-sharp')
    elif (a == pse.Accid.ThreeQuartersSharp):
        return m21.pitch.Accidental('one-and-a-half-sharp')
    elif (a == pse.Accid.Sharp):
        return m21.pitch.Accidental('sharp')
    elif (a == pse.Accid.QuarterSharp):
        return m21.pitch.Accidental('half-sharp')
    elif (a == pse.Accid.Natural):
        return m21.pitch.Accidental('natural')
    elif (a == pse.Accid.QuarterFlat):
        return m21.pitch.Accidental('half-flat')
    elif (a == pse.Accid.Flat):
        return m21.pitch.Accidental('flat')
    elif (a == pse.Accid.ThreeQuartersFlat):
        return m21.pitch.Accidental('one-and-a-half-flat')
    elif (a == pse.Accid.DoubleFlat):
        return m21.pitch.Accidental('double-flat')
    elif (a == pse.Accid.TripleFlat):
        return m21.pitch.Accidental('triple-flat')
    else: 
        print('Invalid argument')

def m21_mode(m):
    """cast a PSE Ton::Mode into a music21 mode name"""
    if (m == pse.Mode.Major):
        return 'major'
    elif (m == pse.Mode.Minor):
        return 'minor'
    elif (m == pse.Mode.MinorNat):
        return 'minor'   # not found
    elif (m == pse.Mode.MinorMel):
        return 'minor'   # not found
    elif (m == pse.Mode.Ionian):
        return 'ionian'
    elif (m == pse.Mode.Dorian):
        return 'dorian'
    elif (m == pse.Mode.Phrygian):
        return 'phrygian'
    elif (m == pse.Mode.Lydian):
        return 'lydian'
    elif (m == pse.Mode.Mixolydian):
        return 'mixolydian'
    elif (m == pse.Mode.Aeolian):
        return 'aeolian'
    elif (m == pse.Mode.Locrian):
        return 'locrian'
    else: 
        print('Invalid argument')
        
def m21_key(ton):
    """cast a PSE Ton into a music21.key.Key"""
#    p = m21.pitch.Pitch(name = m21_step(ton.name()), 
#                        accidental = m21_accid(ton.accidental()))
    return m21.key.KeySignature(ton.fifths()).asKey(m21_mode(ton.mode()))

def ctype_tostring(ct):
    """cast a PSE Cost Type to a string"""
    if (ct == pse.CTYPE_ACCID):
        return 'A'
    elif (ct == pse.CTYPE_ACCIDlead):
        return 'Ad'
    elif (ct == pse.CTYPE_ADplus):
        return 'AD+'   # not found
    elif (ct == pse.CTYPE_ADlex):
        return 'ADlex'   # not found
    elif (ct == pse.CTYPE_UNDEF):
        return 'UNDEF'
    else: 
        print('Invalid Cost Type')



####################
##                ##
## diff PS / M21  ##
##                ##
####################


def compare_name(n, pse_name):
    """compare the name of a music21 note and a PSE note name"""    
    return (n.step == m21_step(pse_name))

def compare_accid(n, pse_accid, print_flag):
    """compare the accidental of a music21 note and a PSE accidental"""    
    if (pse_accid == pse.Accid.Natural):
        # (print_flag == True): mandatory (print_flag == False) courtesy 
        if (n.pitch.accidental == m21.pitch.Accidental('natural')):
            return True  
        elif (n.pitch.accidental == None):
            return True  # (print_flag == False)
        else:
            return False
    else:
        return (n.pitch.accidental == m21_accid(pse_accid))

# k can be a Key or a KeySignature
def compare_key(k, ton):
    """compare a music21 key PSE ton"""  
    if ton.undef():
        return False
    elif isinstance(k, m21.key.Key):
        return (k.sharps == ton.fifths()) # and k.mode == m21_mode(ton.mode()))
    elif isinstance(k, m21.key.KeySignature):
        return (k.sharps == ton.fifths())
    else:
        print('ERROR"', k, 'of unexpected type')
        return False
    
def compare_key_pitches(k0, ton):
    if ton.undef():
        return False
    elif isinstance(k0, m21.key.Key):
        ton=m21_key(ton)
        return (k0.tonic.ps==ton.tonic.ps and k0.mode==ton.mode) or (k0.relative.tonic.ps==ton.tonic.ps and k0.relative.mode==ton.mode)
    elif isinstance(k0, m21.key.KeySignature):
        ton=m21_key(ton)
        kM=k0.asKey(mode='major')
        km=k0.asKey(mode='minor')
        return (kM.tonic.ps==ton.tonic.ps and kM.mode==ton.mode) or (km.tonic.ps==ton.tonic.ps and km.mode==ton.mode)
    else:
        print('ERROR"', k0, 'of unexpected type')
        return False

def diff(ln, sp):
    """compare a list of barred notes and the list of notes of a speller"""    
    assert(len(ln) == sp.size())
    if (len(ln) == 0):
        return []
    i = 0
    ld = []
    for (n, m, simult) in ln:
        if (compare_name(n, sp.name(i)) and
            compare_accid(n, sp.accidental(i), sp.printed(i)) and 
            n.octave == sp.octave(i)):
            i = i+1
        elif n.tie != None and n.tie.type != 'start' :
            i = i+1
        else:
            d = (i, sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            i = i+1
            ld = ld+[d]
    return ld

def diffrec(ln, sp, i, ld):
    if (len(ln) == 0):
        return ld
    else:
        (n, m, simult) = ln[0]
        if (compare_name(n, sp.name(i)) and
            compare_accid(n, sp.accidental(i), sp.printed(i)) and 
            n.octave == sp.octave(i)):
            return diffrec(ln[1:], sp, i+1, ld)
        else:
            #print('diff: ', i, ':', n, 
            #      sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            d = (i, sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            return diffrec(ln[1:], sp, i+1, ld+[d])

def diff_notes(ln, lp):
    """compare a list of barred notes and a list of pitches"""    
    if (len(ln) == len(lp)):
        return diff_notes1(ln, lp, 0, [])        
    else:
        print('ERROR (diff_notes)')
        return []

def diff_notes1(ln, lp, i, ld):
    if (len(ln) == 0):
        return ld
    else:
        n = ln[0][0]  # first element of pair (n, m)
        if (n.pitch  == lp[0]):
            return diff_notes1(ln[1:], lp[1:], i+1, ld)
        else:
            return diff_notes1(ln[1:], lp[1:], i+1, ld+[i])

def colorizeo(ln, i, color):
    assert(i < len(ln))
    ln[i][0].style.color = color

def colorize(ln, i, n, a, o, p, color):
    assert(i < len(ln))
    ln[i][0].style.color = color
    ln[i][0].pitch.step = m21_step(n)
    if ((a != pse.Accid.Natural) or (p == True)):
        ln[i][0].pitch.accidental = m21_accid(a)
    ln[i][0].pitch.octave = o                

def anote_diff(ln, ld, color):
    """mark mispells with color in a note list, based on a diff-list"""
    if (len(ld) > 0):
        for (i, n, a, o, p) in ld:
            colorize(ln, i, n, a, o, p, color)

def anote_rediff(ln, ld0, ld1):
    """mark mispells with colors in a note list, based on 2 diff-list"""
    if (len(ld0) == 0) and (len(ld1) == 0):
        return
    elif (len(ld0) == 0):          # mispell created by rewritting
        assert(len(ld1) > 0)
        (i, n, a, o, p) = ld1[0]
        colorize(ln, i, n, a, o, p, 'violet')
        anote_rediff(ln, ld0, ld1[1:])
    elif (len(ld1) == 0):          # mispell fixed by rewritting
        assert(len(ld0) > 0)
        (i, n, a, o, p) = ld0[0]        
        colorizeo(ln, i, 'green')
        anote_rediff(ln, ld0[1:], ld1)
    else:
        assert(len(ld0) > 0)
        assert(len(ld1) > 0)
        (i0, n0, a0, o0, p0) = ld0[0]        
        (i1, n1, a1, o1, p1) = ld1[0]   
                                   # mispell not rewritten
        if (i0 == i1) and (n0 == n1) and (a0 == a1) and (o0 == o1) and (p0 == p1):
            colorize(ln, i0, n0, a0, o0, p0, 'red')
            anote_rediff(ln, ld0[1:], ld1[1:])        
        elif (i0 == i1):           # mispell rewritten but not fixed
            colorize(ln, i1, n1, a1, o1, p1, 'orange')
            anote_rediff(ln, ld0[1:], ld1[1:])
        elif (i0 < i1):            # mispell fixed by rewritting
            colorizeo(ln, i0, 'green')
            anote_rediff(ln, ld0[1:], ld1)
        else:                      # mispell created by rewritting
            assert(i0 > i1)
            colorize(ln, i1, n1, a1, o1, p1, 'violet')
            anote_rediff(ln, ld0, ld1[1:])

def strk(k):
    """string of m21.key"""
    if isinstance(k, m21.key.Key):
        return str(k)+' ('+str(k.sharps)+')'
    elif isinstance(k, m21.key.KeySignature):
        return '('+str(k.sharps)+')'
    else:
        return str(m21_key(k))


def anote_global_part(part, sp, ton_est):
    k=ton_est
    #k = m21_key(sp.global_ton(0))
    if ton_est != None:
        text = 'est. global: '+strk(k)
    nc = sp.globals()
    if nc > 1:
        text += ' ' 
        text += '(' 
        text += str(nc) 
        text += ' candidates: '         
        for i in range(nc):
            text += strk(m21_key(sp.global_ton(i)))
            text += ' '
        text += ')'            
    e = m21.expressions.TextExpression(text)    
    e.style.fontWeight = 'bold'
    e.placement = 'above'
    e.style.color = 'gray'   
    ml = part.getElementsByClass(m21.stream.Measure)    
    ml[0].insert(0, e)

def anote_local_part(part, sp, i):
    # i = sp.iglobal_ton0(goodgtindex)
    ml = part.getElementsByClass(m21.stream.Measure)    
    for j in range(len(ml)):
        k = m21_key(sp.local_bar(i, j))
        e = m21.expressions.TextExpression(strk(k))
        e.style.fontStyle = 'italic'
        ml[j].insert(0, e)
        
def anote_part(part, ld):
    """mark mispells in red in a part, based on a diff-list"""
    fpart = part.flatten()
    ln = fpart.getElementsByClass(m21.note.Note) 
    anote_diff(ln, ld, 'red')    

def anote_score(score, k, lld):
    """print the given estimated key sign and mark mispells in score"""
    #score.show()
    tb = m21.text.TextBox('estimated key signature = '+str(k), 30, 30)
    score.append(tb)
    lp = score.getElementsByClass(m21.stream.Part)
    assert(len(lp) == len(lld))
    for i in range(0, len(lp)):
        anote_part(lp[i], lld[i])    

def print_score(score, outfile):
    musescore = "/MuseScore 3.app/Contents/MacOS/mscore"
    pdffile = outfile+".pdf"
    mxfile  = outfile+".musicxml"
    os.system(musescore + " -o " + pdffile + " " + mxfile)


#########################
##                     ##
##      feedback       ##
##                     ##
#########################
               
             
# struct for counting evaluation errors 
class Stats:
    _UNDEF_KS = 99

    def __init__(self):        
        # list of lists (one list per part)
        self._table = []
        #
        # counters for all parts sppled so far
        #
        #  number of parts proceeded
        self._global_parts = 0
        # total number of notes proceeded
        self._global_notes = 0
        # total number of wrong note names
        self._global_nerr = 0
        # total number of wrong note names in correct key sig
        self._global_nerr_ks = 0
        # total number of wrong ks
        self._global_kserr = 0        
        #
        # info and counters for current score and part
        #
        # unique id of the score currently proceeded
        self._current_id = 0
        # title of the score currently proceeded
        self._current_title = None
        # composer of the score currently proceeded
        self._current_composer = None
        # start time for spelling of the part currently proceeded
        # self._current_t0 = time.time()
        # array of timers for storing processing time
        self._timer = {}
        
    ## record evaluation results

    def open_score(self, score_id=None, title='', composer=''):
        """register info on a new score being precessed"""
        if score_id == None:
            self._current_id += 1
        else:
            self._current_id = score_id
        assert(self._current_title is None)
        assert(self._current_composer is None)
        assert(title is not None)
        assert(composer is not None)
        self._current_title = title
        self._current_composer = composer
        
    def close_score(self):
        self._current_title = None
        self._current_composer = None
        
    def sort_timers(self):
        self._timer = collections.OrderedDict(sorted(self._timer.items()))

    def start_timer(self, i:int=1):
        """record starting time for timer i"""
        self._timer[i] = time.time()
             
    def stop_timer(self, i:int=1):
        """record processing time for timer i"""
        assert(self._timer.get(i) is not None)
        self._timer[i] = time.time() - self._timer[i]

    def get_timer(self, i=1):
        """recorded value for timer i"""
        if self._timer.get(i) is None:
            return 0
        else:
            return self._timer[i]*1000
                 
    def record_part(self, part_id, k_gt, ton_est, nb_notes, nb_err):
        """add a new row to the evaluation table"""
        self._global_parts += 1
        self._global_notes += nb_notes
        self._global_nerr += nb_err
        if compare_key(k_gt, ton_est):
            self._global_nerr_ks += nb_err
        else:
            self._global_kserr += 1
        row = []
        row.append(self._current_id)
        assert(self._current_title is not None)
        assert(self._current_composer is not None)
        row.append(self._current_title)
        row.append(self._current_composer)
        row.append(part_id)                # current part id        
        if compare_key(k_gt, ton_est):
            row.append('')
        else:
            row.append(strk(k_gt))         # current key gt
        if ton_est.undef():
            row.append('NaN')
        else:
            row.append(strk(m21_key(ton_est))) # current key estimation
        row.append(nb_notes)               # nb notes in part 
        row.append(nb_err)
        # append ordered timer values
        self.sort_timers()
        for i in self._timer:
            row.append(self._timer[i])
        # add row to table
        self._table.append(row)

    ## getters 
    def get_nbparts(self):
        """return the nb of parts evaluated"""
        return self._global_parts
        
    def get_nbnotes(self):
        """return the nb of notes evaluated"""
        return self._global_notes

    def get_nberrors(self):
        """return the total nb of spell errors in evaluations"""
        return self._global_nerr

    def get_nbkserrors(self):
        """return the total nb of KS estimation errors"""
        return self._global_kserr

    def get_nberrorsks(self):
        """return the total nb of spell errors in evaluations with correct KS"""
        return self._global_nerr_ks

    def percent_spell(self):
        return (self._global_notes - self._global_nerr) * 100 / self._global_notes

#    def percent_spell_correctKS(self):
#        return (self._global_notes - self._global_nerr) * 100 / self._global_notes

    def percent_ks(self):
        return (self._global_parts - self._global_kserr) * 100 / self._global_parts

    def show(self):
        """display statistics"""
        print('parts spelled   :', self._global_parts)            
        print('notes spelled   :', self._global_notes)            
        print('correct spelling:', "{:.2f}".format(self.percent_spell()), '%')            
        print('correct KS estim:', "{:.2f}".format(self.percent_ks()), '%')            
             
    def get_table(self):
        """return the global evaluation table"""
        return self._table
    
    def get_dataframe(self):
        """return a panda dataframe of the evaluation"""
        df = pd.DataFrame(self._table)
        timers = [] # list of timer's names
        self.sort_timers()
        for i in self._timer:
            timers.append('time_'+str(i))            
        df.columns = ['id', 'title','composer', 'part', 'KSgt', 'KSest', 
                      'notes', 'err']+timers
        for t in timers:
            df[t] = df[t].map('{:,.3f}'.format)
        # every KSestimated identical to corresp. KSgt becomes NaN
        #df.loc[df['KSgt'] == df['KSest'], 'KSest'] = np.nan
        df.loc[df['KSgt'] == '', 'KSgt'] = np.nan
        # d = df['KSgt'].compare(df['KSest'], keep_shape=True)['other'].fillna('')        
        return df

    def write_dataframe(self, file):
        """write the evaluation table into a cvs file, after some formatting"""
        df = self.get_dataframe()
        df.fillna('').to_csv(file, header=True, index=False)
    
    # useless, there are accessors for that
    def get_datasum(self):
        """return a summary of the evaluation as a dictionary"""
        df = self.get_dataframe()
        #df.loc[df['KSgt'] == df['KSest'], 'KSest'] = np.nan
        # sums and errors
        total = { 'nb_KS'    : [len(df['KSest'])],
                  'err_KS'   : [df['KSgt'].count()], 
                  'nb_note'  : [df['notes'].sum()], 
                  'err_note' : [df['err'].sum()] }
        #df = df.append(pd.DataFrame.from_dict(total), ignore_index=True)
        #df = df.astype({"KSgt": int, "notes": int, "err": int})        
        #df = df.convert_dtypes()
        #df = df.fillna('')
        percent = { 'KS_rate'  : [ pcformat((total['err_KS'][0] * 100) / total['nb_KS'][0]) ], 
                    'err_rate' : [ pcformat(total['err_note'][0] * 100 / total['nb_note'][0]) ] }
        #df = df.append(pd.DataFrame.from_dict(percent), ignore_index=True)
        # replace NaN by empty string
        #df = df.fillna('')
        # reset type of column 'KSgt' to Integer
        #df['KSest'] = df['KSest'].astype('Int64')                
        df = pd.DataFrame(total | percent) # flat dataframe (1 raw) from concatenation of 2 dictionaries       
        return df

    def write_datasum(self, file):
        """write a summary of the evaluation into a cvs file (1 line)"""
        df = self.get_datasum()
        df.to_csv(file, header=True, index=False)
    
def pcformat(x):    
    if abs(x - int(x)) < 1e-6:
        return int(x)
    else:
        return round(x,2)
            
#def ks_errors(df):
#    return len(df['KSgt'].compare(df['KSest'], keep_shape=False))

def sp_errors(df):
    return df['err'].sum() / df['notes'].sum()


# count errors in KS estimation

# count spelling errors
# df2 = df.append(df[['notes','err']].sum(),ignore_index=True).fillna('')


#######################################
##                                   ##
##  speller: wrapper for evaluation  ##
##                                   ##
#######################################


def spellable(part):
    """the given part can be pitch spelled"""
    c = key_changes(part)
    if (c > 0):
        print(c, 'key changes', end =' ')
        return False
    #elif (count_chords(part)):
    #    print('chords', end =' ')
    #    return False        
    return True

def algo(ps13_kpre=0, ps13_kpost=0, # parameters specific to PS13
         nbtons=0,                  # nb of Tons in TonIndex
         t1_costtype=pse.CTYPE_UNDEF, 
         t1_tonal=True, 
         t1_det=True, 
         t2_costtype=pse.CTYPE_UNDEF, 
         t2_tonal=True, 
         t2_det=True, 
         debug=False):
    """summary of algo name and parameters"""
    """"PS13 or PSE nbtons _tablenb costtype1 Tonal or Modal Deterministic of Exhaustive"""
    if ps13_kpre > 0:
        return 'PS13_'+str(ps13_kpre)+'_'+str(ps13_kpre)
    name = 'PSE'
    name += str(nbtons)
    assert(t1_costtype != pse.CTYPE_UNDEF)
    name += '_1'
    name += ctype_tostring(t1_costtype)
    name += 'T' if t1_tonal  else 'M'
    name += 'D' if t1_det  else 'E'            
    if t2_costtype != pse.CTYPE_UNDEF:
        name += '_2'
        name += ctype_tostring(t2_costtype)
        name += 'T' if t2_tonal  else 'M'
        name += 'D' if t2_det  else 'E'            
    return name

# spelling environment: wrapper for a C++ speller (pse.PS13 or pse.Speller)
class Spellew:
    """environment for a spelling algorithm"""
    _UNDEF_KS = 99    
    def __init__(self, 
                 ps13_kpre=0, ps13_kpost=0, # parameters specific to PS13
                 nbtons=0,                  # nb of Tons in TonIndex
                 t1_costtype=pse.CTYPE_UNDEF, 
                 t1_tonal=True, 
                 t1_det=True, 
                 t2_costtype=pse.CTYPE_UNDEF, 
                 t2_tonal=True, 
                 t2_det=True, 
                 debug=False):      # mark flag ?    
        if (ps13_kpre > 0 and ps13_kpost > 0):
            self._algo_name = "PS13"
            self._algo_params = str(ps13_kpre) + '_' + str(ps13_kpre)
            self._speller = pse.PS13()
            self._speller.set_Kpre(ps13_kpre)
            self._speller.set_Kpost(ps13_kpost)
        else:
            assert(t1_costtype != pse.CTYPE_UNDEF)
            self._algo_name = "PSE"
            self._algo_params = str(nbtons)
            assert(t1_costtype != pse.CTYPE_UNDEF)
            self._algo_params += '_1'
            self._algo_params += ctype_tostring(t1_costtype)
            self._algo_params += 'T' if t1_tonal  else 'M'
            self._algo_params += 'D' if t1_det  else 'E'            
            if t2_costtype != pse.CTYPE_UNDEF:
                self._algo_params += '_2'
                self._algo_params += ctype_tostring(t2_costtype)
                self._algo_params += 'T' if t2_tonal  else 'M'
                self._algo_params += 'D' if t2_det  else 'E'  
            self._speller = pse.Speller()
        # debug flag of speller
        self._speller.debug(debug)
        # build the ton index for speller
        add_tons(nbtons, self._speller)         
        # parameters of spelling algo PSE
        self._ct1     = t1_costtype
        self._tonal1  = t1_tonal
        self._det1    = t1_det
        self._ct2     = t2_costtype
        self._tonal2  = t2_tonal
        self._det2    = t2_det
        # self._spelled = False

    def new_dir(self):
        """create unique dir name for recording the results of evaluation with this algorithm"""
        """"PS13 or PSE nbtons _tablenb costtype1 Tonal or Modal Deterministic of Exhaustive"""
        timestamp = datetime.today().strftime('%Y%m%d-%H%M')
        return self._algo_name + '_' + self._algo_params + '_' + timestamp
        
    def get_speller(self):
        return self._speller

    def spell(self, notes, stat):
        """run spell checking algo"""
        self._speller.reset()
        # feed speller with input notes
        for (n, b, s) in notes:   # note, bar number, simultaneous flag
            self._speller.add(midi=n.pitch.midi, bar=b, simultaneous=s)
        if self._algo_name == "PS13":
            stat.start_timer(1) # single timer
            self._speller.spell()
            stat.stop_timer(1)
        else:
            assert(self._algo_name == "PSE")
            assert(self._ct1 != pse.CTYPE_UNDEF)
            print('algo PSE compute Table 1', end=' ', flush=True)
            stat.start_timer(1)
            self._speller.eval_table(self._ct1, self._tonal1, self._det1)
            stat.stop_timer(1)
            print("{0:0.2f}".format(stat.get_timer(1)), 'ms', end='\n', flush=True)
            if self._ct2 != pse.CTYPE_UNDEF:
                self._speller.eval_global(5, False) # 5% tolerance
                print(self._speller.globals(), 'candidates global from 1st table', flush=True)
                print('algo PSE compute Grid', end=' ', flush=True)
                stat.start_timer(2)
                self._speller.eval_grid()
                stat.stop_timer(2)                
                print("{0:0.2f}".format(stat.get_timer(2)), 'ms', end='\n', flush=True)
                print('algo PSE compute Table 2', end=' ', flush=True)
                stat.start_timer(3)
                self._speller.reval_table(self._ct2, self._tonal2, self._det2)
                stat.stop_timer(3)
                print("{0:0.2f}".format(stat.get_timer(3)), 'ms', end='\n', flush=True)
        #self._spelled = True
    
    def diff(self, notes):
        """compute the diff list between original and spelled notes"""
        # assert(self._spelled)
        return diff(notes, self._speller) 
        
    def get_global(self, k0):
        """extract estimated global tonality from speller"""
        # assert(self._spelled)
        sp = self._speller
        goodgtindex=0
        sp.eval_global(0.01, False) # 5% tolerance
        nbg = sp.globals() 
        # no evaluation of global ton (ex. PS13)
        if nbg == 0: 
            print('pse get_global: no gt found')
            return (sp.global_ton(0), 0) # gt is undef 
        # unambigous evaluation of global ton
        elif nbg == 1: 
            assert(not sp.global_ton(0).undef())
            return (sp.global_ton(0), sp.iglobal_ton(0))
        # ambigous evaluation of global ton
        elif nbg > 1: 
            print('pse get_global:', nbg, ' candidates global', end=' ')
            print("real global tone :", k0)
            c = 0
            enharm = False
            present = False
            for i in range(nbg):
                gt = sp.global_ton(i)
                if compare_key(k0, gt):
                    goodgtindex = i
                    present = True
                elif compare_key_pitches(k0, gt):
                    enharm = True
                else:
                    c += 1
            if c > 0:
                enharm=False
            if present and enharm: 
                return (sp.global_ton(goodgtindex), sp.iglobal_ton(goodgtindex))
            else:
                return (sp.global_ton(0), sp.iglobal_ton(0))        
                
    def rename(self, i):
        # assert(self._spelled)
        assert(i < self._speller.nb_tons())
        self._speller.rename(i)

    def rewrite_passing(self):
        # assert(self._spelled)
        self._speller.rewrite_passing()
            
    def anote_local_part(self, part, i):
        # assert(self._spelled)
        if (self._speller.locals()):
            anote_local_part(part, self._speller, i)     
        
    def anote_global_part(self, part, gt):
        # assert(self._spelled)
        anote_global_part(part, self._speller, gt)              
        
    def eval_part(self, part, stats, mark=False):
        """evaluate spelling for one part in a score and mark errors"""        
        assert(stats is not None)
        #if stat == None:
        #    stat=Stats()
        # extract key and notes from part
        k0 = get_key(part)
        ln = extract_part(part)  # input note list
        assert(count_notes(part) == len(ln)) # print('ERROR',  count_notes(part), len(ln))
        print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
        # spell        
        print('spelling with', self._algo_name, self._algo_params, end='\n', flush=True)
        self.spell(ln, stats)   #print('spell finished', end='\n', flush=True)   
        # extract estimated global ton from speller
        (gt, i) = self.get_global(k0)
        # apply the spelling in the row of the estimated global
        self._speller.rename(i)
        # eval estimation of global key
        if not gt.undef:
            if compare_key(k0, gt):
                print('global ton: OK:', '(', m21_key(gt), 
                      '), has the same signature as', k0, end=' ')
            else:
                print('global ton: NO:', '(', m21_key(gt), 'was', k0, '),', end=' ')
                if mark:
                    self.anote_global_part(part, gt) 
        else:
            print('ERROR eval_part: gt undef')
        # compute diff list between reference score and respell
        ld0 = diff(ln, self._speller) 
        print('diff:', len(ld0), end='\n', flush=True)
        # rewrite the passing notes
        print('rewrite passing notes', end='\n', flush=True)
        self._speller.rewrite_passing()
        # compute diff list between reference score and rewritten
        ld1 = diff(ln, self._speller) 
        print('diff after rewriting:', len(ld1), end='\n', flush=True)
        # annotations
        if mark:
            anote_rediff(ln, ld0, ld1) # anote_diff(ln, ld0, 'red')
            if (self._speller.locals()):
                anote_local_part(part, self._speller, i)    
        return (k0, gt, len(ln), ld1)

    def eval_score(self, score, stats=Stats(),
                   score_id=0, title:str='', composer:str='', output_path=None):        
        """evaluate spelling for all parts in a score"""
        # DO IN CALLER
        if not title: 
            title=score.metadata.title
        if not title: 
            title='unknown title'
        if (not composer) and (score.metadata.composer is not None):
            composer=score.metadata.composer       
        mark = (output_path is not None)    
        lp = score.getElementsByClass(m21.stream.Part)
        nbparts = len(lp)
        ls = []
        lld = []
        stats.open_score(score_id=score_id, title=title, composer=composer)
        for i in range(nbparts):    
            print(title, composer, end=' ')
            part = lp[i]
            if (nbparts > 1):
                print('part', i+1, '/', len(lp), end=' ', flush=True)
            if (spellable(part)):
                (k_gt, ton_est, nn, ld) = self.eval_part(part, stats, mark)
                # add one row in stat table for each part
                stats.record_part(i, k_gt, ton_est, nn, len(ld))
                ls.append(ton_est)
                lld.append(ld)
            else:
                print('cannot spell, skip', flush=True)
        stats.close_score()
        if mark and not empty_difflist(lld):
            write_score(score, output_path, title)
        return (ls, lld)
        
    
###################################
##                               ##
##   evaluation and feedback     ##
##                               ##
###################################


def empty_difflist(lld):
    for ld in lld:
        if not ld:
            continue
        else:
            return False
    return True

def write_score(score, output_path, name):
    # create any missing directories along the path
    if not os.path.isdir(output_path):
        output_path.mkdir(parents=True, exist_ok=True) # Python ≥ 3.5
    else:
        print('WARNING: output dir', output_path, 'exists')
        return
    #    if not os.path.isdir(output_path):
    #        os.mkdir(output_path)
    xmlfile = output_path/(name+'.musicxml')
    # M21 export
    score.write('musicxml', fp=xmlfile) 
    # pdffile = dirname+'/'+outname+'.pdf'
    # os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)

               


##################################
##                              ##
##  pdf export with MuseScore   ##
##                               ##
##################################


# path to MuseScore command line
_musescore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'

def is_musicxml(p, filename):
    ''' the given filename in directory of path p is a musicxml'''
    pf = pf = Path(p, filename)
    return os.path.isfile(pf) and pf.suffix in ['.xml', '.mxml', '.musicxml']    

def mk_pdf(p, filename):
    ''' make a pdf from the given musicxml filename, in directory of path p'''
    if not is_musicxml(p, filename):
        print('ERROR mk_pdf: ', filename, 'not a musicxml file')
        return
    xml = Path(p, filename)
    stem = os.path.splitext(filename)[0]
    pdf = Path(p, stem+'.pdf')
    subprocess.run([_musescore, str(xml), "-o", str(pdf)]) 

def mk_pdfs(p):
    ''' make a pdf from every musicxml file found under directory of path p'''
    if not os.path.isdir(p):
        print('ERROR mk_pdfs: ', p, 'not a dir')
        return
    else:
        print('making pdfs in:', p)        
    for f in os.listdir(p):
        if is_musicxml(p, f):
            print('making pdf of', f)            
            mk_pdf(p, f)
        elif os.path.isdir(Path(p, f)):
            mk_pdfs(Path(p, f))
        # else ignore
 

