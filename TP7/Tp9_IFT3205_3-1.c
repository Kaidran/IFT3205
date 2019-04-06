/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205_3-1.c                          */
/* Auteur  : Jessica Gauvin & Andre Lalonde             */
/* Date    : 10/04/2010                                 */
/* version :                                            */
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo9.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
float CARRE(float x){
  return x*x;
}

void initialize(float* mat, int length, float value){
  for(int i=0;i<length;i++)mat[i]=value;
}

void degradationCanal(float* Sign, float* SignResult, int length){
  SignResult[0] = Sign[0];
  SignResult[1] = Sign[1]-(7.0/3.0)*Sign[0];

  for(int i = 2; i<length; i++)
    SignResult[i] = Sign[i]-(7.0/3.0)*Sign[i-1]+(2.0/3.0)*Sign[i-2];
}

void showResult(float* SignCanal, int length, char* message){
  printf("%s", message);
  for(int i=0;i<length;i++)printf("[%f]", SignCanal[i]);
  printf("\n");
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  char BufSystVisuSig[100];
  int length;

  //===============================
  //Question 1
  //===============================

   length=256;
   float*  SignX=fmatrix_allocate_1d(length);
   float*  SignY=fmatrix_allocate_1d(length);

   int lengthSamples = 10;

   float* SignDirac=fmatrix_allocate_1d(lengthSamples);
   float* SignUnite=fmatrix_allocate_1d(lengthSamples);

   float* SignDiracCanal=fmatrix_allocate_1d(lengthSamples);
   float* SignUniteCanal=fmatrix_allocate_1d(lengthSamples);

   initialize(SignDirac, lengthSamples, 0);
   initialize(SignUnite, lengthSamples, 1);
   SignDirac[0] = 1.0;

   degradationCanal(SignDirac, SignDiracCanal, lengthSamples);
   degradationCanal(SignUnite, SignUniteCanal, lengthSamples);

   showResult(SignDiracCanal, lengthSamples, "Reponse-Dirac:: ");
   showResult(SignUniteCanal, lengthSamples, "Reponse-Echelon:: ");
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


