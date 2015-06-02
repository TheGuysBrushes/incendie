#include "firewidget.h"

#include <QtGui/QVBoxLayout> // fenetre chargement

enum Colors{Green0, Green1, Green2, Green3, Green4, Gray, Red, Orange, Blue, Purple, Brownie};


using namespace std;

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
FireWidget::FireWidget(): QWidget()
{	
	buffer = new QImage();
	color = new QColor(Qt::black);
	bufferPainter= new QPainter();
	pictureForest= new QImage();
	
	rubber = NULL;
}

FireWidget::~FireWidget()
{
	delete(forest);
	delete(buffer);
	delete(color);
	delete(bufferPainter);
// 	delete(pictureForest);
}

// ########################
/*** 	Initialisations 	***/
// ########################
/**
 * Fonction de création d'une foret ALEATOIRE lors de la (ré)initialisation
 * @author Florian et Ugo
 * @param all identiques au constructeur de Foret aléatoire
 */
void FireWidget::initialise(int largeur, int hauteur, float proba, float coef_brulure)
{
	#if DEBUG_CREA_FORET
	cout<< "Creation d'une foret aléatoire à partir de paramètres"<< endl;
	#endif

	forest= new Foret(largeur, hauteur, proba, coef_brulure);
	
	setMinimumSize(largeur/2.0, hauteur/2.0);
}

/**
 * Fonction de création d'une foret A PARTIR D'UNE SAUVEGARDE,
 * dans un fichier ouvert dont les tailles ont déjà été lues, lors de la (ré)initialisation
 * @author Florian et Ugo
 * 
 * @param largeur de la nouvelle forêt
 * @param hauteur de la nouvelle forêt
 * @param file fichier binaire, ouvert, contenant la sauvegarde de la forêt (essences-arbres)
 * @return vrai si le fichier est ouvert
 */
bool FireWidget::initialise(int largeur, int hauteur, ifstream * file) 
{
	if (file->is_open()){
		#if DEBUG_CREA_FORET
		cout<< "Chargement d'une foret à partir d'un fichier "<< endl;
		#endif
		#if DEBUG_LOAD
		cout<< "Taille : " << largeur<< " en largeur "<< hauteur<< " en hauteur" <<endl;
		#endif
		
		QWidget* loadWindow= new QWidget();
		loadWindow->resize(400, 30);
		QVBoxLayout* layLoad= new QVBoxLayout(loadWindow);
		
		QLabel* txtLoad= new QLabel("Chargement de la foret");
		// TODO voir si il faut que foret fasse emit d'un signal à connecter à la progressbar
		QProgressBar* PB_load= new QProgressBar();
		
		layLoad->addWidget(txtLoad);
		layLoad->addWidget(PB_load);
		
		loadWindow->show();
		forest= new Foret(largeur, hauteur, file, PB_load);
		loadWindow->hide();
		
		setMinimumSize(largeur/2.0, hauteur/2.0);
		
		return true;
	}
	else{
		#if DEBUG_CREA_FORET
		cout<< "Pas de fichier, essai création par image"<< endl;
		#endif
		return false;
	}
}

/**
 * Fonction de création d'une foret ALEATOIRE lors de la (ré)initialisation
 * @author Florian et Ugo
 * 
 * @param largeur de la nouvelle forêt
 * @param hauteur de la nouvelle forêt
 * @param imageForet image d'une foret, pour l'instant une image converie du jpg au tif
 * @return vrai si l'image est chargée
 */
bool FireWidget::initialise(int largeur, int hauteur, QImage* imageForet)
{
	if (!imageForet->isNull()){
		pictureForest= imageForet;
		#if DEBUG_CREA_FORET
		cout << "Réussite ouverture fichier, creation foret à partir de l'image"<< endl;
		#endif
		
		loadFromPicture(largeur, hauteur, imageForet);
		setMinimumSize(largeur/2.0, hauteur/2.0);
		
		return true;
	}
	else {
		// 	TODO prévenir l'utilisateur ou faire une fonction par défaut en cas d'echec de chargement à partir d'une image
		// 		cout << "Echec de création d'une foret à partir de l'image"<< endl;
		#if DEBUG_CREA_FORET
		cout << "Pas d'image"<< endl;
		#endif
		
		return false;
	}
}


