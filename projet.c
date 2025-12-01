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

// Afficher un caractère à une position dans la console avec une couleur et l'ajoute a la matrice 
void afficherObjet(char plateau[LIGNE][COLONNE],char c, int x, int y, int couleur) {
    gotoxy(x, y);
    text_color(couleur);  
    printf("%c",c); 
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
            afficherObjet(plateau, item, x, y, couleur);
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
    for (int y = 0; y < LIGNE-1; y++) {
        for (int x = 0; x < COLONNE-1; x++) {
            plateau[y][x] = ' ';
        }
    }
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

//detecte un rectangle
int rectangle(char plateau[LIGNE][COLONNE], int *largeur,int *longueur,Coordonne debut){
    for (int y = 0; y < LIGNE-1; y++) {
        for (int x = 0; x < COLONNE-1; x++) {
            char item = plateau[y][x];
            int larg = 1;
            while(x + larg < COLONNE && plateau[y][x+larg] == item){
                larg++;
            }
            int haut = 1;
            while(y + haut < LIGNE && plateau[y+haut][x] == item){
                haut++;
            }
            if (larg<=haut || haut<=larg){
                int est_rectangle = 1;
                for (int i = 0; i < haut && est_rectangle; i++) {
                    for (int j = 0; j < larg && est_rectangle; j++) {
                        if (plateau[y + i][x + j] != item) {
                            est_rectangle = 0;
                        }
                    }
                }
                
                if (est_rectangle) {
                    *largeur = larg;
                    *longueur = haut;
                    debut.x = x;
                    debut.y = y;
                    return 1; 
                }
            }
        }
    }
    return 0;
}

// Fonction pour manger un rectangle
void manger_rectangle(char plateau[LIGNE][COLONNE], int largeur, int longueur, Coordonne debut, int *score) {
    char item = plateau[debut.y][debut.x];
    
    //mange le rectangle
    for (int i = 0; i < longueur; i++) {
        for (int j = 0; j < largeur; j++) {
            plateau[debut.y + i][debut.x + j] = ' ';
        }
    }
    
    // Calculer les points : 2 * (largeur x longueur)
    int points = 2 * (largeur * longueur);
    *score += points;
}

void deplacement_curseur(char touche, int *x, int *y){
    //pour aller vers le haut
    if (touche == 'z' && *y > 1){
        *y = *y - 1;
    }
    //pour aller vers le bas
    if (touche == 's' && *y < LIGNE){
        *y = *y + 1;
    }
    // pour aller a gauche 
    if (touche == 'q' && *x > 1){
        *x = *x - 1;
    }
    //pour aller a droite 
    if (touche == 'd' && *x < COLONNE){
        *x = *x + 1;
    }
}

void permuter_items(char plateau[LIGNE][COLONNE], int x1, int y1, int x2, int y2) {
    char temp = plateau[y1][x1];
    plateau[y1][x1] = plateau[y2][x2];
    plateau[y2][x2] = temp;
}

void manger(){

}

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


int jouer_1(char plateau[LIGNE][COLONNE]){
    //contrat
    gotoxy(20,0);
    text_color(7);
    printf("Contrat: 20 fraises, 50 oignons et 20 mandarines en 30 coups maximum");
    int nb_fraises = 20;
    int nb_oignon = 50;
    int nb_mandarines = 20;

    int score=0;
    //variables pour la gestion du temps
    clock_t temps_debut;
    double temps_ecoule;

    //initialisation du temps
    temps_ecoule=0;
    temps_debut = clock();
    //variables pour la saise utilisateur (choix de direction de déplacement)
    char touche='a';
    initialisation(plateau);
    do{
        //calcul du temps écoulé
        temps_ecoule = (double)(clock() - temps_debut) / CLOCKS_PER_SEC;
        //affichages
        //affichage du temps écoulé et du score
        gotoxy(20,2);
        text_color(7);
        printf("Temps : %.1f s  ", temps_ecoule);
        gotoxy(20,3);
        text_color(7);
        printf("Score : %d  ", score);

        // partie évènementielle : ce que fait le programme si l'utilisateur appuie sur une touche 
        if(kbhit()){ //si une touche a été appuyée
            touche=getch(); //on récupère la touche

        }
        //Petite pause pour que ça n'aille pas trop vite
        Sleep(50);
    }while(temps_ecoule < 10.0);
    clrscr();//efface la console
    text_color(7);
    printf("\nFin du jeu sur le premier tableau. Score final = %d\n", score);
    return 0;
    
}

int jouer_2(){

    return 0;
}

int jouer_3(){

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

