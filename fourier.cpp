#include <limits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <string>
#include <sstream>

#include <cstdlib>
#include <iostream>
#include <ctime>


#include <fftw3.h>

#define N 16


int getData(){ //renvoie 0 ou 1 ; pour fill in pour test 
	return(std::rand()%2);
}

//cette fonction prend en entrée un vecteur de double et renvoie la transformée de fourier sous la forme d'un tableau de complexe
fftw_complex* fourierTransform(std::vector<double> data){
	int i = 0;

	double *in;	
	fftw_complex *out;
	fftw_plan my_plan;

	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);		//allocation de la sortie (complexes)
	in = (double*) fftw_malloc(sizeof(double) * N);					//allocation du tableau d'entrée (double)
	my_plan = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE); 	//plan from in double ; out fftw_complex 

	
	
	for(i=0;i<N;i++){ 	//copie des data dans in
		in[i] = data.at(i);
	}
	
	fftw_execute(my_plan); //Execute FFTW
	
	
	fftw_destroy_plan(my_plan);							//Destroy plan
	fftw_free(in);			 						   //Free memory
	
	return(out);
	
}


void fillWithDataAndPrint(double *tab){
	int i = 0, j = 0;
	for(i = 0; i < sqrt(N); i++){
		for(j=0 ; j < sqrt(N); j++){
			tab[i*j+j] = getData(); //on met des données dans l'entrée
			printf("%d ", (int) tab[i*j+j]); //on affiche les données 
		}
		printf("\n");
	}
	printf("\n");
	return;
}


void printComplex(fftw_complex *tab){
	int i = 0 , j = 0; 
	for(i = 0; i < sqrt(N); i++)
	{
		for(j = 0 ; j < sqrt(N) ; j++){
			printf("%lf;%lf ", tab[0][i*j+j], tab[1][i*j+j]); //on affiche les sorties (cast int pour l'affichage pas dégueu..)
		}
		printf("\n");
	}
	printf("\n");
}

double ComputeDifference( fftw_complex* proj1, fftw_complex* proj2, int m_u, int m_v){
	/*auto computeDistance = [&] (fftw_complex a, fftw_complex b) -> double{
        double dist = 0.0;
        dist = sqrt( pow((b[0] + b[1]),2) + pow((a[0] + a[1]),2) );
       // printf("1 : %lf ; %lf // 2 : %lf ; %lf ", b[0], b[1], a[0], a[1]); printf("\n");
       // printf("dist in computeDistance : %lf\n",dist);
       //printf("%lf ", b[0]);
        return dist;
	};*/
	
	double dist = 0.0;
	double sumDist = 0.0;
	int i = 0;
	for(i = 0 ; i < N ; i++){
		dist = sqrt( pow((proj2[0][i] - proj1[0][i] ),2) + pow(( proj2[1][i] - proj1[1][i]),2) );
		//sumDist += computeDistance(proj1[i], proj2[i]);
		sumDist += dist;
	}
	
	return sumDist;
}





int main(){
	
	srand(time(NULL));
	double* proj1 = (double*) fftw_malloc(sizeof(double) * N);
	double* proj2 = (double*) fftw_malloc(sizeof(double) * N);
	
	double diff = 0.0;

	fillWithDataAndPrint(proj1);
	fillWithDataAndPrint(proj2);
	
	fftw_complex* fProj1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex*) * N);
	fftw_complex* fProj2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex*) * N);
	
	fftw_plan my_plan = fftw_plan_dft_r2c_1d(N, proj1, fProj1, FFTW_ESTIMATE);
	
	fftw_execute(my_plan);
	
	my_plan = fftw_plan_dft_r2c_1d(N, proj2, fProj2, FFTW_ESTIMATE);
	
	fftw_execute(my_plan);
	
	
	
	
	diff = ComputeDifference(fProj1, fProj2, sqrt(N), sqrt(N));
	
	printf("%lf\n",diff);
	
	
	
	fftw_destroy_plan(my_plan);

	
	return 0;
}
