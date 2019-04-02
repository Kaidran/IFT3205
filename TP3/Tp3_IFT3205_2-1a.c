/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205_2-1a.c                         */
/* Auteur  : Andre Lalonde, Jessica Gauvin              */
/* Date    : 28/02/2019                                 */
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

#include "FonctionDemo3.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "lena128"
#define NAME_IMG_OUT "image-TpIFT3205-2-1a"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void img2d_NN(float**, float**, int, int, int);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  int scaling=4;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** MatriceImg2=fmatrix_allocate_2d(length*scaling,width*scaling);

  //Interpolation 

  img2d_NN(MatriceImg1, MatriceImg2, length*scaling, width*scaling, scaling);

  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT,MatriceImg2, length*scaling, width*scaling);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImg1);
  free_fmatrix_2d(MatriceImg2);  
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

//=================================================================

/* Re-scaling of an image by Nearest-Neighbor interpolation.
   Takes in an img source, out and the length and width of 
   the output image. */
void img2d_NN(float** img_in, float** img_out, int len, int wid, int scalar) {
  int x1, y1, x2, y2;

  for (x2=0; x2 < wid; x2++) {
    for (y2=0; y2 < len; y2++) {
      x1 = floor(x2/scalar);
      y1 = floor(y2/scalar);
      img_out[x2][y2] = img_in[x1][y1];
    }
  }
}