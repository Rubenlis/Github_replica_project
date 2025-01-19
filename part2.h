#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#ifndef PART2_H
#define PART2_H

#define N 10

typedef struct{
    char* name;
    char* hash;
    int mode;
} WorkFile;

typedef struct { 
    WorkFile* tab;
    int size;
    int n;
} WorkTree;

int getChmod(const char *path);

void setMode(int mode, char* patch);

WorkFile* createWorkFile( char* name );

char* wfts(WorkFile* wf);

WorkFile* stwf(char* ch);

WorkTree* initWorkTree();

int inWorkTree(WorkTree* wt , char* name );

int appendWorkTree(WorkTree* wt ,char* name ,char* hash, int mode );

char* wtts(WorkTree* wt );

WorkTree* stwt(char* ch);

int wttf(WorkTree* wt, char* path);

WorkTree* ftwt(char* file);

char* hashToFile(char* hash);

char* blobWorkTree(WorkTree* wt);

char* concat_paths(char* path1, char* path2);

char* saveWorkTree(WorkTree* wt, char* path);

int isWorkTree(char* hash);

void restoreWorkTree(WorkTree* wt, char* path);



#endif