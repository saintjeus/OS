/*  jesus barba - cs474 prog 3-4

    multiprocess implementation of mergesort, adapted from example code given by Dr. Shaun Cooper.

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
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "helper.h"

/* value to size the arrays */
#define MAXNUM 100000000

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
void merge(int loc1,int loc2,int loc3,int loc4)
{
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
void sort(int loc1, int loc2)
{ 
	int mid;
  /* base case */
  if (loc1 >=loc2) return;
  /* recursive call, calculate mid point, and bisect the segment */
  else
    {  mid=((loc1 + loc2)/2);
       sort(loc1,mid);
       sort(mid+1,loc2);
       merge(loc1,mid,mid+1,loc2);
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
 *  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | <- array segments
 *
 *  a   b   c   d   e   f   g   h   i <- index boundaries of array segments
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
  int a,b,c,d,e,f,g,h;
  int i,j;
  int done;
  int beg,mid,end;
  int level;
  int share_key_in;
  int share_key_out;
  pid_t child1, child2, child3, child4, child5, child6, child7;
  struct shmid_ds item;
  long long s, ns;
  struct timespec s_starttimer;
  struct timespec s_endtimer;

  /* define shared memory */
  if ( (share_key_in=shmget(IPC_PRIVATE,MAXNUM * 4 , IPC_CREAT|0666)) < 0)
      perror("Cannot get shared memory\n");
  /* attach shared memory segment */
  if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1)
     perror("cannot attach to shared memory\n");
  done=0;

  /* generate array of random size from command line arg*/
  if(argc == 2){
	  size = atoi(argv[1]);
  	}
  else{ 
   perror(
	"this program takes an int command line argument. please try again.\n");
  	exit (1);}
  int * arr = randomArray(size);
  i=0;
  /* read in the data from generated array */
  while (! done )
  {
      if ( i == size-1 )
	done =1;
      else{
	in[i]=arr[i];
	i++;
      }

  }
  free(arr);

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
     sort(0,i);
     if (clock_gettime(CLOCK_REALTIME, &s_endtimer)!=0){
                     perror("clock_gettime error\n");
                     exit(1);
                     }
      sortCheck(in, size);
      s = s_endtimer.tv_sec - s_starttimer.tv_sec;
      ns = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
      ns = s*1000000000+ns;
      printf("8 process mergesort on array size %d took:\n%lld nanoseconds\n", size, ns);
     shmctl(share_key_in,IPC_RMID,&item);
   }

  /* we need to fork processes and then do the work.  
     STUDENTS: This is where you will make changes to support
     8 processes */
  else {
       //mid=i/2;
       //beg=0;
       //end=i;
       a = 0;
       b = i/8;
       c = i/4;
       d = (3 * i) / 8;
       e = i / 2;
       f = (5 * i) / 8;
       g = (3 * i) / 4;
       h = (7 * i) / 8;

	child1=fork();
	if ( child1==0) {
	 if ( (in=shmat(share_key_in,(void *) 0,SHM_RND)) == (void *) -1) //attach shared mem for child1
	 perror("cannot attach to shared memory\n");
	 sort(a,b);//sort segment 1 in child process 1
	}
	else {
	 child2=fork();
	 if (child2==0) {
	 if ( (in=shmat(share_key_in,(void *) 0,SHM_RND)) == (void *) -1) //attach shared mem for child2
	 perror("cannot attach to shared memory\n");
	 sort(b+1, c);//sort segment 2 in child process 2
	 }
	 else {
	  child3=fork();
	  if (child3==0) {
	  if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1) //attach shared mem for child3
	  perror("cannot attach to shared memory\n");
	  sort(c+1, d);//sort segment 3 in child process 3
	  }
	  else {
           child4=fork();
           if (child4==0) {
           if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1) //attach shared mem for child4
           perror("cannot attach to shared memory\n");
           sort(d+1, e);//sort segment 4 in child process 4
           }
	   else {
            child5=fork();
            if (child5==0) {
            if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1) //attach shared mem for child5
            perror("cannot attach to shared memory\n");
            sort(e+1, f);//sort segment 5 in child process 5
            }
	    else {
             child6=fork();
             if (child6==0) {
             if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1) //attach shared mem for child6
             perror("cannot attach to shared memory\n");
             sort(f+1, g);//sort segment 6 in child process 6
             }
	     else {
              child7=fork();
              if (child7==0) {
              if ( (in=shmat(share_key_in,(void *) 0, SHM_RND)) == (void *) -1) //attach shared mem for child7
              perror("cannot attach to shared memory\n");
              sort(g+1, h);//sort segment 7 in child process 7
              }
	      else {//this will be the else body for child 7

	       /* parent process */
	       waitpid(child1, &status1, 0);
	       waitpid(child2, &status2, 0);
	       merge(a, b, b+1, c); //merge array segments 1 and 2
	       waitpid(child3, &status3, 0);
	       waitpid(child4, &status4, 0);
	       merge(c+1, d, d+1, e); //merge array segments 3 and 4
	       
	       merge(a, c, c+1, e); //merge array segments 1-2 and 3-4 into segment 1-4

	       waitpid(child5, &status5, 0);
	       waitpid(child6, &status6, 0);
	       merge(e+1, f, f+1, g); //merge array segments 5 and 6
	       merge(a, e, e+1, g); //merge array segment 1-4 and 5-6 into segment 1-6

	       waitpid(child7, &status7, 0);
	       merge(a, g, g+1, h); //merge array segment 1-6 and 7 into segment 1-7

	       sort(h+1, i); //parent sorts segment 8
	       merge(a, h, h+1, i); //merge array segment 1-7 and 8
	/* end of sorting in main */

		if (clock_gettime(CLOCK_REALTIME, &s_endtimer) != 0) {
			perror("clock_gettime error\n");
			exit(-1);
		}
		s = s_endtimer.tv_sec - s_starttimer.tv_sec;
		ns = s_endtimer.tv_nsec - s_starttimer.tv_nsec;
		ns = s*1000000000+ns;
	       sortCheck(in, size); //check that array is sorted, if so print elapsed time
	       printf("8 process mergesort on array size %d took:\n%lld nanoseconds\n",size, ns);
               shmctl(share_key_in,IPC_RMID,&item);
	  }//end else8
	}//end else7
  	}//end else6
	}//end else5
	}//end else4
	}//end else3
	}//end else2
	}//end else
}
