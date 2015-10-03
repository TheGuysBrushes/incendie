#include "firewidget.h"

#include <QtGui/QVBoxLayout> // fenetre chargement

enum Colors{Green0, Green1, Green2, Green3, Green4, Gray, Red, Orange, BlueTrans, BrownCut, Brownie};


using namespace std;

// ##################################
/*** Constructeur et destructeur ***/
// #################################
FireWidget::FireWidget(): QWidget()
{	
	buffer = new QImage;
	color = new QColor(Qt::white);
	bufferPainter= new QPainter;
	pictureForest= new QImage;
	
	rubber = NULL;
	forest = NULL;
}

FireWidget::~FireWidget()
{
	delete(forest);
	delete(buffer);
	delete(color);
	delete(bufferPainter);
	delete(pictureForest);
	
	delete rubber;
}

// ########################
/*** 	Initialisations 	***/
// ########################
void FireWidget::initialise(int largeur, int hauteur, float proba, float coef_brulure, time_t graine)
{
	#if DEBUG_CREA_FORET
	cout<< "Creation d'une foret aléatoire à partir de paramètres"<< endl;
	#endif

	delForest(); // Pour empêcher fuite mémoire
	forest= new Foret(largeur,hauteur, proba, coef_brulure, graine);
	
	setMinimumSize(largeur/2.0, hauteur/2.0);
}