// #################################
/*** 	Gestion des sauvegardes 	***/
// #################################
/**
 * Fonction à commenter par son auteur :p
 * @author Florian
 * @param 
 */
void FireWidget::loadFromPicture(int largeurImage, int hauteurImage, QImage* imageForet)
{
	cout<< "Creation à partir d'image ..."<< endl;
	vector< vector< int > >* matrice= new vector< vector< int > >;
	QColor* pix= new QColor();
	
	for (int i= 0; i< hauteurImage; ++i){
		vector< int > ligne;
		
		for (int j= 0; j< largeurImage; ++j){
			pix->setRgba(imageForet->pixel(j, i));
			#if DEBUG_IMAGE_POS
			cout << "qté vert en "<< j<< " ; "<< i<<" : "<< pix->green();
			cout << "\tqté red : " << pix->red() << " ; bleu : "<< pix->blue()<< endl;
			#endif
			
			int green= pix->green();
			if (0.8*pix->red()< green && pix->blue()< 140)
				ligne.push_back(green);
			else ligne.push_back(0);
		}
		matrice->push_back(ligne);
	}
	forest= new Foret(largeurImage, hauteurImage, matrice);
// 		QColormap map(pictureForest->tr);
// 	img.trueMatrix()
}
	
/**
 * Charge une foret à partir d'un nom de fichier
 * @author Ugo et Florian
 * @deprecated
 */
bool FireWidget::loadForest(std::string filename)
{	
	ifstream* file = new ifstream(filename.c_str(), std::ios::in|std::ios::binary);

	if (!file->is_open()){
		std::cout<< "Echec ouverture fichier de sauvegarde"<< std::endl;
		return false;
	}
	else {
		// Réinitialisation des tailles de la matrice...
		forest->loadSizes(file);
		// ... puis chargement de la nouvelle matrice
		forest->load(file,NULL);
		return true;
		
	}
}

/**
 * Sauvegarde la foret dans un fichier de sauvegarde (IMPROVEIT par défaut pour l'instant)
 * @author Florian
 */
void FireWidget::saveForest(string filepath) const
{
	forest->save(filepath);
	cout << "filePath fWidget : " << filepath << endl;
}

/**
 * Méthodes de destruction de la forêt pour gérer la 
 * ré-initialisation de la matrice
 * @author Florian
 */
void FireWidget::delForest(){
	delete(forest);
}

void FireWidget::delPicture()
{
	delete pictureForest;
	pictureForest= new QImage;
}



/**
 * Eteint un arbre à une position donnée
 * @author Florian
 * @param int/int indices de la colonne et de la ligne de l'arbre à éteindre
 * @return vrai si il y avait un arbre en feu
 */
// bool FireWidget::eteindreFeu(int colonne, int ligne)
// {
// 	#if DEBUG_ALLUME
// 	cout << "eteindre cellule ? : (l,c)"<< ligne<< " : "<< colonne << endl; 
// 	#endif
// 
// 	if(ligne>=0 && ligne < forest->height()){
// 		vector< Cellule* >* line= (*forest)[ligne];
// 
// 		if (colonne>=0 && colonne < forest->width()){
// 			Cellule* cell= (*line)[colonne];	
// 
// 			if (cell->getState()==2){
// 				Arbre* ab= dynamic_cast< Arbre* >(cell);
// 				forest->delay(ab);
// 
// 				return true;
// 			}
// 		}
// 	}
// 	// cas d'erreur par défaut
// 	return false;
// }

