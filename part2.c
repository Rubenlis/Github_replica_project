#include "part1.h"
#include "part2.h"

//Focntions données

int getChmod(const char* path){
    struct stat ret;

    if(stat(path, &ret) == -1){
        return -1;
    }

    return (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);
}

void setMode(int mode, char* path){
    char buff[100];
    sprintf(buff, "chmod %d %s", mode ,path);
    system(buff);
}

// EXERCICES 4 et 5  !!!!

// Exercice 4

WorkFile* createWorkFile( char* name ){
    WorkFile* wf = malloc(sizeof(WorkFile) ) ;
    wf -> name = strdup(name ) ;
    wf -> hash = NULL ;
    wf -> mode = 0;
    return wf;
}

char* wfts(WorkFile* wf){
    char* ch= malloc(sizeof(char)*MAXDATA);
    sprintf(ch, "%s \t %s \t %d", wf->name, wf->hash, wf->mode) ;
    return ch;
}

WorkFile* stwf(char* ch){
    char* name = malloc(sizeof(char)*MAXDATA);
    char* hash = malloc(sizeof(char)*MAXDATA);
    int mode;
    sscanf(ch, "%s\t%s\t%d", name, hash, &mode) ;
    WorkFile* wf = createWorkFile( name);
    wf->mode= mode;
    wf->hash= hash;
    return wf;
}

WorkTree* initWorkTree(){
    WorkTree* wt=malloc(sizeof(WorkTree));
    wt->tab = malloc(N*sizeof(WorkFile));
    wt->size=N;
    wt->n=0;
    return wt;
}

int inWorkTree(WorkTree* wt , char* name ){
    for(int i=0; i<wt->n;i++){
        if(strcmp(wt->tab[i].name,name) == 0){
            return 1;
        }
    }
    return -1;
}

int appendWorkTree(WorkTree* wt ,char* name ,char* hash, int mode ){
    if(inWorkTree(wt,name) >= 0){
        printf("Name: %s is already in the WT \n", name);
        return -1;
    }
    if(wt->size > wt->n){
        wt->tab[wt->n].mode = mode;
        wt->tab[wt->n].name = strdup(name);
        if( hash != NULL){
            wt->tab[wt->n++].hash = strdup(hash);
        }
        else{
            wt->tab[wt->n++].hash = NULL;
        }
        return 0;
    }
    return -1;
}   

char* wtts(WorkTree* wt ){
    char* ch = malloc(sizeof(char)*MAXDATA*N);
    
    for(int i =0; i<wt->n; i++){
        char* ch2 = wfts(wt->tab + i);
        strcat(ch, ch2);
        strcat(ch, "\n");
    }
    return ch;

}

WorkTree* stwt(char* ch){
    int pos = 0;
    int n_pos =0;
    int sep= '\n';
    char* ptr;
    char* result = malloc(sizeof(char)*MAXDATA*N);
    WorkTree* wt = initWorkTree();
    while(pos < strlen(ch)){
        ptr = strchr( ch + pos, sep);
        if(ptr==NULL){
            n_pos = strlen(ch)+1;
        }
        else{
            n_pos = ptr-ch+1;
        }
        memcpy(result, ch+pos, n_pos-pos-1);
        result[n_pos - pos -1]= '\0';
        pos = n_pos ;
        WorkFile* wf = stwf(result);
        appendWorkTree(wt, wf->name, wf->hash, wf->mode);
    }
    return wt;
}


int wttf(WorkTree* wt, char* path){
    FILE *fp = fopen(path,"w");
    if(fp != NULL){
        fputs(wtts(wt),fp);
        fclose(fp);
        return 0;
    }
    return -1;
}

WorkTree* ftwt(char* file){
    char* buff = malloc(sizeof(char)*N);
    FILE* f= fopen(file,"r");
    char* all_wf = malloc(sizeof(char)*N*MAXDATA);
    strcpy( all_wf, "");
    while(fgets(buff, N, f)!= NULL){
        strcat(all_wf, buff);
    }
    return stwt(all_wf);
}

//Exercice 5

char* hashToFile(char* hash){
    char* ch2 = strdup(hash);
    ch2[2]='\0';
    struct stat st;
    if(stat(ch2, &st )==-1){
        mkdir(ch2, 0700);
    }
    return hashToPath(hash);
}

char* blobWorkTree(WorkTree* wt){
    char* fname = malloc(sizeof(char) * MAXDATA) ;
    strcpy( fname, "/tmp/myfileXXXXXX");
    mkstemp(fname);
    wttf(wt,fname);

    char* hash = sha256file(fname);
    char* ch = malloc(sizeof(char) * MAXDATA);
    strcpy(ch, hashToFile(hash) );
    strcat(ch,".t");
    cp(ch, fname);

    char buffer[MAXDATA];
    sprintf(buffer, "rm %s", fname);
    system(buffer);

    return hash;
}

/*
char* blobWorkTree( WorkTree * wt ) {
    char fname [100] = "/tmp/myfileXXXXXX";
    int fd = mkstemp( fname );
    wttf(wt , fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch ,".t");
    cp(ch , fname);
    return hash ;
}*/

char* concat_paths(char* path1, char* path2){
    char* result = malloc( (strlen(path1)+strlen(path2)+1)*sizeof(char) );
    if (result == NULL ){
        printf("Error: unable to allocate memory \n");
        return NULL;
    }
    //on concatène path1 et 2 séparés par un "/"
    strcpy(result,path1); 
    strcat(result,"/");
    strcat(result, path2);
    return result;
}

char* saveWorkTree(WorkTree* wt, char* path){
   

    for(int i=0; i<wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name);

        if (opendir(absPath)== NULL) {
            
            blobFile(absPath);
            wt->tab[i].hash = sha256file(absPath);
            wt->tab[i].mode = getChmod(absPath);

        }

        else{
            
            WorkTree* wt2 = initWorkTree();
            List* L= listdir(absPath);
            for(Cell* ptr = *L; ptr != NULL; ptr = ptr->next){
                if(ptr->data[0]=='.'){
                    continue;
                }
                appendWorkTree(wt2, ptr->data, NULL, 0);
            }
            wt->tab[i].hash = saveWorkTree(wt2, absPath);
            wt->tab[i].mode = getChmod(absPath);
            free(wt2);
        }
        
    }

    return blobWorkTree(wt);
}

int isWorkTree(char* hash){
    if( file_exists(strcat(hashToPath(hash), ".t") ) ){
        return 1;
    }
    if( file_exists(hashToFile(hash))){
        return 0;
    }
    return -1;
}

void restoreWorkTree(WorkTree* wt, char* path){
    for(int i=0; i < wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name);
        char* copyPath = hashToPath(wt->tab[i].hash);
        char* hash = wt->tab[i].hash;

        if(isWorkTree(hash)==0){// si c'est un fichier
            cp(absPath, copyPath);
            setMode( getChmod(copyPath),absPath);
        }

        else if(isWorkTree(hash) == 1){
            strcat(copyPath, ".t");
            WorkTree* nwt = ftwt(copyPath);
            restoreWorkTree(nwt, absPath);
            setMode(getChmod(copyPath),absPath );
        }
    }
}

