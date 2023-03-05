
## critères pour algo PSE

For computing a cost value stored in table $PS[i, j]$ where
- $i$ current bar number
- $j$ current ton number

Criteria: ( (1) and (2) are the steps in the algo below)

1. (1) nb of printed accident except leading…
2. (1) names not in chromatic harmonic scale of current tone
3. (1) non diatonic moves for ≤ 1 ton (hor. cost)
4. (1) nb chords non chiffrables (vertical cost)
5. (2) names not in chromatic harmonic scale of local ton of current bar.
6. (1) Cumul distance state / global
7. (2) Cumul distance state / local

Redondance 2-6 et 5-7 ?  
en définissant la distance comme nb noms du state avec accid diff. KS.


## algo

-  (1) computation of all costs $PS[i][j]$, including
   - 1
   - 2
   - 3
   - 4
   - 6

- estimatation **globals**, with criteria
  - 1 approx
  - 3+4
  - ...
  lexico. composition, sum ??

- estimate locals

- (2) Complete costs  with
  - 5
  - 7

- Estimate global, using  
  - 5, 7 (how ?)

- Spell



## question sur pertinence des critères harmoniques et mélodiques
Dans l’état actuel,  un "accords" est des ensembles de notes commençant sur le même onset. C’est bien sûr une vision très restrictive, qui nous fait rater des accords. Donc des notes considérées comme constitutives d'un même accord (en analyse) pourront être traitées comme "mouvement mélodique" dans notre analyse sommaire.  
Inversement, on va considérer des groupes qui ne sont pas de vrais accords.?

Du point de vue analyse harmonique, c'est bien sûr très limité.
Donc est que des notes considérées comme constitutives d'un même accord (en analyse) pourront être traitées comme "mouvement mélodique" dans notre analyse sommaire.  
Une conséquence est donc l'application d'une pénalité non justifiée, en cas d'intervalle de deux 1/2 tons au plus entre notes non conjointes. Non justifiée: par exemple si il s'agit d'une tierce diminuée - est-ce que c'est possible?

D'autre part, il existe de nombreux cas de secondes dans des accords de la gamme (renversements de 7e mineure, majeure, dim, 9e de dominante...) ou altérés (6te augmentées...). Mais dans ce sens, cela ne devrait pas poser de problème.