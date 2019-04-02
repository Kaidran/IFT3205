/*------------------------------------------------------*/
/* Prog    : Tp1_IFT3205-2-1a.c                         */
/* Auteur  : Andre Lalonde                              */
/* Date    : 18/05/2010                                 */
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

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "photograph"
#define NAME_IMG_OUT "image-Tp1_IFT3205-2-1a"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void CenterImg(float** ,int, int);
void VisualHF(float**, int, int);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  char BufSystVisuImg[100];
  float** MatriceImgR;
  float** MatriceImgI;
  float** MatriceImgM;

  /*Allocation memoire pour la matrice image*/
  MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
  MatriceImgI=fmatrix_allocate_2d(length,width);
  MatriceImgM=fmatrix_allocate_2d(length,width);

  /*Initialisation a zero de toutes les matrices */
  for(i=0;i<length;i++) 
    for(j=0;j<width;j++) 
      {
        MatriceImgI[i][j]=0.0;
        MatriceImgM[i][j]=0.0;
      }
  
  /*Pre-traitement centrer image*/
  CenterImg(MatriceImgR, length, width);

  /*FFT*/
  FFTDD(MatriceImgR,MatriceImgI,length,width);
  
  /*Crop center 60x60*/
  for (i=length/2 - 30; i < length/2 + 30; i++) {
    for (j=length/2 - 30; j < length/2 + 30; j++) {
      MatriceImgR[i][j] = 0.0;
      MatriceImgI[i][j] = 0.0;
    }
  }

  /*Module*/
  Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);

  /*IFFT*/
  IFFTDD(MatriceImgR, MatriceImgI, length, width);
  CenterImg(MatriceImgR, length, width);

  /*Pour visu*/
  Recal(MatriceImgR,length,width);             


  /*Sauvegarde de MatriceImgR sous forme d'image pgm*/
  SaveImagePgm(NAME_IMG_OUT,MatriceImgR,length,width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI); 
  free_fmatrix_2d(MatriceImgM);    

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  /*retour sans probleme*/ 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}
/**/
void CenterImg(float** data, int len, int width) {
  int i;
  int j;
  for (i=0; i < len; i++) {
    for (j=0; j < width; j++) {
      if ((i + j) % 2 == 0) {
        data[i][j] = -data[i][j];
      }
    }
  }
}

/*Visualisation hautes frequences*/
void VisualHF(float** data, int len, int width) {
  int i;
  int j;
  for (i=0; i < len; i++) {
    for (j=0; j < width; j++) {
      data[i][j] = log(1 + data[i][j]);
    }
  }
}