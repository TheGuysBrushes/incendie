#include "firewidget.h"
#include <QtGui/qevent.h>

using namespace std;

// Constructeur
FireWidget::FireWidget(int hauteur, int largeur, float proba, float coef_brulure)
{
	foret = new Foret(hauteur,largeur,proba,coef_brulure);
	buffer = new QImage();
	color = new QColor(Qt::black);
	
	setMinimumSize(600,600);
	
	probaMatriceReset= proba;
}

// Destructeur
FireWidget::~FireWidget()
{
	delete(buffer);
	delete(color);
	delete(foret);
}

// void FireWidget::newForet(int hauteur, int largeur, float proba, float coef_brulure)
// {
// 	
// }


//Autres méthodes
void FireWidget::setColor(int colorIndice)
{
	switch(colorIndice){
		case 0:
			this->color->setRgb(1,100,0,255);
			break;
		case 1:
			this->color->setRgb(34,139,34,255);
			break;
		case 2:
			this->color->setRgb(107,142,35,255);
			break;
		case 3:
			this->color->setRgb(0,205,0,255);
			break;
		case 4:
			this->color->setRgb(46,139,87,255);
			break;								
	}
}

// #################
//		Affichages
// #################

/**
 * Dessine les arbres et cellules vides dans le buffer
 * @author Florian and Ugo
 */
void FireWidget::drawForest()
{	
	QPainter paint(this->buffer);
	
	int current_largeur= 0;
	for(int i=0; i<this->foret->largeur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= foret->operator[](i);
		
		int current_hauteur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if( cell->getEtat() == 0){
				this->color->setNamedColor("black");
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
			}
			else if(cell->getEtat() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				Arbre* ab= dynamic_cast < Arbre* >(cell);
				unsigned indice= ab->getEssence()->getIndice();
// 				int brulure= ab->getEssence()->get;
				setColor(indice );
				#if DEBUG_TMATRICE
				cout <<"draw cell ; ";
				#endif
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
			}
			
			// Incrémentations des positions des cellules
			current_hauteur += tailleCell;
		}
		#if DEBUG_TMATRICE
		cout << endl;
		#endif
		current_largeur += tailleCell;
	}
	
	#if DEBUG_TMATRICE
	cout <<"fin draw forest ; "<< endl;
	#endif	
}

/**
 * Dessine la matrice dans le buffer, sur l'ancienne matrice
 * 	On réutilise les cellules non susceptibles d'avoir été modifiées
 * @author Ugo and Florian
 */
void FireWidget::drawFire()
{
	#if DEBUG_TMATRICE
	cout <<"draw fire "<< endl;
	#endif
	
	QPainter paint(this->buffer);
	
	int current_largeur= 0;
	for(int i=0; i<this->foret->largeur(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= foret->operator[](i);
		
		int current_hauteur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;

			if(cell->getEtat() == 2){
				this->color->setNamedColor("red");
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
				
			}else if(cell->getEtat() ==3){
				this->color->setNamedColor("gray");
				paint.fillRect(current_largeur, current_hauteur, tailleCell, tailleCell, *(color));
			}

			#if DEBUG_TMATRICE
			cout <<"draw arbre feu ; ";
			#endif

			// Incrémentations des positions des cellules
			current_hauteur += tailleCell;
		}
		#if DEBUG_TMATRICE
		cout << endl;
		#endif
		current_largeur += tailleCell;
	}
	
	#if DEBUG_TMATRICE
	cout <<"fin draw fire" <<  endl;
	#endif
}

/**
 * IMPROVEIT
 * J'ai essayé de créer une fonction qui dessinerai les cellules grâce à l'appel d'une autre fonction donné en paramètre,
 * 	mais "function/std::function/function::function avec #include <functional> ne marche pas
 * @deprecated
 * @author Florian
 */

/**
 * premiere fonction, il aurait fallu créer drawStatic ...
 */
// void FireWidget::drawVariable(int posWidth, int posHeight, const Cellule* cell)
// {
// 	int cell_larg = width() / this->foret.largeur();
// 	int cell_haut = height() / this->foret.hauteur();
// 	
// 	QPainter paint(this->buffer);
// 	
// 	if( cell->getEtat() == 0){
// 		this->color->setNamedColor("black");
// 		
// 	}else if(cell->getEtat() == 1){
// 		// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
// 		unsigned indice= dynamic_cast < const Arbre* >(cell)->getEssence()->getIndice();
// 		setColor(indice);
// 	}
// 	paint.fillRect(posWidth, posHeight, cell_larg, cell_haut, *(color));	
// }


