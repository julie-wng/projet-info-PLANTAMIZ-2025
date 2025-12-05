#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "affichage_console.h"

#define LIGNE 15
#define COLONNE 10

typedef struct{
    int x;
    int y;
}Coordonne;

typedef struct infos {
	char type1;
	char type2;
	int nb;
	int x1;
	int y1;
	int x2;
	int y2;
}infos;

//affichage du menu
int menu() {
    int choix;
    printf("\n1. Jouer");
    printf("\n2. Regles du jeu");
    printf("\n3. Quitter\n");
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

//initialise le jeu sans 3 ou plus d'items consecutifs
void initialisation(char plateau[LIGNE][COLONNE]){
    int item;
    for (int y = 0; y < LIGNE; y++) {
        for (int x = 0; x < COLONNE; x++) {
            do {
                item = genererItem();
            } while ((y >= 2 && plateau[y-1][x] == item && plateau[y-2][x] == item) || (x >= 2 && plateau[y][x-1] == item && plateau[y][x-2] == item));
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

//mange le H (vertical = horizontal = 3 items)
char figure_speciale_H(char plateau[LIGNE][COLONNE],int *score){
    for (int y = 0; y < LIGNE; y++) {
        for (int x = 0; x < COLONNE; x++) {
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

int rectangle(char plateau[LIGNE][COLONNE], int *largeur, int *longueur, Coordonne *debut) {
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
                        *largeur = larg;
                        *longueur = haut;
                        debut->x = x;
                        debut->y = y;
                        return 1; // Rectangle trouvé
                    }
                }
            }
        }
    }
    return 0; // Aucun rectangle trouvé
}

// Fonction pour manger un rectangle
void manger_rectangle(char plateau[LIGNE][COLONNE], int largeur, int longueur, Coordonne debut, int *score) {
    char item = plateau[debut.y][debut.x];
    
    // Effacer le contour uniquement
    for (int j = 0; j < largeur; j++) {
        // ligne du haut
        plateau[debut.y][debut.x + j] = ' ';
        // ligne du bas
        plateau[debut.y + longueur - 1][debut.x + j] = ' ';
    }

    for (int i = 1; i < longueur - 1; i++) {
        // colonne gauche
        plateau[debut.y + i][debut.x] = ' ';
        // colonne droite
        plateau[debut.y + i][debut.x + largeur - 1] = ' ';
    }

    // Calculer les points : 2 * (largeur x longueur)
    int points = 2 * (largeur * longueur);
    *score += points;
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
    for(int i = 0; i < LIGNE; i++) {
        for(int j = 0; j < COLONNE; j++) {
            char item = plateau[i][j];
            int couleur;
            int couleur_fond = 0;
            
            // Fond gris si c'est le curseur
            if(i == curseur_y && j == curseur_x) {
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
            
            afficherObjet(plateau, item, j, i, couleur, couleur_fond);
        }
    }
}

int jouer_1(char plateau[LIGNE][COLONNE]){
    //contrat
    gotoxy(0,0);
    text_color(7);
    printf("Contrat: 20 fraises, 50 oignons et 20 mandarines en 30 coups maximum");
    int nb_fraises = 20;
    int nb_oignon = 50;
    int nb_mandarines = 20;
    int nb_coups = 0;
    int score[5]= {0};
    //variables pour la gestion du temps
    clock_t temps_debut;
    double temps_ecoule;

    //initialisation du temps
    temps_ecoule=0;
    temps_debut = clock();
    //variables pour la saise utilisateur (choix de direction de déplacement)
    int touche;
    int x = COLONNE / 2;
    int y = LIGNE / 2;
    int selection = 0, ligne_select = -1, colonne_select = -1;
    initialisation(plateau);
    do{
        //calcul du temps écoulé
        temps_ecoule = (double)(clock() - temps_debut) / CLOCKS_PER_SEC;
        //affichages
        //affichage du temps écoulé et du score
        gotoxy(0,2);
        text_color(7);
        printf("Temps : %.1f s  ", temps_ecoule);
        gotoxy(0,3);
        text_color(7);
        printf("Score : %d Coups: %d ", score, nb_coups); 

        afficherGrille(plateau, x, y);
        
        if(manger)

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
                        nb_coups++;
                    }
                }
            }
            // espace = sélection/désélection
            else if(touche == 32) {
                selectionne_deselectionne(plateau, y, x, &selection, &ligne_select, &colonne_select);
            }
        }
        //Petite pause pour que ça n'aille pas trop vite
        Sleep(50);
    }while(temps_ecoule < 10.0);
    clrscr();//efface la console
    text_color(7);
    printf("\nFin du jeu sur le premier tableau. Score final = %d\n", score);
    return 0;
    
}

int jouer_2(char plateau[LIGNE][COLONNE]){
    
    return 0;
}

int jouer_3(char plateau[LIGNE][COLONNE]){

    return 0;
}

//lire dans un fichier 
int lire_fichier(char *file,char *nom_joueur,int *niveau_joueur){
    FILE *f = fopen(file,"r");
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

//sauvegarder le joueur et le niveau dans un fichier .txt
void ecrire_fichier(char *file, char *nom_joueur, int niveau_joueur) {
    int niveau_actuel;
    if (lire_fichier(file, nom_joueur, &niveau_actuel)) {
        FILE *f_lecture = fopen(file, "r");
        FILE *f_temp = fopen("temp.txt", "w");
        
        if (f_lecture == NULL || f_temp == NULL) {
            printf("Erreur d'ouverture du fichier\n");
            if (f_lecture) fclose(f_lecture);
            if (f_temp) fclose(f_temp);
            return;
        }
        
        char nom[20];
        int niveau;
        while (fscanf(f_lecture, "%s %d", nom, &niveau) == 2) {
            if (strcmp(nom, nom_joueur) == 0) {
                fprintf(f_temp, "%s %d\n", nom, niveau_joueur);
            } else {
                fprintf(f_temp, "%s %d\n", nom, niveau);
            }
        }
        
        fclose(f_lecture);
        fclose(f_temp);
        
        remove(file);
        rename("temp.txt", file);
    } else {
        FILE *f = fopen(file, "a");
        if (f == NULL) {
            printf("Erreur d'ouverture du fichier\n");
            return;
        }
        fprintf(f, "%s %d\n", nom_joueur, niveau_joueur);
        fclose(f);
    }
}

//fonction principale
int main(){
    srand(time(NULL));
    char plateau[LIGNE][COLONNE];
    int nb_vie = 5;
    /*int choix;
    char joueur[20];
    printf("Entrez votre prenom\n");
    scanf("%20s",joueur);
    choix=menu();*/
    hide_cursor();
    clrscr();
    initialisation(plateau);
    jouer_1(plateau);
    show_cursor();
    text_color(7);
    return 0;
}

