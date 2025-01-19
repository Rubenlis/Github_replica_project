#include "part1.h"

//EXERCICES 1 à 3 !!!!


// Exercice 1

int hashFile(char* source, char* dest){
	char* buffer = malloc(sizeof(char)*MAXDATA);
	const char* s="cat ";
	const char* s2 = "| sha256sum > ";
	strcat( strcpy(buffer, s) , source);
	strcat( buffer , s2) ;
	strcat( buffer, dest );
	 
	system( buffer );
	return 0;
}

char* sha256file(char* file){
	char buf[256];
	char* hash= malloc(sizeof(char)*256);
	
	hashFile( file, "f.tmp");
	
	FILE* fic = fopen("f.tmp" , "r");
	
	if(fic==NULL){
		printf("Probleme ouverture fichier");
		return "erreur";
	}
	
	fgets(buf, 256, fic );
	sscanf(buf, "%s", hash); 
	
	fclose(fic);
	system( "rm f.tmp" );
	
	return hash;
}

// Exercice 2 


List* initList(){
	List* L=malloc(sizeof(List));
	*L = NULL;
	return L;
}


Cell* buildCell( char* ch ){
	Cell* c = malloc( sizeof (Cell) ) ;
	c->data = strdup( ch ); 
	c->next = NULL;
	return c;
}

void insertFirst( List* L, Cell* C){
	C->next = *L;
	*L = C;
}

char* ctos(Cell* c){
	return c->data;
}

char* ltos(List* L){

	if(*L == NULL){
		return "";
	}

    List pt = *L;

    int compt=0;

    while(pt != NULL){
        compt+=1;
        pt= pt->next;
    }
    free(pt);

	char* buf= malloc(sizeof(char)*MAXDATA*compt);
    strcpy(buf,"");
	List ptr = *L;
	while(ptr != NULL){
		strcat(buf, ctos(ptr) );
		ptr = ptr->next;
		if(ptr!=NULL){
			strcat(buf,"|");
		}
	}
    free(ptr);
	return buf;
}

Cell* listGet(List* L, int i){
	Cell* ptr = *L;
	int k=0;
	
	while(ptr!= NULL){
		if(k==i){
			return ptr;
		}
		k = k+1;
		ptr = ptr -> next; 
		
	}
	
	printf( "Error, index out of range ! \n");
	return ptr;
}

Cell* searchList(List* L, char* str){
	List ptr = *L;
	while(ptr !=NULL){
		if( strcmp(str, ptr->data) == 0){
		
			return ptr;
		}
		ptr=ptr->next;
	}
	
	return NULL;
}
	
	
List* stol(char* s){
    int pos = strlen(s)-1 ; 
    int end = strlen(s);
    List* L = initList();

    while( pos > 0){

        pos = end;

        char*res = malloc( sizeof(char)*MAXDATA);

        while( *(s+pos-1)!= '|' && pos>0 ){
            pos = pos -1;
        }
        
        for(int i= 0; i< end-pos; i++){
            *(res+i)=*(s+i+pos);
        }
        
        insertFirst(L, buildCell(res));
        
        end = pos-1;
        free(res);
    }

    return L;

}
void ltof(List* L, char* path) {
    FILE* f = fopen(path, "w");

    if (f==NULL) {
        printf("Probleme ouverture fichier");
        return ;
    }

    fputs(ltos(L), f);
    fclose(f);
}


List* ftol(char* path) {
    FILE *f = fopen(path, "r");

    if (f==NULL) {
        printf("Probleme ouverture fichier");
        return NULL;
    }

    char buffer[MAXDATA];
    fgets(buffer, MAXDATA, f);
    fclose(f);
    return stol(buffer);
}


// Exercice 3;


List* listdir(char* root_dir) {
    DIR* dp;
    struct dirent *ep;
    List* L = initList();
    Cell* cell_tmp;
    dp = opendir(root_dir);
    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            cell_tmp = buildCell(ep->d_name);
            insertFirst(L, cell_tmp);
            List ptr = *L;
            while (ptr != NULL) {
                ptr = ptr->next;
            }
        }
        (void) closedir(dp);
    }
    else {
        return NULL;
    }

    return L;
}

/*
int file_exists(char *file){
    List* L = listdir(".");
    List ptr = *L;
    char* tmp = malloc(sizeof(char)*MAXDATA);
    strcpy(tmp,"");
    int i = 0;
    while(file[i]!='\0') {
        if(file[i]=='/') {
            continue;
        }
        strcat(tmp, file[i]);
        i++;
    }

    if(tmp != "") {
        while(ptr != NULL ){
        if(strcmp(ptr->data,tmp)==0){
            char buff[100];
            sprintf(buff, "cd %s", ptr->data);
            system(buff);
            file_exists()
        }
        ptr =ptr->next ; 
        }
    }

    else {
        while(ptr != NULL ){
            if(strcmp(ptr->data,file)==0){
                return 1;
            }
            ptr =ptr->next ; 
        }
    }


    return 0;
}*/

/*
int file_exists(char *file){
    List* L = listdir(".");
    List ptr = *L;
    while(ptr != NULL ){
        if(strcmp(ptr->data,file)==0){
            return 1;
        }
        ptr =ptr->next ; 
    }
    return 0;
}
*/

struct stat st = {0};

int file_exists ( char * file ) {
    struct stat buffer ;
    return ( stat ( file , & buffer ) == 0) ;
}


void cp(char *to, char *from) {
    if (from==NULL) {
        return;
    }

    FILE *fTo = fopen(to, "w");
    if (fTo==NULL){
        printf("Erreur ouverture de fichier");
        return;
    }

    FILE *fFrom = fopen(from, "r");
    if (fFrom==NULL){
        printf("Erreur ouverture de fichier");
        return;
    }

    char buffer[MAXDATA];
    while(fgets(buffer, MAXDATA, fFrom) != NULL) {
        fputs(buffer, fTo);
    }

    fclose(fTo);
    fclose(fFrom);
}


char* hashToPath(char* hash) {
    char* path = malloc(sizeof(char)*(strlen(hash)+1));
    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    int i = 2;
    while(*(hash+i) != '\0') {
        path[i+1] = *(hash+i);
        i++;
    }
    path[i+1] = '\0';
    return path;
}

void blobFile(char* file){
    char* hash = sha256file(file);
    char* ch2 = strdup(hash);
    ch2[2]='\0';
    if(!file_exists(ch2)){
        char buff[100];
        sprintf(buff, "mkdir %s", ch2);
        system(buff);
    }
    char* ch = hashToPath(hash);
    cp(ch, file);
}








	




























































