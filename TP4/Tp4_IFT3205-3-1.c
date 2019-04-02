//------------------------------------------------------
// Prog    : Tp4_IFT3205_3-1                          
// Auteur  : Jessica Gauvin & André Lalonde                                           
// Date    : 20 mars 2019                                 
// version :                                             
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------

//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo4.h"

//------------------------------------------------
// DEFINITIONS -----------------------------------   
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "cameraman"
#define NAME_IMG_OUT1 "image-TpIFT3205-3-1"
#define NAME_IMG_OUT2 "cameraman_degraded"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------   
//------------------------------------------------

//------------------------------------------------
// VARIABLES GLOBALES  ---------------------------   
//------------------------------------------------

float** mat;
float** mat_img;
float** mat_rest;
float** mat_rest_prec;
float** mat_rest_best;
float** mat_psf;

float** degradedImgI;
float** degradedImg;
float** mat_rest_precI;
float** mat_tmp3;
float** mat_tmp4;
float** hReal;
float** hImaginary;
float** hImaginaryConjugate;
float** squareModFPrev;
float** squareModH;
float** mat_tmp10;
float** mat_tmp11;
float** matRestoreI;

//>Taille Image
int length=256;
int width=256;
int size_image=256;

//>Parametre de degradation
int size=9;
float variance_noise=0.5;
int psf=1;

//>Nb d'Iterations
int nbiter=20;

//>ImprovmentSNR
float isnr;

//------------------------------------------------
//------------------------------------------------
// FONCTIONS  ------------------------------------   
//------------------------------------------------
//------------------------------------------------
void ModSquare(float**,float**,float**,int,int);
void MultMatrix(float**,float**,float**,float**,float**,float**,int,int);
void initializeMatrice(float**,int,int);
void adjustGrey(float**,int,int);
void copyMatrice(float**,float**,int,int);
void createBlurryFilter(float**,int,int,float);
void conjugate(float**, float**,int,int);
void restore(float**,float**,float**,float**,float**,int,int,float,float);
float isnrResult(float**,float**,float**,int,int);

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int i,j,k;
  int lgth,wdth;
  char BufSystVisuImg[100];

  //Allocation memoire matrice
  mat=fmatrix_allocate_2d(length,width);
  mat_rest=fmatrix_allocate_2d(length,width);
  mat_rest_prec=fmatrix_allocate_2d(length,width); 
  mat_rest_best=fmatrix_allocate_2d(length,width); 
  mat_psf=fmatrix_allocate_2d(length,width);

  degradedImgI=fmatrix_allocate_2d(length,width);
  degradedImg=fmatrix_allocate_2d(length,width);
  mat_rest_precI=fmatrix_allocate_2d(length,width);
  mat_tmp3=fmatrix_allocate_2d(length,width);
  mat_tmp4=fmatrix_allocate_2d(length,width);
  hReal=fmatrix_allocate_2d(length,width);
  hImaginary=fmatrix_allocate_2d(length,width);
  hImaginaryConjugate=fmatrix_allocate_2d(length,width);
  squareModFPrev=fmatrix_allocate_2d(length,width);
  squareModH=fmatrix_allocate_2d(length,width);
  mat_tmp10=fmatrix_allocate_2d(length,width);
  mat_tmp11=fmatrix_allocate_2d(length,width);
  matRestoreI=fmatrix_allocate_2d(length,width);

 
  //=========================================================
  //== PROG =================================================
  //=========================================================

  //>Lecture Image
  mat_img=LoadImagePgm(NAME_IMG_IN,&lgth,&wdth);

  //--------------------------------------------------------
  //>Degradation
  //--------------------------------------------------------
  // Cette fonction ajoute un flou créé par une psf uniforme 
  // (fonction porte) de taille sizexsize puis ajoute sur
  // cette image rendue floue, un bruit Gaussien de variance
  // variance_noise
  //
  // Entrée : mat_img :: image non dégradée
  // Sortie : mat     :: image dégradée
  //--------------------------------------------------------
  degradation(mat_img,mat,length,width,psf,size,variance_noise);
  degradation(mat_img,degradedImg,length,width,psf,size,variance_noise);

  //============
  //WIENER
  //============
  //Initially F'(u, v) = G(u, v)
  copyMatrice(mat_rest_prec, degradedImg, length, width);

  initializeMatrice(degradedImgI, length, width);
  FFTDD(degradedImg, degradedImgI, length, width);

  for(int i=0; i<20; i++){
    initializeMatrice(mat_rest_precI, length, width);
    FFTDD(mat_rest_prec, mat_rest_precI, length, width);

    restore(mat_rest, degradedImg, degradedImgI, mat_rest_prec, mat_rest_precI, length, width, size, variance_noise);

    adjustGrey(mat_rest, length, width);
    copyMatrice(mat_rest_prec, mat_rest, length, width);
    printf("\nISNR %d: %f", i, isnrResult(mat_img, mat, mat_rest, length, width));
  }
    


  //---------------------------------------------
  // SAUVEGARDE et VISU
  // -------------------
  // Le resultat de la restoration > mat_rest
  // L'image dégradée              > mat
  // L'image non dégradée          > mat_img
  //----------------------------------------------
  SaveImagePgm(NAME_IMG_OUT1,mat_rest,length,width);
  SaveImagePgm(NAME_IMG_OUT2,mat,length,width);
  
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_IN);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  
  //Liberation memoire pour les matrices
  if (mat)            free_fmatrix_2d(mat); 
  if (mat_img)        free_fmatrix_2d(mat_img); 
  if (mat_rest)       free_fmatrix_2d(mat_rest);
  if (mat_rest_prec)  free_fmatrix_2d(mat_rest_prec);
  if (mat_rest_best)  free_fmatrix_2d(mat_rest_best);
  if (mat_psf)        free_fmatrix_2d(mat_psf);
  if (degradedImgI)  free_fmatrix_2d(degradedImgI);
  if (degradedImg)  free_fmatrix_2d(degradedImg); 
  if (mat_rest_precI)  free_fmatrix_2d(mat_rest_precI); 
  if (mat_tmp3)  free_fmatrix_2d(mat_tmp3); 
  if (mat_tmp4)  free_fmatrix_2d(mat_tmp4); 
  if (hReal)  free_fmatrix_2d(hReal); 
  if (hImaginary)  free_fmatrix_2d(hImaginary); 
  if (hImaginaryConjugate)  free_fmatrix_2d(hImaginaryConjugate); 
  if (squareModFPrev)  free_fmatrix_2d(squareModFPrev);
  if (squareModH)  free_fmatrix_2d(squareModH); 
  if (mat_tmp10) free_fmatrix_2d(mat_tmp10);
  if (mat_tmp11) free_fmatrix_2d(mat_tmp11);
  if (matRestoreI) free_fmatrix_2d(matRestoreI);

  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0;    
}

