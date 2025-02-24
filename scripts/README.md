# Evaluation scripts for PSE

arguments common in script functions 

dir and files

- output_dir
  where the output files will be written.
  by default, it is the current directory.
- tablename
  filename of the output csv table summarising the evaluation results.

for an evaluation with PSE

- `tons`
  nb of tonalities in Ton Index (PSE)

- `costtype1`
  domain of cost values for the computation of the first spelling table, by best path search, for each measure and each tonality in Ton Index. The possible values are:
  
  - `pse.CTYPE_ACCID`
    number of printed accidentals, no discount for lead tons.
  
  - `pse.CTYPE_ACCIDtb`

    number of printed accidentals and tie breaking measures
  
    (the comparison is lexicographic):
  
    - number of accidentals with a colour different from the local ton,
      or from the global ton if the local ton is unknown.
    - number of non lead Cb B# E# Fb.
    - number of non-lead double sharps or double flats.
    - number of accidentals not present in the chromatic harmonic scale.
  
  - `pse.CTYPE_ACCIDtbs`
  
    number of printed accidentals and tie breaking measures,
  
    some TB measures are summed before comparison.
  
  - `pse.CTYPE_ACCIDlead`
    number of accidentals with discount for lead tons (obsolete).
  
  - `pse.CTYPE_ADplus`
    number of printed accidentals plus distance to local ton, with the above TB measures.
  
  - `pse.CTYPE_ADpluss`
    number of printed accidentals plus distance to local ton, with TB measures.
    some TB measures are summed before comparison.
  
  - `pse.CTYPE_ADlex`
    lexicographic composition of the number of accidentals and distance to local ton,
    with the above TB measures.
  
  - `pse.CTYPE_ADlexs`
    lexicographic composition of the number of accidentals and distance to local ton.
    some TB measures are summed before comparison.
  
  - `pse.CTYPE_UNDEF` 
    unset (default), using an algorithm without best cost search (e.g. PS13).
  
- `tonal1`

  option for the definition of the initial state in best path search for the computation of the first spelling table, for each measure and each tonality (scale):
  
  - true: the state contains only the accidentals of the key signature,
  - false (default):  the state contains all the accidentals of the scale, including lead tons.
  
- `det1`

  option for transitions in best path search for the computation of the first spelling table:
  
  - true: deterministic transitions: only one output transition (spelling choice) is possible, 
    it is determined by the chromatic harmonic scale associated with the current  ton.
  - false: exhaustive: consider all alternative spellings (2 or 3 choices of names for all MIDI pitch).
  
- `grid`
  name of algorithm for the computation of the grid of local tonalities. The possible values are: 
  
  - `Grid_Best`
    greedy algo with best costs.
  - `Grid_Rank`
    greedy algo with best mean of 2 ranks.
  - `Grid_Exhaustive`
    best path search.
  - `Grid_Undef`
    the grid is not computed.

- `global1`
  percentage of approximation for the selection of an intermediate list of global candidates, after the computation of the first spelling table (optimisation).
  It is the distance to best global in percent of the spelling cost for the row in first table.
  100 means that all tons are selected as global candidate.
  
- `costtype2`
  domain of cost values for the computation of the second (and final) spelling table, by best path search.

  If unset, skip the computation of the second table.

- `tonal2`
  option for the definition of the initial state in best path search for the computation of the second spelling table.  
  
- `det2`
  option for transitions in best path search for the computation of the second spelling table.
  
- `csflag`: options for processing chord symbols (in lead sheets). 
  see also some more details below.
  
  - 0: ignore the chord symbols.
  - 1: spell the notes of chord symbols (as for other chords).
  - 2: spell the notes of chord symbols forcing their names in 1st spelling table and ignore them in 2d spelling table.
  - 3: spell the notes of chord symbols forcing their names in 1st and 2d tables.
  

for an evaluation with PS13

- kpre
  parameter specific to PS13
- kpost
  parameter specific to PS13

other flags

- dflag
  print debug information on the console

- mflag
  mark flag: write the following output files:

  - musicXML
    original score with error marked (coloured notes)
  
  - `table1.csv` 1st spelling table, with best spelling costs, of dimension measures $\times$ T Ton Index.
  
  - `table2.csv`  2d spelling table.
  
  - `grid.csv`  table of estimated local modes, of dimension mesures $\times$ Ton Index.
  



## Note Enumerators

The speller can use 2 different note enumerators in the 1st and 2d spelling steps.
In that case, it uses an auxiliary enumerator in the first step only , and the main enumerator in the second step. Otherwise, it uses the same main enumerator in both steps.

The `cs_flag`, relative to the processing of chord symbols in lead sheets (see above), determines which note are fed to the main and auxiliary note enumerators of the speller,  as follows.

| `cs_flag` | main enumerator | aux. enumerator |
| --------- | --------------- | --------------- |
| 0         | ignore          | (none)          |
| 1         | add             | (none)          |
| 2         | ignore          | force           |
| 3         | force           | (none)          |

- ignore: the chord symbols are ignored (not fed)
- add: the notes of chord symbols are added for spelling (as usual chord notes)
- force: the notes of chord symbols are added with forced name  (constrained spelling) 



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



## evalFiloSax

command





 ## evalFiloBass 





## PSeval classe `Spellew`

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
