#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(int argc, char * argv[]) {
    char line[40];
    char * linetokens[4];
    char * temp;
    int counter;
    /*for (int i = 0; i < 4; i++){
        linetokens[i] = (char*)malloc(sizeof(char)*10);
    }*/
    while (fgets(line, 40, stdin)!=NULL){
        temp = strdup(line);
       // strtok(temp, " \n\r");
        strtok(temp, " \n\r");
        while(temp != NULL){
            linetokens[counter] = strdup(temp);
            counter++;
            strtok(NULL, " \n\r");
        }
        for (int k = 0; k < 4; k++)
            printf("%s",linetokens[k]);
        counter = 0;
    }
    /* 
    */

    

}