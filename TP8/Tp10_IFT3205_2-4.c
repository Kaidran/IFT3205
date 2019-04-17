/*------------------------------------------------------*/
/* Prog    : Tp10_IFT3205_2-4.c                         */
/* Auteur  : Andre Lalonde - Jessica Gauvin             */
/* Date    : 14/04/2019                                 */
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

#include "FonctionDemo10.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,n;
  char BufSystVisuSig[100];
  int length;

  //=================================================
  //Question 2.1
  //------------
  //
  // Sample Rate = 11025 ech/sec
  // Frequence Échant= 11025
  //
  //  
  //=================================================

  float*  SignX=LoadSignalDat("SOUND_GoodMorningVietnam",&length);
  float*  SignY=fmatrix_allocate_1d(length);
  float*  bb=fmatrix_allocate_1d(length);
  
   for(n=0;n<length;n++) {
        bb[n] = (rand()/(float)RAND_MAX) * 2 - 1;
    }
  //--------------------------------
  //Restauration  Équation Récurente
  //--------------------------------
  // 
  // y(n) = b(n) + b(n - n_0)
  //
  //--------------------------------
  float SamplingRate=11025;
  float G=0.9;
  int   Retard=14;

  for(n=0;n<length;n++) {
    SignY[n]= bb[n];
    if (n>(Retard-1)) 
      SignY[n]+= G*bb[n-Retard];
    SignY[n] = SignY[n] * 0.1;
   }

   //Sauvegarde
   SaveSignalDatWav("SignalOut4",SignY,length,SamplingRate); 
   //SaveSignalDat("SOUND_GoodMorningVietnam1",SignY,length);
 
   //Visu
   //strcpy(BufSystVisuSig,NAME_VISUALISER);
   //strcat(BufSystVisuSig,"SOUND_GoodMorningVietnam1.dat&");
   //printf(" %s",BufSystVisuSig);
   //system(BufSystVisuSig);
       
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


