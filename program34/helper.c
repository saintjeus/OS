#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* function that takes an integer size, dynamically allocates an integer array in memroy,
 * fills it with random values, and returns a pointer to this array
 * make sure to free the pointer to this array after done with it.
 */
int * randomArray(int size){
	int * A = (int *)malloc(sizeof(int)*size);
	srand(time(NULL));
	for (int i = 0; i < size; i++){
		A[i] = rand();
	}
	return A;	
}
/* function that takes a pointer to an array and the size of the array, and determines if it is
 * sorted in ascending order
 * if the array is not sorted, the function will exit with an error message
 */
void sortCheck(int *A, int size){
	for (int i = 1; i < size; i++)
		if (A[i]<A[i-1]){
			perror("array is not sorted\n");
			exit(EXIT_FAILURE);
		}
}
