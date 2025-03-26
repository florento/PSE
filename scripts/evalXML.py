#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  5 14:02:31 2024
@author: jacquema

Framework for the evaluation of a set of XML scores all in the same directory
"""

#import sys
#import logging
import os
from pathlib import Path, PosixPath
from datetime import datetime
import music21 as m21
import PSeval as ps

########################
##                    ##
##  global variables  ##
##                    ##
########################

# root of evaluation dir
_eval_root = '../../PSeval'

# MuseScore commandline executable
_mscore = '/Applications/MuseScore 4.app/Contents/MacOS/mscore'

#################################
##                             ##
## extraction of dataset files ##
##                             ##
#################################

def extract1(directory_path, score_suffix):
	"""return the first file with given suffix found in the given directory path"""
	files = os.listdir(directory_path)
	for file in files:    
		file_ext = os.path.splitext(file)[1]   # get the extension in the file name
		if (file_ext in score_suffix):
			return directory_path/file
	return # not found
	
# corpus can be 'leads' or 'piano'
def get_corpus(dataset_path, flat=True):
	"""build a dictionary of XML scores with corresponding paths"""
	"""flat: all XML files are contained in the given path or in subdirs"""
	# default score file name
	score_suffix = ['.musicxml', '.xml', '.mxml']
	assert isinstance(dataset_path, PosixPath)
	if not os.path.exists(dataset_path):
		print('Error: ', dataset_path, 'not found')
		return
	# map: opus_name -> path
	dataset = dict()
	files = os.listdir(dataset_path)
	for file in files:    
		filepath = dataset_path/file
		# skip directories
		if not flat and os.path.isdir(filepath):
			target = extract1(filepath, score_suffix)
			if target is not None:
				dataset[file] = target
		# check the extension in the file name
		elif flat and (os.path.splitext(file)[1] in score_suffix):
			# map score name to file path
			dataset[os.path.splitext(file)[0]] = filepath
	# sort the list alphabetically
	dataset = dict(sorted(dataset.items()))
	return dataset

#####################################
##                                 ##
## automatic evaluation of dataset ##
##                                 ##
#####################################

# PS13: kpre=33, kpost=23
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_corpus(speller, dataset, skip=[],
				eval_root='.', output_dir='', tablename='',
				mflag=True, csflag=0):
	"""eval a whole corpus with a given speller"""
	"""speller: instance of speller for the evaluation"""
	"""dataset: a dictionary as produced by XML_corpus"""
	"""skip: list of names in corpus to avoid during evaluation"""
	"""eval_root: directory where the evaluation directory will lie"""
	"""output_dir: name of directory where the evaluation files will be written"""
	"""tablename: file name of evaluation table. will be written in the output_dir"""
	"""mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
	"""csflag: see pse.eval_score"""   
	assert(csflag in [0, 1, 2, 3])
	# prepare the output dir
	algoname = speller.algo()    
	timestamp = datetime.today().strftime('%Y%m%d-%H%M')
	output_path = Path(eval_root)
	if not os.path.exists(output_path):
		print('ERROR output dir: ', output_path, 'not found')
		return
	if output_dir == '':
	   output_dir = timestamp+'_'+algoname
	output_path = output_path/output_dir
	if not os.path.isdir(output_path):
		os.mkdir(output_path)
	else:
		print('WARNING: output dir', output_path, 'exists')

	# input data and processing   
	stat = ps.Stats()   
	names = sorted(list(dataset)) # list of index in dataset   
	print('\n', 'starting evaluation of FRB dataset -', len(names), 'entries\n')
	i = 0 # score id
	for name in names:
		if (name in skip):
			print('\n', name, 'SKIP\n', flush=True)
			continue
		if (not dataset.get(name)):
			print('\n', name, "not found in dataset, skip")
			continue
		if mflag:
			output_path2 = output_path/name
			if not os.path.isdir(output_path2):  
				os.mkdir(output_path2)
		else:
			output_path2 = None
		file = dataset[name]
		print('\n', name, '\n')
		s = m21.converter.parse(file.as_posix())
		(ls, lld) = speller.eval_score(score=s, stats=stat, score_id=i, 
								  title=name, composer='', 
								  output_path=output_path2, 
								  chord_symb=csflag, 
								  reset_globals = True)
		i += 1
		#if mflag and not ps.empty_difflist(lld): # done in eval_score
		#    write_score(s, output_path2, name)
	# display and save evaluation table
	# default table file name
	if not tablename:
	   tablename =  'eval_'+algoname+'_'+timestamp
	stat.show()    
	df = stat.get_dataframe() # create pandas dataframe
	df.pop('part') # del column part number (always 0)
	df.to_csv(output_path/(tablename+'.csv') , header=True, index=False)
		
