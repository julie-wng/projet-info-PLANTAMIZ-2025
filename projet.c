#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib")

#include "affichage_console.h"

#define LIGNE 10
#define COLONNE 15

#define FICHIER "joueurs.txt"

typedef struct infos {
	char type_precedent;
	char type_actuel;
	int nb_alligne;
	int x_initial;
	int y_initial;
	int x_final;
	int y_final;
}infos;

//affichage du menu
int menu() {
    int choix;
    printf("\n1. Jouer");
    printf("\n2. Regles du jeu");
	printf("\n3. Musique");
    printf("\n4. Quitter\n");
    scanf("%d",&choix);
    return choix;
}

//affichage des regles
void afficherRegles(){
    printf("\nUtilisez les fleches du clavier pour jouer\n");
}

// Afficher un caractère à une position dans la console avec une couleur et l'ajoute a la plateau
void afficherObjet(char plateau[LIGNE][COLONNE],char c, int x, int y, int couleur, int couleur_fond) {
    gotoxy(x*3 + 1, y + 4);
    text_color(couleur);
    bg_color(couleur_fond);
    printf("%c",c);
    bg_color(0); //réinitialiser le fond à noir
    plateau[y][x] = c;
}

// Génère un item aléatoirement
char genererItem() {
    int val = rand()%5;
    char item;
    switch (val) {
        case 0:
            item = 'S'; //Soleil
            break;
        case 1:
            item = 'F'; //Fraise
            break;
        case 2:
            item = 'P'; //Pomme
            break;
        case 3:
            item = 'O'; //Oignon
            break;
        case 4:
            item = 'M'; //Mandarine
            break;
    }
    return item;
}

/*char figure_speciale_H(char plateau[LIGNE][COLONNE],int *nb_vertical_1,int *nb_vertical_2,int *nb_horizontal,Coordonne debut_vertical,Coordonne debut_horizontal){
    for (int y = 0; y < LIGNE-2; y++) {
        for (int x = 0; x < COLONNE-2; x++) {
            char item = plateau[y][x];
            *nb_vertical_1 = 0;
            *nb_vertical_2 = 0;
            for(int i=y;i<LIGNE;i++){
                if (item == plateau[i][x]){
                    (*nb_vertical_1)++;
                }
                else{
                    break;
                }
            }
            for(int i=y+1;i<=*nb_vertical_1;i++){
                *nb_horizontal = 0;
                for(int j=x;j<COLONNE;j++){
                    if (item == plateau[i][j]){
                        *nb_horizontal++;
                    }
                    else{
                        break;
                    }
                }
            }
            for(int i=y;i<LIGNE;i++){
                if (item == plateau[i][*nb_horizontal]){
                    *nb_vertical_2++;
                }
            }
            if(*nb_horizontal>=3 && *nb_vertical_1 >= 3 && *nb_vertical_2 >=3){
                return 1;
            }
            debut_vertical.x = x;
            debut_vertical.y = y;
            debut_horizontal ;

        }
    }
    return 0;
}*/

// Fonction pour manger un H
/*void manger_H(char plateau[LIGNE][COLONNE],int *nb_vertical_1,int *nb_vertical_2,int *nb_horizontal){

}*/

// Transforme la lettre de l'item en chiffre
int num_item(char item){
    int val;
    switch (item) {
        case 'S':
            val = 0; //Soleil
            break;
        case 'F':
            val = 1; //Fraise
            break;
        case 'P':
            val = 2; //Pomme
            break;
        case 'O':
            val = 3; //Oignon
            break;
        case 'M':
            val = 4; //Mandarine
            break;
    }
    return val;

}

