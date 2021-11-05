/*
Jesus Barba CS474-Program 5
Memory Management Simulation
Purpose: Simulate memory management using swapping and first fit
Strategy: Maintain linked lists for active processes and for free memory segments, as well as maintaining a counter for total free memory
INPUT FROM FILE IN COMMAND LINE (PREFERRED): line by line input from a txt file as a command line argument "./program5 < test.txt"
Input line by line from stdin: will take input line by line from stdin and will run simulation with line
    -WILL TAKE INPUT ONLY IN THE FOLLOWING COMMAND TEMPLATES GIVEN IN THE ASSIGNMENT PROMPT
        - RAM:             RAM size
        - START PROCESS:   time START Pn Size
        - LOC:             time Pn LOC address
        - STOP PROCESS:    time STOP Pn
        - DUMP:            time DUMP
        - STOP SIMULATION: time STOP

Output: simulation results
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char name[4];
    int size;
    int base;
    int bound;
    struct Process * next;
} Process;

struct freeNode {
    int base;
    int bound;
    struct freeNode * next;
} freeNode;

void main(int argc, char * argv[]) {
    char line[40];
    char * linetokens[4];
    for (int i = 0; i < 4; i++)
        linetokens[i] = NULL;
    char * temp;
    char * tokens;
    int counter = 0, totalFreeMem = -1, size, tempBase, tempBound, tempSize, address;
    int * mem;
    struct Process* ProcessTable;
    struct freeNode* freeTable;
    
    printf("MEMORY MANAGEMENT SIMULATION: FIRST FIT SWAPPING\n");
    printf("------------------------------------------------\n");
    printf("> ");

    while (fgets(line, 40, stdin)!=NULL){
        printf(">>> %s", line);

        printf("total free memory is %d\n", totalFreeMem); //FIXME delete line before submission

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
            if (strcmp(linetokens[0], "RAM")==0){
                printf("reached RAM command\n");
                size = atoi(linetokens[1]);
                printf("RAM is size %d words\n", size);
                //malloc size
                mem = (int *)malloc(sizeof(int) * size);
                struct freeNode* temp = (struct freeNode *)malloc(sizeof(struct freeNode));
                temp -> base = 0;
                temp -> bound = size;
                temp-> next = NULL;
                freeTable = temp;
                totalFreeMem = size;
            }
            // STOP COMMAND
            // format: line# STOP
            // description: ends memory allocation simulation
            if (strcmp(linetokens[1], "STOP")==0){
                printf("------------------------------------------------\n");
                printf("END OF SIMULATION\n");
                exit(EXIT_SUCCESS);
            }
            // DUMP COMMAND
            // format: line# DUMP
            // description: gives the base and bounds of all active processes
            if (strcmp(linetokens[1], "DUMP")==0){
                struct Process* curr = ProcessTable;
                while(curr != NULL){
                    printf("Process %s: Base %d Bound %d\n", curr->name, curr->base, curr->bound);
                    curr = curr->next;
                }
            }
        }

        else{
            // START COMMAND
            // format: line# START process# size
            // description: simulate allocating memory to the process in memory
            if (strcmp(linetokens[1], "START")==0){
                printf("reached START command\n");
                size = atoi(linetokens[3]);
                if (totalFreeMem >= size){ //check if totalFree >= procSize
                    struct freeNode * curr = freeTable;
                    tempSize = curr -> base - curr -> bound;
                    if (tempSize >= size){
                        //first free slot can be allocated to process
                        printf("we have the technology!!!!");
                    }
                    else{
                        while (curr != NULL){
                            curr = curr -> next;
                          }
                      }
                      // if there is a free slot <= procSize
                      
                        //allocate first free slot to process and update free space
                    // else try consolidating all free space then allocate free space to process and update free
                //else totalFreeSpace < procSize
                    //can't allocate space to process, barf and give user error message
            }
            }
            // LOC COMMAND
            // format: line# process# LOC address#
            // description: simulate translating virtual address to physical address
            if (strcmp(linetokens[2], "LOC")==0){
                printf("reached LOC command\n");
                struct Process* curr = ProcessTable;
                address = atoi(linetokens[3]);
                while(curr!=NULL){
                    if (strcmp(curr->name, linetokens[1])==0){
                        printf("Physical location of %s VA %d: %d", curr->name, address, curr->base+address);
                        break;
                    }
                    curr = curr->next;
                }
                if (curr == NULL){
                    printf("Process not found or invalid process given.\n");
                }
                //get process node
                    //print base + address if found
                //print error if process not found
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
    // free(mem);
    // for (int i = 0; i<4; i++)
    //     free(linetokens[i]);
}