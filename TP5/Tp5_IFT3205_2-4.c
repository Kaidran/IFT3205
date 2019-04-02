//------------------------------------------------------
// Prog    : Tp5_IFT3205_2-4                          
// Auteur  : Jessica Gauvin - Andre Lalonde                                           
// Date    : 26/03/2019                                 
// version : 1.0                                            
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------


//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include "FonctionDemo5.h"

//------------------------------------------------
// DEFINITIONS -----------------------------------
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "lena512"
#define NAME_IMG_OUT  "lena512_Restored"
#define NAME_IMG_DEG  "lena512_Degraded"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------
//------------------------------------------------
//----------------------------------------------------------
// copy matrix
//----------------------------------------------------------
void copy_matrix(float** mat1,float** mat2,int lgth,int wdth)
{
 int i,j;

 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) mat1[i][j]=mat2[i][j];
}



//------------------------------------------------
// CONSTANTES ------------------------------------
//------------------------------------------------
#define SIGMA_NOISE  30 

//------------------------------------------------
//------------------------------------------------
// FONCTIONS  ------------------------------------   
//------------------------------------------------
//------------------------------------------------
void initializeMatrice(float**,int,int);
void copySectionMatrix(float**,float**, int, int, int, int, int);
void debruitage(float**, float**, float**, int, int, int, int, int);
void debruitageN(float**, float***, float**, float**, float**, int, int, int);
void findT(float**, float***, float**, float**, float**, int, int);


//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char** argv)
{
 int length,width;
 char BufSystVisuImg[NBCHAR];

 //>Lecture Image 
 float** Img=LoadImagePgm(NAME_IMG_IN,&length,&width);
 
 //>Allocation memory
 float** ImgDegraded=fmatrix_allocate_2d(length,width);
 float** ImgDenoised=fmatrix_allocate_2d(length,width); 
 initializeMatrice(ImgDenoised, length, width);

 float** bloc8x8=fmatrix_allocate_2d(8, 8);
 initializeMatrice(bloc8x8, 8, 8); 

 //>Degradation 
 copy_matrix(ImgDegraded,Img,length,width);
 add_gaussian_noise(ImgDegraded,length,width,SIGMA_NOISE*SIGMA_NOISE);

 float*** mat3d=fmatrix_allocate_3d(64,length,width);

 findT(ImgDenoised, mat3d, ImgDegraded, Img, bloc8x8, length, width); 

 
 printf("\n\n  Info Noise");
 printf("\n  ------------");
 printf("\n    > MSE = [%.2f]",computeMMSE(ImgDenoised,Img,length)); 
 

 //=========================================================
 //== PROG =================================================
 //=========================================================

 // .....

  //---------------------------------------------
  // SAUVEGARDE 
  // -------------------
  // L'image dégradée             > ImgDegraded
  // Le resultat du debruitage    > ImgFiltered
  //----------------------------------------------
 SaveImagePgm(NAME_IMG_DEG,ImgDegraded,length,width);
 SaveImagePgm(NAME_IMG_OUT,ImgDenoised,length,width);  

  //>Visu Img
 strcpy(BufSystVisuImg,NAME_VISUALISER);
 strcat(BufSystVisuImg,NAME_IMG_IN);
 strcat(BufSystVisuImg,".pgm&");
 printf("\n > %s",BufSystVisuImg);
 system(BufSystVisuImg);

  //Visu ImgDegraded
 strcpy(BufSystVisuImg,NAME_VISUALISER);
 strcat(BufSystVisuImg,NAME_IMG_DEG);
 strcat(BufSystVisuImg,".pgm&");
 printf("\n > %s",BufSystVisuImg);
 system(BufSystVisuImg);

  //Visu ImgFiltered
 strcpy(BufSystVisuImg,NAME_VISUALISER);
 strcat(BufSystVisuImg,NAME_IMG_OUT);
 strcat(BufSystVisuImg,".pgm&");
 printf("\n > %s",BufSystVisuImg);
 system(BufSystVisuImg);


//--------------- End -------------------------------------     
//----------------------------------------------------------

  //Liberation memoire pour les matrices
 if (Img)          free_fmatrix_2d(Img);
 if (ImgDegraded)  free_fmatrix_2d(ImgDegraded);
 if (ImgDenoised)  free_fmatrix_2d(ImgDenoised);

  //Return
 printf("\n C'est fini... \n");; 
 return 0;
}

