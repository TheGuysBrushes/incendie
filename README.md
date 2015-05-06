# incendie
<h1>Automate de simulation d'incendie</h1>

<h2>Semaine 1</h2>

<h3>Mardi</h3>
  - Réfléxion commune 

<h3>Mercredi</h3>
  - Flo : génération et affichage matrice
  - Ugo : Cahiers des charges et réflexion

<h3>Jeudi</h3>
  - Flo : déclenchement d'un incendie et propagation simple 
  - Ugo : réflexion sur SDD et != arbres

<h3>Vendredi</h3>
  - Flo : Passage des fonctions via pointeurs et nettoyage du code 
  - Ugo : reflexion sur la modélisation des !=arbres

<h2>Semaine 2</h2>

<h3>Lundi</h3>
  - Flo :
      + les cellules/arbres sont stockés par pointeurs dans le vecteur de vecteurs
      + la cellule ne contient qu'un état, si c'est un arbre (dynamic_cast) il a des attributs spécifiques
      + faire une sous-classe de arbre : arbre en cendres ? ou alors sous-classe de cellule ou seulement arbre dans état brulé (3) : plus simple
      + Nettoyage du code et commentaire.
      
  - Ugo : Implémentation de la notion d'essence.

<h3>Mardi</h3>
  - Intégration de la notion d'essence dans une forêt (Gestion moteur et graphique)
  - Gestion couleurs != abres
    + Florian : Cellule adjacentes en diagonale
 
<h3>Mercredi</h3>
  - Calcul et prise en compte des points de vie en fonction des paramètres de l'essence et des caractères discrets de l'arbre.

<h3>Jeudi</h3>
  - Sélection aléatoire essence avec notion de bosquet. Nettoyage du code, debugage, tests mémoires ( concluant).
 
<h3>Vendredi</h3>
  - Ugo : Transmission liée à l'humidité, réflexion sur modification système pv. 
  - Florian  : Recherches sur vents + creation de la classe et des opérations sur les vecteurs de vents

<h2>Semaine 3</h2>

<h3>Lundi</h3>
  - Mise en place de l'intégration Qt.
    + Florian : création de quelques primitives de calculs pour le vent.
    
<h3>Mardi</h3>
  - Premières optimisations de l'affichage de la foret + bouton pas à pas.
    + Ugo : ajout de différentes couleurs d'arbres.
    + Florian : ajout des clics de la souris pour enflammer des arbres 
    => Problème étrange lorsqu'on "glisse" la souris.
    Résolution : on s'est aperçu que l'application plantait lorsque la souris sortait de la foret, on tentait d'accéder à une cellule qui était en dehors de la matrice : ajout d'une vérification qu'il y a une cellule à la position demandée.
 
<h3>Mercredi</h3>
  - Mise en place de l'interface, enrobage graphique.
    + Florian : Correction du calculs de PV et des essences, optimisations.

<h3>Jeudi</h3> 
  - Ugo : Ajout de fonctionnalités de l'interface.
  - Florian : Modification du redimensionnement de la foret et optimisation du déroulement de l'affichage : Ajout dans la foret d'une liste d'arbre qui viennent de s'eteindre suite à une combustion total.

<h3>Vendredi</h3>
  - Gestion des tailles minimales et maximales de la fenetre selon la taille de la foret et de l'écran.
  - Organisation du code.
  - Passage en revue des commentaires "inutiles" du code, nettoyage et ajout de commentaires.
    + Ugo : Amélioration du bouton reset.
    + Florian : ajout dans la foret d'une liste d'arbre qui viennent de s'allumer pour optimiser l'affichage, voir si ce sera toujours pertinent lorsque l'on ajoutera des "phases de brulure" aux arbres.
  - Florian : Mise en page du readme et ajouts d'étapes et détails.

<h2> Semaine 4 </h2>

<h3> Lundi </h3>
  - Ugo : Réfléxion sur la mise en place des vents, reprise de la classe dédiée et améliorations
  - Florian : mise à jour du code et nettoyage à partir des TODO et autres commentaires
    
<h3> Mardi </h3>
  - Ugo : Implémentation et tests de l'algorithme de transition avec prise en compte du vent.
  - Florian : homogénisation du lors de l'utilisation de Colonnes / Lignes et Width / Height
    
<h3> Mercredi </h3>
  - Ugo : Mise en place de la dynamisation du vent au cours du temps via widget dédié.
  - Florian : Divers correctif de dimension, factorisation du code pour la création et la recréation des fôrets.
    
<h3> Jeudi </h3>
  - Samed Incorporation du widget WindWidget, mise en place, tests et détection des bugs.
  - Debut des recherches de possibilité d'implémentation des images satellites et de sauvegarde/création de matrice à partir d'images.
    
<h3> Samedi </h3>
  -Florian : Premiere implémentation basique de test d'ajout d'image dans le programme.
    
<h2> Semaine 5 </h2>

<h3> Lundi </h3>
  - Ugo : Correction des bugs sur le vents, détection de nouveaux bugs
  - Florian : Travaux sur la matrice d'arbres avec des images<h3> Lundi </h3>
    
<h3> Mardi </h3>
  - Florian  : Ajouts et modif pour les couleurs d'arbres et les dimensions de la fenetre, commentaires et corrections.

<h3> Mercredi </h3>
  -Florian
    
<h3> Jeudi </h3>
    
<h3> Vendredi </h3>



<h2> Semaine X </h2>

<h3> Lundi </h3>
    
<h3> Mardi </h3>
    
<h3> Mercredi </h3>
    
<h3> Jeudi </h3>
    
<h3> Vendredi </h3>
