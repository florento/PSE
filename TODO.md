# PSE
Pitch Spelling Algorithms
based on optimization of Engraving-based info.


---
29 nov. 2022
visite Augustin
premiers résultats d'évaluation Pitch Spelling'

**tonalités**:
- ajouter mode mineur mélodique ascendant
  ascendant: pour degrés 5-6-7-1
- modes jazz (dorien etc) : pour PS jazz?
  dataset FiloBass, FiloSax, Weimar (Jazzomath)



## todo (court terme)

- [x] ajout critère "disjoint" au coût 
     nb mouvements mélodiques non-conjoints alors qu'ils pourraient l'être

- [ ] respell avec tonalité globale donnée
      `setGlobal()` dans `PST` (complémentaire `estimateGlobal()`) 
      flag global est connu
      pas d'appel auto à  `estimateGlobal()` dans `rename()`

- [x] pb de tonalité (`Key`) non trouvée dans M21
      erreur ` -1 key changes FAIL` 
      = presque tous index pas dans  `LG_ok` `LG_assert`  de `LG_ok.py`
      explorer les partitions à probleme

- [x] diff (python) : ne pas compter les bécarres affichés (flag `print` = true)

- [x] debug les crashs 
      cf  `LGassert`   dans `LGok.py`

- [ ] ajouter mode mineur mélodique ascendant
  ascendant: pour degrés 5-6-7-1
  
- [x] table de Weber: distance entre tons  


## todo (moyen terme)

  
- [ ] autoriser modulations? en nombre borné?

- [ ] modes jazz (dorien etc) : pour PS jazz?


## experimentations

- [ ] datasets FiloBass, FiloSax, Weimar (Jazzomath)
- [ ] dataset Meredith (subsety of Musedata)
- [ ] dataset ASAP: subset of Francesco et al.  
- [x] dataset ASAP: Bach
- [x] dataset ASAP: Beethoven
- [ ] dataset ASAP: Chopin
- [x] dataset Lamarque Goudard
- [x] bundles M21


