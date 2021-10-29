// jesus barba - nmsu cs 474 prog 3-4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "helper.h"

/* merge.c - program that runs mergesort on an array without creating additional processes or threads.
 * this function takes a command line argument int for the size of the array
 * the functions merge and mergeSort are adapted from geeksforgeeks.org/in-place-merge-sort*/

void merge(int *arr, int start, int mid, int end){
	int start2 = mid + 1;
	if (arr[mid] <= arr[start2]){
		return;
	}
	while (start <= mid && start2 <= end) {
		if (arr[start] <= arr[start2]){
			start++;
		}
		else{
			int value = arr[start2];
			int index = start2;
			while (index != start) {
				arr[index] = arr[index - 1];
				index --;
			}
			arr[start] = value;
			start++;
			mid++;
			start2++;
		}
	}
}
void mergeSort(int *arr, int L, int R){
	if (L < R){
		int M = L + (R - L) / 2;
		mergeSort(arr, L, M);
		mergeSort(arr, M+1, R);
		merge (arr, L, M, R);
		}
}

int main(int argc, char *argv[]) {
	if (argc != 2){ //make sure the user provided a command line argument
		perror("this program takes an int command line argument. please try again\n");
		exit(1);
	}
	long long j, k;
	struct timespec s_starttimer;
	struct timespec s_endtimer;

	int size = atoi(argv[1]); //get size of array from command line arg
	int * arr = randomArray(size); //create random array
	if (clock_gettime(CLOCK_REALTIME, &s_starttimer) !=0) {// get start time
		perror("clock_gettime error\n");
		exit(1);
	}
	mergeSort(arr, 0, size-1); //sort array
	if (clock_gettime(CLOCK_REALTIME, &s_endtimer) != 0) {//get end time
		perror("clock_gettime error\n");
		exit(1);
	}
	sortCheck(arr, size); //check that array is sorted
	//if sorted then calculate elapsed time and print to user
	j = s_endtimer.tv_sec - s_starttimer.tv_sec; 
	k = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
	j = j*1000000000 + k;
	printf("uniprocess unithread mergesort on array size %d took:\n%lld nanoseconds\n", size, j);
	free(arr); //free array mem
}
