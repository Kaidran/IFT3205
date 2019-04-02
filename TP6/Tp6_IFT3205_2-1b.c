/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_2-1b.c                         */
/* Auteur  : Andre Lalonde - Jessica Gauvin             */
/* Date    : 01/04/2019                                 */
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

#include "FonctionDemo6.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
int getMaxIndex(float*,int);
int getClicIndex(float*,int);
/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k, N,t;
  int length;
  float dt;
  char BufSystVisuSig[100];

  //===============================
  //Question 2.1.(a)
  //===============================
  float*  Sign1=LoadSignalDat("moteur1",&length);
  float*  Sign1I=fmatrix_allocate_1d(length);
  float*  Sign1M=fmatrix_allocate_1d(length);
  FFT1D(Sign1,Sign1I,length);
  ModVct(Sign1M,Sign1,Sign1I,length);
//  CenterVct(Sign1M,length);
  
  N = 8192;
  t = 11025;
  dt = (float)t/N;

  int imax = getMaxIndex(Sign1M, N/2);
  int tpm = (imax * dt * 60) / 2;

  int iclic = getClicIndex(Sign1M, imax);
  float clicFreq = t/(float)iclic;

  printf("\nIndex: %d\n", imax);
  printf("Tours par minute: %d\n", tpm);
  printf("Index clic: %d\n", iclic);
  printf("Frequence du clic: %f\n", clicFreq);
  // index = parcour(Sign1M);
  // index *= techantillonage / # values ca donne mu_0, trouvé wtf is mu_0 in notes

  SaveSignalDat("FFT_Moteur1",Sign1M,length);  
   
  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"FFT_Moteur1.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

/*------------------------------------------------*/
/* FONCTIONS  ------------------------------------*/   
/*------------------------------------------------*/

/* returns index of max value. returns -1 if failed */
int getMaxIndex(float* Sign1M, int N) {
  int i,index,tmpmax;
  index = -1;
  tmpmax = 0;
  for (i=0;i<N;i++) {
    if (tmpmax < Sign1M[i]) {
      tmpmax = Sign1M[i];
      index = i;
    }
  }
  return index;
}

/* takes max value index and returns second max value index */
int getClicIndex(float* Sign1M, int imax) {
  int i,index,tmpmax;
  tmpmax = 0;
  for (i=0;i<imax;i++) {
    if (tmpmax < Sign1M[i]) {
      tmpmax = Sign1M[i];
      index = i;
    }
  }
  return index;
}