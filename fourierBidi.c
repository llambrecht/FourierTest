#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <complex.h> //doit être avant fftw3.h
#include <fftw3.h>

#define N 16

int getData(){ //renvoie 0 ou 1 ; pour test 
	return(rand()%2);
}


void createPlansR(fftw_plan* my_plan, double** in, fftw_complex** out){ //crée les plans avec des Réels en entrée
	int i = 0;
	for(i=0; i < sqrt(N) ; i++){ //on veut appliquer fourier à toutes les lignes
		my_plan[i] = fftw_plan_dft_r2c_1d(N, in[i], out[i], FFTW_ESTIMATE); 	//transform 1D
	}
	return;
}

void createPlansC(fftw_plan* my_plan, fftw_complex** in, fftw_complex** out){ //crée les plans avec des Complexes en entrée
	int i = 0;
	for(i=0; i < sqrt(N) ; i++){ //on veut appliquer fourier à toutes les lignes
		my_plan[i] = fftw_plan_dft_1d(N, in[i], out[i],FFTW_FORWARD, FFTW_ESTIMATE); 	//transform 1D
	}
	return;
}

void fillWithDataAndPrint(double **tab){
	int i = 0, j = 0;
	for(i = 0; i < sqrt(N); i++){
		for(j=0 ; j < sqrt(N); j++){
			tab[i][j] = getData(); //on met des données dans l'entrée
			printf("%d ", (int) tab[i][j]); //on affiche les données 
		}
		printf("\n");
	}
	printf("\n");
	return;
}

void executePlans(fftw_plan* my_plan){
	int i = 0;
	for(i = 0 ; i < sqrt(N) ; i++){
		fftw_execute(my_plan[i]); //execute FFTW pour chaque plan (un plan par ligne)
	}
	return;
}

void copyComplex(fftw_complex** base, fftw_complex** cpy){ //pour que ça soit plus simples apres, on copie les lignes en colonnes
	int i = 0, j = 0;
	for(i = 0; i < sqrt(N) ; i++){
		for(j = 0; j < sqrt(N) ; j++){
			cpy[j][i] = base[i][j];
		}
	}
	return;
}

void printComplex(fftw_complex **tab){
	int i = 0 , j = 0; 
	for(i = 0; i < sqrt(N); i++)
	{
		for(j = 0 ; j < sqrt(N) ; j++){
			printf("%d;%d ", (int)creal(tab[i][j]), (int)cimag(tab[i][j])); //on affiche les sorties (cast int pour l'affichage pas dégueu..)

		}
		printf("\n");
	}
	printf("\n");
}

void destroyAndFree(double** in, fftw_complex** out, fftw_plan *my_plan){
	int i = 0;
	for(i = 0 ; i < sqrt(N); i++){
		fftw_destroy_plan(my_plan[i]);	//Destroy les plans

	}

	// /!\ FREE IN ET OUT ET COPYOUT ET OUT2 NEEDED
}

int main(){
	
	srand(time(NULL));
	
	
	double **in;
	fftw_complex **out, **copyOut, **out2, **copyOut2;
	fftw_plan* my_plan, *my_planC;
	
	int i = 0;
	//on fait des tableaux en deux dimensions. On appliquera fourier à toutes les lignes de in[0] à in[sqrt(N)]
	in = (double**) fftw_malloc(sizeof(double*)*sqrt(N)); //allocation
	out = (fftw_complex**) fftw_malloc(sizeof(fftw_complex*)*sqrt(N)); //allocation
	copyOut = (fftw_complex**) fftw_malloc(sizeof(fftw_complex*)*sqrt(N)); //allocation
	out2 = (fftw_complex**) fftw_malloc(sizeof(fftw_complex*)*sqrt(N)); //allocation
	copyOut2 = (fftw_complex**) fftw_malloc(sizeof(fftw_complex*)*sqrt(N)); //allocation

	for(i = 0 ; i < sqrt(N) ; i++){ //allocation
		in[i] = (double*)fftw_malloc(sizeof(double)*sqrt(N));
		out[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sqrt(N));
		copyOut[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sqrt(N));
		out2[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sqrt(N));
		copyOut2[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*sqrt(N)); //allocation

	}
	
	my_plan = (fftw_plan*) fftw_malloc(sizeof(fftw_plan) * sqrt(N)); //création des plans
	my_planC = (fftw_plan*) fftw_malloc(sizeof(fftw_plan) * sqrt(N)); //création des plans

	
	
	/*----- FOURIER LIGNE PAR LIGNE -----*/
	createPlansR(my_plan, in, out);
	
	printf("\nEntree:\n\n"); //affichage des entrées 1
	fillWithDataAndPrint(in);
	
	executePlans(my_plan);
	
	printf("Sortie:\n\n"); //affichage des sorties
	printComplex(out);


	/*----- FOURIER COLONNE PAR COLONNE -----*/
	
	//on copy le résultat de fourier précédent (out) dans (copyOut) ; c'est la nouvelle entrée
	copyComplex(out, copyOut);
	//on affiche la nouvelle entrée
	printf("\nEntree 2:\n\n"); //affichage des entrées 1
	printComplex(copyOut);
	//on fait Fourier colonne par colonne sur copyOut (les lignes de copyOut sont les colonnes de out)
	createPlansC(my_planC, copyOut, out2);
	executePlans(my_planC);

	//on affiche le résultat
	printf("Sortie 2 re-inversee:\n\n"); //affichage des sorties
	copyComplex(out2, copyOut2); //ça va re-inverser lignes et colonnes
	printComplex(copyOut2);



	//Destroy And Free Needed

	
	return 0;
}
