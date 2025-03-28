#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan 17 14:42:57 2025

@author: jacquema
"""



import evalDRB 



sp = ps.Spellew(nbtons=30, t1_costtype=ps.pse.CTYPE_ACCID, t1_tonal=False, t1_det=True, t2_costtype=ps.pse.CTYPE_ADplus, t2_tonal=True, t2_det=True, global1=5, global2=5, debug=True)
name = 'Afternoon in Paris'

name = 'Airegin'

dataset = FRB_corpus('leads')
file = dataset[name]
score = m21.converter.parse(file.as_posix())
stat = ps.Stats()
opath = Path(os.getcwd())


(ls, lld) = sp.eval_score(score=score, stats=stat, score_id=0, title=name)


#(ls, lld) = sp.eval_score(score=score, stats=stat, score_id=0, title=name, output_path=opath)
