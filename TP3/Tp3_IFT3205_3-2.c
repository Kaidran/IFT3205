/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205_3-2.c                          */
/* Auteur  : Andre Lalonde, Jessica Gauvin              */
/* Date    : 05/03/2019                                 */
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
#define NAME_IMG_IN  "barbara512_Degraded"
#define NAME_IMG_OUT "image-TpIFT3205-3-2-"
#define WHITE 255

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void img2d_NN(float**,float**,int,int,int);
void img2d_BL(float**,float**,int,int,int);
void img2d_IS(float**,float**,float**,float**,int,int);
void filter_HT(float**,float**,int,int,int,int,int,float);
float get_imgValue(float**,int,int,int,int);
void CenterImg(float**,int,int);
void save_and_visu(float**,int,int,char*);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** MatriceImg2=LoadImagePgm(NAME_IMG_IN,&length,&width);

  save_and_visu(MatriceImg1,length,width,"0");
  filter_HT(MatriceImg1,MatriceImg2,length,width,512,0,0,0.04f);
  save_and_visu(MatriceImg2,length,width,"1");
  filter_HT(MatriceImg1,MatriceImg2,length,width,256,128,128,0.02f);
  save_and_visu(MatriceImg2,length,width,"2");
  filter_HT(MatriceImg1,MatriceImg2,length,width,256,240,128,0.01f);
  save_and_visu(MatriceImg2,length,width,"3");
  filter_HT(MatriceImg1,MatriceImg2,length,width,256,168,128,0.005f);
  save_and_visu(MatriceImg2,length,width,"4");
  filter_HT(MatriceImg1,MatriceImg2,length,width,256,200,128,0.0025f);
  save_and_visu(MatriceImg2,length,width,"5");


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
   the output image aswell as the scaling coefficient.    */
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
   the output image aswell as the scaling coefficient.  */
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

/* Re-scaling of an image by Ideal Spectral Interpolation.
   Takes in an img source inside a Fourier Transform and 
   an empty output matrix. Takes in the length and width
   of the input matrix.                                 */
void img2d_IS(float** img_inR, float** img_inI, float** img_outR, 
              float** img_outI, int len, int width) 
{
  int i,j;
  for(i=0; i < len; i++) {
    for(j=0; j < width; j++) {
      img_outR[i + len*3/2][j + width*3/2] = img_inR[i][j];
      img_outI[i + len*3/2][j + width*3/2] = img_inI[i][j];
    }
  }
}

/* Hard thresholding interpolation. Takes it the unmodified image
   with the data to remove, the current modified image, the length
   and width of the image, the size of the FFT transform, the offset
   as to where to focus the thresholding and the threshold lambda */
void filter_HT(float** img_init, float** img_curr, int len, int wid,
               int size, int offset_i, int offset_j, float lam)
{
  int i,j;

  float** MatriceTmpR=fmatrix_allocate_2d(size,size);
  float** MatriceTmpI=fmatrix_allocate_2d(size,size);
  float** MatriceTmpM=fmatrix_allocate_2d(size,size);

  //init re-centering to offset
  for (i=0; i < size; i++) {
    for (j=0; j < size; j++) {
      MatriceTmpR[i][j] = img_curr[i + offset_i][j + offset_j];
      MatriceTmpI[i][j] = 0.0;
    }
  }

  /* FFT */
  FFTDD(MatriceTmpR, MatriceTmpI, size, size);
  
  /* MOD */
  Mod(MatriceTmpM,MatriceTmpR,MatriceTmpI,size,size);

  /* Threshold value based on max and a lambda tolerance level*/
  float maxval = fabs(MatriceTmpR[0][0]) * lam;

  /* Thresholding */
  for (i=0; i < size; i++) {
    for (j=0; j < size; j++) {
      if (MatriceTmpM[i][j] < maxval) {
        MatriceTmpR[i][j] = 0.0;
        MatriceTmpI[i][j] = 0.0;
      }
    }
  }

  /* IFFT */
  IFFTDD(MatriceTmpR, MatriceTmpI, size, size);

  /* Removal of White spots */
  for (i=0; i < len; i++) {
    for (j=0; j < wid; j++) {
      if (img_init[i][j] == WHITE) {
        img_curr[i][j] = MatriceTmpR[i - offset_i][j - offset_j];
      }
    }
  }

  /* Free memory */
  free_fmatrix_2d(MatriceTmpR);
  free_fmatrix_2d(MatriceTmpI);
  free_fmatrix_2d(MatriceTmpM);
}

//=== Helper Functions ====================================

float get_imgValue(float** img, int x, int y, int w, int l) {
  if (x >= w) { x = x % w; }
  if (y >= l) { y = y % l; }
  return img[x][y];
}

void CenterImg(float** data, int len, int width) {
  int i,j;
  for (i=0; i < len; i++) {
    for (j=0; j < width; j++) {
      if ((i + j) % 2 == 1) {
        data[i][j] = -data[i][j];
      }
    }
  }
}

void save_and_visu(float** MatriceImg, int len, int wid, char* iter) {
  //Sauvegarde
  char savename[21] = NAME_IMG_OUT;
  char BufSystVisuImg[100];
  strcat(savename,iter);

  SaveImagePgm(savename,MatriceImg,len,wid);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,savename);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
}