// Detecte et mange 3 ou plus items consecutif
int manger(char matrice[LIGNE][COLONNE], int score[5]) {

	infos allignement[10];
	int n=0, item;
	for (int i = 0; i < 10; i++) {
		allignement[i].nb_alligne = 1;
		allignement[i].type_precedent = ' ';
		allignement[i].type_actuel = ' ';
	}

	//	--- Détection par ligne ---

	for (int i=0; i<LIGNE; i++) {
		allignement[n].nb_alligne = 1;  // AJOUT: Réinitialiser au début de chaque ligne
		allignement[n].type_precedent = ' ';
		allignement[n].type_actuel = ' ';

		for (int j=0; j<COLONNE; j++) {

			allignement[n].type_precedent = allignement[n].type_actuel;
			allignement[n].type_actuel = matrice[i][j];


			if (allignement[n].type_precedent == allignement[n].type_actuel) {
				if (allignement[n].nb_alligne == 1) {
					allignement[n].x_initial = i; allignement[n].y_initial = j-1;
				}
				allignement[n].nb_alligne++;
				allignement[n].x_final = i; allignement[n].y_final = j;
			}
			else if(allignement[n].nb_alligne>2) {n++;}
			else {allignement[n].nb_alligne = 1;}

		}
		if(allignement[n].nb_alligne>2) {n++;}  // AJOUT: Enregistrer l'alignement en fin de ligne
	}

	//	--- Détection par colonne ---

	for (int j=0; j<COLONNE; j++) {
		allignement[n].nb_alligne = 1;  // AJOUT: Réinitialiser au début de chaque colonne
		allignement[n].type_precedent = ' ';  // AJOUT
		allignement[n].type_actuel = ' ';  // AJOUT

		for (int i=0; i<LIGNE; i++) {

			allignement[n].type_precedent = allignement[n].type_actuel;
			allignement[n].type_actuel = matrice[i][j];


			if (allignement[n].type_precedent == allignement[n].type_actuel) {
				if (allignement[n].nb_alligne == 1) {
					allignement[n].x_initial = i-1; allignement[n].y_initial = j;
				}
				allignement[n].nb_alligne++;
				allignement[n].x_final = i; allignement[n].y_final = j;
			}
			else if(allignement[n].nb_alligne>2) {n++;}
			else {allignement[n].nb_alligne = 1;}

		}
		if(allignement[n].nb_alligne>2) {n++;}  // AJOUT: Enregistrer l'alignement en fin de colonne
	}

	//	--- Suppression des cases "alignées" ---

	for (int k=0; k<n; k++) {
		item = num_item(allignement[k].type_precedent);
		int vertical = 0;
		if (allignement[k].x_initial == allignement[k].x_final) {
			vertical = 1;
		}
		for (int j=0; j<COLONNE; j++) {
			for (int i=0; i<LIGNE; i++) {

			if ((i==allignement[k].x_initial)&&(j==allignement[k].y_initial)) {
				matrice[i][j]=' ';
				score[item]++;
				if (vertical) {
					allignement[k].y_initial++;
					if (allignement[k].y_initial > allignement[k].y_final){
						allignement[k].y_initial = allignement[k].y_final;
					}
				}
				else {
					allignement[k].x_initial++;
					if (allignement[k].x_initial > allignement[k].x_final){
						allignement[k].x_initial = allignement[k].x_final;
					}
				}
			}
			if ((allignement[k].nb_alligne>5)&&(matrice[i][j]==allignement[k].type_precedent)) {matrice[i][j]=' '; score[item]++;}

			}
		}
	}
	if (n>0) {return 1;}
	else {return 0;}
}

// Gravitation
void gravite(char plateau[LIGNE][COLONNE]){
    for (int x = 0; x < COLONNE; x++) {
        for (int y = LIGNE - 1; y >= 0; y--) {
            if (plateau[y][x] == ' ') {
                for (int k = y - 1; k >= 0; k--) {
                    if (plateau[k][x] != ' ') {
                        plateau[y][x] = plateau[k][x];
                        plateau[k][x] = ' ';
                        break;
                    }
                }
            }
        }
    }
}

// Fonction pour remplir les trous
void remplir_trous(char plateau[LIGNE][COLONNE]) {
    for (int y = 0; y < LIGNE; y++) {
        for (int x = 0; x < COLONNE; x++) {
            if (plateau[y][x] == ' ') {
                plateau[y][x] = genererItem();
            }
        }
    }
}

