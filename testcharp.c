#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
 int a;
 int p;
 char* temp;

 p = 5;
 a = p + 5;
 printf("The number to init the array %d\n", a);
 char* arr[a];
 
 temp = (char*)malloc(sizeof(char) * (strlen("file")+5));
 //arr = malloc(sizeof(char*) * 10);
 for(p = 0; p < a; p++){
  memset(temp, '\0', sizeof(temp));
  sprintf(temp,"%s%d","file",p);
  arr[p] = (char*)malloc(sizeof(char)*strlen(temp));
  printf("The file name is %s\n", temp);
  strcpy(arr[p],temp);
  printf("Pointer %d has the value of %s.\n", p, *arr[p]); 

 }
 
 return 0;

}
