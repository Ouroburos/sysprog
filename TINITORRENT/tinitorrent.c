#include "tinitorrent.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <openssl/sha.h>


void main_menu(ttcmode_t* mode){

    unsigned char num, opt;
    int ret;
    char * filepath = NULL;


    printf("Choose Mode:               Key:  \n");
    printf("Make .tinitorrent file......0  \n");
    printf("Standard mode...............1  \n");
    printf("Leech mode..................2  \n");
    printf("Config......................3  \n");
    printf("Quit......................Ctr1-C \n");

    scanf("%c",&num);

    switch(num) {

     case '0':
        printf("Please specify a path to the file: \n");
        //scanf("%s", filepath);
        ret = make_tinitorrent(filepath);

          if(ret == 1){
              printf("The file was not valid.");
          }
        break;

     case '1':
        /* Here, we fork/daemonize and otherwise wait for cancellation signal */
        if(*(mode) == STANDARD)
        printf("The client is already in Standard Mode. Press Ctrl-Z to abort mode. \n");
        else if(*(mode) == LEECH)
        printf("The client is already in Leech Mode. Press Ctrl-Z to abort mode. \n");
        else {
            *(mode) = STANDARD;
        }
        break;

     case '2':
        /* Here, we fork/daemonize and otherwise wait for cancellation signal */
        if(*(mode) == STANDARD)
        printf("The client is already in Standard Mode. Press Ctrl-Z to abort mode. \n");
        else if(*(mode) == LEECH)
        printf("The client is already in Leech Mode. Press Ctrl-Z to abort mode. \n");
        else {
            *(mode) = LEECH;
        }
        break;

     case '3':
        /* Possibly change defined listen port? Number of connections possible? */
        printf("Would you like to change the default configuration? (y/n)\n");
        scanf("%c", &opt);

         if(opt == 'y' || 'Y'){
            // chconfig();
         }
         else if(opt == 'n' || 'N'){
         }
         else
            printf("That was not a valid option.\n");

        break;

     default:
        printf("That was not a valid option.\n");

    }
}

/* Makes a .tinitorrent and pieces of a given file */
int make_tinitorrent(char* fpath){

     int i, wi, numpc, lpsize;
     size_t fnlen;
     struct stat info;
     FILE *file, *pf;
     char* temp;
     tinitorrent ttor;

     unsigned char  piece[PIECE_SIZE];
     unsigned char  hash[SHA_DIGEST_LENGTH];
     unsigned char  buff[SHA_DIGEST_LENGTH*2];

     stat(fpath,&info);

         if((info.st_mode & S_IFMT) != S_IFDIR)
           { printf("A valid path was not specified.");
             return 1;
           }else{
             file = fopen(fpath, "r");
         }

         /* Normally, request tracker URL but here only ip is set */
         ttor.ptcl = PTCL;
         ttor.ipv4_tadr = (unsigned char*)"127.0.0.1";
         ttor.ipv4_len = (unsigned short)strlen(ttor.ipv4_tadr);

         /* Storing original filename */
         fpath = basename(fpath);
         ttor.fname = (unsigned char*)malloc(sizeof(char)*strlen(fpath));
         ttor.fname = (unsigned char*)fpath;
         fnlen = strcspn(fpath, ".");

         unsigned char fname[fnlen+1];

         fname[fnlen] = '\0'; //capture the filename minus .ext
         numpc = ((int)info.st_size / PIECE_SIZE) +1;

         lpsize = ttor.filesize - ((numpc-1)*PIECE_SIZE);

         /* Storing Ori.namelen, filesize, num pieces */
         ttor.fnlen = strlen(fpath);
         ttor.filesize = (unsigned int)info.st_size;
         ttor.numpc = (unsigned short)numpc;

         temp = (char*)malloc(sizeof(char) * strlen(TTORDIR)+(strlen(fname)+5));
         ttor.hsh_list = (char*)malloc(sizeof(char)*40*numpc);

/**************************************************************/
/*====================Assembling Hash List====================*/
/*=====================making piece files=====================*/
     for(wi = 0; wi < numpc; wi++){

          memset(temp, '\0', sizeof(temp));
          memset(piece, 0x0, PIECE_SIZE);
          memset(hash, 0x0, SHA_DIGEST_LENGTH);
          memset(buff, 0x0, SHA_DIGEST_LENGTH*2);

          if(wi < (numpc-1)){
              fread(piece, sizeof(char), PIECE_SIZE, file);
          }else{
              fread(piece, sizeof(char), lpsize, file);
          }


           sprintf(temp,"%s%s%d",TTORDIR,fname,wi+1);

           pf = fopen (temp, "w"); //making piece files
           fwrite(piece, sizeof(char), strlen(piece), pf);
           fclose(pf);

          SHA1(piece, strlen(&piece), hash);

            for (i=0; i < SHA_DIGEST_LENGTH; i++) {
              sprintf((char*)&(buff[i*2]), "%02x", temp[i]);
           }

            if(wi == 0)//first hash, strcpy
                strcpy(ttor.hsh_list, buff);
            else
                strcat(ttor.hsh_list, buff);

          }

         memset(temp, '\0', sizeof(temp));
         sprintf(temp,"%s%s%s",TTORDIR,fname,".tinitorrent");


/**************************************************************/
/*====================Writing to .tinitorrent=================*/

    pf = fopen(temp,"wb");

    fwrite(&ttor.ptcl, sizeof(short), 1, pf);
    fwrite(&ttor.ipv4_len, sizeof(short), 1, pf);
    fwrite(ttor.ipv4_tadr, sizeof(char), ttor.ipv4_len, pf);
    fwrite(&ttor.fnlen, sizeof(short), 1, pf);
    fwrite(ttor.fname, sizeof(char), ttor.fnlen, pf);
    fwrite(&ttor.filesize, sizeof(int), 1, pf);
    fwrite(&ttor.numpc, sizeof(short), 1, pf);
    fwrite(ttor.hsh_list, SHA_DIGEST_LENGTH, ttor.numpc, pf);

    fclose(pf);
    fclose(file);

    return 0;
}