# PS13: kpre=33, kpost=23
# PSE:  tons=135, 
# PSE table1:  costtype1 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
# PSE table2:  costtype2 = ps.CTYPE_ACCID | CTYPE_ACCIDlead | CTYPE_ADplus | CTYPE_ADlex
def eval_item(speller, dataset, name, output_dir='', mflag=False, csflag=0):   
	"""eval one item of the FRB corpus with given algo and parameters"""
	"""speller: speller instance for the evaluation"""
	"""dataset: a dictionary as produced by XML_corpus"""
	"""name: filename of item (prefix) in the dataset"""
	"""output_dir: where files will be written"""
	"""mflag: mark flag: write anotation files in a dedicaced dir for each opus"""
	"""csflag: see pse.eval_score"""   
	assert(csflag in [0, 1, 2, 3])
	# input data
	assert(len(name) > 0)
	if (dataset.get(name) == None):
		print(name, "not found in dataset")
		return
	file = dataset[name]
	score = m21.converter.parse(file.as_posix())
	stat = ps.Stats()   
	opath = Path(os.getcwd()) if output_dir=='' else Path(output_dir)
	# ground truth ks, estimated ks, nb of notes and list of diff notes
	#(k_gt, gt_est, nn, ld) = ps.eval_part(part=part, stat=stat, nbtons=tons, 
	#                                      debug=dflag, mark=mflag)
	(ls, lld) = speller.eval_score(score=score, stats=stat, score_id=0, 
								   title=name, composer='', output_path=opath, 
								   chord_symb = csflag, 
								   reset_globals = False)    
	stat.show()   
		
def write_score(score, output_path, outname):
	if not os.path.isdir(output_path):
		os.mkdir(output_path)
	xmlfile = output_path/(outname+'.musicxml')
	score.write('musicxml', fp=xmlfile)

def write_score2(score, output_path, outname):
	assert(len(outname) > 0)
	if not os.path.isdir(output_path):
		os.mkdir(output_path)
	output_path = output_path/outname
	if not os.path.isdir(output_path):
		os.mkdir(output_path)
	xmlfile = output_path/(outname+'.musicxml')
	score.write('musicxml', fp=xmlfile)
	# pdffile = dirname+'/'+outname+'.pdf'
	# os.system(_mscore + ' -o ' + pdffile + ' ' + xmlfile)

def print_adds(ln, aux):
	for (n, b, sf, ff) in ln:   
		assert(isinstance(n, m21.note.Note))
		a = 'sp.add4(' if ff else 'sp.add0('
		a += str(n.pitch.midi)
		a += ', '
		a += str(b)
		a += ', '
		a += 'true' if sf else 'false'
		a += ', '
		if ff:
			a += 'pse::NoteName::'+n.pitch.step
			a += ', '
			accid = n.pitch.accidental
			if accid == m21.pitch.Accidental('triple-sharp'):
				a += 'pse::Accid::TripleSharp'
			elif accid == m21.pitch.Accidental('double-sharp'):
				a += 'pse::Accid::DoubleSharp'
			elif accid == m21.pitch.Accidental('one-and-a-half-sharp'):
				a += 'pse::Accid::ThreeQuartersSharp'
			elif accid == m21.pitch.Accidental('sharp'):
				a += 'pse::Accid::Sharp'
			elif accid == m21.pitch.Accidental('half-sharp'):
				a += 'pse::Accid::QuarterSharp'
			elif accid == m21.pitch.Accidental('natural') or accid is None:
				a += 'pse::Accid::Natural'
			elif accid == m21.pitch.Accidental('half-flat'):
				a += 'pse::Accid::QuarterFlat'
			elif accid == m21.pitch.Accidental('flat'):
				a += 'pse::Accid::Flat'
			elif accid == m21.pitch.Accidental('one-and-a-half-flat'):
				a += 'pse::Accid::ThreeQuartersFlat'
			elif accid == m21.pitch.Accidental('double-flat'):
				a += 'pse::Accid::DoubleFlat'
			elif accid == m21.pitch.Accidental('triple-flat'):
				a += 'pse::Accid::TripleFlat'
			else:
				print('ERROR unexpected m21 accidental', accid )
			a += ', '
			assert(n.pitch.octave is not None)
			a += str(n.pitch.octave)
			a += ', '
			a += 'false'  # printed: not significant                     
			a += ', '
		a += 'true' if aux else 'false'
		a += ');'
		print(a)        
	
