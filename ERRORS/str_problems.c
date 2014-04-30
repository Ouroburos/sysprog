#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This is giving a segfault at line 26
//
int main(){

     unsigned char* path = "the/path/to/the/file/file.f";
     size_t dlen, fnlen = 0;
     unsigned char* dirpth = (unsigned char*)malloc(strlen(path));
     unsigned char* fname = (unsigned char*)malloc(strlen(path));
    
     memset((void *)dirpth, '\0', strlen(dirpth));
     memset((void *)fname, '\0', strlen(fname));

     fname = strrchr(path, '/');
     fnlen = strlen(fname); //7
     dlen  = strlen(path) - fnlen; //20
 
     strncpy(dirpth, path, dlen+1); 

     //getting the filename
     for(int i = 0; i < fnlen; i++)
     {
        fname[i] = path[dlen+i+1];
     }

         // do some stuff with the hash
 return 0;

}
