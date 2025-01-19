#include "part1.h"
#include "part2.h"
#include "part3.h"

// EXERCICES 6 à 9 + Exercice 11 !!!!

//Exercice 6

kvp* createKeyVal(char* key, char* val){
    kvp* kv = malloc(sizeof(kvp));
    kv->key = strdup(key);
    if (val != NULL) {
        kv->value = strdup(val);
    }
    else {
        kv->value = NULL;
    }
    return kv;
}

void freeKeyVal(kvp* kv){
    free(kv->key );
    free(kv->value);
    free(kv);
}

char* kvts(kvp* k){
    char* ch= malloc(sizeof(char)*(MAXDATA*2+1) );
    strcpy(ch, "");
    strcat(ch, k->key);
    strcat(ch, " : ");
    if(k->value!=NULL) {
        strcat(ch, k->value);
    }
    else {
        strcat(ch,"NULL");
    }

    return ch;
}

kvp* stkv(char* str){
    char key[MAXDATA];
    char val[MAXDATA];
    sscanf(str, "%s : %[^\n]", key, val);
    return createKeyVal(key, val);
}

unsigned long hashF(char* str){
    unsigned long hash = 5381;
    int c;

    while (c == *str++ ){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
return hash;
}

Commit* initCommit(){
    Commit* c = malloc(sizeof(Commit));
    c->T = malloc(sizeof(kvp*)*Ncommit);
    c->size = Ncommit;
    for(int i = 0; i<c->size; i++){
        c->T[i] = NULL;
    }
    c->n=0;
    return c;
}

void commitSet(Commit* c, char* key, char* value){
    int p = hashF(key)%c->size;
    while(c->T[p]!=NULL){
        p= (p+1)%c->size; 
    }
    c->T[p]= createKeyVal(key, value);
    c->n++;
}

Commit* createCommit(char* hash){
    Commit* c = initCommit();
    commitSet(c, "tree", hash);
    return c;
}

char* commitGet(Commit* c, char* key){
    int p = hashF(key) % c->size;
    int attempt = 0;
    while(c->T[p] != NULL && attempt < c->size){
        if(strcmp(c->T[p]->key, key) == 0){
            return c->T[p]->value;
        }
        p= (p+1)%c->size;
        attempt += 1;
    }
    return NULL;
}

char* cts(Commit* c ){
    char* str = malloc(sizeof(char)*MAXDATA*c->n);
    strcpy(str, "");
    for(int i = 0; i< c->size; i++){
        if(c->T[i] != NULL){
            strcat(str, kvts(c->T[i]));
            strcat(str, "\n");
        }
    }
    return str;
}

Commit* stc(char* ch){
    int pos = 0; 
    int n_pos = 0;
    int sep = '\n';
    char* ptr;
    char* result = malloc(sizeof(char)*MAXDATA*Ncommit);
    Commit* c = initCommit();
    while( pos < strlen(ch)){
        ptr = strchr(ch + pos, sep);
        if( ptr == NULL){
            n_pos= strlen(ch)+1;
        }
        else{
            n_pos= ptr - ch + 1;
        }
        memcpy(result, ch+pos, n_pos - pos -1);
        result[n_pos - pos -1 ]= '\0';
        pos = n_pos;
        kvp* elem = stkv(result);
        commitSet(c, elem->key, elem->value);
    }
    return c;
}

Commit* ftc(char* file){
    char buff[MESSAGE_SIZE];
    char* all = malloc(sizeof(char) * MESSAGE_SIZE);
    strcpy(all, "");
    FILE* f= fopen(file, "r");
    if(f == NULL){
        printf( "ERROR : file doesn't exist \n");
        return NULL;
    }
    while(fgets(buff, N, f) != NULL){
        strcat(all, buff);
    }
    Commit* c = stc(all);
    return c;
}

void ctf(Commit* c, char* file){
    FILE* fp = fopen(file, "w");
    if( fp != NULL){
        fputs(cts(c), fp);
        fclose(fp);
    }
}
/*
char* blobCommit(Commit* c){
    char fname[MAXDATA]="/tmp/myfileXXXXXX";
    mkstemp(fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".c");
    cp(ch, fname);
    return hash;
}*/



char* blobCommit(Commit* c){
    char fname[MAXDATA]="/tmp/myfileXXXXXX";
    mkstemp(fname);
    ctf(c, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".c");
    cp(ch, fname);
    
    char buffer[MAXDATA+4];
    sprintf(buffer, "rm %s", fname);
    system(buffer);
    return hash;
}



//Exercice 7

void initRefs(){
    if(!file_exists(".refs")){
        system("mkdir .refs");
        system("touch .refs/master");
        system("touch .refs/HEAD");
    }
}


void createUpdateRef(char* ref_name, char* hash){

    //fonction d'écriture dans un fichier pour écrire le hash dans la branche
    char* nom_fic = malloc( sizeof(char)*MAXDATA);
    sprintf(nom_fic, ".refs/%s", ref_name);
    FILE* fic = fopen(nom_fic, "w");

    if( fic == NULL){
        printf("Problème d'ouverture du fichier.");
    }

    fputs(hash, fic);
    fclose(fic);
}



void deleteRef(char* ref_name){
    char buff[MAXDATA];
    sprintf(buff, ".refs/%s", ref_name);
    if(!file_exists(buff)){
        printf("The reference %s does not exist", ref_name);
    }
    else {
        sprintf( buff, "rm .refs/%s ", ref_name);
        system(buff);
    }
}


char* getRef(char* ref_name){
    FILE* fp;
    char* result=malloc(sizeof(char)*MAXDATA);
    char buff[MAXDATA];
    sprintf(buff, ".refs/%s", ref_name);
    if(!file_exists(buff)){
        printf("The reference %s does not exist\n", ref_name);
        return NULL;
    }
    fp = fopen(buff, "r");
    if( fp == NULL){
        printf("Error opening file. \n");
        return NULL;
    }
    fgets(result, MAXDATA, fp);
    fclose(fp);
    return result;
}

void createFile(char* file){
    char buff[100];
    sprintf(buff, "touch %s", file);
    system(buff);
}

void myGitAdd(char* file_or_folder){
    WorkTree* wt;
    if(!file_exists(".add")){
        createFile(".add");
        wt = initWorkTree();
    }
    else{
        wt = ftwt(".add");
    }
    if( file_exists(file_or_folder)){
        appendWorkTree(wt, file_or_folder, NULL , 0);
        wttf(wt, ".add");
    }
    else{
        printf("file or folder %s does not exist \n", file_or_folder);
    }
}

void myGitCommit(char* branch_name, char* message){
    if(!file_exists(".refs")){
        printf("il faut d'abord initialiser les références du projet\n");
        return;
    }

    char buff[MAXDATA];
    sprintf(buff, ".refs/%s", branch_name);

    if(!file_exists(buff)){
        printf("La branche n'existe pas\n");
        return;
    }
    char* last_hash = getRef(branch_name);
    char* head_hash = getRef("HEAD");
    if(strcmp(last_hash, head_hash) != 0){
        printf("HEAD doit pointer sur le dernier commit de la branche\n");
        return ;
    }

    WorkTree* wt = ftwt(".add");
    char* hashwt = saveWorkTree(wt, ".");
    Commit* c = createCommit(hashwt);

    if(strlen(last_hash)!=0){
        commitSet(c, "predecessor", last_hash);
    }
    if(message != NULL){
        commitSet(c, "message", message);
    }
    char* hashc = blobCommit(c);
    createUpdateRef(branch_name, hashc);
    createUpdateRef("HEAD", hashc);
    system("rm .add");
}

//Exercice 8

void initBranch(){
    FILE* f = fopen(  ".current_branch", "w");
    fputs("master",f);
    fclose(f);
}

int branchExists(char* branch){
    List* refs = listdir( ".refs");
    return searchList(refs, branch) != NULL ;
}


void createBranch(char* branch){
    char* hash = getRef("HEAD");
    createUpdateRef( branch, hash);
}

char* getCurrentBranch(){
    FILE* f = fopen(".current_branch", "r" );
    char* ch = malloc(sizeof(char)*MAXDATA);
    fscanf(f, "%s", ch);

    return ch;
}

char* HashToPathCommit(char* hash){
    char* buff = malloc(sizeof(char)*100);
    sprintf(buff, "%s.c", hashToPath(hash));
    return buff;
}

void printBranch(char* branch){
    char* commit_hash = getRef(branch);
    Commit* c = ftc(HashToPathCommit(commit_hash));

    while( c != NULL){

        if( commitGet(c,"message") != NULL){
            printf( "%s -> %s\n", commit_hash, commitGet(c, "message"));
        }
        else{
            printf("%s \n", commit_hash);
        }

        if( commitGet(c,"predecessor") != NULL ){
            commit_hash = commitGet(c, "predecessor");
            c = ftc( HashToPathCommit( commit_hash ));
        }
        else{
            c = NULL;
        }
    }
}

List* branchList(char* branch){
    List* L = initList();
    char* commit_hash = getRef(branch);
    Commit* c = ftc(HashToPathCommit(commit_hash));

    while( c != NULL){

        Cell* cell_hash = buildCell(commit_hash);
        insertFirst(L, cell_hash);

        if( buildCell( commitGet(c, "predecessor"))  != NULL){
            commit_hash = commitGet(c, "predecessor");
            c = ftc( HashToPathCommit( commit_hash ));
        }
        else{
            c = NULL;
        }
    }

    return L;
}


List* getAllCommits(){

    List* L = initList();
    List* content = listdir(".refs");

    for(Cell* ptr = *content; ptr != NULL; ptr = ptr->next){
        if(ptr->data[0] == '.'){
            continue;
        }
        List* list = branchList( ptr->data);
        Cell* cell = *list;
        while(cell != NULL){
            if(searchList(L, cell->data) == NULL){
                insertFirst( L, buildCell(cell->data));
            }
            cell = cell->next;
        }
    }
    return L;
}

//Exercice 9 

void restoreCommit(char* hash_commit){
    char* commit_path = HashToPathCommit(hash_commit);
    Commit* c = ftc(commit_path);
    char* tree_hash = malloc(sizeof(char) * MAXDATA);
    strcpy(tree_hash, "");
    strcat(tree_hash, hashToPath(commitGet(c, "tree")) ) ;
    strcat( tree_hash, ".t");
    WorkTree* wt = ftwt(tree_hash);
    restoreWorkTree(wt, ".");
}

void myGitCheckoutBranch(char* branch){
    //Change current_branch
    FILE* f = fopen(".current_branch", "w");
    fprintf(f, "%s", branch);
    fclose(f);

    char* hash_commit = getRef(branch);
    createUpdateRef("HEAD",hash_commit);
    restoreCommit(hash_commit);
}

List* filterList(List* L, char* pattern){
    List* filtered = initList();
    for(Cell* ptr = *L; ptr != NULL; ptr = ptr->next){
        char* ch = strdup(ptr->data);
        ch[strlen(pattern)] = '\0';
        if( strcmp(ch, pattern) == 0){
            insertFirst(filtered, buildCell(ptr->data));
        }
        free(ch);
    }
    return filtered;
}

void myGitCheckoutCommit(char* pattern){
    List* L = getAllCommits() ; 
    List* filtered = filterList( L, pattern ) ;
    Cell* ptr = *filtered;

    if(ptr == NULL){
        printf("No pattern matching \n");
    }

    else if(ptr->next == NULL){
        createUpdateRef("HEAD", ptr->data);
        restoreCommit(ptr->data);
    }

    else{
        printf( "Multiple matching found : \n");
        for(Cell* pt= *filtered; pt!= NULL; pt = pt->next){
            printf("-> %s \n", pt->data);
        }
    }
}

// Exercice 11

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts){
    //On initialise le workTree qui sera renvoyé par la fonction
    WorkTree* wt = initWorkTree();
    
    //On trouve les fichiers en conflit entre les 2 WT et on met leur nom dans *conflicts
    for(int i = 0; i < wt1->n ; i++ ){
        for(int j = 0; j < wt->n ; j++ ){
            if( wt1->tab[i].name == wt2->tab[j].name){
                //Si les hashs des 2 fichiers de même nom sont différents on les ajoute à la liste  des conflits
                if(wt1->tab[i].hash != wt2->tab[j].hash){
                    insertFirst( *conflicts, buildCell(wt1->tab[i].name));
                }
            }
            j++;
        }
        i++;
    }

    //On ajoute à wt les Workfile qui ne sont pas en conflits de wt1 puis de wt2

    for(int i2 = 0; i2 < wt1->n ; i2++ ){
        if( searchList(*conflicts, wt1->tab[i2].name) == NULL){
            appendWorkTree(wt, wt1->tab[i2].name, wt1->tab[i2].hash, wt1->tab[i2].mode );
        }
        i2++;
    }
    
    for(int j2 = 0; j2 < wt2->n ; j2++ ){
        if( searchList(*conflicts, wt2->tab[j2].name) == NULL){
            appendWorkTree(wt, wt1->tab[j2].name, wt1->tab[j2].hash, wt1->tab[j2].mode );
        }
        j2++;
    }

    return wt;
}

