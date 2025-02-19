# Evaluation scripts for PSE

arguments common in script functions 

dir and files

- output_dir
  where files will be written.
  by default, it is the current directory.
- tablename
  filename of output csv table

for an evaluation with PSE

- tons
  nb of Tons in TonIndex (PSE)

- costtype1
  domain of cost values for the computation of the first spelling table, 
  by best path search for each measure.
  possible values:

  - `pse.CTYPE_ACCID`
    number of accidentals without discount for lead tons.

  - `pse.CTYPE_ACCIDtb`
  
    number of accidentals and tie breaking components:
  
    - number of accidentals not present in the chromatic harmonic scale.
    - number of accidentals with a color different from global ton.
    - number of non lead Cb B# E# Fb.
    - number of non-lead double sharps or double flats.
  
  - `pse.CTYPE_ACCIDtbs`
  
    number of accidentals and tie breaking components,
  
    some TB components are summed before comparison.

  - `pse.CTYPE_ACCIDlead`
    number of accidentals with discount for lead tons (obsolete).
  
  - `pse.CTYPE_ADplus`
    number of accidentals plus distance to local ton, with tie-breaking components.

  - `pse.CTYPE_ADpluss`
    number of accidentals plus distance to local ton, with tie-breaking components.
    some TB components are summed before comparison.
  
  - `pse.CTYPE_ADlex`
    lexicographic composition of the number of accidentals and distance to local ton.
    
  - `pse.CTYPE_ADlexs`
    lexicographic composition of the number of accidentals and distance to local ton.
    some TB components are summed before comparison.
    
  - `pse.CTYPE_UNDEF` 
    unset (default),  PS13 will be used.
  
- tonal1

  option for the definition of initial state in best path search 
  for the computation of the first spelling table,  
  for each measure and each tonality (scale):

  - true: the state contains only the accidentals of the key signature
  - false (default):  the state contains all the accidentals of the scale (including lead tons)

- det1

  option for transitions in best path search 
  for the computation of the first spelling table,  
  for each measure and each tonality (scale):

  - true: deterministic: only one output transition (spelling choice), 
    according to the chromatic harmonic scale associated with the current  ton.
  - false: exhaustive: consider all alternative spellings (2 or 3 choices)

- grid
  name of algorithm for the computation of the grid.
  possible values

  - `Grid_Best`
    greedy algo with best costs
  - `Grid_Rank`
    greedy algo with best mean of 2 ranks
  - `Grid_Exhaustive`
    best path search
  - `Grid_Undef`

- global1
  percentage of approximation for selection of an intermediate list of global candidates

  after the computation of the first spelling table.
  (distance to best global in percent of the spelling cost for the row in first table).
  100 means that all tons are selected.

- costtype2
  domain of cost values for the computation of the second (and final) spelling table, by best path search.

  if unset, skip table2

- tonal2
  option for the definition of initial state in best path search 
  for the computation of the second spelling table.  

- det2
  option for transitions in best path search 
  for the computation of the second spelling table.

for an evaluation with PS13

- kpre
  parameter specific to PS13
- kpost
  parameter specific to PS13

flags

- csflag
  spell also chord symbols

- dflag
  print debug information on the console

- mflag
  mark flag: write output files

  

output files

- `musicXML`
  marked
- `table1.csv`, `table2.csv`
- `grid.csv`





## Chord Symbols

`cs_flag` for processing chord symbols or not.

it determines which note are fed in the the note enumerators of the speller (main and aux enumerator):

- ignore: chord symbols are ignored (not fed)
- add: notes of chord symbols are added for spelling (as usual chord notes)
- force: notes of chord symbols are added with forced name  (constrained spelling) 

| `cs_flag` | main enumerator | aux. enumerator |
| --------- | --------------- | --------------- |
| 0         | ignore          | (none)          |
| 1         | add             | (none)          |
| 2         | ignore          | force           |
| 3         | force           | (none)          |



## evalFRB

evaluation of the dataset Fake Real Book

commands:





## evalOmnibook

evaluation of the dataset CharlieParkerOmnibook

50 musicXML files in the sub-directory `musicxml`



evaluation of the whole corpus:

```
eval_Omnibook(tons, 
              costtype1, tonal1, det1, 
              costtype2, tonal2, det2, 
              kpre, kpost, 
              output_dir, tablename, 
              dflag, mflag, csflag)
```



evaluation of one opus:

```
eval_Omnibookitem(name, output_dir   
                 kpre, kpost, 
								 tons,          
                 costtype1, tonal1, det1,       
                 global1, grid=ps.pse.Grid_Rank,
                 costtype2=ps.pse.CTYPE_UNDEF, tonal2=True, det2=True,      
                 dflag=True, mflag=False, csflag=False): 
```











PSeval

## classe `Spellew`

speller on top of  ~~`pypse::PSE`~~   `pypse::SpellerEnum` 

- [ ] cstr SpellerEnum with nb tons, debug flag



- constructor Spellew
  - tons
  - costtype1, tonal1, det1
  - costtype2, tonal2, det2
  - output_dir 
  - tablename (filename csv table)
  - dflag (debug)
  - mflag (mark)



- spell
  - notes list
  - stats (obj)
