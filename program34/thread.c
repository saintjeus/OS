/*  jesus barba - cs474 prog3-4

    multithreaded implementation of mergesort, adapted from example code given by Dr. Shaun Cooper.

    Input:  from STDIN, integers in unsorted fashion.  Data
       ends when a 0 (zero) is read.

    Output: the data in sorted order

    Notes:  Since mergesort actually relies upon external storage to
       store the merge of two segments, we use an additional array to
       store the merge called "out[]" and then place the results
       back into "in[]" for subsequent merges.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "helper.h"

/* value to size the arrays */
#define MAXNUM 10000000

/* define the two arrays */
/* a pointer to the shared memory segment */
int *in;
/* the internal array for merging */
int out[MAXNUM];
/* subroutine which takes locations and merges the results into "out[]"
   and then places the result into "in[]".  Note that this routine uses
   two global vectors and this routine assumes that it can work with
   both of them 
*/

typedef struct {int start1, end1, start2, end2;} mergeargs;
typedef struct {int start, end;} sortargs;

void merge(mergeargs * ma){ //merge function changed to work with struct passed by pthread creation
   int loc1 = (*ma).start1;
   int loc2 = (*ma).end1;
   int loc3 = (*ma).start2;
   int loc4 = (*ma).end2;

   /* variable i is used to index in the first half
      variable j is used to index the second half 
      variable insert is used to keep track of the out[] vector in inserts
   */
   int i,j,k,insert;
   insert=loc1;
   i=loc1;
   j=loc3;

   /* merge the two sublists -- we assume the lists are contiguous */
   while ( (i <= loc2) && ( j <= loc4))
     {
         if ( in[i] > in[j])
	   out[insert++]=in[j++];
	 else
	   out[insert++]=in[i++];
     }
   /* take care of any of the remainder in first segment */
   for(k=i;k<=loc2;k++)
      out[insert++]=in[k];
   /* take care of any of the remainder in second segment */
   for(k=j;k<=loc4;k++)
      out[insert++]=in[k];
   /* transfer the merged data back to the original vector */
   for(i=loc1;i<=loc4;i++)
      in[i]=out[i];
}
/* recursive calls to sort: break the vector up into single units.
   subroutine merge is then used to place the results back together
*/
void sort(sortargs * sa){ //sort function changed to work with struct passed by pthread creation 
	int loc1 = (*sa).start;
	int loc2 = (*sa).end;
	int mid;
  /* base case */
  if (loc1 >=loc2) return;
  /* recursive call, calculate mid point, and bisect the segment */
  else
    {   mid=((loc1 + loc2)/2);
	sortargs s1 = {loc1, mid};
        sort(&s1);
        sortargs s2 = {mid+1, loc2};
        sort(&s2);
        mergeargs m1 = {loc1, mid, mid+1, loc2};
        merge(&m1);
    }
}
/* helper subroutine to print out the results of the in[] vector,
   one line at a time */
void printar(int NUM)
{
   int i;
   for(i=0;i<=NUM;i++)
     printf("%d\n",in[i]);
  printf("\n");
}


/* approach: segment array into 8 pieces
 *
 *  |     entire array of size n    |
 *
 *  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
 *
 *  a   b   c   d   e   f   g   h   i
 * 
 *  where a is index 0, i is size of array - 1, and e is mid of entire array
 *  assign parent process segment 8, assign child process n to sort segment n
 *  and merge segments n and n+1 when they're done?
 *  
 * */
