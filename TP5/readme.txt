----------------------------------
---- TP no 5 ---------------------
----------------------------------

Creer un repertoire TP5 et copier les differents progs dans ce repertoire
  
Le programme Tp5_IFT3205_2-1  chargera l'image lena512.pgm dans un tableau 2D,
et dégradera cette image  avec un bruit additif Gaussien blanc d'écart type
sigma=30 (cad qu'il repondra à la question 2.1).

Bon Tp,
Max

 

-------------

Pour information, j'obtiens les valeurs de MSE suivantes :

 Q2.2 :: MSE=157    (n=6)
 Q2.3 :: MSE=140    (T=100)
 Q2.4 :: MSE=65     (T=90)


===========
Conseils
===========

N'oubliez pas 

  ddct8x8s(-1, a); DCT sur le vecteur (spatial) a
  ddct8x8s(1, a);  Inverse DCT (IDCT) sur le vecteur (spectral_ a

-----------

Pour la question Q2.4 (algorithme 1)
le plus simple est d'allouer de la mémoire pour une matrice 3D
de 64 niveaux de profondeur et de longueur/largeur; la 
longueur/largeur de l'image du style:

float*** mat3d=fmatrix_allocate_3d(SizeWindow*SizeWindow,lgth,wdth);


et de stocker dans cette matrice 3D:

au 0-ième niveau de profondeur, cad dans mat3d[0][][]; 
l'ensemble des blocs 8x8 décalé de 0 pixel horizontal et 0 pixel vertical,

dans le 1-ier niveau de profondeur cad dans mat3d[1][][]; 
l'ensemble des blocs 8x8 décalé de 1 pixel horizontal et 0 pixel vertical,

dans le 2-ieme niveau de profondeur,  l'ensemble
des blocs 8x8 décalé de 2 pixel horizontal et 0 pixel vertical,

....

dans le 8-ieme niveau de profondeur,  
l'ensemble des blocs 8x8 décalé de 0 pixel horizontal et 1 pixel vertical,
etc...
et moyenner ces 64 images.





 



