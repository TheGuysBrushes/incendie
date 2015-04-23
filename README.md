# incendie
Automate de simulation d'incendie

Semaine 1

Mardi : réfléxion commune 

Mercredi : Flo : génération et affichage matrice ; Ugo : Cahiers des charges et réflexion

Jeudi : Flo : déclenchement d'un incendie et propagation simple ; Ugo : réflexion sur SDD et != arbres

Vendredi : Flo : Passage des fonctions via pointeurs et nettoyage du code ; Ugo : reflexion sur la modélisation des !=arbres

Semaine 2

Lundi : Flo : - les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
              - la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
              - faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simple
              - Nettoyage du code et commentaire.
              
        Ugo : Implémentation de la notion d'essence.
        
 Mardi : Intégration de la notion d'essence dans une forêt (Gestion moteur et graphique). Gestion couleurs != abres
         Cellule adjacentes en diagonale
 
 [h1]Mercredi :[/h1] Calcul et prise en compte des points de vie en fonction des paramètres de l'essence et des caractères discrets de l'arbre.
 
 Jeudi : Sélection aléatoire essence avec notion de bosquet. Nettoyage du code, debugage, tests mémoires ( concluant).
 
 Vendredi : Ugo : Transmission liée à l'humidité, réflexion sur modification système pv. 
              Florian  : Recherches survent + creation de la classe et des opérations sur les vecteurs de vents

Semaine 3 :

  Lundi-Mardi : Mise en place de l'intégration Qt, premières optimisations.
 
  Mercredi    : Mise en place de l'interface, enrobage graphique. Florian : Correction du calculs de PV et des essences, optimisations.
  
  Jeudi       : Ugo : Ajout de fonctionnalités de l'interface.
              - Florian : Modification du redimensionnement de la foret et optimisation du déroulement de l'affichage : => Ajout dans la foret d'une liste d'arbre qui viennent de s'eteindre suite à une combustion total, 
  
 
