#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 23 13:18:18 2022

@author: jacquema
"""

#import sys
import music21 as m21



# import module with full path
# see https://www.geeksforgeeks.org/how-to-import-a-python-module-given-the-full-path/

#sys.path.append('/Users/jacquema/Code/PSE')
#import pse

import importlib.util
spec = importlib.util.spec_from_file_location("pse","/Users/jacquema/Code/qparse/PSE/pse.cpython-310-darwin.so")
pse = importlib.util.module_from_spec(spec)
spec.loader.exec_module(pse)

print(pse.excuseme)




# a barred note is a pair made of 
# - a note and 
# - the number of the  bar it belongs to



# Key object is the more expressive (tonic, mode...)
# KeySignature object is just the number of sharps (> 0) or flats, < 0)
def extract_keys(part):
    """extract the list of all keys or key signatures occurring in a part"""    
    fpart = part.flatten()
    kl = fpart.getElementsByClass([m21.key.Key, m21.key.KeySignature])
    return kl


def key_changes(part):
    """check whether a music21 part contains one key signature change"""
    kl = extract_keys(part)
    return (len(kl) - 1)


def get_key(part):
    """return the key signature of a part, if unique, otherwise None"""
    kl = extract_keys(part)
    if (len(kl) == 1):
        return kl[0]
    else:
        return None


def count_notes(part):
    """return the number of notes in a part"""    
    fpart = part.flatten()
    nl = fpart.getElementsByClass(m21.note.Note)
    return len(nl)    


def count_chords(part):
    """return the number of chords in a part"""    
    cl = part.getElementsByClass(m21.chord.Chord)
    return len(cl) 


def count_measures(part):
    """return the number of measures in a part"""    
    ml = part.getElementsByClass(m21.stream.Measure)
    return len(ml) 


def compare_name(n, pse_name):
    """compare the name of a Music21 note and a PSE note name"""    
    return (n.step == mk_step(pse_name))

            
def compare_accid(n, pse_accid, print_flag):
    """compare the accidental of a Music21 note and a PSE accidental"""    
    if (pse_accid == pse.Accid.Natural):
        if (n.pitch.accidental == m21.pitch.Accidental('natural')):
            return True  # (print_flag == True): mandatory (print_flag == False) courtesy 
        elif (n.pitch.accidental == None):
            return (print_flag == False)
        else:
            return False
    else:
        return (n.pitch.accidental == mk_accid(pse_accid))


def diff(ln, sp):
    """compare a list of barred notes and list of notes in speller"""    
    if (len(ln) == sp.size()):
        return diff1(ln, sp, 0, [])        
    else:
        print('ERROR (diff)')
        return []


def diff1(ln, sp, i, ld):
    if (len(ln) == 0):
        return ld
    else:
        (n, m) = ln[0]
        if (compare_name(n, sp.name(i)) and
            compare_accid(n, sp.accidental(i), sp.printed(i)) and 
            n.octave == sp.octave(i)):
            return diff1(ln[1:], sp, i+1, ld)
        else:
            #print('diff: ', i, ':', n, 
            #      sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            d = (i, sp.name(i), sp.accidental(i), sp.octave(i), sp.printed(i))
            return diff1(ln[1:], sp, i+1, ld+[d])


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
                    

def extract_notes(part):
    """extract the list of barred notes occurring in a part"""    
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
    """feed a speller with the (MIDI) notes of the part"""
    i = 0    
    for (n, b) in ln:
        #print('add', i, ':', n.pitch.midi, b, flush=True)
        sp.add(midi=n.pitch.midi, bar=b)
        i = i+1


def get_pitches(sp):
    """extract the list of M21 pitches from a speller"""    
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
    """cast a PSE NoteName into a Music21 character"""
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


def spellable(part):
    """the given part can be pitch spelled"""
    if (get_key(part) == None):
        print(key_changes(part), 'key changes', end =' ')
        return False
    elif (count_chords(part)):
        print('chords', end =" ")
        return False        
    return True


def pseval_part(part, dflag=False):
    """evaluate spelling for one part in a score"""
    k0 = get_key(part)
    ln = extract_notes(part)
    if (count_notes(part) != len(ln)):
        print('ERROR',  count_notes(part), len(ln))
        return
    print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
    sp = pse.Speller()
    sp.debug(dflag)
    add_notes(ln, sp)
    sp.respell()
    if (sp.sig() == k0.sharps):
        print('global ton: OK:', '(', sp.sig(), '),', end=' ')
    else:
        print('global ton: NO:', '(', sp.sig(), 'was', k0, '),', end=' ')
    ld = diff(ln, sp)
    print('diff:', len(ld), end='\n', flush=True)
    return ld
 

def pseval_score(score):
    """evaluate spelling for all parts in a score"""
    print(score.metadata.composer, score.metadata.title, end=' ')
    lp = score.getElementsByClass(m21.stream.Part)
    nbparts = len(lp)
    for i in range(nbparts):    
        part = lp[i]
        if (nbparts > 1):
            print('part', i+1, '/', len(lp), end=' ', flush=True)
        if (spellable(part)):
            pseval_part(part, False)
        else:
            print(score.metadata.composer, score.metadata.title, 'cannot spell, skip')
        

def pseval_m21(str):
    bundle = m21.corpus.corpora.CoreCorpus().search(str, 'composer')
    nb = len(bundle)
    for i in range(nb):
        print('bundle', str, i, '/', nb, end=' ') 
        score = bundle[i].parse()
        pseval_score(score)
        input("Press Enter to continue...")




class PSStats:
    _UNDEF_KS = 99

    def new_tuple(self, score_name, part):
        """start a new row in evaluation table"""
        self._current_name = score_name
        self._current_part = part
    

    def __init__(self):
        #
        # counters for all parts
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
        # counters for current part
        #
        # number of notes in part currently proceeded
        self._current_notes = 0
        # real key sig. of part currently proceeded
        self._current_ks_gt = self._UNDEF_KS 
        # estimated key sig. of part currently proceeded
        self._current_ks_est = self._UNDEF_KS 
        # number of wrong note names in part currently proceeded
        self._current_nerr = 0
        # ame of score currently proceeded
        self._current_part = 0
        # part bb currently proceeded
        self._current_name = ''
        
            

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
    
#    k0 = get_key(part)
#    print(k0, count_notes(part), 'notes,', count_measures(part), 'bars,', end = '\n')
#    ln = extract_notes(part)
#    sp = pse.Speller()
#    sp.debug(True)
#    add_notes(ln, sp)
#    print('respell')
#    sp.respell()



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




  
    
    