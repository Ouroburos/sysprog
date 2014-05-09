#include <stdio.h>
#include <signal.h>
#include <unistd.h>


#include "tinitorrent.h"

Config_t* confg;
ttcmode_t* mode;

void handlesig(int sig){
  if(sig == SIGQUIT) 
     main_menu(mode);
  /* if sig == SIGINT abort mode
        sig == SIGSTP quit all rev */
}

int main(void)
{
    struct sigaction catchall;
    sigset_t mask;
    mode = malloc(sizeof(ttcmode_t));

    *mode = IDLE;
   
   //load configuration
    confg = lconfig();

    //Intializing the sig handler
    sigfillset(&mask);
    sigdelset(&mask, SIGQUIT);
    sigdelset(&mask, SIGTSTP);
    sigdelset(&mask, SIGINT);
    catchall.sa_handler = &handlesig;
    catchall.sa_mask = mask;

    sigaction(SIGQUIT, &catchall, NULL);

    printf("___  ___       \n");
    printf(" |    |        \n");
    printf(" |INY |ORRENT  \n");
    printf("\n");

    main_menu(mode);
    /* Here, wait for any incoming signals */
    while(1) { sleep(1); }

    return 0;
}

