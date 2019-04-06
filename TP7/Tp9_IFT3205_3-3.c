/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205_3-3.c                          */
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

float rsbCalculation(float* Sign, int length){
  //TODO
  float rsb = 0;

  return rsb;
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
   float*  Sign=LoadSignalDat("SoundFileInCanal",&length);
   float* SignRestore=fmatrix_allocate_1d(length);

   //TODO: restore and calculate rsb


   showRsbResult(rsbCalculation(SignRestore, length), "RSB=");

   SaveSignalDatWav("SoundFileInCanalRestor",SignRestore,length, sampleRate);
   SaveSignalDat("SoundFileInCanalRestor",SignRestore,length);

   //Visu Ecran
   strcpy(BufSystVisuSig,NAME_VISUALISER);
   strcat(BufSystVisuSig,"SoundFileInCanalRestor.dat&");
   printf(" %s",BufSystVisuSig);
   system(BufSystVisuSig);
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


