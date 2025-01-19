#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "part1.h"
#include "part2.h"
#include "part3.h"

int main(int argc, char* argv[]){

    if(strcmp(argv[1], "init") == 0){
        initRefs();
        initBranch();
    }

    if (strcmp(argv[1], "list-refs") == 0){
        printf("REFS : \n");
        if(file_exists(".refs")){
            List* L= listdir(".refs");
            for( Cell* ptr = *L; ptr != NULL; ptr = ptr->next){
                if(ptr->data[0]!='.'){
                    char* content = getRef(ptr->data);
                    printf(" %s \t %s \n", ptr->data, content);
                }
            }
        }
    }

    if(strcmp(argv[1],"create-ref") == 0){
        createUpdateRef(argv[2],argv[3]);
    }

    if(strcmp(argv[1],"delete-ref") == 0){
        deleteRef(argv[2]);
    }

    if(strcmp(argv[1],"add") == 0){
        for( int i = 2; i < argc ; i++){
            myGitAdd(argv[i]);
        }
    }

    if(strcmp(argv[1],"clear-add") == 0){
        system( "rm .add");
    }

    if(strcmp(argv[1],"list-add") == 0){
        printf("Zone de préparation : \n");
        if(file_exists(".add")){
            WorkTree* wt = ftwt(".add");
            printf("%s \n", wtts(wt));
        }
    }   

     if(strcmp(argv[1],"commit") == 0){
        if(strcmp(argv[3],"-m") == 0){
            char* mess = malloc(sizeof(char)*(MESSAGE_SIZE+argc));
            strcpy(mess,"");
            //On concatène tout les arguments suivant -m pour recréer le message
            int i = 4;
            while( argv[i] !=  NULL){
                strcat(mess, argv[i]);
                strcat(mess, " ");
                i++;
            }
            myGitCommit(argv[2], argv[4]);
        }
        else{
            myGitCommit(argv[2], NULL);
        }
    }

    if(strcmp(argv[1],"get-current-branch") == 0){
        printf("current branch : %s \n", getCurrentBranch());
    }

    if(strcmp(argv[1],"branch") == 0){
        char* br = argv[2];
        if(branchExists(br) == 1){
            printf("Branch %s already exists", br);
        }
        else{
            createBranch(br);
        }
    }

    if(strcmp(argv[1],"branch-print") == 0){
        char* br = argv[2];
        if(branchExists(br) == 0){
            printf("Branch %s does not exist", br);
        }
        else{
            printBranch(br);
        }
    }

    if(strcmp(argv[1],"checkout-branch") == 0){
        char* br = argv[2];
        if(branchExists(br) == 0){
            printf("Branch %s does not exist", br);
        }
        else{
            myGitCheckoutBranch(br);
        }
    }

    if(strcmp(argv[1],"checkout-commit") == 0){
        myGitCheckoutCommit(argv[2]);
    }

    if(strcmp(argv[1], "merge") == 0){
        //On récupère la liste de colisions
        List* L_colis = merge( argv[2], argv[3]);
        
        //La fusion s'est bien passée si la liste est nulle
        if( L_colis == NULL){
            printf( "La fusion s'est bien passée.");
        }

        //Sinon on propose 3 choix à l'utilisateur 
        else{
            int choix;
            printf("Choisissez la proposition 1, 2 ou 3. \n  Choix 1 : Garder les fichiers de la branche courante et supprimer les fichiers de la branche secondaire \n Choix 2 : Garder les fichiers de la branche secondaire et supprimer les fichiers de la branche principale \n Choix 3 : Choisir manuellemrnt quels fichiers supprimer \n ");
            scanf("Choix numéro : %d", &choix);

            if(choix == 1){
                //commit de suppression pour la branche courante
                createDeletionCommit(argv[2], L_colis, argv[3]);
                //on fusionne les 2 branches
                merge(argv[2], argv[3]);
                printf( "La fusion s'est bien passée.");
            }

            else if(choix == 2){
                //commit de suppression pour la branche courante
                char* cur_branch = getCurrentBranch();
                createDeletionCommit(cur_branch, L_colis, argv[3]);
                //on fusionne les 2 branches
                merge(argv[2], argv[3]);
                printf( "La fusion s'est bien passée.");
            }

            else if(choix == 3){
                //On récupère la branche courante
                char* cur_branch = getCurrentBranch();
                //création des listes de suppression pour chacune des branches
                List* L1 = initList();
                List* L2 = initList();
                //On choisit dans quelle branche on veut supprimer chaque fichiers en collision
                Cell* ptr = *L_colis;

                while(ptr != NULL){
                    //On choisit dans quel branche on veut supprimer chaque fichier
                    int choix2;
                    printf("Ecrire 1 pour supprimer %s de %s ou écrire 2 pour le supprimer de %s", ptr->data, cur_branch, argv[2] );
                    scanf("Choix numéro : %d", &choix2);

                    //On ajoute le fichier à suppimer dans la liste de suppression de la branche choisit
                    if(choix2 == 1){
                        insertFirst(L1, buildCell(ptr->data) );
                    }

                    else if(choix2 == 2){
                        insertFirst(L2, buildCell(ptr->data) );
                    }
                    
                    else{
                        printf( "Erreur : la valeur choisit n'est pas reconnue. \n Il faut refaire la commande ./myGit merge");
                    }
                    ptr = ptr->next;
                }

                //On crée les commits de suppression puis on fusionne les branches
                createDeletionCommit(cur_branch, L1, argv[3]);
                createDeletionCommit(argv[2], L2, argv[3]);
                merge(argv[2], argv[3]);
                printf( "La fusion s'est bien passée.");

            }


            //Si choix n'est pas 1, 2 ou 3
            else{
                printf( "%d n'est pas un choix.", choix);
            }

            //On supprime la liste de collision
            Cell* pt = *L_colis;
            while (pt != NULL){
                Cell* tmp = pt->next;
                free(pt);
                pt = tmp;
            }
        }
    }

    return 0;
}