




## Single bar computation
Let us call *state* a mapping from note names to accidentals.  
Intuitively, follows the principle that accidentals are not recalled within a measure...
For instance...

A *configuration* is a pair $C = (s, E)$, 
where $s$ is one state and $E$ is a set of simultaneous notes (notes with the same onset).
When $E = \emptyset$,  $C$ is called an *initial configuration*.

A *transition* $\tau$ between two configurations $C = (s, E)$ and $C' = ('s, E')$ is valid when
- the onset of $E'$ is after the onset of $E$, 
- the state $s$ is updated into $s'$ according to $E'$.  

In this case, several cost values are associated to the transition, 
some of which are defined wrt a tonality $t$:
- graphical costs:
	- $gca(\tau)$: number of printed accidentals, the size of $s' \setminus s$
	- $gcd_t(\tau)$: the distance $d(s', t)$ 
	- $gcc_t(\tau)$: nb of accidentals of $s' \setminus s$ of $\neq$ color than $t$.
- horizontal cost:  
  $hc(\tau)$: nb of non-diatonic move, when $E$ and $E'$ are singleton set and...
- vertical cost:  
  $vc(\tau)$ when $|E'| > 1$ and chord uncategorizable...

path = sequence of transitions and 
extension of the notion of cost of path.


*Shortest path* for a ton $t$ and a measure B = \bar{E}$ = 
smallest cost foor path 
$(s_0, \emptyset), (s_1, E_1),\dots, (s_n, E_n)$, 
starting from one initial config $C_0 = (s_0, \emptyset)$, 
where $s_0$ is defined by the time signature of the tonality $t$, 
and 




## Procedure

1. Table Initialization.  
   Compute a bag $SP[i][j]$ of shortest paths for each tonality $t_i \in T$ and each bar $B_j$, $j \in 1..m$.
   For the computation of the $SP$, we consider the cost measure which is the lexicographic composition of
   - $gca$
   - $hc + vc$
   There might be several paths of identical cost for each $t_i$ and $B_j$.  
   [other option] cost = sum of the 2 above.

2. Estimation of candidate global tonalities, according to
   - the measure of 1 (summation of costs of best paths on each row), and in case of tie (or almost tie)


3. Estimation of one local tonality $\ell(t, j)$ for each bar $b_j$ and each candidate global tonality $t$, as:
	- $\ell(t, j) = t_i$ with the best $SP[i][j]$, and in case of tie:
   	- the distance to estimated local previous bar and $t$ (or $t$ for first bar)
   	- the distance to $t$

4. partial re-computation of the table $SP'$: for every global candidate $t_i$,  we add to $SP[i][j]$ components for 
   - $gcd_{\ell(t_i, j)}(\tau)$
   - $gcd_{t_i}(\tau)$
   - $gcc_{\ell(t_i, j)}(\tau)$
   - $gcc_{t_i}(\tau)$

5. choice of one estimated global tonality $gt$, amongst the candidates.
   according to the above distance measure (total for the resp. rows of each candidate).

6. spelling, based on the row of $SP'$ corresponding to $gt$.
   chosr global ton. = 1 row, and for each measure in this row, the shortest path defines a spelling.