List* merge(char* remote_branch, char* message){
    //initialisation de la liste de collision
    List* L = initList();
    List** ptr_L = &L ; 

    //On trouve le nom de la branche courante
    char* current_branch =getCurrentBranch();

    //On récupère les WT associés aux branches 
    Commit* c1 = ftc( HashToPathCommit( getRef(current_branch) ) );
    Commit* c2 = ftc( HashToPathCommit( getRef(remote_branch) ) );

    WorkTree* wt1 = ftwt( hashToPath( commitGet(c1, "tree") ) );
    WorkTree* wt2 = ftwt( hashToPath( commitGet(c2, "tree") ) );

    //On crée le WT, fusion des 2 WT précédents
    WorkTree* wt_fus =  mergeWorkTrees(wt1, wt2, ptr_L);
    
    //Si il y a des collisions on retourne la liste des collisions 
    if(L != NULL){
        return L;
    }

    //On crée le commit associé au WT
    char* hash_wt_fus = blobWorkTree( wt_fus );
    Commit* c_fus = createCommit( hash_wt_fus );

    //On cherche la liste des hash des commits des 2 branches
    List* hash_pred = branchList(current_branch);
    List* hash_merged_pred = branchList(remote_branch);
    //On ajoute à c_fus le hash du dernier commit de la branche courante et de la branche à supprimer
    commitSet(c_fus, "predecessor", (*hash_pred)->data);
    commitSet(c_fus, "merged_predecessor", (*hash_merged_pred)->data);

    //On ajoute le message descriptif en paramètre au commit
    commitSet(c_fus, "message", message);

    //Enregistrement instantané de c_fus
    char* hash_c_fus = blobCommit( c_fus );

    createUpdateRef(current_branch, hash_c_fus);
    createUpdateRef("HEAD", hash_c_fus);

    //On supprime remote_branch
    deleteRef( remote_branch );
    
    //On restaure le projet associé au worktree de fusion
    restoreCommit(hash_c_fus);

    //retourn NULL car il n'y a pas de collisions
    return NULL;
}


void createDeletionCommit(char* branch, List* conflicts, char* message){

    //On récupère la branche dans laquelle on est actuellement 
    char* cur_branch = getCurrentBranch();

    //On se  déplace sur la branche branch
    myGitCheckoutBranch(branch);

    //On récupère le dernier commit de la branche et son workTree associé
    char* last_commit_hash = getRef(branch);
    Commit* last_c = ftc(HashToPathCommit(last_commit_hash));
    char* hash_last_wt = commitGet( last_c, "tree");
    WorkTree* wt = ftwt( hashToPath( hash_last_wt ) );

    //On vide la zone de préparation puis on ajoute les fichiers/répertoires du worktree qui ne sont pas en conflits
    system( "rm .add");

    for(int i = 0; i < wt->n; i++){
        if(searchList( conflicts, wt->tab[i].name) == NULL){
            myGitAdd(wt->tab[i].hash ); 
        }
        i++;
    }
    //On crée le commit de suppression
    myGitCommit( branch, message);
    //On revient sur la branche précèdente
    myGitCheckoutBranch(cur_branch);
}   
