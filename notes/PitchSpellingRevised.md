# Pitch Spelling Revised

#qparse 

revision of algos for Engraving-based Pitch Spelling 


## classes

- **AltState** (1 alteration (in -2..2 + UNDEF) / note name i 0..6)
  
  - constructor(ton, true) : = key sig.
  - constructor(ton, false) : jokers for minor (sensible…)  
     not counted in cost

- **PSC0** (PS Config source)
  
  - accid. state
  - [private] cumul nb accid.
  - [private] cumul dist.
  - cost() sum of the 2 above = nb accidents
  - prev
  - alt(name)  (access to state)
  - successor(Pitch, Ton, [queue&])

- **PSC** (PS Config) = PSC0 + 
  
  - name
  - printed (flag)
  - id last note read
  - alt() (from state and name)
  - unfold() = stack of configs to this config

- **PSB** (bag of PS Configs)
  all the configs in the bag have the same weight
  all the configs in the bag have the same source
  
  - cost the cost value of every PS Config

- **PSV** (vertical vector of PSB, one for each tonality)
  
  - static **TON[]** vector of tonalities (some size as the PSV). = order of enum of vector
  - static **ALTSTATEs** key sigs and sensibles for tons in TON.
  - enum : note enumerator
  - first note
  - note after last note
  - constructor(3 above args)
  - local tonality (index in TON) (initially UNDEF)
  - updateLocal(prev._local)  
    collecte ensemble des index de TON de cout min dans la colonne  
    si plusieurs, choisit celle la plus proche de prev._local  
    second tie break ?

- **PSP** (path of PS Configs)  
  idem actuel

## procédure v.0

1. calcul de 1 PSV / mesure.  
   cost des PSC : uniquement # accidents

2. estimation tonalité globale (function hors classe, arg = vec. PSV)  
   f.e. TON : calcul cost cumulé de la ligne correspondante  
   mémorisation de l’index du best

3. a. estimation tonalité locale pour chaque PSV  
   première = globale  
   f.e. PSV : updateLocal(prev)

4. b. calcul pour chaque PSV et TON[globale] du meilleur PSP  
   pour ch. PSV, extrait _b_ = le PSB de TON[globale]  
   cast _b_ en bag de stacks (map de unfold))  
   toutes les stacks de _b_ ont le même top  
   [pop] fait un pop de ch. stack de _b_  
   détermine parmi tous les top celui (nom de note _n_) qui est le plus proche TON[locale du PSV]  
   efface de _b_ tous les stacks avec top ≠ _n  
   _goto [pop]

