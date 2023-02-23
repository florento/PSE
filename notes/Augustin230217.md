# Pitch Spelling

réunion avec Augustin 
17 février 2023 9:00
CNAM

## 1. misc

### complexité

- pb. des accords (cf. sonate Waldstein)

- pb des long chromatismes (Chopin etc)
  à la main droite en général

- [ ] traitement différencié main droite - main gauche dans le cas du piano?
  
  analyse tonale pas nécessaire pour un passage chromatique

- [ ] détection et traitement spécial ? (pour éviter explosion)

### grace notes

- [ ] prises en compte ?
  oui mais pas comme accord (séparées de notes préc. et suivante)

### octaves et rappel

dans la même mesure, un accident doit être rappelé quand la seconde occurrence n'est pas à la même octave

- dans l'algo, on ne compte dans le `cost` qu'1 alteration
  c'est plus pertinent pour la recherche de tonalité
- mais la seconde  altération (rappel) doit être marquée du flag `print`

### intervalles

2d augmentée interdite comme intervale mélodique (?)

cf. Danhauser

## 2. mouvements mélodiques conjoints

autre critère de coût pour mouvement horisontaux
principe: viser la **résolution**

### définition mouvement conjoint

- distance ≤ 2 demi-tons 
  - [ ] ou 3 demi-tons ?
- mouvement horizontal 
  - entre 2 notes
  - entre 1 note $n_1$ et 1 interval dont la note la plus proche de $n_1$ est à distance ≤ 2 demi-tons 
    et inversement entre 1 interval et 1 note, dans les même conditions
  - entre une note $n_1$ et un interval $(n_2, n'_2)$ tel que $d(n_1, n_2) = d(n_1, n'_2)$ on ignore

### valeur de coût

pour le spelling de $n$, 

- on regarde le mouvement  entre $n$ et la note suivante $n'$ ($n'$ peut-être dans un interval).

- si il est conjoint, on privilégie le mouvement diatonique (intervale mélodique de seconde)
  i.e. on pénalise un autre mouvement (unison ou tierce)

- ~~si le mouvement est ascendant, on choisit la plus grande des altérations possible ~~
  ~~si le mouvement est descendant, on choisit la plus petite des altérations possible ~~
  contrex:  do do# mib

- [ ] énumérer tous les cas possibles

- [ ] indépendant du contexte? (notes avant $n$ et apres $n'$)

### notes étrangères

cf. Danhauser

- retard
  exception du retard à la tierce
- appogiature
- broderie
- échappée

les 3 premières sont des mvts conjoints

### place dans algorithme

- comme un des critères de l'étape 3 (spelling final, tonalité locale connues)
  combinaison lexico avec les autres critères
- calcul initial de la table (avant de la déterminaison de la tonalité globale)
  - combiné avec le coût du nb altération
  - combinaison lexico ou somme ?

### exemple

> vent d'hiver
> Chopin

## 3. accords

intervales harmoniques :

- spelling accord (parcour/construction de `PSC2`)
- calcul des intervalles entre basse et autres notes (sans répétition)
  = chiffrage
- détermination du coût 
  - faible si on a un des chiffrages usuels
    altérés (e.g. 6te allemande, 6te française) ou non
    - [ ] écrire la table de ces chiffrages
  - forte si l'accord n'est pas chiffrable    

ex: 6te augmentée, [3 cas](https://fr.wikipedia.org/wiki/Sixte_augmentée)

- 6te française  
  lab - do - ré - fa# (en do)

> Duparc 
> l'invitation au voyage
> https://youtu.be/Vwjzp1tNuKI
> https://www.radiofrance.fr/francemusique/podcasts/musicopolis/1870-henri-duparc-compose-l-invitation-au-voyage-6855823

- sixte allemande 
  lab - do - mib - fa#

- sixte italienne
    lab - do - fa#

- sixte suisse = sixte allemande avec quarte sur-augmentée 
  lab - do - ré# - fa#
  pour faire suivre d'un accord de 4 et 6 sol - do - mi  (ré# se résoud sur le mi)

http://musictheory.pugetsound.edu/mt21c/TypesOfAugmentedSixthChords.html

## 4. tonalités locales

actuellement, on détermine une tonalité locale par mesure
donc les bornes de tonalités locales sont les mesures

détermination d'autres bornes?

- sur une note/accord (i.e. onset) qui n'appartient pas à la tonalité globale
- limitation au temps ?

algo de tonalité locale indépendant (de la table)

- détermination des bornes
- entre 2 bornes, calcul du plus court chemin (bag de bests) pour chaque tonalité
- détermination de la tonalité locale comme avant:
  - minimisation du nb altérations
  - minimisation de la distance à la tonalité locale précédente (ou globale pour le début)
  - minimisation de la distance à la tonalité globale (3 ieme critère lexico)

## 5. tie breaks

### tonalité globale

- nb minimal altérations (écrites) - sur une ligne
- min abs(KeySig)

### tonalité locale

dans une mesure (ou une zone au sens de §4)

- nb minimal altérations (écrites) 
- min dist(ton. locale précédente)  (ou min dist(ton. globale) pour la premiere zone).
- min dist(ton. globale) 

## spelling

sur la ligne de la tonalité globale,
dans une mesure

- nb minimal altérations (écrites) 

- min nb mouvement mélodiques conjoints non-diatoniques

- distance(state, ton. locale)
  indépendant du choix de tonalité locale ?

- couleur (?)
  ex. pb: en G maj, Bb préférable à A#

- [ ] nb de tie break fail dans les tables

## 6. plans

### quantitatif

tables de résultats, avec 

- tonalité globale estimée et réelle (err. ou non)
- nb erreurs de spelling
- tie breaks

### qualitatif

sortie pdf de la partition avec

- tonalité globale estimée
- erreurs de spelling en rouge
- tonalités locales par mesure
- tie breaks (?)

## refs

> Théorie de la musique
> A. Danhauser
> 1889

- https://fr.wikisource.org/wiki/Théorie_de_la_musique_(Danhauser,_1889)/Texte_entier (online)
- https://archive.org/details/thoriedelamusiqu00danh/page/6/mode/2up (embedded)
- https://gallica.bnf.fr/ark:/12148/bpt6k9627276t/f21.item.texteImage (embedded)
- https://vdoc.pub/download/theorie-de-la-musique-2iq633ieptq0 (pdf)
- https://imslp.org/wiki/Théorie_de_la_musique_(Danhauser,_Adolphe) (pdf)
- https://www.laflutedepan.com/partition/41545/adolphe-danhauser-theorie-de-la-musique-solfege-formation-musicale.html
