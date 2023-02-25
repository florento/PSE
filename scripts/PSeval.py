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

# a barred note is a pair made of 
# - a note and 
# - the number of the  bar it belongs to
# - a flag saying whether the onset of the note is the same as the onset of the nest note

# Key object is the more expressive (tonic, mode...)
# KeySignature object is just the number of sharps (> 0) or flats, < 0)
def extract_keys(part):
    """extract the list of all keys or key signatures occurring in a music21 part"""    
    fpart = part.flatten()
    kl = fpart.getElementsByClass([m21.key.Key, m21.key.KeySignature])
    return kl

def key_changes(part):
    """check whether a music21 part contains one key signature change"""
    kl = extract_keys(part)
    return (len(kl) - 1)

def get_key(part):
    """return the key signature of a music21 part, if unique, otherwise None"""
    kl = extract_keys(part)
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
    elif (tons == 26):
        for k in range(-6, 7):            # key signature in [-6 .. 6]
            sp.add_ton(k, pse.Mode.Major)
            sp.add_ton(k, pse.Mode.Minor)
    elif (tons == 30):
        for k in range(-7, 8):            # key signature in [-7 .. 7]
            sp.add_ton(k, pse.Mode.Major)
            sp.add_ton(k, pse.Mode.Minor)

        
###############
##           ##
## PS to M21 ##
##           ##
###############

def get_pitches(sp):
    """extract the list of music21 pitches from a speller"""    
    pl = []
    for i in range(sp.size()):
        p = mk_pitch(name = sp.name(i),  
                     accid = sp.accidental(i),
                     octave =sp.octave(i))
        pl.append(p)
    return pl

def mk_pitch(name, accid, octave):
    """construct a music21.pitch from a PSE NoteName, a PSE Accidental ad an octave number"""    
    return  m21.pitch.Pitch(step=mk_step(name), 
                            accidental=mk_accid(accid), 
                            octave=octave)

def mk_step(nn):
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

def mk_accid(a):
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


####################
##                ##
## diff PSE / M21 ##
##                ##
####################

