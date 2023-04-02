# David Meredith's PS13 pitch-spelling algorithm

## first step
for a pitch class $p$,  
a note $n$ (in the sequence of notes to spell),  
a note name $\ell$ (in A..G)

1. let $C(p, n)$ be the nb of occurrences of $p$ 
   in a window of $K_{pre}$ notes before $n$ and the $K_{post}$ after $n$.  
   This is a rough approx. of Krumhansl and Kessler approach to estimation of tonality.

2. let $L(p, n)$ be the name of $n$ in the chromatic harmonic scale of $p$.

3. let $N(\ell, n)$ = sum of $C(p, n)$ for all $p$ such that $L(p, n) = \ell$.

4. spell $n$ with $\ell$ maximizing  $N(\ell, n)$.

## chromatic harmonic scale

1 b2 2 b3 3 4 #4 5 #6 6 b7 7 8

## second step
correct the "passing notes" 
passing note = middle note in non-diatonic move between 3 close notes.

[Meredith] 6 kinds, characterized by the distance in 1/2 tons between the 3 notes:
- **a.** -1 +1   (broderie 1)  
  ex. `C` `Cb` `C` $\to$ `C` `B` `C`
- **b.** +1  -1  (broderie 2)  
  ex. `C` `C#` `C` $\to$ `C` `Db` `C`
- **c.** -1 -2  (descending 1)  
  ex. `C` `Cb` `A` $\to$ `C` `B` `A`
- **d.** +2 +1  (ascending 1)  
  ex. `A` `Cb` `C` $\to$ `A` `B` `C`
- **e.** -2 -1  (descending 2)  
  ex. `C` `A#` `A` $\to$ `C` `Bb` `A`
- **f.** +1 +2  (ascending 2)  
  ex. `A` `A#` `C` $\to$ `A` `Bb` `C`

could we add 2 more kinds?: chromatic
- **g.** -1 -1  (descending chr.)  
  ex. `C` `Cb` `A#` $\to$ `C` `B` `A#`  
  ex. `B#` `Cb` `Bb` $\to$ `C` `B` `A#`  
  contradicts the rule of thumb:  
  chromatic descending $\to$ b  
  ex. `C` `B` `Bb`  
- **h.** +1 +1  (ascending chr.)  
  ex. `A` `A#` `Cb` $\to$ `A` `Bb` `Cb`  
  contradicts the rule of thumb:  
  chromatic ascending $\to$ #  
  ex. `A` `A#` `B`

correction strategy: 
- left to right? 
- with overlapping?

## parameters

$K_{pre}$ and $K_{post}$ are parameters that have been fixed manually (about 32)  
on Meredith's PS dataset (a subset of Musedata).

## complexity

$O(n . K)$  
linear in nb of notes if $K_{pre}$ and $K_{post}$ fixed.

## refs Meredith

http://www.titanmusic.com/papers/public/ps-ircam.pdf  
(slides)  
a good summary of thepapers of Meredith on the topic.

see also

> David Meredith  
>   The ps13 pitch spelling algorithm  
>   Journal of New Music Research, 2006

> David Meredith  
>   Comparing pitch spelling algorithms on a large corpus of tonal music  
>   Second International Symposium Computer Music Modeling and Retrieval (CMMR) 2004.

> David Meredith and Geraint A. Wiggins  
>   Comparing pitch spelling algorithms  
>   ISMIR 2005

on the use of the chromatic harmonic scale in spelling,  
see also 

> The Chromatic Modal Scale:  
>   Proper Spelling for Tonal Voice-Leading  
>   Jody Nagel  
>   https://www.jomarpress.com/nagel/articles/ChromaticModal.html

see also refs in 

> Francesco Foscarin, Nicolas Audebert, Raphaël Fournier-S'Niehotta  
>  PKSpell: Data-Driven Pitch Spelling and Key Signature Estimation  
> ISMIR 2021  
> https://arxiv.org/abs/2107.14009

---

adaptation to cost measure:

for tonality $i$ (correspondign to the above $p$) andd bar $j$.

- number of notes NOT spelled as in the chromatic harmonic scale of $i$.
  note: it embeds major and harmoninc minor.
