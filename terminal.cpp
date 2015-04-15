#include "terminal.h"

using namespace std;

Terminal::Terminal(int hauteur, int largeur, float proba, long int nTemps, bool t)
	:foret(hauteur, largeur, proba), temps(nTemps), b_taille(t)
{
	// lance le mode curses et le mode keypad pour les touches clavier
	initscr();
// 	keypad(stdscr, 1);
	
	// initialise le mode couleur de la fenetre si disponible
	if(!has_colors())
	{	
	printw("Your terminal does not support color\n");
	endwin();
	exit(1);
	}
	start_color();
	
	//initalise les paires couleurs, utilisées pour afficher les cases coloriées en couleur
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
 	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
 	init_pair(5, COLOR_BLACK, COLOR_CYAN);
	init_pair(6, COLOR_BLACK, COLOR_BLUE);
	init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
}

void Terminal::end()
{
	refresh();
	getch();
	
	printw("\n Vous allez quitter la partie");
	refresh();
	getch();
	endwin();
}



//#########################################
//===	fonctions d'affichage		====
//#########################################

// ecrit la premiere/derniere ligne du plateau, avec le numéros de colonnes
void Terminal::afficheContour(int largeur)
{
	int j;
	printw("| ");
	for(j=0; j<largeur; j++)
		if (j < 10)
			if (b_taille) printw("=", j);
			else printw("==", j);
		else 
			if (b_taille) printw("-", j);
			else printw("--", j);
			
		printw("||\n");
}

// affiche une case vide, dans sa couleur

void Terminal::afficheCase(const Cellule* cell)
{
	int etat_cell= cell->getEtat();
	if (etat_cell==1){
		unsigned int color;
		unsigned int indice= dynamic_cast < const Arbre* >(cell)->getEssence()->getIndice();
		if(indice == 0){
			color = 1;
		}else{
			color =indice + 3;
		}

		// 		printw("| ");
		attron(COLOR_PAIR(color ));
		//mvprintw(ligne*2+1, colonne*4+3, "X");
		if (b_taille) printw(" ");
		else printw("  ");
		attroff(COLOR_PAIR(color ));
		// 		printw(" ");
	}
	else if (etat_cell==2){
		attron(COLOR_PAIR(2));
		if (b_taille) printw(" ");
		else printw("  ");
		attroff(COLOR_PAIR(2));
	}
	else if (etat_cell==3){
		attron(COLOR_PAIR(3));
		if (b_taille) printw(" ");
		else printw("  ");
		attroff(COLOR_PAIR(3));
	}
	else if (b_taille) printw(" ");
		else printw("  "  );// 1 espaces
			
}

// ecrit une ligne de cases avec son numéro; utilise les fonctions affiche_case et affiche_joueur
void Terminal::afficheLigne(int num_ligne)
{
	int largeur= foret.largeur();
	vector< Cellule* >* ligne= foret[num_ligne];
	
	
	if (num_ligne < 10)
		printw("|");
	
	printw("%d|", num_ligne);
	// verifie si il y un joueur sur la case, l'affiche si oui
	for(int j=0; j<largeur; ++j){
		// on verifie si il y a un joueur sur la case
		afficheCase( (*ligne)[j] );
	}
	printw("||\n");
}


void Terminal::afficheForet()
{
	afficheContour(foret.largeur());
	
	for (int i= 0; i<foret.hauteur(); ++i)
		afficheLigne(i);
	
	afficheContour(foret.largeur());
	refresh();
}

void Terminal::next()
{
	foret.NextMove();
	usleep(temps);
	
	clear();
	afficheForet();
}

void Terminal::run()
{
	// tant que la foret est modifiee
	while (foret.NextMove()) {
		usleep(temps);
		
		clear();
		afficheForet();
	}
}

