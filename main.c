#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "part1.h"
#include "part2.h"
#include "part3.h"



int main(){

    /*
    printf("Test exo 1.1 : %s \n", sha256file("main.c" ) );
    
    List* L= initList();
    Cell* c1 = buildCell("help1");
    Cell* c2 = buildCell("help2");
    Cell* c3 = buildCell("help3");
    
    insertFirst(L, c3);
    insertFirst(L, c2);
    insertFirst(L, c1);
    
    printf("Test ltos exo 2.4 : %s \n", ltos(L) );
    printf("Test ctos exo 2.4 : %s \n", ctos( listGet(L,2) ) );
    searchList(L, "help4");
    
    ltof(L, "testEx2_8.txt");
    printf("Test ftol et ltof exo 2.8 : %s \n", ltos(ftol("testEx2_8.txt"))); 

    List* Ldir=  listdir(".");

    printf("Test exo 3.1 : %s \n", ltos(Ldir));

    ltof(Ldir, "Test3_1.txt");

    printf("Test exo 3.2 : %d\n", file_exists("Makefile"));

    cp("Test3_3.txt","main.c");

    char* hash = sha256file("main.c" );
    char* path = hashToPath(hash);
    printf("Test exo 3.4 \n hash : %s\n path : %s\n", hash, path);
    */

    //Test 3.5
    //blobFile("main.c");




    //Tests Ex4
/*
    WorkFile* wf1 = createWorkFile("workF1");
    printf("Test exo 4.1/4.2 : %s \n", wfts(wf1));

    //Test 4.3

    WorkFile* wf2 = stwf("workF2 (null)  0 ");
    printf("Test exo 4.3 : %s \n", wfts(wf2));
    free(wf1);free(wf2);*/

    //Tests 4.4 4.6

    WorkTree* wt1 = initWorkTree();
    appendWorkTree(wt1, "part1.h", sha256file("part1.h"), 777);
    appendWorkTree(wt1, "part2.h", sha256file("part2.h"), 777);

    printf("Test inWorkTree() 4.5, position de part2.h : %d\n", inWorkTree(wt1, "part2.h"));

//    char* swt1 = wtts(wt1);
//    printf("\nTest wtts() 4.7\n%s\n", swt1);
//    printf("Test stwt() 4.8\n%s\n", wtts(stwt(swt1)));

    appendWorkTree(wt1, "Makefile", sha256file("Makefile"), 777);
//    appendWorkTree(wt1, "cat", sha256file("cat"), 777);
    

    wttf(wt1, "Test4_9.txt");
    WorkTree* wt2 = ftwt("Test4_9.txt");
    char* swt2 = wtts(wt2);
    printf("Test ftwt() 4.10\n%s\n", swt2);



    //Tests Ex5

//    char* hashWT = blobWorkTree(wt2);
//    printf("Test blobWorkTree() 5.1 : %s\n", hashWT);
    
    
    char* hashWT = saveWorkTree(wt1, ".");
    printf("Test saveWorkTree() 5.2 : %s\n", hashWT);

//    restoreWorkTree(wt1, ".");






    /*
    
    ./myGit init
    ./myGit list-refs

    ./myGit add part1.c part1.h part2.c part2.h part3.c part3.h
    ./myGit list-add
    ./myGit add Makefile myGit.c main.c
    ./myGit list-add
    
    ./myGit get-current-branch

    ./myGit commit HEAD -m 1er_essai
    ./myGit branch-print HEAD

    ./myGit checkout-branch HEAD
    ./myGit get-current-branch
    */


    return 0;
}