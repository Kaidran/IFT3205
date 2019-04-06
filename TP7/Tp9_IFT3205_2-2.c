/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205_2-2.c                          */
/* Auteur  : Jessica Gauvin & Andre Lalonde             */
/* Date    : 10/04/2010                                 */
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

#include "FonctionDemo9.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void degrad(float* Sign1, int length, float rep, float pi){
  for(int i=0; i<length; i++){
    Sign1[i] = Sign1[i]+sin(2.0*pi*i/rep);
  }
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  char BufSystVisuSig[100];
  int length;
  int sampleRate = 11025;

  const float freq = 500;
  const float pi = 3.141593;

  //===============================
  //Question 2.2
  //===============================
   float*  Sign1=LoadSignalDat("SoundFile",&length);

   //TODO: Modifier Sign1 --> ajouter une fréquence sinusoïdale parasite
   //de fréquence 500 Hz et d'amplitude 1.0
   degrad(Sign1, length, sampleRate/freq, pi);
  
   //Sauvegarde en fichier .dat
   SaveSignalDatWav("SoundFileDeg",Sign1,length, sampleRate);
   SaveSignalDat("SoundFileDeg",Sign1,length);

   //Visu Ecran
   strcpy(BufSystVisuSig,NAME_VISUALISER);
   strcat(BufSystVisuSig,"SoundFileDeg.dat&");
   printf(" %s",BufSystVisuSig);
   system(BufSystVisuSig);


   //---------------------------------------------
   //Sauvegarde de SignZ (30000 echantillons 
   //aléatoire entre [0::200]) dans un 
   //fichier .wav avec une periode 
   //d'echantillonnage de 10000: Nb echant/secondes
   //(pour 3 secondes d'écoute)
   //----------------------------------------------
   //----------------------------------------------
   if (1)
      {
       length=30000;
       float*  SignZ=fmatrix_allocate_1d(length);
       for(i=0;i<length;i++) SignZ[i]=(int)(((float)rand()/RAND_MAX)*200.0);
       SaveSignalDatWav("SignalZ",SignZ,length,10000);
      }
     

     //Rappel -Pour Lecture/Sauvegarde/Visu [Fichier Son]
     //=================================================
     //float*  SignIn=LoadSignalDat("NameSignalIn",&length);
     //SaveSignalDat("NameSignalOut",SignIn,length); 
     //strcpy(BufSystVisuSig,NAME_VISUALISER);
     //strcat(BufSystVisuSig,"NameSignalOut.dat&");
     //printf(" %s",BufSystVisuSig);
     //system(BufSystVisuSig);
  
 
  //==End=========================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