def debug(dataset, name, csflag=0, part=0):    
	"""compute the C++ add instructions for given part, for debugging with gdb"""
	"""dataset: a dictionary as produced by XML_corpus"""
	"""name: filename of item (prefix) in the dataset"""
	"""part: number of part in the score file"""
	"""csflag: 'ignore' if we do not add the notes of chord symbols"""
	"""        'add'    if we add them"""   
	"""        'force'  if we add them and force their names"""   
	assert(len(name) > 0)
	assert(csflag in [0, 1, 2, 3])
	if (dataset.get(name) == None):
		print(name, "not found in dataset")
		return
	file = dataset[name]
	score = m21.converter.parse(file)
	lp = score.getElementsByClass(m21.stream.Part)
	ln0 = [] # notes for main enumerator
	ln1 = [] # notes for aux enumerator
	if csflag == 0:
		ln0 = ps.extract_part(lp[part], 'ignore')  # input note list
	elif csflag == 1:
		ln0 = ps.extract_part(lp[part], 'add') 
	elif csflag == 2:
		ln0 = ps.extract_part(lp[part], 'ignore')  # notes for second step
		ln1 = ps.extract_part(lp[part], 'force')  # notes for first step
	elif csflag == 3:
		ln0 = ps.extract_part(lp[part], 'force')  # input note list        
	print_adds(ln0, False)
	print_adds(ln1, True)

def cflat(dataset):
	"""count and display the Cb, B#, Fb, E# in the given dataset"""
	names = sorted(list(dataset))
	c = 0
	for name in names:
		file = dataset[name]
		score = m21.converter.parse(file.as_posix())
		lp = score.getElementsByClass(m21.stream.Part)
		if (len(lp) > 1):
			print(name, 'has', len(lp), 'parts')
		part = lp[0]
		ln = ps.extract_part(part, 'ignore')
		for (n, b, simult, force) in ln:
			assert(isinstance(n, m21.note.Note))
			if (n.pitch.step == 'C')   and (n.pitch.accidental == m21.pitch.Accidental('flat')):
				print(name, 'bar', b, 'Cb')
				c += 1
			elif (n.pitch.step == 'B') and (n.pitch.accidental == m21.pitch.Accidental('sharp')):
				print(name, 'bar', b, 'B#')
				c += 1
			elif (n.pitch.step == 'F') and (n.pitch.accidental == m21.pitch.Accidental('flat')):
				print(name, 'bar', b, 'Fb')
				c += 1
			elif (n.pitch.step == 'E') and (n.pitch.accidental == m21.pitch.Accidental('sharp')):
				print(name, 'bar', b, 'E#')
				c += 1
	return c

def doublaccids(dataset):
	"""count and display the bb and ## in the given dataset"""
	names = sorted(list(dataset))
	c = 0
	for name in names:
		file = dataset[name]
		score = m21.converter.parse(file.as_posix())
		lp = score.getElementsByClass(m21.stream.Part)
		if (len(lp) > 1):
			print(name, 'has', len(lp), 'parts')
		part = lp[0]
		ln = ps.extract_part(part, 'ignore')
		for (n, b, simult, force) in ln:
			assert(isinstance(n, m21.note.Note))
			if (n.pitch.accidental is not None and n.pitch.accidental.alter < -1):
				print(name, 'bar', b, 'double flat')
				c += 1
			elif (n.pitch.accidental is not None and n.pitch.accidental.alter > 1):
				print(name, 'bar', b, 'double sharp')
				c += 1
	return c
