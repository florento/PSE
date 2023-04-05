# Variant of PS13 with tabulation
for quantified input with time signature info (bars). 

### step 1.
for each pitch class in 0..11, 
we build 2 or 3 rows, with, for each row:
- 1 KS,
- 1 tonic note name, 
- 1 mode (major or harm. minor),  

according to the following table:

| pc | major        | minor harm.  |
|:--:|:------------:|:------------:|
|  0 | C(0)         | C(-3)        |
|  1 | C#(7) Db(-5) | C#(4)        |
|  2 | D(2)         | D(-1)        |
|  3 | Eb(-3)       | Eb(-6) D#(6) |
|  4 | E(4)         | E(1)         |
|  5 | F(-1)        | F(-4)        |
|  6 | F#(6) Gb(-6) | F#(3)        |
|  7 | G(1)         | G(-2)        |
|  8 | Ab(-4)       | Ab(-7) G#(5) |
|  9 | A(3)         | A(0)         |
| 10 | Bb(-2)       | Bb(-5) A#(7) |
| 11 | B(5) Cb(-7)  | B(2)         |

in a row for pitch class $c$ and ton $n.a$ (note name, accidental), maj or min:
- spell according to the chromatic harmonic scale of $n.a$
- count the number of printed accidents, with the KS (and mode) for the row.

rem: same algo as PSE, except for computation of successor configs:
only 1 successor config instead of 3 (determinism)

### step 2.
select the row with least number of printed accidents.  
This is the estimated global ton for the piece.

### step 3.
correct the passing note errors as in PS13.

### variant
after step 2 (estimation global),  
find an estimated local ton for each bar, according to (lexico):
- number of printed accidents in bar (for KS and mode of ton)
- distance ton to previous local
- distance ton to estimated global.

then, for each bar:
- spell according to the chromatic harmonic scale of the tonic of local ton
- correct the passing note errors as in PS13 (step 3).

Remark (variant more costly and redundant...):
- step 1 can be replaced by the DP best path computation checking
  all possible spellings
- in case of tie break, we use the chormatic scale criteria.
