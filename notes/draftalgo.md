# Pitch Spelling Revised

algo for Engraving-based Pitch Spelling 

## Principe

le choix d'alteration, entre enharmoniques, dépend du contexte musicale dans lequel intervient la note

- mouvement mélodique (ascendant ou descendant)

- tonalité du morceau

- contexte harmonique

→ altération donne souvent une indication sur l'intention du compositeur.

## Representations of notes

We call *part* a polyphonic sequence of notes, organized in measures (bars).
Typically, it represents one staff in CWN.

Every note is given be a spelling, an onset and a duration.  
The two later time values are rational numbers, expressed in fraction of bar.  
We call *grace-note* a note of duration 0 
(i.e. we use the term generically for a note which is part of an ornament).

The *spelling* of a note is made of

- its name in 'A'..'G'
- one accidental in ...
- one octave number in ...

We also consider, for note heights, the alternative representation 
by *MIDI pitch* value, in 1..128, 
and this value modulo 12, called *pitch-class*.

Two notes are called *simultaneous* if they have the same onset and are not grace notes.
For every set $S$ of simultaneous notes, we write $K(S)$ for the set of pitch-classes
of notes of $S$.
The *class* of $S$ is the cardinality $|K(S)|$ of $K(S)$
(it might be smaller than the cardinality $|S|$ of $S$).  
Sets of class 1 are called single note, of class 2 *intervals* and of class 3 and more *chords*.

TBC: note on 12 tons  separation and octave intervals...

Given a tonality in which the part (sequence of notes) is written, 
we assume that every accidental may be printed or not, according to the usual conventions.
It means that we have an additional boolean flag (called *printed*) for each note.

## Tonalities

K.S. and modes...

lead tons in minor modes...

Tonality distance...

## Measures of cost

We consider several measures of cost used below for shortest path computations.  

Let us represent a given part by $\bar{S} = S_1, ..., S_n$, the sequence of sets of simultaneous notes, ordered by onsets.
More precisely, all notes of $S_i$ have the same onset, 
and the onset of $S_{i-1}$ is stricly smaller that the onset of $S_{i}$, 
unless $S_{i-1}$ contains only grace notes (in this latter case the onsets are equal).
It is possible for $S_{i-1}$ to contain several grace-notes, in case of a grace chord.

TBC: note that $S_i$ is not necessarily a chord in the writte score.
It might contain several notes or chords in different voices.

A singleton set $S_i = \{ e \}$ is called single note, 
and we make no distinction below between the singleton set $S_i$
and the note $e$.

TBC: extend the notion to class 1 (and in the def of GC, HC) ?

- (GC) graphical cost:  number of printed accidentals.  
  For every note (in $S_i$), except lead if $t$ in minor modes:
  (GC) is the number of printed accidentals, 0 or 1 or 2.

Depends on the tonality $t$  in which the notes of the part $\bar{S}$ are written.

- (HC) horizontal cost: number of non-diatonic stepwise (conjunct) melodic motions.  
  For every single note $S_i$ such that $S_{i-1}$ is also a single note, 
  and the motion between $S_{i-1}$ and $S_i$ is *conjunct*
  (the difference $d$ of MIDI-pitchs is such that $0 < d \leq 2$):
  the cost (HC) is 0 if names of $S_{i-1}$ and $S_i$ are successive (interval of second), 
  and 1 otherwise.

TBC: extend to the case where $S_{i-1}$ and $S_i$ of class 1 or 2 (interval).

- (VC) vertical cost: number of uncategorizable chords.  
  For a non-singleton set $S_i$ of simultaneous notes, 
  we distinguish as *bass* note (note of $S_i$ lowest pitch)
  and we consider the sequence $I_1,\ldots, I_m$ 
  of intervals from the pitch-class of the bass to the other pitch classes of $K(S_i)$
  (with $m = |K(S_i)| -1$).  
  The cost (VC) for $S_i$ is 0 if the sequence $I_1,\ldots, I_m$ belongs
  to one of the usual chord categories in classical harmony and 1 otherwise.  

Categories: chords of 5, 7, 9 with notes of the scale + inverse + missing root
and some "conventional" chords with accidents (aug. 6te...).

REF: cf classical textbook on music theory: 