bool FireWidget::tryInitialise(int largeur, int hauteur, ifstream * file)
{
	if (file->is_open()){

		LoadWindow* progressWindow= createProgressWindow();
		delForest(); //Pour empêcher fuite mémoire
		forest= new Foret(largeur, hauteur, file, progressWindow);
		
		progressWindow->closeProgress();
// 		delete progressWindow; WARNING pourquoi en commentaire ?
		
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

bool FireWidget::tryInitialise(int largeur, int hauteur, QImage* imageForet, float coef_brulure)
{
	if (!imageForet->isNull()){
		// on crée une copie de l'image, pour éviter de pointer sur une image détruite, 
		//		on pourrait enlever la destruction de pictureForest dans fwelcome pour gagner en performance,
		//		mais il y a un risque de fuite mémoire en cas d'une utilisation différente de fwelcome
		delPicture();
		pictureForest= new QImage(*imageForet);
		#if DEBUG_CREA_FORET
		cout << "Réussite ouverture fichier, creation foret à partir de l'image"<< endl;
		#endif
		
		loadFromPicture(largeur,hauteur, imageForet, coef_brulure);
		setMinimumSize(largeur/2.0,hauteur/2.0);
		
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


LoadWindow* FireWidget::createProgressWindow() const
{
	LoadWindow* progressWindow= new LoadWindow();
	return progressWindow;
}


//######################
/*** 		Setters 		***/
//######################

// TODO setColor : sauvegarder l'indice précédent pour éviter de redéfinir la mm couleur ?
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
			this->color->setRgb(45,45,45);
			break;
		case Orange:
			this->color->setRgb(255,165,00);
			break;
		case BlueTrans:
			this->color->setRgb(83,104,120,	200);
			break;
		case BrownCut:
			this->color->setRgb(150,75,0,	200);
			break;
		case Red:
			this->color->setRgb(255,0,0);
			break;
		case Brownie:
			this->color->setRgb(40, 30, 20);
			break;
		default :
			this->color->setRgb(255,255,255);
	}
}

// #################################
/*** 		Gestion de la foret		***/
// #################################
void FireWidget::delForest(){
	#if DEBUG_CREA_FORET
	cout << "effacement foret"<< endl;
	#endif
	
	delete(forest);
}

void FireWidget::delPicture()
{
	delete pictureForest;
	pictureForest= new QImage;
}


// #################################
/*** 	Gestion des sauvegardes 	***/
// #################################
void FireWidget::loadFromPicture(int largeurImage, int hauteurImage, QImage* imageForet,float coef_brulure)
{
	cout<< "Creation à partir d'image ..."<< endl;
	vector< vector< int > >* matrice= new vector< vector< int > >;
	QColor* pix= new QColor;
	
	for (int i= 0; i< hauteurImage; ++i){
		vector< int > ligne;
		
		for (int j= 0; j< largeurImage; ++j){
			pix->setRgba(imageForet->pixel(j, i));
			#if DEBUG_IMAGE_POS
			cout << "qté vert en "<< j<< " ; "<< i<<" : "<< pix->green();
			cout << "\tqté red : " << pix->red() << " ; bleu : "<< pix->blue()<< endl;
			#endif
			
			int green= pix->green();
			// Une cellule est un arbre si il y a un peu moins de rouge que de vert et si il n'y a pas trop bleu, ni de blanc
			if (0.8*pix->red()< green && pix->blue()*1.65 -55 < green)
				ligne.push_back(green);
			else ligne.push_back(0);
		}
		matrice->push_back(ligne);
	}
	
	delForest();
	forest= new Foret(largeurImage, hauteurImage, matrice,coef_brulure);
// 		QColormap map(pictureForest->tr);
// 	img.trueMatrix()
}


bool FireWidget::trySaveForest(string& filePath) const
{
	return forest->trySave(filePath);
}

bool FireWidget::trySaveSeed(string& filePath) const
{
	return forest->trySaveSeed(filePath);
}

bool FireWidget::trySaveImage(QString filename) const
{
	if ( !buffer->isNull() ){
		// TEST verifier que la taille est correcte
		QImage tmp= buffer->scaled(tailleCell*forest->width(), tailleCell*forest->height());
		tmp.save(filename);
		return true;
	}
	else{
		#if DEBUG_SAVE
		cout << "Impossible de sauvegarder l'image, de forêt ouverte !"<< endl;
		#endif
		return false;
	}
}


//######################################
/* Modifications de l'état des arbres */
//######################################
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

bool FireWidget::tryAllumerFeu(int colonne, int ligne)
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

bool FireWidget::tryFinirFeu(int colonne, int ligne)
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


// ########################
/***		Affichages	***/
// ########################
void FireWidget::drawCell(int colonne, int ligne)
{
	bufferPainter->fillRect(colonne, ligne, 1, 1, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw cell ; ";
	#endif
}

void FireWidget::drawTree(const Arbre* ab)
{
// 	drawCell(ab->getPos().col, ab->getPos().row);
	bufferPainter->fillRect(ab->getPos().col, ab->getPos().row, 1, 1, *(color));
	#if DEBUG_TMATRICE
	cout <<"draw tree ; "<< ab->getPos().col << ab->getPos().row ;
	#endif
}

void FireWidget::drawList( list< Arbre* > * arbres){

	for( list< Arbre * >::const_iterator j( arbres->begin() ); j != arbres->end(); ++j){
		drawTree(*j);
	}
	arbres->clear();
}

void FireWidget::drawForest()
{
	// essai de dessin de l'image de fond, et de la foret, si présente
	if (!tryDrawPictureForest()){
		// si il n"y a oas d'image, on dessine toute la foret dont les arbre

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
							setColor(BlueTrans);
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
					setColor(BrownCut);
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
	}
}

bool FireWidget::tryDrawPicture()
{
	if (!pictureForest->isNull()){
		bufferPainter->begin(buffer);
		bufferPainter->drawImage(0, 0, *pictureForest);
		bufferPainter->end();
		
		return true;
	}
	else return false;
}

bool FireWidget::tryDrawPictureForest()
{
	// essai de dessin de l'image de fond, si présente
	if (tryDrawPicture()){
		// si il y a une image de fond, on ne dessine pas les arbres "neutres"
		
		bufferPainter->begin(buffer);
		int current_hauteur= 0;
		for(int i=0; i<forest->height(); ++i){
			// On ne passe pas la hauteur de la grille mais le nombre de colonne*taille de colonne pour
			// éviter la petite zone en bas de grille
			vector< Cellule* >* ligne= (*forest)[i];
			
			int current_largeur= 0;
			for( vector< Cellule* >::const_iterator j( ligne->begin() ); j!=ligne->end(); ++j){
				Cellule* cell= *j;
				if(cell->getState() == 1){
					// On vérifie ici si l'arbre a recu un retardateur
					// i.e son coefficient est inférieur à 1
					if(dynamic_cast < Arbre* >(cell)->getCoeff() < 1){
						setColor(BlueTrans);
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
					setColor(BrownCut);
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
		
		return true;
	}
	else return false;
}


void FireWidget::drawChanged()
{
	bufferPainter->begin(buffer);
	
	setColor(Red);
	drawList(forest->getBurned());
	forest->clearBurned();
	
	setColor(Gray);
	drawList(forest->getCarbonized());
	forest->clearCarbonized();
	
	setColor(BlueTrans);
	drawList(forest->getDelayed());
	forest->clearDelayed();
	
	setColor(Orange);
	drawList(forest->getDelayBurned());
	forest->clearDelayBurned();
	
	setColor(BrownCut);
	drawList(forest->getUprooted());
	forest->clearUprooted();
	
	bufferPainter->end();
}

// Test perf
#if PERF_REDRAW
int num_redraw= 0;
#endif

void FireWidget::redraw()
{
	#if PERF_REDRAW
	++num_redraw;
	cout << "test redraw firewidget"<< num_redraw<< endl;
	#endif
	
	if (!buffer->isNull()){
		delete(buffer);
	}
	buffer = new QImage(forest->width(), forest->height(), QImage::Format_ARGB32);

	drawForest();
	drawChanged();
	update();	// TODO apparemment non utile, update fait resize
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
	#if PERF_RESIZE
	cout << "test resizeEvent firewidget"<< endl;
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
		tryAllumerFeu(colonne, ligne);
	else if (event->button()==Qt::MiddleButton)
		tryFinirFeu(colonne, ligne);
	else if (event->button()==Qt::RightButton)
	{
		initRubber(event);
		
		#if DEBUG_IMAGE_COLOR
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
		tryAllumerFeu(colonne, ligne);
	else if (event->buttons().testFlag(Qt::MiddleButton) )
		tryFinirFeu(colonne, ligne);
	else if (event->buttons().testFlag(Qt::RightButton) ){
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
}
