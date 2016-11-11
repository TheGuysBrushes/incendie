# Suivi de l'avancée du projet
## Semaine 1

### Mardi
  - Réfléxion commune 

### Mercredi
  - Flo : génération et affichage matrice
  - Ugo : Cahiers des charges et réflexion

### Jeudi
  - Flo : déclenchement d'un incendie et propagation simple 
  - Ugo : réflexion sur SDD et != arbres

### Vendredi
  - Flo : Passage des fonctions via pointeurs et nettoyage du code 
  - Ugo : reflexion sur la modélisation des !=arbres

##Semaine 2

### Lundi
  - Flo :
    + les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
    + la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
    + faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simple
    + Nettoyage du code et commentaire.

  - Ugo : Implémentation de la notion d'essence.

### Mardi
  - Intégration de la notion d'essence dans une forêt (Gestion moteur et graphique)
  - Gestion couleurs != abres
    + Florian : Cellule adjacentes en diagonale

### Mercredi
  - Calcul et prise en compte des points de vie en fonction des paramètres de l'essence et des caractères discrets de l'arbre.

### Jeudi
  - Sélection aléatoire essence avec notion de bosquet. Nettoyage du code, debugage, tests mémoires ( concluant).

### Vendredi
  - Ugo : Transmission liée à l'humidité, réflexion sur modification système pv. 
  - Florian  : Recherches sur vents + creation de la classe et des opérations sur les vecteurs de vents

##Semaine 3

### Lundi
  - Mise en place de l'intégration Qt.
    + Florian : création de quelques primitives de calculs pour le vent.

### Mardi
  - Premières optimisations de l'affichage de la foret + bouton pas à pas.
    + Ugo : ajout de différentes couleurs d'arbres.
    + Florian : ajout des clics de la souris pour enflammer des arbres 
    => Problème étrange lorsqu'on "glisse" la souris.
    Résolution : on s'est aperçu que l'application plantait lorsque la souris sortait de la foret, on tentait d'accéder à une cellule qui était en dehors de la matrice : ajout d'une vérification qu'il y a une cellule à la position demandée.
 
### Mercredi
  - Mise en place de l'interface, enrobage graphique.
    + Florian : Correction du calculs de PV et des essences, optimisations.

### Jeudi
  - Ugo : Ajout de fonctionnalités de l'interface.
  - Florian : Modification du redimensionnement de la foret et optimisation du déroulement de l'affichage : Ajout dans la foret d'une liste d'arbre qui viennent de s'eteindre suite à une combustion total.

### Vendredi
  - Gestion des tailles minimales et maximales de la fenetre selon la taille de la foret et de l'écran.
  - Organisation du code.
  - Passage en revue des commentaires "inutiles" du code, nettoyage et ajout de commentaires.
    + Ugo : Amélioration du bouton reset.
    + Florian : ajout dans la foret d'une liste d'arbre qui viennent de s'allumer pour optimiser l'affichage, voir si ce sera toujours pertinent lorsque l'on ajoutera des "phases de brulure" aux arbres.
  - Florian : Mise en page du readme et ajouts d'étapes et détails.

## Semaine 4

### Lundi
  - Ugo : Réfléxion sur la mise en place des vents, reprise de la classe dédiée et améliorations
  - Florian : mise à jour du code et nettoyage à partir des TODO et autres commentaires

### Mardi
  - Ugo : Implémentation et tests de l'algorithme de transition avec prise en compte du vent.
  - Florian : homogénisation du lors de l'utilisation de Colonnes / Lignes et Width / Height

### Mercredi
  - Ugo : Mise en place de la dynamisation du vent au cours du temps via widget dédié.
  - Florian : Divers correctif de dimension, factorisation du code pour la création et la recréation des fôrets.

### Jeudi
  - Samed Incorporation du widget WindWidget, mise en place, tests et détection des bugs.
  - Debut des recherches de possibilité d'implémentation des images satellites et de sauvegarde/création de matrice à partir d'images.

### Samedi
  -Florian : Premiere implémentation basique de test d'ajout d'image dans le programme.

## Semaine 5

