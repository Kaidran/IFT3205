/*------------------------------------------------------*/
/* Prog    : Tp10_IFT3205_2-7.c                         */
/* Auteur  : Andre Lalonde - Jessica Gauvin             */
/* Date    : 14/04/2019                                 */
/* version : 1.0                                        */ 
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

#include "FonctionDemo10.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "
#define CARRE(X) ((X)*(X))
/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

float c(float fb, float Fe) {
  return (tan(PI * fb / Fe) - 1) / (tan(PI * fb / Fe) + 1);
}

float d(float fm, float Fe) {
  return -cos(2 * PI * fm / Fe);
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,n;
  char BufSystVisuSig[100];
  int length;

  //=================================================
  //Question 2.1
  //------------
  //
  // Sample Rate = 11025 ech/sec
  // Frequence Échant= 11025
  //
  //  
  //=================================================

  float*  SignX=LoadSignalDat("SOUND_GoodMorningVietnam",&length);
  float*  SignY=fmatrix_allocate_1d(length);

  //Restauration  Équation Récurente
  //--------------------------------
  // 
  // y(n) = (1 + c)/2 * (x(n) - x(n-2)) - d*(1-c)*y(n-1) + c*y(n-2)
  // c = tan(pi f_b/Fe) - 1 / tan(pi fb/Fe) + 1
  // d = -cos (2pi fm/Fe)
  //
  //--------------------------------
  float SamplingRate=11025;
  float G=0.9;
//  int   Retard=3;
  float R = 0.6;
  float f0 = 0.4;

  for(n=0;n<length;n++) {
    SignY[n] = SignX[n];
    if (n >(0))
      SignY[n] += 2*R*cos(2*PI*n*f0)*SignY[n-1];
    if (n>(1)) 
      SignY[n] += -SignX[n-2] + CARRE(R)*SignY[n-2];
    SignY[n] = 0.5*SignY[n];
  }


   //Sauvegarde
   SaveSignalDatWav("SignalOut7",SignY,length,SamplingRate); 
   //SaveSignalDat("SOUND_GoodMorningVietnam1",SignY,length);
 
   //Visu
   //strcpy(BufSystVisuSig,NAME_VISUALISER);
   //strcat(BufSystVisuSig,"SOUND_GoodMorningVietnam1.dat&");
   //printf(" %s",BufSystVisuSig);
   //system(BufSystVisuSig);
       
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


