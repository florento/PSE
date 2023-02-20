# visite Augustin
29 nov. 2022
Pitch Spelling (premiers résultats)

cf. 
- `PitchSpellingRevised.md`
- `PSEeval.md`



**étapes**
0. calcul table des best configs / cost
1. estimation tonalité globale
   - améliorer tie breaks
   - autoriser modulations? en nombre borné?
2. estimation tonalité locale pour chaque vecteur (mesure)
   - utiliser l'état?
   - tie break:
	   - cost
	   - dist. précédente ton. locale
	   - distance ton. globale [NEW]
3. respell (de la ligne de tonalité globale)
   - tie break:
	   - privilégier les mouvements mélodiques conjoints [NEW]
	     i.e. notes successives avec noms différents


**tonalités**:
- ajouter mode mineur mélodique ascendant
  ascendant: pour degrés 5-6-7-1
- modes jazz (dorien etc) : pour PS jazz?
  dataset FiloBass, FiloSax, Weimar (Jazzomath)



