/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205_2-3.c                          */
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
float carre(float x){
  return x*x;
}

void restore(float* restore, float* degrad, float length, float rho, float theta){
  restore[0] = degrad[0];
  restore[1] = degrad[1] - 2*cos(theta)*degrad[0] + 2*rho*cos(theta)*restore[0];

  for(int i=2; i<length; i++)
    restore[i] = degrad[i] - 2*cos(theta)*degrad[i-1] + degrad[i-2] 
    + 2*rho*cos(theta)*restore[i-1] - carre(rho)*restore[i-2];
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

  const float pi = 3.141593;
  const float rho = 0.99;
  const float f0 = 500.0 / sampleRate;
  const float theta = 2.0*pi*f0;

  //===============================
  //Question 2-3
  //===============================
   float*  Sign=LoadSignalDat("SoundFileDeg",&length);
   float* SignRestore=fmatrix_allocate_1d(length);

   restore(SignRestore, Sign, length, rho, theta);


   //TODO: Restaurer le signal
  
   //Sauvegarde en fichier .dat
   SaveSignalDatWav("SoundFileRest",SignRestore,length, sampleRate);
   SaveSignalDat("SoundFileRest",SignRestore,length);

   //Visu Ecran
   strcpy(BufSystVisuSig,NAME_VISUALISER);
   strcat(BufSystVisuSig,"SoundFileRest.dat&");
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