def compare_name(n, pse_name):
    """compare the name of a music21 note and a PSE note name"""    
    return (n.step == mk_step(pse_name))

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
        return (n.pitch.accidental == mk_accid(pse_accid))

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
        else:
            d = (i, sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            i = i+1
            ld = ld+[d]
    return ld

def diffrec(ln, sp, i, ld):
    if (len(ln) == 0):
        return ld
    else:
        (n, m) = ln[0]
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
    """compare a list of barred notes ad a list of pitches"""    
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

def anote_diff(ln, ld):
    """mark mispells in red in a note list, based on a diff-list"""
    if (len(ld) > 0):
        for (i, n, a, o, p) in ld:
            ln[i][0].style.color = 'red'
            ln[i][0].pitch.step = mk_step(n)
            if ((a != pse.Accid.Natural) or (p == True)):
                ln[i][0].pitch.accidental = mk_accid(a)
            ln[i][0].pitch.octave = o                

def anote_ks_part(part, k):
    e = m21.expressions.TextExpression('ks='+str(k))    
    ml = part.getElementsByClass(m21.stream.Measure)
    ml[0].insert(0, e)
    e.placement = 'above'
    e.style.color = 'red'   

def anote_part(part, ld):
    """mark mispells in red in a part, based on a diff-list"""
    fpart = part.flatten()
    ln = fpart.getElementsByClass(m21.note.Note) 
    anote_diff(ln, ld)    

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
                 
    def record_part(self, part_id, k_gt, k_est, nb_notes, nb_err):
        """add a new row in evaluation table"""
        self._global_parts += 1
        self._global_notes += nb_notes
        self._global_nerr += nb_err
        if (k_est == k_gt.sharps):
            self._global_nerr_ks += nb_err
        else:
            self._global_kserr += 1
        row = []
        row.append(self._current_id)
        row.append(self._current_title)
        row.append(self._current_composer)
        row.append(part_id)       # current part id
        row.append(k_gt.sharps)   # current ks gt
        row.append(k_est)         # current ks estimation
        row.append(nb_notes)      # nb notes in part 
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
        df.loc[df['KSgt'] == df['KSest'], 'KSest'] = np.nan
        # d = df['KSgt'].compare(df['KSest'], keep_shape=True)['other'].fillna('')        
        return df

    def write_dataframe(self, file):
        """write the evaluation table into a cvs file, after some formatting"""
        df = self.get_dataframe(self)
        df.fillna('').to_csv(file, header=True, index=False)
    
    # useless, there are accessors for that
    def get_datasum(self):
        """return a summary of the evaluation as a dictionary"""
        df = self.get_dataframe(self)
        #df.loc[df['KSgt'] == df['KSest'], 'KSest'] = np.nan
        # sums and errors
        total = { 'nb_KS'    : len(df['KSgt']),
                  'err_KS'   : [df['KSest'].count()], 
                  'nb_note'  : [df['notes'].sum()], 
                  'err_note' : [df['err'].sum()] }
        #df = df.append(pd.DataFrame.from_dict(total), ignore_index=True)
        #df = df.astype({"KSgt": int, "notes": int, "err": int})        
        #df = df.convert_dtypes()
        #df = df.fillna('')
        percent = { 'KS_rate'  : [ pcformat(total['err_KS'][0] * 100 / total['nb_KS'][0]) ], 
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
        df = self.get_datasum(self)
        df.to_csv(file, header=True, index=False)
    
def pcformat(x):    
    if abs(x - int(x)) < 1e-6:
        return int(x)
    else:
        return round(x,2)
            
def ks_errors(df):
    return len(df['KSgt'].compare(df['KSest'], keep_shape=False))

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

def spellable(part):
    """the given part can be pitch spelled"""
    if (get_key(part) == None):
        print(key_changes(part), 'key changes', end =' ')
        return False
    #elif (count_chords(part)):
    #    print('chords', end =' ')
    #    return False        
    return True
              
def eval_part(part, stat, nbtons=0, debug=False, mark=False):
    """evaluate spelling for one part in a score and mark errors in red"""
    if stat == None:
        stat=Stats()
    k0 = get_key(part)
    ln = extract_part(part)  # input note list
    #assert(count_notes(part) == len(ln))
    if (count_notes(part) != len(ln)):
        print('ERROR',  count_notes(part), len(ln))
        return
    print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
    sp = pse.Speller()
    sp.debug(debug)
    add_tons(nbtons, sp)
    for (n, b, s) in ln:   # note, bar number, simultaneous flag
        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
    stat.start_timer()
    sp.spell()
    stat.stop_timer()
    if (sp.sig() == k0.sharps):
        print('global ton: OK:', '(', sp.sig(), '),', end=' ')
    else:
        print('global ton: NO:', '(', sp.sig(), 'was', k0, '),', end=' ')
    ld = diff(ln, sp)
    print('diff:', len(ld), end='\n', flush=True)
    if mark:
        anote_diff(ln, ld)
        if (sp.sig() != k0.sharps):
            # k0.style.color = 'red' 
            anote_ks_part(part, sp.sig())
    return (k0, sp.sig(), len(ln), ld)

def eval_score(score, sid, title, composer,
               stat, tons=0, debug=False, mark=False):
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
        print('\n')
        print(score.metadata.composer, score.metadata.title, end=' ')
        part = lp[i]
        if (nbparts > 1):
            print('part', i+1, '/', len(lp), end=' ', flush=True)
        if (spellable(part)):
            (ks_gt, ks_est, nn, ld) = eval_part(part, stat, tons, debug, mark)
            stat.record_part(i, ks_gt, ks_est, nn, len(ld))
            ls.append(ks_est)
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

