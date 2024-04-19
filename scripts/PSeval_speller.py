#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 12 18:40:25 2024

@author: jacquema
"""

# garbage code


#import sys
import os
import time
from pathlib import Path  #PosixPath
#from dataclasses import dataclass
import numpy as np
import pandas as pd
import music21 as m21
import subprocess # run command line
import pse

 

def make_speller(code, ln, k0, stat, fdebug):
    if (code == "PS13"):
        return speller_PS13(ln, stat, fdebug)
    if (code == "PS24T1D"):
        return speller_T1(notes=ln, stats=stat, nbtons=24, 
                          cost_type=00, tonal=False, det=True, debug=fdebug)
    else:
        print("Unknown algo code")
        
def speller_PS13(ln, stat, debug):
    print('algo PS13', end='\n', flush=True)
    kpre=33
    kpost=23
    sp = pse.PS13()
    sp.set_Kpre(kpre)
    sp.set_Kpost(kpost)
    sp.debug(debug)           
    # feed speller with input notes
    for (n, b, s) in ln:   # note, bar number, simultaneous flag
        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
    print('spelling...', end='\n', flush=True)
    stat.start_timer(1) # single timer
    sp.spell()
    stat.stop_timer(1)
    return sp

def base_speller(notes, nbtons, debug):
    sp = pse.Speller()
    sp.debug(debug)  
    # build the ton index
    add_tons(nbtons, sp)         
    # feed speller with input notes
    for (n, b, s) in notes:   # note, bar number, simultaneous flag
        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
    return sp;
     
def speller_T1(notes, stats, nbtons, cost_type, tonal, det, debug):
    print('algo PSE 1 Table', end='\n', flush=True)
    sp = base_speller(notes, nbtons, debug)
    print('spelling...', end='\n', flush=True)
    stats.start_timer(1)
    sp.eval_table(cost_type, tonal, det)
    stats.stop_timer(1)
    return sp

def speller_T2(notes, stats, nbtons, 
               cost_type1, tonal1, det1, 
               cost_type2, tonal2, det2, debug):
    print('algo PSE 2 Tables', end='\n', flush=True)
    sp = base_speller(notes, nbtons, debug)
    print('spelling...', end='\n', flush=True)
    stats.start_timer(1)
    sp.eval_table(cost_type1, tonal1, det1)
    stats.stop_timer(1)

    stats.start_timer(2)
    sp.eval_grid()
    stats.stop_timer(2)
    
    stats.start_timer(3)
    sp.reval_table(cost_type2, tonal2, det2)
    stats.stop_timer(3)
    return sp




### EVAL ###




# with parameters Spellew (new version)
def eval_part(part, stat, 
              kpre=0, kpost=0, # PS13 parameters
              nbtons=0,        # PSE: nb of Tons in TonIndex
              costtype1=pse.CTYPE_UNDEF, tonal1=True, det1=True, # PSE: table1
              costtype2=pse.CTYPE_UNDEF, tonal2=True, det2=True, # PSE: table2 
              debug=False, mark=False):
    """evaluate spelling for one part in a score and mark errors"""
    
    if stat == None:
        stat=Stats()
    k0 = get_key(part)
    ln = extract_part(part)  # input note list
    #assert(count_notes(part) == len(ln))
    if (count_notes(part) != len(ln)):
        print('ERROR',  count_notes(part), len(ln))
        return
    print(len(ln), 'notes,', count_measures(part), 'bars,', end=' ')
    # create speller
    sp = Spellew(notes=ln, 
                 ps13_kpre=kpre, ps13_kpost=kpost,
                 nbtons=nbtons,
                 t1_costtype=costtype1, t1_tonal=tonal1, t1_det=det1,
                 t2_costtype=costtype2, t2_tonal=tonal2, t2_det=det2,
                 debug=debug)
    # spell
    print('spelling...', end='\n', flush=True)
    sp.spell(stat)   #print('spell finished', end='\n', flush=True)   
    # extract estimated global ton from speller
    (gt, i) = sp.get_global(k0)
    sp.rename(i)
    if not gt.undef:
        if compare_key(k0, gt):
            print('global ton: OK:', '(', m21_key(gt), 
                  '), has the same signature as', k0, end=' ')
        else:
            print('global ton: NO:', '(', m21_key(gt), 'was', k0, '),', end=' ')
            if mark:
                anote_global_part(part, sp, gt) 
    # compute diff list between reference score and respell
    ld0 = sp.diff() 
    print('diff:', len(ld0), end='\n', flush=True)
    # rewrite the passing notes
    print('rewrite passing notes', end='\n', flush=True)
    sp.rewrite_passing()
    # compute diff list between reference score and rewritten
    ld1 = sp.diff()
    print('diff after rewriting:', len(ld1), end='\n', flush=True)
    
    # annotations
    if mark:
        anote_rediff(ln, ld0, ld1) # anote_diff(ln, ld0, 'red')
        sp.anote_local_part(part, i)         
    return (k0, gt, len(ln), ld1)


# with parameters Spellew (new version)
def eval_score(score, stat, sid, title, composer,  
               kpre=0, kpost=0, # PS13 parameters
               nbtons=0,        # PSE: nb of Tons in TonIndex
               costtype1=pse.CTYPE_UNDEF, tonal1=True, det1=True, # PSE: table1
               costtype2=pse.CTYPE_UNDEF, tonal2=True, det2=True, # PSE: table2 
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
                                                kpre, kpost, 
                                                nbtons,       
                                                costtype1, tonal1, det1,
                                                costtype2, tonal2, det2, 
                                                debug, mark)
            stat.record_part(i, k_gt, ton_est, nn, len(ld))
            ls.append(ton_est)
            lld.append(ld)
        else:
            print('cannot spell, skip', flush=True)
    return (ls, lld)


   
# parameter algo (previous version)
def eval_part_algo(part, stat, 
                   algo=pse.Algo_PSE,
                   nbtons=0,          # for PSE (default)
                   kpre=33, kpost=23, # for PS13 (window size)
                   debug=False, mark=False):
    """evaluate spelling for one part in a score and mark errors"""
    
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
    elif algo == pse.Algo_PSD:
        print('algo PS14', nbtons, 'tons', end='\n', flush=True)
        sp = pse.PS14()
        add_tons(nbtons, sp)
    else:
        print('algo PSE', nbtons, 'tons', end='\n', flush=True)
        sp = pse.PSE()
        add_tons(nbtons, sp)
    sp.debug(debug)           
    # feed speller with input notes
    print('adding', len(ln),'notes', end='\n', flush=True)
    for (n, b, s) in ln:   # note, bar number, simultaneous flag
        sp.add(midi=n.pitch.midi, bar=b, simultaneous=s)
    # spell
    stat.start_timer() # singla timer
    print('spelling...', end='\n', flush=True)
    sp.spell()
    stat.stop_timer()
    print('spell finished', end='\n', flush=True)
    # extract tonality estimation results
    goodgtindex=0
    if (algo == pse.Algo_PSE or algo == pse.Algo_PSD):
        nbg = sp.globals0()
        ton_est = sp.global_ton(0)
        #print(ton_est)
        #print(nbg)
        c = 0
        if nbg > 1:
            print("real global tone :", k0)
            enharm=False
            present=False
            for i in range(nbg):
                gt = sp.global_ton0(i)
                # print("possible tone : " , m21_key(gt))
                if compare_key(k0, gt):
                    goodgtindex = i
                    # print("good index : " , i)
                    present=True
                elif compare_key_pitches(k0, gt):
                    enharm=True
                else:
                    c+=1
            if c > 0:
                enharm=False
            if present and enharm: 
                sp.rename0(goodgtindex)
                ton_est = sp.global_ton0(goodgtindex)
                #print(ton_est)
            else:
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

    # print('BEFORE diff', end='\n', flush=True)
    # compute diff list between reference score and respell
    ld0 = diff(ln, sp) 
    print('diff:', len(ld0), end='\n', flush=True)
    # rewrite the passing notes
    print('rewrite passing notes', end='\n', flush=True)
    sp.rewrite_passing()
    # compute diff list between reference score and rewritten
    ld1 = diff(ln, sp)
    print('diff after rewriting:', len(ld1), end='\n', flush=True)
    
    # annotations
    if mark:
        anote_rediff(ln, ld0, ld1) # anote_diff(ln, ld0, 'red')
        if (algo == pse.Algo_PSE or algo == pse.Algo_PSD):
            anote_local_part(part, sp, sp.global_ton0(goodgtindex))         
    return (k0, ton_est, len(ln), ld1)





# parameter algo (previous version)
def eval_score_algo(score, stat, 
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
            (k_gt, ton_est, nn, ld) = eval_part_algo(part, stat, 
                                                     algo, nbtons, kpre, kpost, 
                                                     debug, mark)
            stat.record_part(i, k_gt, ton_est, nn, len(ld))
            ls.append(ton_est)
            lld.append(ld)
        else:
            print('cannot spell, skip', flush=True)
    return (ls, lld)





   
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