/* Loads the configuration for the client
 * If it doesn't exist yet, default values are used
 * stored.
 */
Config_t* lconfig(){

    Config_t* conf = (Config_t *)malloc(sizeof(Config_t));
    struct stat confbuf;
    FILE* cf = NULL;

    if(stat("../DATA001/peer.conf",&confbuf) != 0 && errno == ENOENT){

       conf->PeerId = PEERID;
       conf->TorDir = TTORDIR;
       conf->TrkDir = TTRKDIR;
       conf->AsmDir = ASSMDIR;
       conf->ttorlen = strlen(TTORDIR);
       conf->ttrklen = strlen(TTRKDIR);
       conf->assmlen = strlen(ASSMDIR);

       cf = fopen("../DATA001/peer.conf","w");

         fwrite(conf->PeerId, sizeof(char), 20, cf);
         fwrite(&conf->ttorlen, sizeof(unsigned int), 1, cf);
         fwrite(&conf->ttrklen, sizeof(unsigned int), 1, cf);
         fwrite(&conf->assmlen, sizeof(unsigned int), 1, cf);
         fwrite(conf->TorDir , sizeof(char), conf->ttorlen, cf);
         fwrite(conf->TrkDir , sizeof(char), conf->ttrklen, cf);
         fwrite(conf->AsmDir, sizeof(char), conf->assmlen, cf);

   }
    else{ //it already exists
       cf = fopen("../DATA001/peer.conf","r");
       
       conf->PeerId = (char*)malloc(sizeof(char)* 20);
         fread( conf->PeerId, sizeof(char), 20, cf);
         fread( &conf->ttorlen, sizeof(unsigned int), 1, cf);
         fread( &conf->ttrklen, sizeof(unsigned int), 1, cf);
         fread( &conf->assmlen, sizeof(unsigned int), 1, cf);

       conf->TorDir = (char*)malloc(sizeof(char)* conf->ttorlen);
       conf->TrkDir = (char*)malloc(sizeof(char)* conf->ttrklen);
       conf->AsmDir = (char*)malloc(sizeof(char)* conf->assmlen);
         fread( conf->TorDir, sizeof(char), conf->ttorlen, cf);
         fread( conf->TrkDir, sizeof(char), conf->ttrklen, cf);
         fread( conf->AsmDir, sizeof(char), conf->assmlen, cf);

     }
       fclose(cf);

    return conf;
}

/*Config_t* chconfig(){


}*/


/* void cleanup(pid_t child){ 
   //cleanup here
   need to free Config_t and mode

} */
