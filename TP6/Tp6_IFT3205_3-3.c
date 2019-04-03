/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205_3-3.c                          */
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
void initMat1D(float*,int);
void averageSampling(float*,float*,float*,float*,float*,int,int,int);
float hammingWindow(int,int);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
  int i,j,k;
  int length;
  int samplesize = 512;
  int step = samplesize/2;
  char BufSystVisuSig[100];

  //===============================
  //Question 2.1.(a)
  //===============================
  float*  Sign1=LoadSignalDat("moteur2",&length);
  float*  Echantillons=fmatrix_allocate_1d(samplesize);
  float*  EchantillonsI=fmatrix_allocate_1d(samplesize);
  float*  EMoyens=fmatrix_allocate_1d(samplesize);
  float*  EMoyensI=fmatrix_allocate_1d(samplesize);
  float*  EMoyensM=fmatrix_allocate_1d(samplesize);
  
  initMat1D(EMoyens,samplesize);
  initMat1D(EMoyensI,samplesize);

  averageSampling(Sign1, Echantillons, EchantillonsI, 
                  EMoyens, EMoyensI, 
                  length-step, step, samplesize);


  ModVct(EMoyensM,EMoyens,EMoyensI,samplesize);
  CenterVct(EMoyensM,samplesize);
  SaveSignalDat("FFT_Moteur2_PermMoyHann",EMoyensM,samplesize);  
   
  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"FFT_Moteur2_PermMoyHann.dat&");
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

/* initialize values of a 1D matrix */
void initMat1D(float* mat, int length) {
  int i;
  for (i=0; i < length; i++) {
    mat[i] = 0.0;
  }
}

/* Using the FFT on a subset of data that overlaps half of it, we
   calculate a periodogram by an iterative process. */
void averageSampling(float* datMat, 
                     float* mat1, float* mat1I,
                     float* matMoy, float* matMoyI, 
                     int length, int step, int samplesize)
{
  int i,j;
  for (i=0;i < length; i += step) {
    for (j=0; j<samplesize; j++) {
      mat1[j] = datMat[i+j] * hammingWindow(j,samplesize);
      mat1I[j] = 0.0;
    }
    
    FFT1D(mat1,mat1I,samplesize);
    
    for (j=0; j<samplesize; j++) {
      matMoy[j] += fabs(mat1[j]);
      matMoyI[j] += fabs(mat1I[j]);
    }
  }
  // thought had to divide by sample size since we want an average
  // but somehow this doesnt give right result?
  int samplecount = length / step;
  for (j=0; j<samplesize; j++) {
    matMoy[j] /= samplecount;
    matMoyI[j] /= samplecount;
  }
}

float hammingWindow(int t, int T) {
  if (t >= 0 && t <= T)
    return 0.54 - 0.46 * cos(2*PI * t/T);
  else
    return 0;
}