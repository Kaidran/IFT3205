/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205_2-1b.c                         */
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
#define NAME_IMG_OUT "image-TpIFT3205-2-1b"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void img2d_NN(float**, float**, int, int, int);
void img2d_BL(float**, float**, int, int, int);
float get_imgValue(float**, int, int, int, int);

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

  img2d_BL(MatriceImg1, MatriceImg2, length*scaling, width*scaling, scaling);

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

//=== Interpolation Functions ==============================

/* Re-scaling of an image by Nearest-Neighbor interpolation.
   Takes in an img source, out and the length and width of 
   the output image aswell as the scaling coefficient. */
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

/* Re-scaling of an image by Bilinear interpolation.
   Takes in an img source, out and the length and width of
   the output image aswell as the scaling coefficient. */
void img2d_BL(float** img_in, float** img_out, int len, int wid, int scalar) {
  int x1, y1, x2, y2;
  float v1, v2, v3, v4;
  float b1, b2;
  float dx, dy;
  
  int w1 = wid/scalar;
  int l1 = len/scalar;

  for (x2=0; x2 < wid; x2++) {
    for (y2=0; y2 < len; y2++) {
      x1 = x2/scalar;
      y1 = y2/scalar;
      
      dx = (float)x2/(float)scalar - x1;
      dy = (float)y2/(float)scalar - y1;

      v1 = get_imgValue(img_in, x1  , y1  , w1, l1);
      v2 = get_imgValue(img_in, x1+1, y1  , w1, l1);
      v3 = get_imgValue(img_in, x1  , y1+1, w1, l1);
      v4 = get_imgValue(img_in, x1+1, y1+1, w1, l1);

      b1 = v1 + (dx * (v2-v1));
      b2 = v3 + (dx * (v4-v3));

      img_out[x2][y2] = b1 + (dy * (b2-b1));

    }
  }
}

//=== Helper Functions ====================================
float get_imgValue(float** img, int x, int y, int w, int l) {
  if (x >= w) { x = x % w; }
  if (y >= l) { y = y % l; }
  return img[x][y];
}