//detecte et mange le H (vertical = horizontal = 3 items)
int figure_speciale_H(char plateau[LIGNE][COLONNE],int score[5]){
    for (int y = 0; y < LIGNE-2; y++) {
        for (int x = 0; x < COLONNE-2; x++) {
            char item = plateau[y][x];
            if (item == plateau[y+1][x] && item ==plateau[y+2][x]){
                if (item == plateau[y][x+2] && item ==plateau[y+1][x+2] && item == plateau[y+2][x+2]){
                    if (item == plateau[y+1][x+1]){
                        int val = num_item(item);
                        score[val] += 2*7;
                        plateau[y][x] = ' ';
                        plateau[y+1][x] = ' ';
                        plateau[y+2][x] = ' ';
                        plateau[y][x+2] = ' ';
                        plateau[y+1][x+2] = ' ';
                        plateau[y+2][x+2] = ' ';
                        plateau[y+1][x+1] = ' ';
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// Fonction pour manger un rectangle
void manger_rectangle(char plateau[LIGNE][COLONNE], int largeur, int longueur,int x, int y, int score[5]) {
    char item = plateau[y][x];

    // Effacer le contour uniquement
    for (int j = 0; j < largeur; j++) {
        // ligne du haut
        plateau[y][x + j] = ' ';
        // ligne du bas
        plateau[y + longueur - 1][x + j] = ' ';
    }

    for (int i = 1; i < longueur - 1; i++) {
        // colonne gauche
        plateau[y + i][x] = ' ';
        // colonne droite
        plateau[y + i][x + largeur - 1] = ' ';
    }

    // Calculer les points : 2 * (largeur x longueur)
    int val = num_item(item);
    int points = 2 * (largeur * longueur);
    score[val] += points;
}

// Detecte et mange le rectangle
int rectangle(char plateau[LIGNE][COLONNE], int score[5]) {
    for (int y = 0; y < LIGNE - 1; y++) {
        for (int x = 0; x < COLONNE - 1; x++) {
            char item = plateau[y][x];

            // Tester toutes les largeurs possibles ≥ 2
            for (int larg = 2; x + larg <= COLONNE; larg++) {
                // Tester toutes les hauteurs possibles ≥ 2
                for (int haut = 2; y + haut <= LIGNE; haut++) {
                    int est_rectangle = 1;

                    // Vérifier bord supérieur et inférieur
                    for (int i = 0; i < larg; i++) {
                        if (plateau[y][x + i] != item || plateau[y + haut - 1][x + i] != item) {
                            est_rectangle = 0;
                            break;
                        }
                    }

                    // Vérifier bord gauche et droit
                    for (int j = 0; j < haut; j++) {
                        if (plateau[y + j][x] != item || plateau[y + j][x + larg - 1] != item) {
                            est_rectangle = 0;
                            break;
                        }
                    }

                    if (est_rectangle) {
                        manger_rectangle(plateau,larg,haut,x,y, score);
                        return 1; // Rectangle trouvé et le manger
                    }
                }
            }
        }
    }
    return 0; // Aucun rectangle trouvé
}

//initialise le jeu sans 3 ou plus d'items consecutifs
void initialisation(char plateau[LIGNE][COLONNE]){
    int item;
    for (int y = 0; y < LIGNE; y++) {
        for (int x = 0; x < COLONNE; x++) {
            do {
                item = genererItem();
            } while ((y >= 2 && plateau[y-1][x] == item && plateau[y-2][x] == item) || (x >= 2 && plateau[y][x-1] == item && plateau[y][x-2] == item) || (y >= 1 && x >= 1 && plateau[y-1][x] == item && plateau[y][x-1] == item && plateau[y-1][x-1] == item ));
            plateau[y][x] = item;
            int couleur;
            switch(item) {
                case 'S': couleur = 14; break; // jaune
                case 'F': couleur = 4;  break; // rouge
                case 'P': couleur = 2;  break; // vert
                case 'O': couleur = 5;  break; // magenta
                case 'M': couleur = 1;  break; // bleu
            }
            afficherObjet(plateau, item, x, y, couleur, 0);
        }
    }
}

//Déplace le curseur et permute si sélection
void deplacerCurseur(int *y, int *x, int touche, char plateau[LIGNE][COLONNE], int selection, int *ligne_select, int *colonne_select) {

    int nouvY = *y;
    int nouvX = *x;

    switch(touche) {
        case 72: if(*y>0) nouvY--; break; // ↑
        case 80: if(*y<LIGNE-1) nouvY++; break; // ↓
        case 75: if(*x>0) nouvX--; break; // ←
        case 77: if(*x<COLONNE-1) nouvX++; break; // →
    }

    // Si une lettre est sélectionnée, permuter avec la nouvelle position
    if(selection) {
        int diffY = nouvY - *ligne_select;
        int diffX = nouvX - *colonne_select;

        if((abs(diffY) == 1 && diffX == 0) || (abs(diffX) == 1 && diffY == 0)) {
            char tmp = plateau[nouvY][nouvX];
            plateau[nouvY][nouvX] = plateau[*ligne_select][*colonne_select];
            plateau[*ligne_select][*colonne_select] = tmp;

            // Remettre en majuscule les deux items après permutation
            plateau[nouvY][nouvX] = toupper(plateau[nouvY][nouvX]);
            plateau[*ligne_select][*colonne_select] = toupper(plateau[*ligne_select][*colonne_select]);
        }
    }
    // Mettre à jour la position du curseur
    *y = nouvY;
    *x = nouvX;
}

//Sélectionne/Désélectionne un item
void selectionne_deselectionne(char plateau[LIGNE][COLONNE], int y, int x, int *selection, int *ligne_select, int *colonne_select) {
    if(!(*selection)) {
        // Sélectionner : mettre en minuscule
        plateau[y][x] = tolower(plateau[y][x]);
        *selection = 1;
        *ligne_select = y;
        *colonne_select = x;
    } else {
        plateau[*ligne_select][*colonne_select] = toupper(plateau[*ligne_select][*colonne_select]);
        *selection = 0;
        *ligne_select = -1;
        *colonne_select = -1;
    }
}

// Réaffiche toute la grille avec le curseur
void afficherGrille(char plateau[LIGNE][COLONNE], int curseur_x, int curseur_y) {
    for(int y = 0; y < LIGNE; y++) {
        for(int x = 0; x < COLONNE; x++) {
            char item = plateau[y][x];
            int couleur;
            int couleur_fond = 0;

            // Fond gris si c'est le curseur
            if(y == curseur_y && x == curseur_x) {
                couleur_fond = 7;
            }

            switch(toupper(item)) {
                case 'S': couleur = 14; break;
                case 'F': couleur = 4;  break;
                case 'P': couleur = 2;  break;
                case 'O': couleur = 5;  break;
                case 'M': couleur = 1;  break;
                default: couleur = 7; break;
            }

            afficherObjet(plateau, item, x, y, couleur, couleur_fond);
        }
    }
}


int jouer_1(char plateau[LIGNE][COLONNE]){
    hide_cursor();
    //contrat
    gotoxy(0,0);
    text_color(7);
    printf("Contrat: 20 fraises, 50 oignons et 20 mandarines en 30 coups maximum");
    Sleep(3000); // pause de 3000 millisecondes = 3 secondes

    int nb_coups = 30;
    int score[5]= {0,0,0,0,0};
    //variables pour la gestion du temps
    clock_t temps_debut;
    double temps_ecoule;
    double duree_totale;
    double temps_restant;
    //initialisation du temps
    temps_ecoule=0;
    temps_debut = clock();
    duree_totale = 50.0;

    //variables pour la saise utilisateur (choix de direction de déplacement)
    int touche;
    int x = COLONNE / 2;
    int y = LIGNE / 2;
    int selection = 0, ligne_select = -1, colonne_select = -1;
    initialisation(plateau);
    do{
        //calcul du temps écoulé
        //calcul du temps écoulé

        temps_ecoule = (double)(clock() - temps_debut) / CLOCKS_PER_SEC;
        temps_restant = duree_totale - temps_ecoule;

        //affichage du temps écoulé et du score
        gotoxy(0,2);
        text_color(7);
        printf("Temps restant : %.1f s  ", temps_restant);
        gotoxy(0,3);
        text_color(7);
        printf("Score : %d fraises %d oignons %d mandarines           Coups restant : %d ", score[1], score[3], score[4], nb_coups);

        afficherGrille(plateau, x, y);
        if(figure_speciale_H(plateau,score) == 1 || rectangle(plateau, score) == 1 || manger(plateau,score) == 1 ){
            gravite(plateau);
            remplir_trous(plateau);
        }
        else{
        // partie évènementielle : ce que fait le programme si l'utilisateur appuie sur une touche
            if(kbhit()){ //si une touche a été appuyée
                touche=getch(); //on récupère la touche

                if(touche == 224) {
                    touche = getch();   // la vraie touche des fleches (72/80/75/77)
                    int ancien_selection = selection;
                    deplacerCurseur(&y, &x, touche, plateau, selection, &ligne_select, &colonne_select);

                    // Si permutation effectuée
                    if(ancien_selection && selection) {
                        int diffY = y - ligne_select;
                        int diffX = x - colonne_select;
                        if((abs(diffY) == 1 && diffX == 0) || (abs(diffX) == 1 && diffY == 0)) {
                            selection = 0;
                            ligne_select = -1;
                            colonne_select = -1;
                            nb_coups--;
                        }
                    }
                }
                // espace = sélection/désélection
                else if(touche == 32) {
                    selectionne_deselectionne(plateau, y, x, &selection, &ligne_select, &colonne_select);
                }
            }
        }

        if (score[1] >= 20 && score[3] >= 50 && score[4] >= 20){
            clrscr();//efface la console
            text_color(7);
            printf("\nBravo !! Vous avez reussi le contrat\n");
            show_cursor();
            return 1;
        }

        for(int y = 0; y < LIGNE; y++) {
            for(int x = 0; x < COLONNE; x++) {
                if(plateau[y][x] == ' '){
                    gravite(plateau);
                    remplir_trous(plateau);
                    break;
                }
            }
            break;
        }

        //Petite pause pour que ça n'aille pas trop vite
        Sleep(50);
    }while(temps_restant > 0 && nb_coups > 0);
    clrscr();//efface la console
    text_color(7);
    printf("\nVous avez perdu...\n");
    return 0;

}

int jouer_2(char plateau[LIGNE][COLONNE]){
    return 0;
}

int jouer_3(char plateau[LIGNE][COLONNE]){

    return 0;
}

//lire dans le fichier joueurs.txt
int lire_fichier(char *nom_joueur,int *niveau_joueur){
    FILE *f = fopen(FICHIER,"r");
    if (f == NULL){
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }
    char nom[20];
    int niveau;
    while (fscanf(f,"%s %d",nom,&niveau) == 2) {
        if (strcmp(nom, nom_joueur) == 0) {
            *niveau_joueur = niveau;
            fclose(f);
            return 1;
        }
    }
    //printf("Aucune personne trouvée avec le nom %s dans le fichier\n", nom_joueur);
    fclose(f);
    return 0;
}

//sauvegarde le joueur et le niveau dans le fichier joueurs.txt
void ecrire_fichier(char *nom_joueur, int niveau_joueur) {
    FILE *f = fopen(FICHIER, "r");          // lecture si existe
    FILE *temp = fopen("temp.txt", "w");    // fichier temporaire
    if (!temp) {
        printf("Erreur ouverture fichier temporaire\n");
        return;
    }

    int trouve = 0;
    char nom[20];
    int niveau;

    if (f) {
        while (fscanf(f, "%19s %d", nom, &niveau) == 2) {
            if (strcmp(nom, nom_joueur) == 0) {
                fprintf(temp, "%s %d\n", nom_joueur, niveau_joueur);
                trouve = 1;
            } else {
                fprintf(temp, "%s %d\n", nom, niveau);
            }
        }
        fclose(f);
    }

    if (!trouve) {
        fprintf(temp, "%s %d\n", nom_joueur, niveau_joueur);
    }

    fclose(temp);

    // Remplacer l'ancien fichier par le nouveau
    if (remove(FICHIER) != 0) {
    }
    if (rename("temp.txt", FICHIER) != 0) {
        printf("Erreur lors du renommage du fichier temporaire\n");
    }
}


void toggleMusic(int* musique) {
	if(*musique==1) {*musique = 0;}
	else {*musique = 1;}
	if (*musique == 1) {
		PlaySound("QueensGardens.wav",NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	}
	else {
		PlaySound(NULL, 0, 0);
	}
}

//fonction principale
int main(){
    srand(time(NULL));
    char plateau[LIGNE][COLONNE];
    int nb_vie = 5;
    int choix;
    char joueur[20];
    int niveau;
    int musique = 0;
    char ch;
    printf("Entrez votre prenom\n");
    scanf("%20s",joueur);

    do{
        printf("\nNombre de vie restant : %d vies\n",nb_vie);
        //gestion du menu
        choix=menu();

        switch(choix){
            case 1 : // lancement d'une partie
                clrscr();
                if(lire_fichier(joueur,&niveau)){
                    if(niveau == 1){
                        if(jouer_1(plateau)){
                            niveau++;
                            ecrire_fichier(joueur,niveau);
                        }
                        else{
                            nb_vie--;
                            ecrire_fichier(joueur,niveau);
                        }
                    }
                    else if(niveau == 2){
                        if(jouer_2(plateau)){
                            niveau++;
                            ecrire_fichier(joueur,niveau);
                        }
                        else{
                            nb_vie--;
                            ecrire_fichier(joueur,niveau);
                        }
                    }
                    else if(niveau == 3){
                        if(jouer_3(plateau)){
                            niveau = 1 ;
                            ecrire_fichier(joueur,niveau);
                        }
                        else{
                            nb_vie--;
                            ecrire_fichier(joueur,niveau);
                        }
                    }
                }
                else{
                    niveau = 1;
                    if(jouer_1(plateau)){
                        niveau++;
                        ecrire_fichier(joueur,niveau);
                    }
                    else{
                        nb_vie--;
                        ecrire_fichier(joueur,niveau);
                    }
                }
                break;
            case 2 :
                afficherRegles(); break;

			case 3 :
				toggleMusic(&musique);
				break;
        }
        text_color(7);

    }while(nb_vie != 0 && choix != 4);
    text_color(7); //retour a la couleur normale

	PlaySound(NULL, 0, 0);

    return 0;
}

