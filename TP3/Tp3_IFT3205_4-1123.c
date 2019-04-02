/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205_4-1.c                          */
/* Auteur  : Andre Lalonde, Jessica Gauvin              */
/* Date    : 06/03/2019                                 */
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
#define NAME_IMG_IN  "ParcMontRoyal"
#define NAME_IMG_OUT "image-TpIFT3205-4-1"
#define WHITE 255

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void img2d_NN(float**,float**,int,int,int);
void img2d_BL(float**,float**,int,int,int);
void img2d_IS(float**,float**,float**,float**,int,int);
void filter_HT(float**,float**,int,int,int,int,int,int,float);
float get_imgValue(float**,int,int,int,int);
void CenterImg(float**,int,int);
void save_and_visu(float**,int,int,char*);
float averageValue(float**,int,int,int);
void merge(float**,float**,int,int,int);
void adjust(float**, int, int);
void IDFT(float**,float**,int,int);
void DFT(float**,float**,int,int);

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  char BufSystVisuImg[100];
  const int n = 30;

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** MatriceImg3=fmatrix_allocate_2d(length+n,width);

  float** MatriceTopOriginal=fmatrix_allocate_2d(n, width);
  float** MatriceTopExtrapol=fmatrix_allocate_2d(n, width);

  for(i=0; i<n; i++)
    for(j=0; j<width; j++){
      MatriceImg3[i][j] = averageValue(MatriceImg1, j, length, n);
      MatriceTopOriginal[i][j] = MatriceImg1[i][j];
      MatriceTopExtrapol[i][j] = MatriceImg3[i][j];
    }

  for(i=n; i<length+n; i++)
    for(j=0; j<width; j++)
      MatriceImg3[i][j] = MatriceImg1[i-n][j];


  save_and_visu(MatriceImg3, length+n, width, "0");
  for (i=0;i<1;i++) {
    filter_HT(MatriceTopOriginal, MatriceTopExtrapol, n, width, n, 256, 0,0,0.04f);
    merge(MatriceImg3,MatriceTopExtrapol,length,width,n);
    adjust(MatriceImg3,length+n,width);
    //save_and_visu(MatriceImg3,length+n,width,"1");
  }
  //filter_HT(MatriceTopOriginal, MatriceTopExtrapol, n, width, n/2, 128, n/4,75,0.02f);
  //merge(MatriceImg3,MatriceTopExtrapol,length,width,n);
  //adjust(MatriceImg3,length+n,width);
  save_and_visu(MatriceImg3,length+n,width,"2");

}

/* Hard thresholding interpolation */
void filter_HT(float** img_init, float** img_curr, int len, int wid,
               int sizex, int sizey, int offset_i, int offset_j, float lam)
{
  int i,j;

  float** MatriceTmpR=fmatrix_allocate_2d(sizex,sizey);
  float** MatriceTmpI=fmatrix_allocate_2d(sizex,sizey);
  float** MatriceTmpM=fmatrix_allocate_2d(sizex,sizey);

  //init
  for (i=0; i < sizex; i++) {
    for (j=0; j < sizey; j++) {
      MatriceTmpR[i][j] = img_curr[i + offset_i][j + offset_j];
      MatriceTmpI[i][j] = 0.0;
    }
  }

  DFT(MatriceTmpR, MatriceTmpI, sizex, sizey);
  Mod(MatriceTmpM,MatriceTmpR,MatriceTmpI,sizex,sizey);

  float maxval = fabs(MatriceTmpR[0][0]) * lam;

  for (i=0; i < sizex; i++) {
    for (j=0; j < sizey; j++) {
      if (MatriceTmpM[i][j] < maxval) {
        MatriceTmpR[i][j] = 0.0;
        MatriceTmpI[i][j] = 0.0;
      }
    }
  }

  IDFT(MatriceTmpR, MatriceTmpI, sizex, sizey);
  for (i=0; i < len; i++) {
    for (j=0; j < wid; j++) {
      if (img_init[i][j] == WHITE) {
        img_curr[i][j] = MatriceTmpR[i - offset_i][j - offset_j];
      }
    }
  }

  //free
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

float averageValue(float** img, int column, int length, int rows){
  float average = 0;
  for(int i=0; i<rows; i++)
    average += img[i][column];

  return average/rows;
}

void adjust(float** img, int width, int length){
  int i,j;
  for (i=0; i < length; i++)
    for (j=0; j < width; j++) {
      img[i][j] = img[i][j] > 255? 255 : img[i][j];
      img[i][j] = img[i][j] < 0? 0 : img[i][j];
    }
}

void merge(float** img,float** imgToMerge, int length, int width, int n){
  int i,j;
  for(i=0; i<n; i++)
    for(j=0; j<width; j++)
      img[i][j] = imgToMerge[i][j];
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


