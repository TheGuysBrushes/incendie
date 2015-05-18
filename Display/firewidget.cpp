#include "firewidget.h"
#include <QtGui/qevent.h>

#define GRAY 	5
#define RED_TRANSPARENT 	6
#define ANTI_RED_TRANSPARENT	7
#define BLUE 8
#define PURPLE 9
#define RED 10

using namespace std;

/* TODO 	1- Création de la zone de selection clics souris
 * TODO 	2- Sur le release, confirmation de la zone et application de l'effet choisi
 * TODO 	3- Pour les effets, deux boutons ( coupure et retardateur ) dont un par défaut activé
 * 
 * TODO	4- utiliser des Qrgba pour définir les couleurs
 * TODO 	4_bis- utiliser buffer->pixel(i, 0); pour changer couleur ?
 */

// ##################################
/*** Constructeur et destructeur ***/
// #################################
/**
 * Constructeur de classe. Initialise les différents pointeurs
 * et fixe la taille minimale du widget
 * @param int _largeur : nombre de colonnes de la matrice 
 * @param int _hauteur : nombre de lignes de la matrice
 * @param float _proba : probabilité qu'une cellule deviennent un arbre
 * @param float _coef : coefficient de combustion de l'incendie
 * @author Ugo et Florian
 * 
 */
FireWidget::FireWidget(int _largeur, int _hauteur, float proba, float coef_brulure): QWidget()
{
	initialise(_largeur, _hauteur, proba, coef_brulure);

	buffer = new QImage();
	color = new QColor(Qt::black);
	bufferPainter= new QPainter();
	loadPicture("../foret_pay.png");
	rubber = NULL;
}

FireWidget::FireWidget(): QWidget(){
	
	buffer = new QImage();
	color = new QColor(Qt::black);
	bufferPainter= new QPainter();
	loadPicture("../foret_pay.png");
	rubber = NULL;
}

FireWidget::~FireWidget(){
	
	delete(forest);
	delete(buffer);
	delete(color);
	delete(bufferPainter);
	delete(pictureForest);
	
}

// ########################
/*** 	Autres Méthodes 	***/
// ########################
/**
 * Fonction agissant comme le constructeur. Permet de gérer la 
 * ré-initialisation de la matrice
 * @author Florian et Ugo
 * @param identique_constructeur
 */
void FireWidget::initialise(int largeur, int hauteur, float proba, float coef_brulure)
{
	forest= new Foret(largeur, hauteur, proba, coef_brulure);
	
	setMinimumSize(largeur, hauteur);
}

void FireWidget::initialise(int largeur, int hauteur, ifstream * file, QProgressBar* PB) 
{
	forest= new Foret(largeur, hauteur, file, PB);
	
	setMinimumSize(largeur, hauteur);
}


/**
 * Méthodes de destruction de la forêt pour gérer la 
 * ré-initialisation de la matrice
 */
void FireWidget::delForest(){
	delete(forest);
}

void FireWidget::saveForest() const
{
	forest->save("foret1");
}


/**
 * Fonction à commenter par son auteur :p
 * @author Florian
 * @param QString ?
 */
bool FireWidget::loadPicture(QString filename){
	
	QImage img;
// 	img.fill(qRgba(50, 50, 50, 255));
	img.load(filename);
	pictureForest= new QPixmap();
	if (pictureForest->convertFromImage(img)){
		#if DEBUG_IMAGE
		cout<< "image chargée"<< endl;
		#endif

		return true;
	}
	else {
		#if DEBUG_IMAGE
		cout<< "image non chargée"<< endl;
		#endif

		return false;
	}
}

/**
 * Eteint un arbre à une position donnée
 * @author Florian
 * @param int/int indices de la colonne et de la ligne de l'arbre à éteindre
 * @return vrai si il y avait un arbre en feu
 */