### Lundi (4/05)
  - Ugo : Correction des bugs sur le vents, détection de nouveaux bugs, clarification du code
  - Florian : Travaux sur la matrice d'arbres avec des images

###  Mardi (5/05)
  - Florian : Ajouts et modif pour les couleurs d'arbres et les dimensions de la fenetre, commentaires et corrections.
  - Ugo : Nettoyage du code, réflexion sur le système de coupure.

###  Mercredi (6/05)
  - Ugo : Mise en place de la zone de selection et des bouttons coupure/retardateur
  - Florian : homogenisation de la langue du code (anglais pour variables et méthodes de classes) et modif diverses

###  Jeudi (7/05)
  - Ugo : Liaison couche graphique/matrice pour la coupure (point à détailler dans le rapport SIGNALS/SLOTS ) 
  - Florian : Angliscisation du code et ajout possibilité de suppression des arbres.

###  Vendredi (8/05) (férié)
- Ugo : Réflexion sur implémentation coupure/retardateur au sein du moteur
- Florian : Organisation des fichiers en dossiers et essais d'amélioration DES CMakeLists, moteur indépendant de l'affichage.

## Semaine 6

###  Lundi (11/05)
- Ugo : Mise en place coupure/retardateur dans moteur
- Florian :
  + Modification des setteurs et signaux de vents (angle et force)
  + commentaires et TODOS

###  Mardi (12/05)
- Ugo : Reprise du système d'orientation du vent, mise du 0 au Nord.
- Florian :
  + Mode français/anglais de l'interface.
  + Ajout de l'aléatoire dans la transimission du feu, probabilité selon l'humidité de l'arbre.
  + Correction de la transmission aux arbres en contact direct et ajout d'une décrémentation de la valeur de transmission du feu proportionnelement à la distance à l'arbre en feu à partir d'une récursion, ATTENTION coûteux en ressources, retiré ensuite en attendant de vérifier la pertinence.

###  Mercredi (13/05)
- Ugo : Correction différents bugs, tests graphiques pour coupure/retardateur
- Florian :
  + Corrections : quelques inversions colonne/ligne (burnAdjacentsWind), signaux doubles, et intensité de la transmission du feu selon distance (avec i*j)
  + Recherche de possibilité et première ébauche d'enregistrement d'une foret dans un fichier(binaire).

### Jeudi (14/05)(férié)
- Ugo : férié, indisponible
- Florian : Ajout ébauche de chargement d'une forêt.

###  Vendredi (15/05)(pont)
- Ugo : indisponible
- Florian : Première version de la sauvegarde et du chargement de la fôret finalisée, ajout d'une barre de progression triviale. (Je laisse à Ugo le soin de s'occuper d'implémenter proprement dans l'interface les boutons de saave/load)

###  Samedi (16/05)
- Ugo : Amélioration direction vent,comment pour TODOs

## Semaine 7

### Lundi (18/05)
- Ugo : Validation coupure/retardateur, correction "problème" KDev (TODO et autres)
- Florian : Commentaires, corrections d'erreurs, TODOs de fais, optimisation pour performances et ajout d'un fichier avec ajout de remarques pour le rapport.

### Mardi (19/05)
- Ugo : Correction marqueurs KDev (TODOs, IMPROVE_IT Etc)
- Florian :
  + Autres TODOs de fais, corrections diverses
  + Essai de sauvegarde/chargement de la chaine de caractère du nom de l'Essence
  + Reprise des recherches pour création d'une forêt à partir d'une image et lecture de l'intensité de vert pour chaque pixel.

### Mercredi (20/05)
- Ugo : Mise en place varation automatique du vent avec bouton d'activation. Début rapport
- Florian :
  + Correction QProgressBar
  + Début de création d'une fôret à partir d'une image.

### Jeudi (21/05)
- Florian : Mise à jour du Readme et ajouts pour commencer le rapport.

### Vendredi (22/05)

## Semaine 8

### Lundi (25/05)(férié)
    
### Mardi (26/05)
    
### Mercredi (27/05)
    
### Jeudi (28/05)
    
### Vendredi (29/05)
