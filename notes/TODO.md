# TODO PSE

## todo (court terme)



- [ ]  eval. et debug chord sur ASAP / Beethoven Sonata



- [ ]  mouvements mélodiques conjoints
  
     voir CR réunions

- [ ] respell avec tonalité globale donnée
  e.g. pour la m.g. quand la m.d. évaluée ? cf. résultats Bach
  
     `setGlobal()` dans `PST` (complémentaire `estimateGlobal()`) 
     flag global est connu
     pas d'appel auto à  `estimateGlobal()` dans `rename()`

- [x] pb de tonalité (`Key`) non trouvée dans M21
  
     errreur ` -1 key changes FAIL` 
     = presque tous index pas dans  `LGok` `LGassert`  de `LGok.py`
     explorer les partitions à probleme

- [x]  diff (python) : ne pas compter les bécarres affichés (flag `print` = true)

- [x]  debug les crashs 
  
     cf  `LGassert`   dans `LGok.py`

- [ ]  ajouter mode mineur mélodique ascendant
  (ascendant pour degrés 5-6-7-1)

- [ ]  pondérer poids des accidents par la durée (en MTU = bars)

- [ ]  idem pour autre métriques (mvt conjoints...)

- [x]  ajout chords : dans enumerator
   dans binding M21

- [ ] write score: afficher (lyrics) spelling sous les erreurs
     (des anomalies d'affichage avec musesscore)

 mesures trop longues: 

- [ ]  enumerator cutoff (constructor?) 

- [ ]  appel dans PSBag

- [ ]  respell le reste à la fin

- [ ]  mode accords "light"  :  tient compte seulement de la basse
  (respell le reste à la fin)

## todo (moyen terme)

- [ ]  autoriser modulations? 
    KS change en nombre borné?

- [ ]  multipart : en cumulant les coût de chaque part sur chaque mesure

- [ ]  modes jazz (dorien etc) : pour PS jazz?

## todo (evaluation)

- [ ]  exp. dataset basslines FiloBass (partos Xavier)

- [ ]  exp. dataset impros FiloSax, Weimar (Jazzomath)

- [ ]  contacter Francesco : Pkspell sur FiloBass / FiloSax

- [ ]  comparer tonalités locales détectées avec grille accords

- [ ]  eval sur ASAP :  identifier les 33 extraits du validation subset de PKspell
   entier ?

- [x]  framework eval sur ASAP/Bach

- [x]  framework eval sur ASAP/Beethoven

- [x]  eval sur L.G.

- [ ]  eval sur bundles Music21

- [ ]  eval sur Musedata (humdrum -> M21)
   dataset PS de Meredith (216)
   entier?

- [ ]  eval PKspell sur L.G.

- [ ]  eval PKspell sur Filobass (48)




