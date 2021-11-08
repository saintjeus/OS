/*
Jesus Barba CS474-Program 5
Memory Management Simulation
Purpose: Simulate memory management using swapping and first fit
Strategy: Maintain linked lists for active processes and for free memory segments, as well as maintaining a counter for total free memory.
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
    char* name;
    int size;
    int base;
    struct Process * next;
} Process;

struct freeNode {
    int base;
    int size;
    struct freeNode * next;
} freeNode;

void printFreeTable(struct freeNode* freeTable){
    struct freeNode* curr = freeTable;
    while (curr!= NULL){
        printf("base: %d, size: %d\n", curr->base, curr->size);
        curr = curr->next;
    }
}

void printProcessTable(struct Process* processTable){
    struct Process* curr = processTable;
    while(curr!=NULL){
        printf("Process: %s, Base: %d, Bound: %d\n", curr->name, curr->base, curr->base+curr->size);
        curr=curr->next;
    }
}

void main(int argc, char * argv[]) {
    char line[40];
    char * linetokens[4];
    for (int i = 0; i < 4; i++)
        linetokens[i] = NULL;
    char * temp;
    char * tokens;
    int counter = 0, totalFreeMem = -1, firstFreeMem = -1, size, tempBase, tempBound, tempSize, address, ramSize;
    int * mem;
    struct Process* ProcessTable = NULL;
    struct freeNode* freeTable;
    
    printf("MEMORY MANAGEMENT SIMULATION: FIRST FIT SWAPPING\n");
    printf("------------------------------------------------\n");
    printf("> ");

    while (fgets(line, 40, stdin)!=NULL){
        printf(">>> %s", line);

        

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
                size = atoi(linetokens[1]);
                mem = (int *)malloc(sizeof(int) * size);
                struct freeNode* temp = (struct freeNode *)malloc(sizeof(struct freeNode));
                temp -> base = 0;
                temp -> size = size;
                temp-> next = NULL;
                freeTable = temp;
                totalFreeMem = size;
                ramSize = size;
                firstFreeMem = 0;
            }
            // STOP COMMAND
            // format: line# STOP
            // description: ends memory allocation simulation
            if ((strcmp(linetokens[1], "STOP")==0)&&(linetokens[2]==NULL)){
                printf("------------------------------------------------\n");
                printf("END OF SIMULATION\n");
                exit(EXIT_SUCCESS);
            }
            // STOP PROCESS COMMAND
            // format: line# STOP process#
            // description: simulate ending a process
            if ((strcmp(linetokens[1], "STOP")==0)&&(linetokens[2]!=NULL)){
                struct Process * stopTemp = ProcessTable;
                struct Process * prev = stopTemp;
                while (stopTemp != NULL){
                    if (strcmp(stopTemp->name, linetokens[2])==0){
                        totalFreeMem += stopTemp->size;
                        struct freeNode* temp = (struct freeNode *)malloc(sizeof(struct freeNode));
                        temp->base = stopTemp->base;
                        temp->size = stopTemp->size;
                        temp->next = freeTable;
                        freeTable=temp;
                        prev->next = stopTemp->next;
                    }
                    prev = stopTemp;
                    stopTemp=stopTemp->next;
                }
            }
            // DUMP COMMAND
            // format: line# DUMP
            // description: gives the base and bounds of all active processes
            if (strcmp(linetokens[1], "DUMP")==0){
                printProcessTable(ProcessTable);
            }
        }

        else{
            // START COMMAND
            // format: line# START process# size
            // description: simulate allocating memory to the process in memory
            if (strcmp(linetokens[1], "START")==0){
                size = atoi(linetokens[3]);
                //if there is enough free memory available, allocate it to the process
                if (totalFreeMem >= size){
                    //iterate through the currently available free memory segments to see if there is a segment large enough to assign to the process
                    struct freeNode * curr = freeTable;
                    struct freeNode * prev = NULL;
                    while (curr != NULL){
                        tempSize = curr -> size;
                        if (tempSize > size){
                            //first free memory slot can be allocated to process
                            struct Process * temp = (struct Process *)malloc(sizeof(struct Process));
                            temp -> base = curr -> base;
                            temp -> name = strdup(linetokens[2]);
                            temp -> size = size;
                            if (ProcessTable == NULL){
                                temp->next = NULL;
                            }
                            else
                            temp -> next = ProcessTable;
                            ProcessTable = temp;
                            curr -> base = curr -> base + size;
                            curr -> size = curr -> size - size;
                            totalFreeMem -= size;
                            break;
                            
                        }
                        
                        if (size == tempSize){
                            struct Process * temp = (struct Process *)malloc(sizeof(struct Process));
                            temp ->base = curr->base;
                            temp->name = strdup(linetokens[2]);
                            temp->size = size;
                            if (ProcessTable == NULL){
                                temp->next = NULL;
                            }
                            else
                                temp -> next = ProcessTable;
                            ProcessTable = temp;
                            totalFreeMem -= size;
                            curr -> base = curr-> base +size;
                            curr ->size = curr->size - size;
                            break;
                        }
                        prev = curr;
                        curr = curr -> next;
                    }
                    if (curr == NULL){
                    //MEMORY DEFRAG
                    printf("Memory defragmentation required. Time: %s\n", linetokens[0]);
                    freeTable->base = ramSize - totalFreeMem;
                    freeTable->size = totalFreeMem;
                    freeTable->next = NULL;
                    firstFreeMem = 0;
                    struct Process * defragTemp = ProcessTable;
                    while (defragTemp!=NULL){
                        defragTemp -> base = firstFreeMem;
                        firstFreeMem += defragTemp->size;
                        defragTemp = defragTemp->next;
                    }
                    struct Process * temp = (struct Process *)malloc(sizeof(struct Process));
                    temp -> name = strdup(linetokens[2]);
                    temp -> base = freeTable->base;
                    temp -> size = size;
                    temp -> next = ProcessTable;
                    ProcessTable = temp;
                    freeTable->base = freeTable->base + size;
                    freeTable->size = freeTable->size - size;
                    totalFreeMem -= size;
                    }

                }
                else{ //not enough totalFreeMemory to give to process, give error message
                    printf("Not enough free memory to allocate to process.\nPlease try stopping a process to allocate more memory.\n");

                }
            }
            // LOC COMMAND
            // format: line# process# LOC address#
            // description: simulate translating virtual address to physical address
            if (strcmp(linetokens[2], "LOC")==0){
                struct Process* curr = ProcessTable;
                address = atoi(linetokens[3]);
                while(curr!=NULL){
                    if (strcmp(curr->name, linetokens[1])==0){
                        printf("Physical location of %s VA %d: %d\n", curr->name, address, curr->base+address);
                        break;
                    }
                    curr = curr->next;
                }
                if (curr == NULL){
                    printf("Process not found or invalid process given.\n");
                }
            }
            
        }
        counter = 0;
        for (int k = 0; k < 4; k++)
            linetokens[k] = NULL;
    }
    free(mem);
    for (int i = 0; i<4; i++)
         free(linetokens[i]);
}