int main(int argc, char *argv[])
{
  int size;
  int status1, status2, status3, status4, status5, status6, status7;
  int a,b,c,d,e,f,g,h,i;
  pthread_t t_child1, t_child2, t_child3, t_child4, t_child5, t_child6, t_child7;
  int threadcheck;
  long long sec, nsec;
  struct timespec s_starttimer;
  struct timespec s_endtimer;

  /* generate array of random size from command line arg*/
  if(argc == 2){
	  size = atoi(argv[1]);
  	}
  else{ 
   perror(
	"this program takes an int command line argument. please try again.\n");
  	exit (-1);}
  in = randomArray(size);
  i = size;

  /* SORTING CODE SEGMENT IN MAIN*/
  /*--------------------------------*/
  
  if (clock_gettime(CLOCK_REALTIME, &s_starttimer) != 0){
	perror("clock_gettime error\n");
	exit(-1);
  }

   
  /* print before sort */
  //printar(i);
 
  /* too few items, no need to fork */
  if ( i <= 8 )
   {
	   
     sortargs s = {0,i};
     sort(&s);
     if (clock_gettime(CLOCK_REALTIME, &s_endtimer)!=0){
		     perror("clock_gettime error\n");
		     exit(1);
		     }
      sortCheck(in, size);
      sec = s_endtimer.tv_sec - s_starttimer.tv_sec;
      nsec = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
      nsec = sec*1000000000+nsec;
      printf("8 thread mergesort on array size %d took:\n%lld nanoseconds\n", size, nsec);
   }

  /* we need to fork processes and then do the work.  
     STUDENTS: This is where you will make changes to support
     8 threads */
  else {
       a = 0;
       b = i/8;
       c = i/4;
       d = (3 * i) / 8;
       e = i / 2;
       f = (5 * i) / 8;
       g = (3 * i) / 4;
       h = (7 * i) / 8;

	sortargs s1 = {a, b};//get s to point to segment 1
	//get threadn to sort segment n and then sort at the end
	if ( (threadcheck=pthread_create(&t_child1, NULL, (void *)sort, &s1 ))!=0) {
		printf("could not create thread 1\n");
		exit(1);
	}
	else {
	 sortargs s2 = {b+1, c};//get s to point to segment 2
         if ( (threadcheck=pthread_create(&t_child2, NULL, (void *)sort, &s2 ))!=0) {
                printf("could not create thread 2\n");
                exit(1);
         }
	 else {
	  sortargs s3 = {c+1, d};//get s to point to segment 3
          if ( (threadcheck=pthread_create(&t_child3, NULL, (void *)sort, &s3 ))!=0) {
                printf("could not create thread 3\n");
                exit(1);
           }
	  else {
	  sortargs s4 = {d+1, e};//get s to point to segment 4
          if ( (threadcheck=pthread_create(&t_child4, NULL, (void *)sort, &s4 ))!=0) {
                printf("could not create thread 2\n");
                exit(1);
          }
	  else {
           sortargs s5 = {e+1, f};//get s to point to segment 5
           if ( (threadcheck=pthread_create(&t_child5, NULL, (void *)sort, &s5 ))!=0) {
                printf("could not create thread 2\n");
                exit(1);
           }
	   else {
            sortargs s6 = {f+1, g};//get s to point to segment 6
            if ( (threadcheck=pthread_create(&t_child6, NULL, (void *)sort, &s6 ))!=0) {
                printf("could not create thread 6\n");
                exit(1);
            }
	    else {
              sortargs s7 = {g+1, h};//get s to point to segment 2
              if ( (threadcheck=pthread_create(&t_child7, NULL, (void *)sort, &s7 ))!=0) {
                printf("could not create thread 7\n");
                exit(1);
              }
	      else {//this will be the else body for child 7

	       /* parent process */
	       
	       pthread_join(t_child1, (void **) &status1);
	       pthread_join(t_child2, (void **) &status2);
	       mergeargs m1 = {a, b, b+1, c};
	       merge(&m1); //merge array segments 1 and 2
	       pthread_join(t_child3, (void **) &status3);
	       pthread_join(t_child4, (void **) &status4);
	       mergeargs m2 = {c+1, d, d+1, e};
	       merge(&m2); //merge array segments 3 and 4
	       mergeargs m3 = {a, c, c+1, e};
	       merge(&m3); //merge array segments 1-2 and 3-4
	       pthread_join(t_child5, (void **) &status5);
	       pthread_join(t_child6, (void **) &status6);
	       mergeargs m4 = {e+1, f, f+1, g};
	       merge(&m4); //merge array segments 5 and 6
	       mergeargs m5 = {a, e, e+1, g};
	       merge(&m5); //merge array segments 1-4 and 5-6
	       pthread_join(t_child7, (void **) &status7);
	       mergeargs m6 = {a, g, g+1, h};
	       merge(&m6); //merge array segments 1-6 and 7
	       sortargs s8 = {h+1, i};
	       sort(&s8); //parent sorts segment 8
	       mergeargs m7 = {a, h, h+1, i};
	       merge(&m7); //merge array segment 1-7 and 8
	/* end of sorting in main */

		if (clock_gettime(CLOCK_REALTIME, &s_endtimer) != 0) {
			perror("clock_gettime error\n");
			exit(-1);
		}
		sec = s_endtimer.tv_sec - s_starttimer.tv_sec;
		nsec = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
		nsec = sec*1000000000+nsec;
	       
		size = atoi(argv[1]);
		sortCheck(in, size); //check that array is sorted correctly, if so, print elapsed time

	       printf("8 process mergesort on array size %d took:\n%lld nanoseconds\n", size, nsec);
	  }//end else8
	}//end else7
  	}//end else6
	}//end else5
	}//end else4
	}//end else3
	}//end else2
	}//end else
}
