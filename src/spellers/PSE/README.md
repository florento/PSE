# PSE algorithm
Pitch Spelling based on measural info and number of printed accidentals.
Systematic exploration of all accidental possible.
exponential in number of notes per measure.

## étapes
(révision de "procédure" ci-dessous)

0. choix d'une liste de **tonalités**
   défault = 30 tons -7..7 maj et min harm.
   choix d'un **énumérateur** de notes (valeur MIDI dans 1..127)

1. calcul **table** de couts (`PST`)
    - 1 **vecteur** (`PSV`) par mesure
    - pour chaque vecteur, 1 **bag** de best config. (`PSB`) par tonalité
    - **config.** (`PSC`) = map note name (C..B) vers accidents (-2..2)
     les configs stockées dans les `PSB` de vecteurs sont les configs finales de plus faible coût dans un parcours des notes de la mesure.
    - le **coût** est la composé lexico des mesures suivantes:
        - nombre d'accidents écrits
        - [AV] nombre de mouvements mélodiques non-conjoints qui pourraient être conjoints?
        mouvements mélodiques conjoint : notes successives avec noms différents (à distance 1).
    - rem: toutes les configs d'un PSB ont le même coût

2. estimation d'une **tonalité globale**, sur la table de coût (`PST`)
    - tonalité minimisant le cumul. par ligne des coûts ci-dessus
    - [ ] améliorer tie breaks
    - [ ] autoriser modulations? en nombre borné?

3. estimation d'une **tonalité locale** pour chaque vecteur (`PSV`, i.e. chaque mesure)
    - c'est l'index (tonalité) dans `PSV` minimisant un mesure lexico faite de:
        - coût ci-dessus
        - distance à tonalité locale de la mesure (`PSV`) précédente
          la première tonalité "précédente" est la tonalité globale estimée en 2.
        - [AV] distance à la tonalité globale?
        - [AV] comparaison entre config. et tonalité ?
    - [ ] nombreux tie breaks...
    - distances entre tonalités: plusieurs choix possibles (les combiner?):
        - table Weber
        - distance de Hamming
        - distance diatonique
        - ...

4. **respell** de la ligne de tonalité globale dans la table. 
   nouveau calcul de plus court chemin 
   - avec **coût** lexico suivant, étendant le coût de l'étape 1:
        - nombre d'accidents écrits        
        - nombre de mouvements mélodiques non-conjoints qui pourraient être conjoints
        - distance (cumulée pour chaque note altérée) à la tonalité locale
        - nb note altérées de couleur différente de la tonalité globale
          couleur = `#` pour tonalité dans 0..7
          couleur = `b` pour tonalité dans -7..-1
    - [ ] tie breaks: peu. exemples?


## procédure
(ancienne version, obsolete. void "étapes" ci-dessus)

0.  calcul de 1 PSV / mesure.  
    cost des PSC : uniquement # accidents

1.  estimation tonalité globale (function hors classe, arg = vec. PSV)  
    f.e. TON : calcul cost cumulé de la ligne correspondante  
    mémorisation de l’index du best

2.  a. estimation tonalité locale pour chaque PSV  
    première = globale  
    f.e. PSV : updateLocal(prev)

3.  b. calcul pour chaque PSV et TON[globale] du meilleur PSP  
    pour ch. PSV, extrait _b_ = le PSB de TON[globale]  
    cast _b_ en bag de stacks (map de unfold))  
    toutes les stacks de _b_ ont le même top  
    [pop] fait un pop de ch. stack de _b_  
    détermine parmi tous les top celui (nom de note _n_) qui est le plus proche TON[locale du PSV]  
    efface de _b_ tous les stacks avec top ≠ _n  
    _goto [pop]
