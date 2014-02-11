/* Sadid Homework#2 
 * 2/10/14        */

#include <stdio.h>

//gets rid of accidentally entered characters
void discard()
{
   char c;
   while(c = getchar()!= '\n');
}

int main(){
   
   char yn; 
   unsigned int i1, i2, i3, i4, ip1, ip2, subnet, netid; 
   
   yn = 'n';
   //requesting ips and subnet mask in decimal
 while(yn == 'n')
 {  printf("Please enter your ip address in the following format: X.X.X.X \n");
    scanf("%d.%d.%d.%d",&i1,&i2,&i3,&i4);
    discard();

    printf("received: %d.%d.%d.%d. Is this correct? (y/n)\n",i1,i2,i3,i4);
    scanf("%c",&yn);
    discard();
 }   
    yn = 'n';

      //bit shifting and | to complete decimal int
       ip1 = i1 << 24; 
       ip1 = ip1 | (i2 << 16);
       ip1 = ip1 | (i3 << 8);
       ip1 = ip1 |  i4;
    
 
    //receiving subnet mask
   while(yn == 'n')
  {  printf("Please enter your subnet mask in the following format: X.X.X.X \n");
     scanf("%d.%d.%d.%d",&i1,&i2,&i3,&i4);
     discard();

     printf("received: %d.%d.%d.%d. Is this correct? (y/n)\n",i1,i2,i3,i4);
     scanf("%c",&yn);
     discard();
  }      
     yn = 'n'; 

        //bit shifting and | to complete decimal subnet mask
        subnet = i1 << 24;
        subnet = subnet | (i2 << 16);
        subnet = subnet | (i3 << 8);
        subnet = subnet | i4;

    //receiving sencond ip
  while(yn == 'n')
 {  printf("Please enter the ip of a device to see if it is within the subnet (X.X.X.X)\n");
    scanf("%d.%d.%d.%d",&i1,&i2,&i3,&i4);                     
    discard();

    printf("received: %d.%d.%d.%d. Is this correct? (y/n)\n",i1,i2,i3,i4);
    scanf("%c",&yn);
    discard();
 }        
         //bit shifting and | to complete second ip address in decimal
         ip2 = i1 << 24;
         ip2 = ip2 | (i2 << 16);
         ip2 = ip2 | (i3 << 8);
         ip2 = ip2 | i4;

    //at this point, we have both ips and the subnet mask
    //if the net id is the same for both ips, they are within the same subnet, I
    //believe. ip & subnet will give the network id.
    
    netid = subnet & ip1; 
    if((subnet & ip2) == netid)
      printf("the address %d.%d.%d.%d is within your sub-network",i1,i2,i3,i4);
    else
      printf("the address %d.%d.%d.%d is not within your sub-network",i1,i2,i3,i4);
      
  return 0;
}
