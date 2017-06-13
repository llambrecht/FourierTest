#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <complex.h> //doit être avant fftw3.h
#include <fftw3.h>

#define N 16

int getData(){
	return(rand()%2);
}

int main(){
	
	srand(time(NULL));
	
	int i = 0;

	double *in;
	fftw_complex *out;
	fftw_plan my_plan;

	in = (double*) fftw_malloc(sizeof(double)*N);			         //allocation de l'entrée (nombres réels)
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);		//allocation de la sortie (complexes)
	my_plan = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE); 	//transform 1D

	printf("\nEntree:\n\n");
	for(i = 0; i < N; i++)
	{
		//in[i] = (double) i;
		in[i] = (double)getData();
		printf("%d ", (int)in[i]);		
		if((i+1)%(int)sqrt(N) == 0)
			printf("\n");
	}
	printf("\n");
	
	

	fftw_execute(my_plan); //Execute FFTW

	printf("Sortie:\n\n");
	for(i = 0; i < N; i++)
	{
		printf("%d;%d ", (int)creal(out[i]), (int)cimag(out[i]));
		if((i+1)%(int)sqrt(N) == 0)
			printf("\n");
	}


	
	fftw_destroy_plan(my_plan);							//Destroy plan
	fftw_free(in);			 						//Free memory
	fftw_free(out);			 						//Free memory
	
	return 0;
}
