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

void restoreCanal(float* Sign, float* SignResult, int length){
  SignResult[0] = -Sign[0]/2.0;
  SignResult[1] = -Sign[1]/2.0+Sign[0]+17.0*SignResult[0]/6.0;
  SignResult[2] = -Sign[2]/2.0+Sign[1]+17.0*SignResult[1]/6.0-11.0*SignResult[0]/6.0;

  


  for(int i = 3; i<length; i++){
    SignResult[i] = -1/2.0 * Sign[i]
                    +Sign[i-1]
                    +17.0/6.0*SignResult[i-1]
                    -11.0/6.0*SignResult[i-2]
                    +1.0/3.0*SignResult[i-3];
    //printf("%d. result: %f\n",i, SignResult[i] );
  }
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
   float* Sign=LoadSignalDat("SoundFileInCanal",&length);
   float* SignRestore=fmatrix_allocate_1d(length);
   for (i=0; i < length; i++) {
    SignRestore[i] = 0;
   }
   restoreCanal(Sign, SignRestore, length);


   showRsbResult(rsbCalculation(Sign, SignRestore, length), "RSB=");

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


