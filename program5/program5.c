#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char * argv[]) {
    char line[40];
    char * linetokens[4];
    for (int i = 0; i < 4; i++)
        linetokens[i] = NULL;
    char * temp;
    char * tokens;
    int counter = 0;
    while (fgets(line, 40, stdin)!=NULL){
        temp = strdup(line);
        tokens = strtok(temp, " \n\r");
        while(tokens!=NULL){
            linetokens[counter] = strdup(tokens);
            counter++; //increment counter
            tokens = strtok(NULL, " \n\r"); //move to next token in line
        }

        /*  for (int k = 0; k < 4; k++) // PRINT TOKENS
            if(linetokens[k]!=NULL)
             printf("%s\n",linetokens[k]);*/
             

             
        if (!linetokens[3]){
            // RAM COMMAND
            // format: line# RAM SIZE
            // description: allocate memory of SIZE
            if (strcmp(linetokens[1], "RAM")==0){
                printf("reached RAM command\n");
                //malloc size
            }
            // STOP COMMAND
            // format: line# STOP
            // description: ends memory allocation simulation
            if (strcmp(linetokens[1], "STOP")==0){
                printf("reached final STOP command\n");
                exit(EXIT_SUCCESS);
            }
            // DUMP COMMAND
            // format: line# DUMP
            // description: gives the base and bounds of all active processes
            if (strcmp(linetokens[1], "DUMP")==0){
                printf("reached DUMP command\n");
            }
        }
        else{
            // START COMMAND
            // format: line# START process# size
            // description: simulate allocating memory to the process in memory
            if (strcmp(linetokens[1], "START")==0){
                printf("reached START command\n");
            }
            // LOC COMMAND
            // format: line# process# LOC address#
            // description: simulate translating virtual address to physical address
            if (strcmp(linetokens[2], "LOC")==0){
                printf("reached LOC command\n");
            }
            // STOP PROCESS COMMAND
            // format: line# STOP process#
            // description: simulate ending a process
            if (strcmp(linetokens[1], "STOP")==0){
                printf("reached STOP PROCESS command\n");
            }
        }
        counter = 0;
        for (int k = 0; k < 4; k++)
            linetokens[k] = NULL;
    }
}