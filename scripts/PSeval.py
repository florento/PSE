#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 23 13:18:18 2022

@author: jacquema
"""

#import sys
import os
import time
from dataclasses import dataclass
import numpy as np
import pandas as pd
import music21 as m21
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
    """check whether a music21 part contains one key signature change"""
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
        
def add_tons(tons, sp):
    """add tonalities to a speller"""
    print('add_tons', tons)
    if (tons == 0): 
        return                            # default tons of module (30)
    elif (tons == 25):                    # Bach DWK
        for k in range(-4, 8):            # maj key signature in [-4 .. 7]
            sp.add_ton(k, pse.Mode.Major) # C, C#, D, Eb, E, F, F#, G, Ab, A, Bb, B 
        for k in range(-6, 7):            # min key signature in [-6 .. 6]
            sp.add_ton(k, pse.Mode.Minor) # C, C#, D, Eb, D#, E, F, F#, G, G#, A, Bb, B
        sp.close_tons()    
    elif (tons == 26):
        for k in range(-6, 7):            # key signature in [-6 .. 6]
            sp.add_ton(k, pse.Mode.Major)
            sp.add_ton(k, pse.Mode.Minor)
        sp.close_tons()    
    elif (tons == 30):
        for k in range(-7, 8):            # key signature in [-7 .. 7]
            sp.add_ton(k, pse.Mode.Major)
            sp.add_ton(k, pse.Mode.Minor)
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
    elif (m == pse.Mode.MinMel):
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

####################
##                ##
## diff PSE / M21 ##
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
        return (k.sharps == ton.fifths())# and k.mode == m21_mode(ton.mode()))
    elif isinstance(k, m21.key.KeySignature):
        return (k.sharps == ton.fifths())
    else:
        print('ERROR"', k, 'of unexpected type')
        return False
    
def compare_key_pitches(k0,ton):
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

def anote_local_part(part, sp, goodgtindex):
    i = sp.iglobal_ton0(goodgtindex)
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
    musescore = "/MuseScore\ 3.app/Contents/MacOS/mscore"
    pdffile = outfile+".pdf"
    mxfile  = outfile+".musicxml"
    os.system(musescore + " -o " + pdffile + " " + mxfile)

#########################
##                     ##
##   error counting    ##
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
        self._current_title = ''
        # composer of the score currently proceeded
        self._current_composer = ''
        # start time for spelling of the part currently proceeded
        self._current_t0 = time.time()
        # time for spelling of the part currently proceeded
        self._current_time = 0
        
    ## record evaluation results

    def new_score(self, score_id, title, composer):
        """register info on a new score being precessed"""
        if score_id == None:
            self._current_id += 1
        else:
            self._current_id = score_id
        if title == None:
            self._current_title = ''
        else:
            self._current_title = title
        if composer == None:
            self._current_composer = ''
        else:
            self._current_composer = composer        

    def start_timer(self):
        """record starting time for processing of a part"""
        self._current_t0 = time.time()
             
    def stop_timer(self):
        self._current_time = time.time() - self._current_t0
                 
    def record_part(self, part_id, k_gt, ton_est, nb_notes, nb_err):
        """add a new row in evaluation table"""
        self._global_parts += 1
        self._global_notes += nb_notes
        self._global_nerr += nb_err
        if compare_key(k_gt, ton_est):
            self._global_nerr_ks += nb_err
        else:
            self._global_kserr += 1
        row = []
        row.append(self._current_id)
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
        row.append(self._current_time)
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
        df.columns = ['id', 'title','composer', 'part', 'KSgt', 'KSest', 'notes', 'err', 'time']
        df['time'] = df['time'].map('{:,.3f}'.format)
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


#########################
##                     ##
##    evaluation       ##
##                     ##
#########################

choix_enharmonie = 0

triche = 0

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
              
def eval_part(part, stat, 
              algo=pse.Algo_PSE,
              nbtons=0,        # for PSE (default)
              kpre=33, kpost=23, # for PS13 (window size)
              debug=False, mark=False):
    """evaluate spelling for one part in a score and mark errors in red"""
    

    global triche
    
    if stat == None:
        stat=Stats()
    k0 = get_key(part)
    ln = extract_part(part)  # input note list
    #assert(count_notes(part) == len(ln))
    if (count_notes(part) != len(ln)):
        print('ERROR',  count_notes(part), len(ln))
        return
    print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
    # create and initialize the speller (default is PSE)
    if algo == pse.Algo_PS13:
        print('algo PS13', end='\n', flush=True)
        sp = pse.PS13()
        sp.set_Kpre(kpre)
        sp.set_Kpost(kpost)
    elif algo == pse.Algo_PS14:
        print('algo PS14', end='\n', flush=True)
        sp = pse.PS14()
        add_tons(nbtons, sp)
    else:
        print('algo PSE', end='\n', flush=True)
        sp = pse.PSE()
        add_tons(nbtons, sp)
    sp.debug(debug)           
    # feed speller with input notes
    for (n, b, s) in ln:   # note, bar number, simultaneous flag
        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
    # spell
    stat.start_timer()
    print('spell', end='\n', flush=True)
    sp.spell()
    stat.stop_timer()
    goodgtindex=0
    print('spell finished', end='\n', flush=True)
    # extract tonality estimation results
    if (algo == pse.Algo_PSE or algo == pse.Algo_PS14):
        nbg=sp.globals0()
        ton_est=sp.global_ton(0)
        #print(ton_est)
        #print(nbg)
        c=0
        if nbg>1:
            print("real global tone :", k0)
            enharm=False
            #boo=False
            present=False
            for i in range(nbg):
                gt = sp.global_ton0(i)
                print("possible tone : " , m21_key(gt))
                if compare_key(k0, gt):
                    goodgtindex = i
                    print("good index : " , i)
                    present=True
                elif compare_key_pitches(k0,gt):
                    enharm=True
                else:
                    c+=1
            if c>0:
                enharm=False
            #if enharm :
            #else :
            #    print("the good global tone was present, but not his enharmonical rival...")
            #    if boo:
            #        triche+=1
            #        print("POTENTIAL CHEATER")
            #    print("triche =",triche)
            if present and enharm: 
                sp.rename0(goodgtindex)
                ton_est=sp.global_ton0(goodgtindex)
                #print(ton_est)
            else :
                sp.rename(0)
        else:
            sp.rename(0)
            #gt = sp.global_ton(0)
        gt = sp.global_ton(0)
        if compare_key(k0, gt):
            print('global ton: OK:', '(', m21_key(gt), '), has the same signature as',k0, end=' ')
        else:
            print('global ton: NO:', '(', m21_key(gt), 'was', k0, '),', end=' ')
            if mark:
                anote_global_part(part, sp, ton_est) 

    print('BEFORE diff', end='\n', flush=True)
    # compute diff list between reference score and respell
    ld0 = diff(ln, sp) 
    print('diff:', len(ld0), end='\n', flush=True)
    # rewrite the passing notes
    print('rewrite passing notes', end='\n', flush=True)
    sp.rewrite_passing()
    # compute diff list between reference score and rewritten
    ld1 = diff(ln, sp)
    print('diff:', len(ld0), end='\n', flush=True)
    
    # annotations
    if mark:
        anote_rediff(ln, ld0, ld1) # anote_diff(ln, ld0, 'red')
        if (algo == pse.Algo_PSE or algo == pse.Algo_PS14):
            anote_local_part(part, sp, goodgtindex)         
    return (k0, ton_est, len(ln), ld1)

def eval_score(score, stat, 
               sid, title, composer,  
               algo=pse.Algo_PSE,
               nbtons=0,          # for PSE (default)
               kpre=33, kpost=23, # for PS13 (window size)
               debug=False, mark=False):
    """evaluate spelling for all parts in a score"""
    if stat == None:
        stat = Stats()
    if title == None:
        title=score.metadata.title, 
    if composer == None:
        composer=score.metadata.composer        
    stat.new_score(score_id=sid, title=title, composer=composer)
    lp = score.getElementsByClass(m21.stream.Part)
    nbparts = len(lp)
    ls = []
    lld = []
    for i in range(nbparts):    
        print(score.metadata.title, score.metadata.composer, end=' ')
        part = lp[i]
        if (nbparts > 1):
            print('part', i+1, '/', len(lp), end=' ', flush=True)
        if (spellable(part)):
            (k_gt, ton_est, nn, ld) = eval_part(part, stat, 
                                                algo, nbtons, kpre, kpost, 
                                                debug, mark)
            stat.record_part(i, k_gt, ton_est, nn, len(ld))
            ls.append(ton_est)
            lld.append(ld)
        else:
            print('cannot spell, skip', flush=True)
    return (ls, lld)
    
def empty_difflist(lld):
    for ld in lld:
        if not ld:
            continue
        else:
            return False
    return True
    
    # struct storing detailed evaluation errors for one score
    #class Errors:
    #    def __init__(self):
    #        self.lks = [] # one pair real ks est_ks for each part
    #        self.lld = [] # onne ld for each part

    #class EvalStruct:
    #    def __init__(self, ks, ln):        
    #        # ground truth key signature
    #        ks_gt = ks
    #        ks_est = ks
    #        notes = ln
    #        difflist = []
        
# pseval_part in the class Stats        
#def pseval_part1(self, part):
#    """evaluate spelling for one part in aa score"""
#    self._global_parts += 1
#    k0 = get_key(part)
#    self._current_ks_gt = k0.sharps
#    ln = extract_part(part)
#    #assert(count_notes(part) == len(ln))
#    if (count_notes(part) != len(ln)):
#        print('ERROR countnote =', count_notes(part), 'ln =', len(ln))
#    print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
#    self._current_notes = len(ln)
#    self._global_notes += len(ln)    
#    sp = pse.Speller()
#    sp.debug(self.debug)       # debug flag for lib PSE
#    add_tons(self.nbtons, sp)
#    #add_notes(ln, sp)
#    for (n, b, s) in ln:   # note, bar number, simultaneous flag
#        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
#    t0 = time.time()
#    sp.spell()
#    self._current_time = time.time() - t0
#    self._current_ks_est = sp.sig()
#    ld = diff(ln, sp)
#    self._current_nerr = len(ld)
#    self._global_nerr += len(ld)
#    if (sp.sig() == k0.sharps):
#        print('global ton: OK:', '(', sp.sig(), '),', end=' ')
#        print('diff:', len(ld), end='\n', flush=True)
#        self._global_nerr_ks += len(ld)
#    else:
#        print('global ton: NO:', '(', sp.sig(), 'was', k0, '),', end=' ')
#        print('diff:', len(ld), end='\n', flush=True)
#        self._global_kserr += 1
#    return (k0, sp.sig(), len(ln), ld)
    


# pseval_score in the class Stats        
#def pseval_score1(self, score, sid, title='', composer=''):
#    """evaluate spelling for all parts in a score"""
#    lp = score.getElementsByClass(m21.stream.Part)
#    nbparts = len(lp)
#    for i in range(nbparts):    
#        print(score.metadata.composer, score.metadata.title, end=' ')
#        part = lp[i]
#        if (nbparts > 1):
#            print('part', i+1, '/', len(lp), end=' ', flush=True)
#        if (spellable(part)):
#            if (len(title) == 0):
#                _title = score.metadata.title
#            else:
#                _title = title                
#            if (len(composer) == 0):
#                _comp = score.metadata.composer
#            else:
#                _comp = composer                
#            self._newtuple(title=_title, composer=_comp, part=i, sid=sid)
#            pseval_part1(self, part)
#            self._table.append(self._gettuple())
#        else:
#            print('cannot spell, skip', flush=True)

    
    
    
#    k0 = get_key(part)
#    print(k0, count_notes(part), 'notes,', count_measures(part), 'bars,', end = '\n')
#    ln = extract_notes(part)
#    sp = pse.Speller()
#    sp.debug(True)
#    add_notes(ln, sp)
#    print('spell')
#    sp.spell()


#bachBundle = corpus.corpora.CoreCorpus().search('bach', 'composer')

# part.show()
    

#sp = pse.Speller()
#sp.add(60,0)
#sp.add(62,0)
#sp.add(77,1)
#sp.add(66,2)
#sp.add(65,2)
#print('speller: size')
#print(sp.size())


#s = corpus.parse('bach/bwv65.2.xml')
#parts = s.getElementsByClass(stream.Part)
#print(len(parts), 'parts')
#part = parts[0] # soprano
# part.show()
#pseval(part)
#bach0.show()


#ks2 = m21.key.KeySignature(2)
#stat = ps.Stats()
#stat.new_score(0, 'pipo', 'Mario')
#stat.record_part(0, ks2, -1, 111, 2)
#stat.record_part(1, ks2, 2, 192, 5)

#s = m21.converter.parse('109-Kuhlau-sonatineop60n1.musicxml')
#lp = s.getElementsByClass(m21.stream.Part)
#eval_part(lp[0], nbtons=0, debug=True, mark=True, stat=None)


