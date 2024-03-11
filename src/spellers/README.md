# Spellers

Spellers are the interface between the C++ classes and the Python binding.

Their methods can be called in Python, through the binding, to specify the notes to spell and estimate names for them as well as a KS and local tonalities.



## speller data

- **array of tonalities**
- **cost** sample (zero) for the construction of tables 
- **mode**: tonal / local
  flag for the construction of initial state for construction of tables
- flag **chromatic** 
  deterministic transitions for construction of tables
- **table1** (contains best spelling paths for each ton and measure)
- **table2**
- **grid** of local tons (1 ton for each initial ton and measure)

- **global1** subarray of tons selected from table1
- **global2**



## speller methods

see `Speller.hpp` (documented)



- `debug(flag)`
  set or unset debug mode (log messages for debugging)



#### notes

- `add`  + `add_pybindwd` `add_pybindwod`  (with or wo duration)
  add a new input note to the enumerator of notes to spell.

- `size`
  number of input notes added to the enumerator of notes to spell.



#### array of tonalities

- `addTon`
  add a ton to the array of tonalities
  the array of tonalities must be open

- `setTonal`  `setModal`

  switch the array of tonalities to tonal / modal mode
  for the computation of Weber distance
  the array of tonalities must be open.
  **attention**: independent from the mode tonal/modal for spelling

- `closeTons` 
  close the array of tonalities and finish its initlialization 
  (computation of a matrix of Weber distances)

- `closedTons`
  the array of tonalities is closed (no ton can be added)

- `resetTons`

  clear the array of tonalities and open it.

- `nbTons`
  size of the array of tonalities

- `ton(i)`
  ton of given index in  the array of tonalities



#### prepare spelling

- `sampleCost` 
  selection from constants  `A` | `Adiscount` | `ADplus` |  `ADlex`
  `costA` is with or without discount for lead degrees
- `setMode`  flag or constant `TONAL` |  `GLOBAL`
- `setChromatic` flag

**Remark**:  redundancy or conflicts between `A`/`Adiscount` and `TONAL`/`GLOBAL` ?



#### spelling

- `evalTable1` : compute `table1`
  using the 3 parameters set in "prepare spelling"
  
- `evalGlobal`  options: full, diff%  using
  
  - `table1`  if `table2` doest not exist
  - `table2`  if it exists
  
- `evalGrid`    using 
  
  - `table1`
  - `global` (select rows to evaluate)
  
- `evalTable2`  using 
  - the 3 parameters set in "prepare spelling"
  - `table1`
  - `grid`
  - `global` (select rows to evaluate)
  
- `rename(iton)`

  rename all notes def to the speller,
  according to given global tonality index. 
  using `table1` or `table2` if is exists.

- `rewrite`   

  rewrite passing notes
  after `rename`



#### results

- `name(i)`
- `accidental(i)`
- `octave(i)`
- `printed(i)`
- `globals`
  size of `global1` or  `global2` if it exists

- `global(i)`
  `i`th tonality in  `global1` or  `global2` if it exists
- `iglobal(i)`
  index of `i`th tonality in  `global1` or  `global2` if it exists

- `local(i, j)`

  local ton in `grid`

- `ilocal(i, j)`
  index of local ton in `grid`
- timings for preparation of TonIndex, construction of tables, grid, globals...