/**
 * Allume un feu sur un arbre "vivant"
 * @author Florian
 * @param colonne de l'arbre à enflammer
 * @param ligne de l'arbre à enflammer
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
		case Green0:
			this->color->setRgb(01,100,00);
			break;
		case Green1:
			this->color->setRgb(34,139,34	);
			break;
		case Green2:
			this->color->setRgb(107,142,35);
			break;
		case Green3:
			this->color->setRgb(00,205,00);
			break;
		case Green4:
			this->color->setRgb(46,139,87);
			break;
		case Gray:
			this->color->setRgb(60,60,60);
			break;
		case Orange:
			this->color->setRgb(255,165,00);
			break;
		case Blue:
			this->color->setRgb(25,25,250,	200);
			break;
		case Purple:
			this->color->setRgb(148,0,211);
			break;
		case Red:
			this->color->setRgb(255,0,0);
			break;
		case Brownie:
			color->setRgb(40, 30, 20);
			break;
		default :
			this->color->setRgb(255,255,255,	100);
	}
}

/**
 * @deprecated opération faite dans le .h
 */
// void FireWidget::setWind(float _hor, float _ver)
// {
// 	forest->setWind(_hor, _ver);
// }

// ########################
/***		Affichages	***/
// ########################
/**
 * Imprime une cellule à une position donnée, utilise la couleur courante
 * @author Florian
 * @param int col,row indices de la colonne et de la ligne de la cellules
 */
