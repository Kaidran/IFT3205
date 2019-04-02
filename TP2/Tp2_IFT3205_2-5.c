/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-5.c                          */
/* Auteur  : Jessica Gauvin - Andre Lalonde             */
/* Date    : 19/02/2019                                 */
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

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "UdM_1"
#define NAME_IMG_IN2  "image-TpIFT3205-2-4"
#define NAME_IMG_OUT1 "image-TpIFT3205-2-5"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void CenterImg2(float** Matrice, int length, int width){
  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++)
      Matrice[i][j] = Matrice[i][j]*pow(-1,i+j);
}
void Rotation(float** matResult,float** mat, float Theta0, int length, int width) {

  float i2;
  float j2;

  float f1;
  float f2;
  float f3;

  /* Values to recenter image when applying a rotation */
  int y0 = width/2-((width/2)*cos(Theta0)-(length/2)*sin(Theta0));
  int x0 = length/2-((width/2)*sin(Theta0)+(length/2)*cos(Theta0));

  for(int i=0;i<length;i++)
    for(int j=0;j<width;j++){
      /* x' and y' */
      i2 = i*cos(Theta0)+j*sin(Theta0)+x0;
      j2 = -1*i*sin(Theta0)+j*cos(Theta0)+y0;

      if(i2 < 0 || i2 >= length-1 || j2<0 || j2>=width-1)
        matResult[i][j] = 0.0;
      else if(ceilf(i2) == i2 && ceilf(j2) == j2)
        matResult[i][j] = mat[(int) i2][(int) j2]; 
      else{
         int i3 = floorf(i2);
         int j3 = floorf(j2);
         
         /* f(x', y) */
         f1 = mat[i3][j3]+(i2-i3)*(mat[i3+1][j3]-mat[i3][j3]);
         /* f(x', y+1) */
         f2 = mat[i3][j3+1]+(i2-i3)*(mat[i3+1][j3+1]-mat[i3][j3+1]);
         /* f(x', y') */
         f3 = f1+(j2-j3)*(f2-f1);
         matResult[i][j] = f3;
       }
    } 
}

float resemblanceError(float** mat1, float** mat2,int length,int width){
  float resemblanceError = 0;

  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++)
      resemblanceError += abs(abs(mat2[i][j])-abs(mat1[i][j]));

  return resemblanceError;
}
/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  float Theta0;
  int x0,y0;
  char BufSystVisuImg[100];

  //Constante
  length=512;
  width=512;
  
  //Allocation Memoire 
  float** MatriceImgI1=fmatrix_allocate_2d(length,width);
  float** MatriceImgM1=fmatrix_allocate_2d(length,width);
  float** MatriceImgR1=fmatrix_allocate_2d(length,width);
  float** MatriceImgI2=fmatrix_allocate_2d(length,width);
  float** MatriceImgM2=fmatrix_allocate_2d(length,width);
  float** MatriceImgR2=fmatrix_allocate_2d(length,width);
  float** MatriceImgI3=fmatrix_allocate_2d(length,width);
  float** MatriceImgM3=fmatrix_allocate_2d(length,width);
  float** MatriceImgR3=fmatrix_allocate_2d(length,width);
  float** MatriceImg3=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN2,&length,&width);
  float** MatriceImg2=LoadImagePgm(NAME_IMG_IN1,&length,&width);
  
  for(i=0; i<length; i++)
    for(j=0; j<width; j++){
      MatriceImgI1[i][j]=0.0;
      MatriceImgI2[i][j]=0.0;
      MatriceImgI3[i][j]=0.0;
    }
  
  FFTDD(MatriceImg1, MatriceImgI1, length, width);
  FFTDD(MatriceImg2, MatriceImgI2, length, width);

  Mod(MatriceImgM1, MatriceImg1, MatriceImgI1, length, width);  

  for(i=0; i<length; i++)
    for(j=0; j<length; j++)  
      MatriceImgI1[i][j] *= -1;
  
  MultMatrix(MatriceImgR3, MatriceImgI3, MatriceImg1, MatriceImgI1, MatriceImg2, MatriceImgI2, length, width);

  for(i=0; i<length; i++)
    for(j=0; j<width; j++){
      MatriceImgR3[i][j] /= (MatriceImgM1[i][j]*MatriceImgM1[i][j]);
      MatriceImgI3[i][j] /= (MatriceImgM1[i][j]*MatriceImgM1[i][j]);
  }
  
  CenterImg2(MatriceImgR3, length, width);
  CenterImg2(MatriceImgI3, length, width);

  IFFTDD(MatriceImgR3, MatriceImgI3, length, width);

  Recal(MatriceImgR3, length, width);

  int iMax = 0;
  int jMax = 0; 
  float maxValue = -INFINITY;
  
  for(int i=0; i< length; i++)
    for(int j =0; j<width; j++)
      if(MatriceImgR3[i][j] > maxValue){ 
        iMax = i; jMax = j;
        maxValue = MatriceImgR3[i][j];}
  
  int vectorX = length/2 - iMax;
  int vectorY = width/2 - jMax;

  printf("Le vecteur est: (%d, %d)\n", vectorX, vectorY);

  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,MatriceImgR3,length,width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImgR1);
  free_fmatrix_2d(MatriceImgI1);
  free_fmatrix_2d(MatriceImgM1);
  free_fmatrix_2d(MatriceImgR2);
  free_fmatrix_2d(MatriceImgI2);
  free_fmatrix_2d(MatriceImgM2);
  free_fmatrix_2d(MatriceImgR3);
  free_fmatrix_2d(MatriceImgI3);
  free_fmatrix_2d(MatriceImgM3);
  free_fmatrix_2d(MatriceImg1);
  free_fmatrix_2d(MatriceImg2);
  free_fmatrix_2d(MatriceImg3);

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