void ModSquare(float** matM,float** matR,float** matI,int lgth,int wdth)
{
 int i,j;

 /*Calcul du module*/
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
 matM[i][j]=(matR[i][j]*matR[i][j])+(matI[i][j]*matI[i][j]);
}

void MultMatrix(float** matRout,float** matIout,float** mat1Rin,float** mat1Iin,
float** mat2Rin,float** mat2Iin,int lgth,int wdth)
{
 int i,j;

 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
   { matRout[i][j]=mat1Rin[i][j]*mat2Rin[i][j]-mat1Iin[i][j]*mat2Iin[i][j];
     matIout[i][j]=mat1Rin[i][j]*mat2Iin[i][j]+mat2Rin[i][j]*mat1Iin[i][j]; }
}

void initializeMatrice(float** mat0, int length, int width){
  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++)
      mat0[i][j] = 0;
}

void adjustGrey(float** matToAdjust, int length, int width){
  for(int i=0; i<length; i++)
      for(int j=0; j<width; j++){
        if(matToAdjust[i][j] < 0)
          matToAdjust[i][j] = 0;
        else if(matToAdjust[i][j] > 255)
          matToAdjust[i][j] = 255;
    } 
}

void copyMatrice(float** mat_destination,float** mat_toCopy, int length,int width){
  for(int i=0; i<length; i++)
      for(int j=0; j<width; j++)
        mat_destination[i][j] = mat_toCopy[i][j];
}

void createBlurryFilter(float** matFiltre, int length, int width, float l){
  int n=0;
  int lowerBound = l/2;
  int upperBound = l-lowerBound;

  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++){
      if((i<upperBound && (j<upperBound || j >= width-lowerBound)) || 
        (i >= length-lowerBound && (j < upperBound || j >= width-lowerBound))) {
        matFiltre[i][j] = (length*width)/(CARRE(l));
        n++;
      }
      else
        matFiltre[i][j] = 0;
    }
}

void conjugate(float** result, float** matI, int length, int width){
   for(int i=0; i<length; i++)
    for(int j=0; j<width; j++)
      result[i][j] = -matI[i][j];
}

void restore(float** matRestore, float** matGR, float** matGI, float** matPrevR, float** matPrevI, int length, int width, float l, float var){
  float denom;
  var /= (length*width);

  //Creating h filter
  initializeMatrice(hReal, length, width);
  initializeMatrice(hImaginary, length, width);
  createBlurryFilter(hReal, length, width, l);
  
  FFTDD(hReal, hImaginary, length, width);
  //CenterImg(hReal, length, width);

  conjugate(hImaginaryConjugate, hImaginary, length,width);

  //Calculating square of module
  ModSquare(squareModFPrev, matPrevR, matPrevI, length, width);
  ModSquare(squareModH, hReal, hImaginary, length, width);

  //Calculating right side of equation
  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++){
      //Denominator: | H |^2 + var / | Fprev |^2
      denom = squareModH[i][j]+(var/squareModFPrev[i][j]);
      //H* / Denominator
      mat_tmp10[i][j] = hReal[i][j]/denom;
      mat_tmp11[i][j] = hImaginaryConjugate[i][j]/denom;
    }

  // G(u, v) x Right side
  MultMatrix(matRestore, matRestoreI, matGR, matGI, mat_tmp10, mat_tmp11, length, width);
  
  //F'(u, v) =­> f'(x, y)
  IFFTDD(matRestore, matRestoreI, length, width);
}

float isnrResult(float** originalImg, float** degradedImg, float** restoreImg, int length, int width){
  float num = 0;
  float denum = 0;

  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++){
      num += CARRE(originalImg[i][j] - degradedImg[i][j]);
      denum += CARRE(originalImg[i][j] - restoreImg[i][j]);
    }

  return 10*log10f(num/denum);
}