void FireWidget::drawCell(int colonne, int ligne)
{
	bufferPainter->fillRect(colonne, ligne, 1, 1, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw cell ; ";
	#endif
}

/**
 * Imprime un arbre selon sa position, utilise la couleur courante.
 * @author Florian
 * @param ab arbre à dessiner
 * @deprecated
 */
void FireWidget::drawTree(const Arbre* ab)
{
// 	drawCell(ab->getPos().col, ab->getPos().row);
	bufferPainter->fillRect(ab->getPos().col, ab->getPos().row, 1, 1, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw tree ; "<< ab->getPos().col << ab->getPos().row ;
	#endif
}
/**
 * Dessine l'ensemble des arbres de la liste passée en paramètre
 * @param list<Arbre*> liste des arbres à dessiner
 * @author Florian et Ugo (commentaires :p )
 * @deprecated
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
// 	if (!drawPictureForest()){
	
		bufferPainter->begin(buffer);
		drawPicture();
		
		int current_hauteur= 0;
		for(int i=0; i<forest->height(); ++i){
			// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
			// éviter la petite zone en bas de grille
			vector< Cellule* >* ligne= (*forest)[i];
			
			int current_largeur= 0;
			for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
				Cellule* cell= *j;
				
				if( cell->getState() == 0){
					setColor(Brownie);
					drawCell(current_largeur, current_hauteur);
				}
				else if(cell->getState() == 1){
					
					if (pictureForest->isNull()){
						// Il faut ici vérifier l'essence de l'arbre pour lui attribuer une variante de vert
						unsigned indice= dynamic_cast < Arbre* >(cell)->getEssence()->getIndice();
						// On vérifie ici si l'arbre a recu un retardateur
						// i.e son coefficient est inférieur à 1
						if(dynamic_cast < Arbre* >(cell)->getCoeff() < 1)
							setColor(Blue);
						else setColor(indice);
						
						drawCell(current_largeur, current_hauteur);
					}
					
				}
				else if (cell->getState() == 2){
					if(dynamic_cast < Arbre* >(cell)->getCoeff() < 1)
						setColor(Orange);	
					else setColor(Red);
					
					drawCell(current_largeur, current_hauteur);
				}
				else if (cell->getState() == -1){
					setColor(Gray);
					drawCell(current_largeur, current_hauteur);
				}
				else if(cell->getState() == -2){
					setColor(Purple);
					drawCell(current_largeur,current_hauteur);
				}
				
				// Incrémentation des positions des cellules
				current_largeur += 1;
			}
			#if DEBUG_TMATRICE
			cout << endl;
			#endif
			current_hauteur += 1;
		}
		
		bufferPainter->end();
		
		#if DEBUG_TMATRICE
		cout <<"fin draw forest ; "<< endl;
		#endif
// 	}
}

/**
 * Fonction à commenter par son auteur :p
 * @author Florian
 */
bool FireWidget::drawPicture()
{
	if (!pictureForest->isNull()){
// 		bufferPainter->begin(buffer);
		bufferPainter->drawImage(0, 0, *pictureForest);
// 		bufferPainter->end();
		
		return true;
	}
	else return false;
}

// bool FireWidget::drawPictureForest()
// {
// 	if (drawPicture()){
// 		
// 		bufferPainter->begin(buffer);
// 		int current_hauteur= 0;
// 		for(int i=0; i<forest->height(); ++i){
// 			// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
// 			// éviter la petite zone en bas de grille
// 			vector< Cellule* >* ligne= (*forest)[i];
// 			
// 			int current_largeur= 0;
// 			for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
// 				Cellule* cell= *j;
// 
// 				if (cell->getState() == 2){
// 					if(dynamic_cast < Arbre* >(cell)->getCoeff() < 1)
// 						setColor(Orange);	
// 					else setColor(Red);
// 					
// 					drawCell(current_largeur, current_hauteur);
// 				}
// 				else if (cell->getState() == -1){
// 					setColor(Gray);
// 					drawCell(current_largeur, current_hauteur);
// 				}
// 				else if(cell->getState() == -2){
// 					setColor(Purple);
// 					drawCell(current_largeur,current_hauteur);
// 				}
// 				
// 				// Incrémentation des positions des cellules
// 				current_largeur += 1;
// 			}
// 			#if DEBUG_TMATRICE
// 			cout << endl;
// 			#endif
// 			current_hauteur += 1;
// 		}
// 		
// 		bufferPainter->end();
// 		
// 		return true;
// 	}
// 	else return false;
// }


/**
 * Redessine les arbres qui ont changés d'état, sur l'ancienne matrice
 * On réutilise les cellules non susceptibles d'avoir été modifiées
 * @author Florian and Ugo
 */
// IMPROVEIT faire une fonction qui prend une couleur et une liste d'arbres, qui "imprime" les arbres avec cette couleur ?	
void FireWidget::drawChanged()
{
	bufferPainter->begin(buffer);
	
	setColor(Red);
	drawList(forest->getBurned());
	forest->clearBurned();
	
	setColor(Gray);
	drawList(forest->getCarbonized());
	forest->clearCarbonized();
	
// 	color->setNamedColor("blue");
	setColor(Blue);
	drawList(forest->getExtinguished());
	forest->clearExtinguished();
	
	setColor(Purple);;
	drawList(forest->getUprooted());
	forest->clearUprooted();
	
	bufferPainter->end();
}

// Debuggage
#if DEBUG_CURRENT
int num_redraw= 0;
#endif

/**
 * Vide le buffer et rafraichit l'affichage
 * @author Florian et Ugo
 */
void FireWidget::redraw()
{
	#if DEBUG_CURRENT
	++num_redraw;
	cout << "test redraw "<< num_redraw<< " TO DELETE (ligne 449 firewidget)"<< endl;
	#endif
	
	if (!buffer->isNull()){
		delete(buffer);
	}
	buffer = new QImage(forest->width(), forest->height(), QImage::Format_ARGB32);
// 	drawPicture();
	drawForest();
	drawChanged();
	update();	// TODO apparemment non utile
}

// ###################
/*** 		Events 	***/
// ##################
void FireWidget::paintEvent(QPaintEvent* event)
{
	QPainter paint(this);
	paint.scale(tailleCell, tailleCell);
	paint.drawImage(0, 0, *buffer);
}

/**
 * 
 * @author
 */
void FireWidget::resizeEvent(QResizeEvent* event)
{
	#if DEBUG_CURRENT
	cout << "test resizeEvent TO DELETE (ligne 471 firewidget)"<< endl;
	#endif
	
	int nbCol= forest->width();
	int nbRow= forest->height();
	tailleCell = min (event->size().width() / (float)nbCol , event->size().height() / (float)nbRow);
	
	// TODO voir comment modifier la taille de de FireWidget sans repasser par resizeEvent, vraiment utile (ajouté pour rubberband, le modifier pour s'adapter?)
// 	resize(tailleCell * nbCol, tailleCell*nbRow); // Fait lagger
	
	#if DEBUG_CURRENT
// 	cout << "test apres resize dans resizeEvent (ligne 488 firewidget)"<< endl;
	#endif
	
	#if DEBUG_DIMENSION
	cout << nbCol<< " / "<< nbRow<< endl;
	cout << "tW: "<< event->size().width()<< " tH: "<< event->size().height()<< endl;
	cout << "taille Cellule : "<< tailleCell<< endl;
	cout << endl;
	#endif

	redraw();
}

void FireWidget::mousePressEvent(QMouseEvent* event)
{
	int colonne= event->x()/tailleCell;
	int ligne= event->y()/tailleCell;
	
	if (event->button()==Qt::LeftButton)
		allumerFeu(colonne, ligne);
	else if (event->button()==Qt::MiddleButton)
		finirFeu(colonne, ligne);
	else if (event->button()==Qt::RightButton)
	{
		initRubber(event);
		
		#if DEBUG_COLOR
		QColor* pix= new QColor(pictureForest->pixel(colonne, ligne));
		cout << "qté vert en "<< colonne<< " ; "<< ligne<<" : "<< pix->green();
		cout << "\tqté red : " << pix->red() << " ; bleu : "<< pix->blue()<< endl;
		#endif
	}
	
	drawChanged();
	update();
}

void FireWidget::initRubber(QMouseEvent* event){
	origin = event->pos();
	
	if(!rubber)
		rubber = new QRubberBand(QRubberBand::Rectangle, this);

	rubber->setGeometry(QRect(origin, QSize(0,0)));
	rubber->show();
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
		if(rubber)
			rubber->setGeometry(QRect(origin,event->pos()).normalized());
	}
	
	drawChanged();
	update();
}

void FireWidget::mouseReleaseEvent(QMouseEvent* event)
{
	
// 	IMPROVEIT Ugo : pourquoi ne pas definir la taille du rubber ici ? :
// 	rubber->setGeometry(QRect(origin,event->pos()).normalized());

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
		arrivee.setX(rubber->width()+depart.x());
		arrivee.setY(rubber->height()+depart.y());
		if(depart.x() < 0){
			depart.setX(0);
		}
		if(depart.y() < 0){
			depart.setY(0);
		}
		
		
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
		cout << "Taille de la zone de selection : " <<	arrivee.x() - depart.x() << ";" << arrivee.y() - depart.y() << endl;
		#endif
		// Emission du signal pour récupérer l'action à effectuer par firescreen
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
// void FireWidget::reset(int _larg, int _haut, float proba, float coef)
// {
// // 	Foret* OldForet= forest;
// // 	forest = new Foret(*OldForet, probaMatriceReset);
// // 	delete(OldForet);
// // IMPROVEIT quelle est la meilleure facon de RAZ une foret?
// // 	buffer->fill(1);
// 
// 	forest->clean();	// suppression de l'ancienne foret
// 	forest->setValues(_larg,_haut, coef); // changement des valeurs de taille et de brulure
// 	forest->randomMatrix(proba);	// création de la nouvelle foret IMPROVEIT faire un randomMatrix dans setValues ?
// 
// 	setMinimumSize(_larg, _haut);
// 	
// 	drawPicture();
// 	drawForest();
// 	drawChanged();
// //  update();
// }


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
	
	if (xArr> forest->width())
		xArr= forest->width();
	
	int yArr = arrivee.y() / tailleCell;
	
	if (yArr> forest->height())
		yArr= forest->height();

	
	#if DEBUG_RETARD
	cout << "Coordonnée en cellule du départ : " << xDep << ";" << yDep << endl;
	cout << "Coordonnée en cellule de l'arrivée : " << xArr << ";" << yArr << endl;
	#endif
	
	// Appel à une fonction de forêt qui parcours la zone et effectue l'action
	
	
	if(x == CUT){
		forest->cut(xDep, yDep, xArr, yArr);
	}else if( x == DELAY){
		forest->delay(xDep, yDep, xArr,yArr);
	}else cerr<< "mauvais index d'action clic droit"<< endl;
	
	drawChanged();
	update();
	emit endAction();
// 	redraw();
}
