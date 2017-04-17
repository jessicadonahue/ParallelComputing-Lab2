#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

/* Jessica Donahue 
   Lab 2          */

/***** Globals ******/
int *primes;
int threads;

/****** Function declarations */
void getPrimes(int num);
void writeFile(int nVal);

int main(int argc, char *argv[])
{
 
 int i;

 int N = atoi(argv[1]);
 threads = atoi(argv[2]);

 omp_set_num_threads(threads);

 //create an array of size num 
 primes = (int *) malloc((N+1) * sizeof(int));


 //fill the array with numbers from 2 to N
 for (i = 0; i <= (N + 1); i++) {
 	primes[i] = i;
 }

 getPrimes(N);

 
 writeFile(N);

 
 exit(0);

}//endMain


void getPrimes(int num) {
	int i;
	int j;


	int stop = floor( (num + 1)/2 );


	if (threads == 1) {
		#pragma omp parallel for num_threads(threads) 
		for (i = 2; i < stop; i++) {


			#pragma omp parallel for num_threads(threads) 
			for(j = i; j <= num; j = j + i) {

				if (j != i) {

					#pragma omp critical 
					primes[j] = 0;	

				}
			}//endinnerloop
		}//endouterloop
	}

	else {
		#pragma omp parallel for num_threads(threads/2) 
		for (i = 2; i < stop; i++) {


			#pragma omp parallel for num_threads(threads/2) 
			for(j = i; j <= num; j = j + i) {

				if (j != i) {

					#pragma omp critical 
					primes[j] = 0;	

				}
			}//endinnerloop
		}//endouterloop

	}

}//endGetPrimes


void writeFile(int name) {


	int i;
	int rank = 0;
	int lastPrime = 2;
	int num = name;

	char fileTemp[6];
	char txt[10];

	sprintf(fileTemp, "%d", name);

	//copy the number name 
	strncpy(txt, ".txt", 4);

	//put together
	strcat(fileTemp, txt);

	FILE *fp;

	fp = fopen(fileTemp, "w+");


    //now lets see what is in there
	 for( i = 2; i <= num; i++) {
	 	if (primes[i] != 0 ) {
	 		lastPrime = primes[i] - lastPrime;
	 		rank++;
	 	
			fprintf(fp, "%d %d %d\n",rank, primes[i], lastPrime);
			lastPrime = primes[i];
	 	}
	 }


    fclose(fp);

}




