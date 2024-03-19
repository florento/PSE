

## PSState

changement de définition de `PSState` pour gammes non diatoniques

- domaine = noms de notes A..G

- codomaine: changement

  - 1 accident (comme actuellement)
  - ajout valeur `UNDEF`
    ex: état initial de si dans la gamme par ton (pas dans la gamme)
    mise a jour en cas de `UNDEF` : on compte toujours l'accident (même bécarre)

  - ajout possibilité de 2 accidents 
    ex: état initial pour la gamme chromatique harmonique
    ex: état initial pour gamme blues C, D, D#, Eb, E, G, A (s valeurs pour D et E)



## PSTable

optimisations

- [ ] supprimer les enharmonies de l'index
  on ne garde que KS = -5, 5 et 6 (ou -6 ?)




## PSV

colonnes des tables

optimisation calcul de la table 1
(premier constructeur `PST`, qui appelle `init_psvs` à 2 args)
(premier constructeur `PSV`, qui appelle `init_psbs` à 2 args)

- en mode modal, certains tons sont équivalents pour calcul de la table1 (sans ton locaux)
  e.g. C maj (KS=0), D dorien (KS=-1), E phrygien (KS=1) *etc*
  car ils donnent le même état initial (cf. les tables de `Ton.cpp`)
- [ ] limiter le calcul des  meilleurs chemin à un représentant
  e.g. plus petit index dans le `TonIndex`
- [ ] ajouter la relation d'équivalence dans le `TonIndex` ? 
  pour tout ton qui n'est pas un représentant, pointer l'index de son représentant
  `add` : pour tout ton ajouté, vérifier (depuis 0) si il a un équivalent (et pointer le premier trouvé)
- [ ] fonction binaire d'équivalence de tons ?
- [ ] calcul des colonnes (`PSV`) : 
  - ~~par copie (des `PSB`)~~ 
  - ou pointeurs intelligents (partagés): le contenu devient un vecteur de `shared_ptr<PSB>`
  - ou références (sur `PSB` représentant)
  - ou redéfinir `bag(i)` = `bag(representant(i))`

- c'est vrai aussi en mode tonal, pour le calcul de la table 1

- ce n'est pas vrai pour le calcul de la table 2
  (second constructeur `PST`, qui appelle `init_psvs` à 5 args)
  (second constructeur `PSV`, qui appelle `init_psbs` à 4 args)
  parce que le meilleur chemin dépend aussi de la grille de tons locaux

  

## KS

pour toute KS, sélection (dans table 2) de la meilleure ligne avec cette KS

spelling sur cette ligne uniquement
