## Single bar computation

Let us call *state* $q$ a mapping from note names to accidentals.  
Intuitively, it follows the principle that accidentals are not recalled within a measure...
For instance...

A *configuration* is a pair $C = (q, S)$, where 

- $q$ is one state and 

- $S$ is a set of simultaneous notes (notes with the same onset).
  When $S = \emptyset$,  $C$ is called an *initial configuration*.

A *transition* $\tau$ between two configurations $C = (q, S)$ and $C' = (q', S')$ is valid when

- the onset of $S'$ is after the onset of $S$, 
- the state $q$ is updated into $q'$ according to $S'$.  

In this case, several cost values are associated to the transition, 
some of which are defined wrt a tonality $t$:

- graphical costs:
  - $ga(\tau)$: number of printed accidentals in $S'$ = the size of $q' \setminus q$,
    except for lead notes of $t$...
  - $gd_t(\tau)$: the distance $d(q', t)$ 
  - $gc_t(\tau)$: the number of notes in $S'$ not in the chromatic harmonic scale of same tonic as $t$.  
    when $t$ is a subset of this scale?  
    redundant with the previous?
  - ~~$gc_t(\tau)$: nb of accidentals of $s' \setminus s$ of $\neq$ color than $t$~~.
- horizontal cost:  
  - $h(\tau)$: nb of non-diatonic moves, when $S$ and $S'$ are singleton set  
    REVISE see `src/ps13/README.md`
- vertical cost:  
  - $v(\tau)$ whether $|E'| > 1$ and chord uncategorizable...

path = sequence of transitions and...  
extension of the notion of cost of path.

*Shortest path* for a ton $t$ and a measure $B = \bar{E}$ = 
smallest cost foor path 
$(s_0, \emptyset), (s_1, E_1),\dots, (s_n, E_n)$, 
starting from one initial config $C_0 = (s_0, \emptyset)$, 
where $s_0$ is defined by the time signature of the tonality $t$, 
and 

## Procedure

Let $T = \{ t_1, \ldots t_n\}$ be a set of tonalities consider for Pitch Spelling.

1. Table Initialization.  
   Compute a bag $SP[i][j]$ of shortest paths for each tonality $t_i \in T$ and each bar $B_j$, $j \in 1..m$.
   For the computation of the $SP$, we consider the cost measure which is the lexicographic composition of
   
   - $gca$ approx.
   - $h + v$.  
   - $gd_{t_i}$ or $gc_{t_i}$...?  
     There might be several paths of identical cost for each $t_i$ and $B_j$.  
     [other option] cost = sum of the 2 above.

2. Estimation of **candidates global** tonalities, according to
   
   - the measure of 1 (summation of costs of best paths on each row), and in case of tie (or almost tie)

3. Estimation of one **local** tonality $\ell(t, j)$ for each bar $b_j$ and each candidate global tonality $t$, as:
   
   - $\ell(t, j) = t_i \in T$ such that $SP[i][j]$ has minimal cost, and in case of tie:
     - the distance to estimated local for previous bar and $t$ (or distance to $t$ for first bar).
     - the distance to $t$.

4. partial re-computation of the table $SP'$: for every global candidate $t_i$,  we add to $SP[i][j]$ components for 
   
   - $gd_{\ell(t_i, j)}$ or/and $gc_{\ell(t_i, j)}(\tau)$

5. choice of one estimated **global** tonality $t_g$ ($1 \leq g \leq n$), amongst the candidates.  
   according to the above distance measure (total for the resp. rows of each candidate).

6. spelling, based on the row of $SP'$ corresponding to $t_g$.  
   chose global ton. = 1 row, and for each measure in this row, the shortest path defines a spelling.
