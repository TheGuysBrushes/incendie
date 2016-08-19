/**
 * Ce fichier contient les macros utilisées pour déclencher une
 * action utilisateur sur la foret. Pour ajouter une nouvelle action,
 * veuillez suivre les instructions suivantes :
 *
 * - définissez une nouvelle macro dans ce fichier
 * - définissez dans la classe Foret l'action correspondante à effectuer sur la matrice
 * - Dans FireScreen::releaseOrdered(), rajoutez la condition sur l'élément
 * 	graĥique (on considère que l'élément graphique dans FireScreen a déjà été ajouté)
 * pour émettre le signal avec la macro correspondante.
 * - Dans FireWidget::actionReceived(int x), rajoutez la condition sur la macro reçue
 * et déclencher la fonction correspondante dans la forêt.
 *
 * @author Ugo
 */

#define CUT		0
#define DELAY	1