- [Claude Abromont](https://www.fayard.fr/musique/guide-de-la-theorie-de-la-musique-9782213609775)
- [Alison Latham](https://www.oxfordmusiconline.com/page/1826)
- Eric Taylor "Music Theory in Practice"
- ?  

other open refs:  

- `muted.io`
- https://open.umn.edu/opentextbooks/textbooks/1101 ...

TBC: details about octaves and class $K$...

TBC: extension to usual jazz chords (i.e. those for which there exists a notation).

The costs measures (GC), (HC), (VC) are extended from one $S_i$ to the sequence $\bar{S}$
by accumulation.

TBC: discussion about such costs GC, VC, HC in Pitch-Spelling literature?

## Shortest path computation

A spelling is a short path in a grace whose nodes are...

Given a sequence of notes fitting in one bar...   
algorithm parameterized by a cost measure $C$ on sequences of notes.

The measure $C$ may be defined using combinations of the above (GC), (HC), (VC)
and also depends on other criteria
(we consider belong criteria associated to estimated global and local tonalities in $T$).

Let us call *state* a mapping from note names to accidentals.  
Intuitively, follows the principle that accidentals are not recalled within a measure...
For instance...

...starting a from source state $s_0$, 
typically defined by the time signature of a tonality, 

### Complexity

exponentional in the length of $\bar{S}$ (notes inside a bar).

we perform an on-the-flight of the graph of states, 
in order to avoid exponential explosion.

remains efficient usually as shown by experiments.
except in extreme cases...

## Procedure

We assume given 

- a fixed array of tonalities $T$
- a sequence of $m$ bars (measures) of written notes in the above representation.

2 parameters: 

- $N$ : one nb for the choice of global tonality
- $C$ : one combination of the above measures  
  (we consider 2 cases $C_\ell$ or $C_+$ defined below).

Spell($N$, $C$):

1. Table computation.  
   Compute a bag $SP[t][b]$ of shortest paths for each tonality $t \in T$ and each bar $b \in 1..m$.
   There might be several paths of identical cost for each $t$ and $b$.  
   For the computation of the table $SP$, we consider a cost measure $C$, wich can be either :
   
       - the lexicographic composition of (GC), (HC) and (VC) (called $C_\ell$),
   
   - the integral sum of  (GC), (HC) and (VC) (called $C_+$).

2. Estimation of a set of $N$ candidate global tonalities $T^g = \{ t^g_1,..., t^g_N \} \subseteq T$.  
   For each tonality $t \in T$, we compute the cumulative sum for the line $t$ in SP, 
   $\sum_{i=1}^{m} cost( SP[t][b])$ and select in $T^g$ the $N$ tonalities with smallest values.

ALT:  no tie brak for global → several (candidate) bests  (no param $N$).
          and cost eq. for nb accid is approximative (2 or 3%)

Next, for all $t^g \in T^g$, do the following.

3. estimation of a local tonality $t^\ell_b \in T$ for every bar $b \in 1..m$.  
   
   - let $t^\ell_0 = t_g$
   - for all $b \in 1..m$, we select $t^\ell_b \in T$ such that has the best cost in the column $(SP[t][b])_{t \in T}$.  
   
   In case of tie break, we use furthermore the lexicographic composition of the 2 following criteria:
   
        - the tonality distance to the previous estimated local tonality $t^\ell_{b-1}$, 
        - the tonality distance to the global tonality $t^g$.

4. Spelling.  
   For all bar $b \in 1..m$, we recompute one best path, following a cost measure defined as a lexicographic composition of:
   
   - $C$
   - the distance between the current state and the estimated local tonality $t^\ell_b$
   - ~~the distance between a spelling and the glocal tonality $t^g$~~
   - 0 if the accidental in spelling has the same color as the glocal tonality $t^g$, 1 otherwise.

The last criterion is a rule of thumb used for tie breaking...

By construction, the best path computed at step 4 belongs to $SP[t^g][b]$.  
Finaly, we select the best spelling of step 4 (according the cost used in this step), 
to all global tonality candidates of $T^g$.

## abstract

We propose a pitch spelling algorithm, associating names to notes given by midi pitch.
Roughly, it involves the computation of a shortest path in a graph, 
minimizing criteria such as the number of written accidentals, 
non-diatonic stepwise melodic motions and uncategorized chords (inclassable, non-chiffrable).
These notions are taken from standard classical textbooks music theory and no corpus training was required for the design of the algorithm whatsoever.
In counterpart, this algorithm is limited to a tonal context and to input notes organised in bars.
This restriction is not found in previous approaches but makes sense for application 
in the context of larger procedures/framework e.g. for music transcription.

Evaluation conducted show...

## Implementation

The algorithm has been implemented in C++, (motivated by efficiency concerns)
for the matter of efficiency and integration in larger systems.
The implementation is OO, with general classes for pitches, intervals… and data structures specific to the algorithm, 
such as classes for states in best path computation, bags/sets of best paths and a table, defined as a sequence of columns (one for each measure bar in input), one column being a vector of bags (one for each tonality considered).

The input must be provided by an note enumerator, associating to each natural number in a segment $i_0..i_1$, a midi pitch, a bar number, and a flag simult…
Therefore, our algorithm can easily be integrated in a larger project. 
This has been done for a MIDI-to-score transcription framework, 
where the timings (in part. the bar boundaries) are computed before pitch spelling.

A Python binding, based on pybind11, was also written and used for evaluation. 
It offers calls (in Python) to methods of the C++ implementation, in particular...
It also allows to observe the spelling produced for the input notes.

## Evaluation

experimentation for several compinations of the cost measures $\{ C_\ell, C_+ \}$ and 
$N=1$ or $N=3$...

datasets: monophonic (complex) and ASAP piano dataset 

results

efficiency

## Discussion

Pertinence des critères harmoniques et mélodiques
résumé de doutes sur ce sujet...

Dans l'approche ci-dessous, les "accords" sont des ensembles de notes commençant sur le même onset. Du point de vue analyse harmonique, c'est bien sûr très limité.
Donc est que des notes considérées comme constitutives d'un même accord (en analyse) pourront être traitées comme "mouvement mélodique" dans notre analyse sommaire.  
Une conséquence est donc l'application d'une pénalité non justifiée, en cas d'intervalle de deux 1/2 tons au plus entre notes non conjointes. Non justifiée: par exemple si il s'agit d'une tierce diminuée - est-ce que c'est possible?

D'autre part, il existe de nombreux cas de secondes dans des accords de la gamme (renversements de 7e mineure, majeure, dim, 9e de dominante...) ou altérés (6te augmentées...). Mais dans ce sens, cela ne devrait pas poser de problème.

## Perspectives

efficiency pbs on some extreme cases:
bars with > 32 notes may result in eval during several seconds / minutes.
-> "light" mode for these case

also possible extensions:

- detection of (written) modulations.
  - DP approaches on the table (detection of a bounded number of change of line minimizing the cost)
  - more subtle musical criteria, like detection of some cadences [ref. Algomus] 
- computation of boundaries for local tonalities better that bars
  - could be the apparition of printed accidentals, but it is not very precise.
- minor natural and melodic modes (added to table of tonalities)
- jazz modes and jazz pitch spelling.
  - evaluation of local tonality estimation
