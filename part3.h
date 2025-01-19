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

#ifndef PART3_H
#define PART3_H

#define Ncommit 100
#define MESSAGE_SIZE 1000

typedef struct key_value_pair{
    char* key;
    char* value;
}   kvp;

typedef struct hash_table{
    kvp** T;
    int n; 
    int size;
}HashTable;

typedef HashTable Commit;

kvp* createKeyVal(char* key, char* val);

void freeKeyVal(kvp* kv);

char* kvts(kvp* k);

kvp* stkv(char* str);

unsigned long hashF(char* str);

Commit* initCommit();

void commitSet(Commit* c, char* key, char* value);

Commit* createCommit(char* hash);

char* commitGet(Commit* c, char* key);

char* cts(Commit* c );

Commit* stc(char* ch);

Commit* ftc(char* file);

void ctf(Commit* c, char* file);

char* blobCommit(Commit* c);

void initRefs();

void createUpdateRef(char* ref_name, char* hash);

void deleteRef(char* ref_name);

char* getRef(char* ref_name);

void createFile(char* file);

void myGitAdd(char* file_or_folder);

void myGitCommit(char* branch_name, char* message);

void initBranch();

int branchExists(char* branch);

void createBranch(char* branch);

char* getCurrentBranch();

char* HashToPathCommit(char* hash);

void printBranch(char* branch);

List* branchList(char* branch);

List* getAllCommits();

void restoreCommit(char* hash_commit);

void myGitCheckoutBranch(char* branch);

List* filterList(List* L, char* pattern);

void myGitCheckoutCommit(char* pattern);

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);

List* merge(char* remote_branch, char* message);

void createDeletionCommit(char* branch, List* conflicts, char* message);

#endif