bool FireWidget::eteindreFeu(int colonne, int ligne)
{
	#if DEBUG_ALLUME
	cout << "eteindre cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif

	if(ligne>=0 && ligne < forest->height()){
		vector< Cellule* >* line= (*forest)[ligne];

		if (colonne>=0 && colonne < forest->width()){
			Cellule* cell= (*line)[colonne];	

			if (cell->getState()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				forest->water(ab);

				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

/**
 * Allume un feu sur un arbre "vivant"
 * @author Florian
 * @param ligne
 * @param colonne ligne et colonne de l'arbre à enflammer
 * @return vrai si il y avait un arbre enflammable
 */
bool FireWidget::allumerFeu(int colonne, int ligne)
{
	#if DEBUG_ALLUME
	cout << "allumer cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif

	if(ligne>=0 && ligne < forest->height()){
		vector< Cellule* >* line= (*forest)[ligne];

		if (colonne>=0 && colonne < forest->width()){
			Cellule* cell= (*line)[colonne];	

			if (cell->getState()==1){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				forest->kindle(ab);

				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

/**
 * Opere une combustion complete sur un arbre en feu (IMPROVEIT faire sur les arbres non en feu ?)
 * @author Florian
 * @param ligne
 * @param colonne ligne et colonne de l'arbre à bruler totalement
 * @return vrai si il y avait un arbre en feu
 */
bool FireWidget::finirFeu(int colonne, int ligne)
{
	#if DEBUG_ALLUME
	cout << "embraser cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
	#endif

	if(ligne>=0 && ligne < forest->height()){
		vector< Cellule* >* line= (*forest)[ligne];

		if (colonne>=0 && colonne < forest->width()){
			Cellule* cell= (*line)[colonne];	

			if (cell->getState()==2){
				Arbre* ab= dynamic_cast< Arbre* >(cell);
				forest->blast(ab);

				return true;
			}
		}
	}
	// cas d'erreur par défaut
	return false;
}

/*** Setters ***/

/**
 * Fonction permettant de fixer la couleur à utiliser pour dessiner un arbre
 * @param int indice de l'essence de l'arbre
 * @author Florian et Ugo
 * IMPROVEIT plus de nuances de couleurs ?, une couleur par cellule : on utiliserai une methode differente de "peinture" ? 
 */
void FireWidget::setColor(int colorIndice)
{
	switch(colorIndice){
		case 0:
			this->color->setRgb(01,100,00,	100);
			break;
		case 1:
			this->color->setRgb(34,139,34	,	100);
			break;
		case 2:
			this->color->setRgb(107,142,35,	100);
			break;
		case 3:
			this->color->setRgb(00,205,00,	100);
			break;
		case 4:
			this->color->setRgb(46,139,87,	100);
			break;
		case GRAY:
			this->color->setRgb(80,80,80,200);
			break;
		case RED_TRANSPARENT:
			this->color->setRgb(255,00,00,	180);
			break;
		case ANTI_RED_TRANSPARENT:
			this->color->setRgb(00,75,75,180);
			break;
		case BLUE:
			this->color->setRgb(25,25,250,50);
			break;
		case PURPLE:
			this->color->setRgb(148,0,211);
			break;
		case RED:
			this->color->setRgb(255,0,0);
			break;
		default :
			this->color->setRgb(255,255,255,	100);
	}
}

void FireWidget::setWind(float _hor, float _ver)
{
	forest->setWind(_hor, _ver);
}

void FireWidget::razRubber(){
	rubber = NULL;
}


/* TODO creation matrice à partir d'image dans forest ou dans firewidget ? ou les deux
 */
// 	Pour créer la matrice
// 	QByteArray data;
// 
// 	for(int i=0; i<forest->height(); ++i){
// 		vector< Cellule* >* ligne= (*forest)[i];
// 
// 		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
// 			data.push_back( (*j)->getState() *10);
// 		}
// 	}


// ########################
/***		Affichages		***/
// ########################
/**
 * Fonction à commenter par son auteur :p
 * @author Florian
 */
void FireWidget::drawPicture(){
	bufferPainter->begin(buffer);
	bufferPainter->drawPixmap(0, 0, *pictureForest);
	bufferPainter->end();
}

/**
 * Imprime une cellule à une position donnée, utilise la couleur courante
 * @author Florian
 * @param int col,row indices de la colonne et de la ligne de la cellue
 */
void FireWidget::drawCell(int colonne, int ligne)
{
	bufferPainter->fillRect(colonne, ligne, tailleCell, tailleCell, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw cell ; ";
	#endif
}

/**
 * Imprime un arbre selon sa position, utilise la couleur courante.
 * @author Florian
 * @param ab arbre à dessiner
 * TODO Utilisation de drawCell ? : soucis de performance//clareté//modularité
 */
void FireWidget::drawTree(const Arbre* ab)
{
// 	drawCell(ab->getPos().col, ab->getPos().row);
	bufferPainter->fillRect(tailleCell* ab->getPos().col, tailleCell* ab->getPos().row, tailleCell, tailleCell, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw tree ; ";
	#endif
}
/**
 * Dessine l'ensemble des arbres de la liste passée en paramètre
 * @param list<Arbre*> liste des arbres à dessiner
 * @author Florian et Ugo (commentaires :p )
 */
void FireWidget::drawList( list< Arbre* > * arbres){

	for( list< Arbre * >::const_iterator j( arbres->begin() ); j != arbres->end(); ++j){
		drawTree(*j);
	}
	arbres->clear();
}

/**
 * Dessine les arbres et cellules vides dans le buffer
 * @author Ugo et Florian
 */
void FireWidget::drawForest()
{	
	bufferPainter->begin(buffer);
	
	int current_hauteur= 0;
	for(int i=0; i<forest->height(); ++i){
		// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
		// éviter la petite zone en bas de grille
		vector< Cellule* >* ligne= (*forest)[i];
		
		int current_largeur= 0;
		for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
			Cellule* cell= *j;
			
			if( cell->getState() == 0){
				color->setNamedColor("black");
				drawCell(current_largeur, current_hauteur);
			}
			else if(cell->getState() == 1){
				// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
				unsigned indice= dynamic_cast < Arbre* >(cell)->getEssence()->getIndice();
				setColor(indice);
				// On vérifie ici si l'arbre a recu un retardateur
				// i.e son coefficient est différent de 1
				if(dynamic_cast < Arbre* >(cell)->getCoeff() != 1)
					setColor(BLUE);					
				drawCell(current_largeur, current_hauteur);
				
			}
			else if (cell->getState() == 2){
				setColor(RED);
				if(dynamic_cast < Arbre* >(cell)->getCoeff() != 1)
					setColor(RED_TRANSPARENT);		
				drawCell(current_largeur, current_hauteur);
			}
			else if (cell->getState() == -1){
				setColor(GRAY);
// 				setColor(ANTI_RED_TRANSPARENT);
				drawCell(current_largeur, current_hauteur);
			}
			else if(cell->getState() == -2){
				setColor(PURPLE);
				drawCell(current_largeur,current_hauteur);
			}
			
			// Incrémentations des positions des cellules
			current_largeur += tailleCell;
		}
		#if DEBUG_TMATRICE
		cout << endl;
		#endif
		current_hauteur += tailleCell;
	}
	
	bufferPainter->end();
	
	#if DEBUG_TMATRICE
	cout <<"fin draw forest ; "<< endl;
	#endif
}

/**
 * Redessine les arbres qui ont changés d'état, sur l'ancienne matrice
 * On réutilise les cellules non susceptibles d'avoir été modifiées
 * @author Florian and Ugo
 */
// IMPROVEIT faire une fonction qui prend une couleur et une liste d'arbres, qui "imprime" les arbres avec cette couleur ?	
// IMPROVEIT faire un fonction de firewidget qui vide toutes les listes?
void FireWidget::drawChanged()
{
	bufferPainter->begin(buffer);
	
	setColor(RED_TRANSPARENT);
	drawList(forest->getBurned());
	forest->clearBurned();
	
	setColor(GRAY);
	// 	setColor(ANTI_RED_TRANSPARENT);
	drawList(forest->getCarbonized());
	forest->clearCarbonized();
	
	color->setNamedColor("blue");
	drawList(forest->getExtinguished());
	forest->clearUprooted();
	
	color->setNamedColor("black");
	drawList(forest->getUprooted());
	forest->clearUprooted();
	
	bufferPainter->end();
}

/**
 * Vide le buffer et rafraichit l'affichage
 * @author Florian et Ugo
 */
void FireWidget::redraw()
{
	if (!buffer->isNull()){
		delete(buffer);
		// 		bufferPainter->end();
	}
	buffer = new QImage(tailleCell*forest->width(), tailleCell*forest->height(), QImage::Format_ARGB32);
	drawPicture();
	drawForest();
	drawChanged();
	update();
}

// ###################
/*** 		Events 	***/
// ##################
void FireWidget::paintEvent(QPaintEvent* event){
	QPainter paint(this);
	paint.drawImage(0, 0, *buffer);
}

void FireWidget::resizeEvent(QResizeEvent* event){

	int nbCol= forest->width();
	int nbRow= forest->height();
	tailleCell = min (event->size().width() / nbCol , event->size().height() / nbRow);
	resize(tailleCell * nbCol, tailleCell*nbRow);
	#if DEBUG_DIMENSION
	cout << nbCol<< " / "<< nbRow<< endl;
	cout << "tW: "<< event->size().width()<< " tH: "<< event->size().height()<< endl;
	cout << "taille Cellule : "<< tailleCell<< endl;
	cout << endl;
	#endif

	redraw();
}

// TODO faire une fonction qui prend le "button" de l'event et fait les instructions en conséquences
void FireWidget::mousePressEvent(QMouseEvent* event)
{
	int colonne= event->x()/tailleCell;
	int ligne= event->y()/tailleCell;
	
	if (event->button()==Qt::LeftButton)
		allumerFeu(colonne, ligne);
	else if (event->button()==Qt::MiddleButton)
		finirFeu(colonne, ligne);
	else if (event->button()==Qt::RightButton){
		/* TODO voir en haut */
// 		eteindreFeu(colonne, ligne);
// 		drawForest();
		origin = event->pos();
		
		if(!rubber)
			rubber = new QRubberBand(QRubberBand::Rectangle, this);

		rubber->setGeometry(QRect(origin, QSize(0,0)));
		rubber->show();
	}
	
	drawChanged();
	update();
}

void FireWidget::mouseMoveEvent(QMouseEvent* event)
{
	int colonne= event->x()/tailleCell;
	int ligne= event->y()/tailleCell;
	
	if (event->buttons().testFlag(Qt::LeftButton) )
		allumerFeu(colonne, ligne);
	else if (event->buttons().testFlag(Qt::MiddleButton) )
		finirFeu(colonne, ligne);
	else if (event->buttons().testFlag(Qt::RightButton) ){
		 //TODO voir en haut 
// 		eteindreFeu(colonne, ligne);
// 		drawForest();
		if(rubber)
			rubber->setGeometry(QRect(origin,event->pos()).normalized());
	}
	
	drawChanged();
	update();
}

void FireWidget::mouseReleaseEvent(QMouseEvent* event)
{
		if(rubber){
			rubber->hide();
			// Sauvegarde des points du rubber pour parcours de la matrice
			depart.setX(rubber->x());
			depart.setY(rubber->y());
			/* Vérification du point d'origine du rubber
			 * Celui étant toujours le point le plus en haut à gauche, il faut simplement vérifier 
			 * qu'il n'est pas en dehors du cadre, auquel cas nous ramenons la (les) coordonnée(s) concernée(s)
			 * à 0.
			 */
			if(depart.x() < 0){
				depart.setX(0);
			}
			if(depart.y() < 0){
				depart.setY(0);
			}
			
			arrivee.setX(rubber->width()+depart.x());
			arrivee.setY(rubber->height()+depart.y());
			
			if(arrivee.x() > size().width() ){
				arrivee.setX(size().width());
			}
			if(arrivee.y() > size().height() ){
				arrivee.setY(size().height());
			}

			#if DEBUG_SELECT
			cout << "Taille du widget : " << this->size().width() << "; " << this->size().height()<< endl;
			cout << "Coordonnée de l'origine : " << rubber->x() << "; " << rubber->y() << endl;
			cout << "Coordonnée de départ : " << depart.x()<< ";" << depart.y() << endl;
			cout << "Coordonnée de l'arrivée : " << arrivee.x()<< ";" << arrivee.y() << endl;
			cout << "Taille de la zone de selection : " <<	rubber->width() << ";" << rubber->height() << endl;
			#endif
			// Emission du signal pour récupérer l'action à effectuer
			emit releaseSignal();
		}
	
}


// #################
/***	 	Slots	 	***/
// #################
/**
 * Passe de l'etat t à t+1 la foret
 * @author Florian
 * @return vrai si la foret a ete modifiée
 */
bool FireWidget::next()
{
	// pas suivant
	if (!forest->NextMove())
		return false;
	
	// mise à jour de l'image puis affichage à l'écran
	drawChanged();
	update();
	
	return true; // cas par défaut, il y a eu un changement
}

/**
 * reinitialise la foret
 * @author Florian et un petit peu Ugo :p
 * @deprecated
 */
void FireWidget::reset(int _larg, int _haut, float proba, float coef)
{
// 	Foret* OldForet= forest;
// 	forest = new Foret(*OldForet, probaMatriceReset);
// 	delete(OldForet);
// IMPROVEIT quelle est la meilleure facon de RAZ une foret?
// 	buffer->fill(1);

	forest->clean();	// suppression de l'ancienne foret
	forest->setValues(_larg,_haut, coef); // changement des valeurs de taille et de brulure
	forest->randomMatrix(proba);	// création de la nouvelle foret IMPROVEIT faire un randomMatrix dans setValues ?

	setMinimumSize(_larg, _haut);
	
	drawPicture();
	drawForest();
	drawChanged();
	update();
}


/**
 * Slot permettant d'éxecuter l'action choisie par l'utilisateur
 * sur la zone enregistrée après un releaseMouseEvent.
 * 0 correspond à une coupure, 1 à un retardateur
 * @author Ugo
 */
void FireWidget::actionReceived(int x)
{
	// Transformation des QPoints depart et arrivée en coordonnée cellulaire
	int xDep = depart.x() / tailleCell;
	int yDep = depart.y() / tailleCell;
	
	int xArr = arrivee.x() / tailleCell;
	int yArr = arrivee.y() / tailleCell;
	
	#if DEBUG_RETARD
	cout << "Coordonnée en cellule du départ : " << xDep << ";" << yDep << endl;
	cout << "Coordonnée en cellule de l'arrivée : " << xArr << ";" << yArr << endl;
	#endif
	
	// Appel à une fonction de forêt qui parcours la zone et effectue l'action

	if(x == 0){
		forest->cut(xDep, yDep, xArr, yArr);
	}else if( x == 1){
		forest->delay(xDep, yDep, xArr,yArr);
	}

	redraw();
}
