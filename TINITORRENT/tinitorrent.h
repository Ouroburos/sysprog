#ifndef TINITORRENT_H
#define TINITORRENT_H

#define PIECE_SIZE 4096
#define TTORDIR "../DATA001/"
#define TTRKDIR "../DATA001/"
#define ASSMDIR "../DATA002/"
#define PEERID "3501-isf47jr2057fgt4"

#define PTCL 6881

#define TTORPORT 69763

#include <string.h>
#include <stdlib.h>

//used to keep track of mode tinitorrent client is running in.
typedef enum ttcmode_t{IDLE, STANDARD, LEECH} ttcmode_t;

//Struct that contains information for .tinitorrent file
typedef struct tinitorrent{
    unsigned short ptcl;
    unsigned short ipv4_len;
    unsigned char* ipv4_tadr;

    unsigned short fnlen;
    unsigned char * fname;
    unsigned int filesize;
    unsigned short numpc;
    unsigned char* hsh_list;

    //char * path; ..suggested location
} tinitorrent;

//Tinitorrent client configuration struct
typedef struct Config_t{
    char* PeerId;
    unsigned int   ttorlen;
    unsigned int   ttrklen;
    unsigned int   assmlen;
    char* AsmDir;
    char* TorDir;
    char* TrkDir;
}Config_t;


void main_menu(ttcmode_t *mode);

//returns 0 on success, returns 1 on failure
int make_tinitorrent(char* fpath);


Config_t* lconfig();
Config_t* chconfig();

/*Possible functions for code coherency
 *----------------------------------------
 * int write_to_ttorf(tinitorrent t);
 *
 *
 */

#endif // TINITORRENT_H
