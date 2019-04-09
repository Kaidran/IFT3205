/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205_3-2.c                          */
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

float rsbCalculation(float* original, float* modified, int length){
  //TODO
  float num = 0;
  float denum = 0;

  for(int i=0;i<length;i++){
    num += CARRE(original[i]);
    denum += CARRE(original[i] - modified[i]);
  }
  // normalizing
  num = sqrt(num);
  denum = sqrt(denum);

  // returning decibel, ratio/log(10)
  return 20*(log((num/denum))/log(10));
}

void showRsbResult(float rsb, char* message){
  printf("%s", message);
  printf("[%f]\n", rsb);
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
  int sampleRate = 11025;

  //===============================
  //Question 1
  //===============================
   float*  Sign=LoadSignalDat("SoundFile",&length);

   float* SignCanal=fmatrix_allocate_1d(length);

   degradationCanal(Sign, SignCanal, length);
   
   showRsbResult(rsbCalculation(Sign, SignCanal, length), "RSB=");

   SaveSignalDatWav("SoundFileInCanal",SignCanal,length, sampleRate);
   SaveSignalDat("SoundFileInCanal",SignCanal,length);

   //Visu Ecran
   strcpy(BufSystVisuSig,NAME_VISUALISER);
   strcat(BufSystVisuSig,"SoundFileInCanal.dat&");
   printf(" %s",BufSystVisuSig);
   system(BufSystVisuSig);
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