void initializeMatrice(float** mat0, int length, int width){
  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++)
      mat0[i][j] = 0;
}

void copySectionMatrix(float** toCopy,float** destination, int copy2bloc, int starti, int startj, int length, int width){
  const int size = 8;

  for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++){
      if(copy2bloc)
        destination[i][j] = toCopy[(i+starti)%length][(j+startj)%width];
      else
        destination[(i+starti)%length][(j+startj)%width] = toCopy[i][j];
    }
}

void cancelCoef(float** bloc8x8, int n){
  const int size = 8;
  
  for(int i=0; i<size; i++)
    for(int j=0; j<size; j++){
      if(abs(bloc8x8[i][j]) <= n)
        bloc8x8[i][j] = 0;
    }
}

void debruitage(float** result, float** mat, float** bloc8x8, int length, int width, int n, int decali, int decalj){

  for(int i=decali; i < length; i+= 8)
    for(int j=decalj; j < width; j+=8){
      copySectionMatrix(mat, bloc8x8, 1, i, j, length, width);
      ddct8x8s(-1, bloc8x8);
      cancelCoef(bloc8x8, n);
      ddct8x8s(1, bloc8x8);
      copySectionMatrix(bloc8x8, result, 0, i, j, length, width);
    }
}

void debruitageN(float** result, float*** mat3d, float** mat, float** img, float** bloc8x8, int length, int width, int n){
  int bestN = 0;
  float mseMin = INFINITY;
  float mse;
  int count = 0; 

  //Débruitage des images pour chaque combinaison de décallement
  for(int i=0; i<8 ; i++)
    for(int j=0; j<8; j++){
      debruitage(result, mat, bloc8x8, length, width, n, i, j);
      copy_matrix(mat3d[count], result, length, width);
      count++;

    }

  //Moyenne des 64 images débruitées
  float average;
  for(int i=0; i<length; i++)
    for(int j=0; j<width; j++){
      average = 0;
      for(int k=0; k<64; k++)
        average += mat3d[k][i][j];
      result[i][j] = average/64.0;
    }
  
}

void findT(float** result, float*** mat3d, float** mat, float** img, float** bloc8x8, int length, int width){
  int bestT = 0;
  float mseMin = INFINITY;
  float mse;

  //Recherche du meilleur seuil T possible
  for(int t=0; t<100 ; t++){
    debruitageN(result, mat3d, mat, img, bloc8x8, length, width, t); 
    mse = computeMMSE(result, img, length);
    if( mse < mseMin){
      mseMin = mse;
      bestT = t;
    }
  }

  printf("Valeur du meilleur seuil T trouve: %d\n",bestT);
  debruitageN(result, mat3d, mat, img, bloc8x8, length, width, bestT);
}


//----------------------------------------------------------
// Allocation matrix 3d float
// --------------------------
//
// float*** fmatrix_allocate_3d(int dsize,int vsize,int hsize)
// > Alloue dynamiquement un tableau 3D [dsize][vsize][hsize]
//
//-----------------------------------------------------------

//----------------------------------------------------------
//  ddct8x8s(int isgn, float** tab)
// ---------------------------------
//
// Fait la TCD (sgn=-1) sur un tableau 2D tab[8][8]
// Fait la TCD inverse (sgn=1) sur un tableau 2D tab[8][8]
//
//-----------------------------------------------------------