// void FireWidget::drawForest(void(*pDraw)(int, int, const Cellule*) )
// {
// 	int cell_larg = width() / this->foret.largeur();
// 	int cell_haut = height() / this->foret.hauteur();
// 	
// 	int current_largeur= 0;
// 	for(int i=0; i<this->foret.largeur(); ++i){
// 		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
// 		// éviter la petite zone en bas de grille
// 		vector< Cellule* >* ligne= foret[i];
// 		
// 		int current_hauteur= 0;
// 		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
// 			Cellule* cell= *j;
// 
// 			pDraw(cell_larg,cell_haut, cell);
// 			
// 	// Incrémentations des positions des cellules
// 			current_hauteur += cell_haut;
// 		}
// 		current_largeur += cell_larg;
//     }
// }


bool FireWidget::eteindreFeu(int ligne, int colonne)
{
	#if DEBUG_ALLUME
	cout << "eteindre cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
			
			if (cell->getEtat()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->water(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

bool FireWidget::allumerFeu(int ligne, int colonne)
{
#if DEBUG_ALLUME
	cout << "allumer cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
		
			if (cell->getEtat()==1){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->allumer(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

bool FireWidget::finirFeu(int ligne, int colonne)
{
	#if DEBUG_ALLUME
	cout << "embraser cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif
	
	if(ligne>=0 && ligne < foret->hauteur()){
		vector< Cellule* >* line= foret->operator[](ligne);
		
		if (colonne>=0 && colonne < foret->largeur()){
			Cellule* cell= (*line)[colonne];	
			
			if (cell->getEtat()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				foret->eteindre(ab);
				drawFire();
				update();
				
				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}


// #############
// 	Events
// #############
void FireWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer); // , 0, 0, width(), height());
}

void FireWidget::resizeEvent(QResizeEvent* event)
{	
	if (!buffer->isNull()){
		delete(buffer);
	}
	
	int tailleCote= min(event->size().width(), event->size().height());
	#if DEBUG_TMATRICE
	cout << "tH: "<< event->size().width()<< " tL "<< event->size().height()<< " ; max donne taille coté : " << tailleCote<< endl;
	#endif
	
	// 	resize(tailleCote,tailleCote);
	
	float nbMax = max(foret->largeur(), foret->hauteur());
	
	tailleCell = tailleCote /	nbMax;

	buffer = new QImage(tailleCote,tailleCote, QImage::Format_ARGB32);
	buffer->fill(Qt::white);
// 	void(*pDraw)(int, int, const Cellule*);
// 	pDraw= drawVariable;
	drawForest();
	drawFire();
	// 	update(); // deja fais apres l'appel à resizeEvent ?
}

void FireWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event); // TODO verifier si à supprimmer
	
	int colonne= event->y()/tailleCell;
	int ligne= event->x()/tailleCell;
	
	if (event->button()==Qt::LeftButton)
		allumerFeu(ligne, colonne);
	else if (event->button()==Qt::MiddleButton)
		finirFeu(ligne, colonne);
	else if (event->button()==Qt::RightButton){
		eteindreFeu(ligne, colonne);
		drawForest();
	}
}


void FireWidget::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);	// TODO verifier si à supprimmer
	
	int colonne= event->y()/tailleCell;
	int ligne= event->x()/tailleCell;
	
	if (event->buttons().testFlag(Qt::LeftButton) )
		allumerFeu(ligne, colonne);
	else if (event->buttons().testFlag(Qt::MiddleButton) )
		finirFeu(ligne, colonne);
	else if (event->buttons().testFlag(Qt::RightButton) ){
		eteindreFeu(ligne, colonne);
		drawForest();
	}
}





bool FireWidget::next()
{
	// pas suivant
	if (!foret->NextMove())
		return false;
	
	// mise à jour de l'image puis affichage à l'écran
	drawFire();
	update();
	
	return true; // cas par défaut, il y a eu un changement
}

/**
 * reinitialise la foret
 * @author Florian et un petit peu Ugo :p
 * BUG ne remet pas à zero le compteur ...
 */
void FireWidget::restart()
{
	Foret* OldForet= foret;
	foret = new Foret(*OldForet, probaMatriceReset);
	delete(OldForet);
	// il faudrait en plus vider la matrice de feu ...
// 	foret->randomMatrice(probaMatriceReset); 
	// plante
// 	foret->initialisation(probaMatriceReset);
	drawForest();
	drawFire();
	update();
}

