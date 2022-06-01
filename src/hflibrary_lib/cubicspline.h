/*==============================================================================

 Copyright 1998, 1999 Valery Adzhiev, Alexander Pasko, Ken Yoshikawa 
 Copyright 2003-2004 Benjamin Schmitt

 This Work or file is part of the greater total Work, software or group of
 files named HyperFun Polygonizer.

 HyperFun Polygonizer can be redistributed and/or modified under the terms 
 of the CGPL, The Common Good Public License as published by and at CGPL.org
 (http://CGPL.org).  It is released under version 1.0 Beta of the License
 until the 1.0 version is released after which either version 1.0 of the
 License, or (at your option) any later version can be applied.

 THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
 CGPL, The Common Good Public License for more information.)

 You should have received a copy of the CGPL along with HyperFun Polygonizer;  
 if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/


#include <malloc.h>
#include <stdio.h>

#ifndef CUBIC_SPLINE_H
#define CUBIC_SPLINE_H

#define MAX_POINTS 200
#define EPSILON 0.01
static double noeuds_u[MAX_POINTS];
static double noeuds_v[MAX_POINTS];
static double noeuds_w[MAX_POINTS];
static int init_spline=1;
static U,V,W;
static double *** ext_ctr_pts;

void InitArray(int u,int v,int w,double * ctr_points){

int i,j,k;
int tu,tv,tw;
//FILE *file;
	ext_ctr_pts = (double ***)malloc((u+1)*sizeof(double**));
	for(i=0;i<v;i++){
		ext_ctr_pts[i] = (double **)malloc((v+1)*sizeof(double*));
		for(j=0;j<w;j++){
			ext_ctr_pts[i][j] = (double *)malloc((w+1)*sizeof(double));
		}
	}

/********Extended Control Points*/
	for(i=0;i<u;i++) 
		for(j=0;j<v;j++){
			ext_ctr_pts[i][j][w-1] = -15.0;
			ext_ctr_pts[i][j][0] =   -15.0;

			ext_ctr_pts[i][v-1][j] = -15.0;
			ext_ctr_pts[i][0][j] =   -15.0;

			ext_ctr_pts[u-1][i][j] = -15.0;
			ext_ctr_pts[0][i][j] =   -15.0;
	}
			/***************************/
	for(i=1;i<u-1;i++) 
		for(j=1;j<v-1;j++){
	
			ext_ctr_pts[i][j][w-2] = -15.0;
			ext_ctr_pts[i][j][1] =   -15.0;

			ext_ctr_pts[i][v-2][j] = -15.0;
			ext_ctr_pts[i][1][j] =   -15.0;

			ext_ctr_pts[u-2][i][j] = -15.0;
			ext_ctr_pts[1][i][j] =   -15.0;
		}
			/***************************/

	for(i=2;i<u-2;i++) 
		for(j=2;j<v-2;j++){
	
			ext_ctr_pts[i][j][w-3] = -15.0;
			ext_ctr_pts[i][j][2] = -15.0;

			ext_ctr_pts[i][v-3][j] = -15.0;
			ext_ctr_pts[i][2][j] = -15.0;

			ext_ctr_pts[u-3][i][j] = -15.0;
			ext_ctr_pts[2][i][j] = -15.0;

			}
/*********Copy the original array (u+3) to (u-3)*/
		tu=u-6;
		tv=v-6;
		tw=w-6;

		for(i=0;i<tu;i++) 
			for(j=0;j<tv;j++) 
				for(k=0;k<tw;k++){
					ext_ctr_pts[i+3][j+3][k+3] = ctr_points[i*tv*tw+j*tw+k];
				}

	/*file=fopen("copy_array.deb","w");
	for(i=0;i<u;i++) {
		for(j=0;j<v;j++){ 
				for(k=0;k<w;k++){
					fprintf(file," %f, ",ext_ctr_pts[i][j][k]);
				}
		fprintf(file,"\n");
		}
		fprintf(file,"\n");

	}
	fclose(file);*/
};


void InitSpline(int u,int v,int w, double *ctr_points){
int i;
  
int valeur_noeud_courante_u;
int valeur_noeud_courante_v;
int valeur_noeud_courante_w;

int increment_noeud_u;
int increment_noeud_v;
int increment_noeud_w;

 valeur_noeud_courante_u=0;
 valeur_noeud_courante_v=0;
 valeur_noeud_courante_w=0;

 increment_noeud_u=1;
 increment_noeud_v=1;
 increment_noeud_w=1;

//InitArray(u,v,w,ctr_points);
/********************************AXE  U******************************/
init_spline = 0;

  
  for ( i=0;i<MAX_POINTS+4;noeuds_u[i++]=0);

	noeuds_u[0]=(double)valeur_noeud_courante_u;
	noeuds_u[1]=(double)valeur_noeud_courante_u;
	noeuds_u[2]=(double)valeur_noeud_courante_u;
	noeuds_u[3]=(double)valeur_noeud_courante_u;
	
	for(i=3;i<u;i++)
    {
      noeuds_u[i]=(double)valeur_noeud_courante_u;
      valeur_noeud_courante_u+=increment_noeud_u;
    }
 for(i=0;i<=3;i++)
   noeuds_u[u+i]=(double)valeur_noeud_courante_u;

/********************************AXE  V******************************/
 

	//for (int i=0;i<MAX_POINTS;point_controles[i++]=0);
  valeur_noeud_courante_v=0;
  increment_noeud_v=1;
  for (i=0;i<MAX_POINTS+4;noeuds_v[i++]=0);

	noeuds_v[0]=(double)valeur_noeud_courante_v;
	noeuds_v[1]=(double)valeur_noeud_courante_v;
	noeuds_v[2]=(double)valeur_noeud_courante_v;
	noeuds_v[3]=(double)valeur_noeud_courante_v;
	
	for(i=3;i<v;i++)
    {
      noeuds_v[i]=(double)valeur_noeud_courante_v;
      valeur_noeud_courante_v+=increment_noeud_v;
    }
 for(i=0;i<=3;i++)
   noeuds_v[v+i]=(double)valeur_noeud_courante_v;
 /********************************AXE  W******************************/
 

	//for (int i=0;i<MAX_POINTS;point_controles[i++]=0);
  valeur_noeud_courante_w=0;
  increment_noeud_w=1;
  for (i=0;i<MAX_POINTS+4;noeuds_w[i++]=0);

	noeuds_w[0]=(double)valeur_noeud_courante_w;
	noeuds_w[1]=(double)valeur_noeud_courante_w;
	noeuds_w[2]=(double)valeur_noeud_courante_w;
	noeuds_w[3]=(double)valeur_noeud_courante_w;
	
	for(i=3;i<w;i++)
    {
      noeuds_w[i]=(double)valeur_noeud_courante_w;
      valeur_noeud_courante_w+=increment_noeud_w;
    }
 for(i=0;i<=3;i++)
   noeuds_w[w+i]=(double)valeur_noeud_courante_w;



/*for(i=4;i<9;i++)
	for(int j=4;j<9;j++)
		for(int k=4;k<9;k++){

		ctr_pts[i][j][k] = 5.0;
		}*/
	


 
}





double bspline_u(double t, int indice, int degre)
{
	double result;

  if (degre == 0)
    return ((t>=noeuds_u[indice])&&
	    (t<noeuds_u[indice+1])) ? 1.0:0.0;
  else 
    {
      double support;
      result = 0.0; 
      support = noeuds_u[indice+degre] - noeuds_u[indice];
      if (support > EPSILON) 
			result += (t - noeuds_u[indice])/support*bspline_u(t,indice,degre-1);
      
	  indice++; 
      
	  support = noeuds_u[indice+degre] - noeuds_u[indice];
      if (support > EPSILON) 
		result += (noeuds_u[indice+degre] - t)/support*bspline_u(t,indice,degre-1);
      return result;
    }
return 0;
}


double bspline_v(double t, int indice, int degre)
{
	double result;

  if (degre == 0)
    return ((t>=noeuds_v[indice])&&
	    (t<noeuds_v[indice+1])) ? 1.0:0.0;
  else 
    {
      double support;
      result = 0.0; 
      support = noeuds_v[indice+degre] - noeuds_v[indice];
      if (support > EPSILON) 
			result += (t - noeuds_v[indice])/support*bspline_v(t,indice,degre-1);
      
	  indice++; 
      
	  support = noeuds_v[indice+degre] - noeuds_v[indice];
      if (support > EPSILON) 
		result += (noeuds_v[indice+degre] - t)/support*bspline_v(t,indice,degre-1);
      return result;
    }
return 0;
}

double bspline_w(double t, int indice, int degre)
{
	double result;

  if (degre == 0)
    return ((t>=noeuds_w[indice])&&
	    (t<noeuds_w[indice+1])) ? 1.0:0.0;
  else 
    {
      double support;
      result = 0.0; 
      support = noeuds_w[indice+degre] - noeuds_w[indice];
      if (support > EPSILON) 
			result += (t - noeuds_w[indice])/support*bspline_w(t,indice,degre-1);
      
	  indice++; 
      
	  support = noeuds_w[indice+degre] - noeuds_w[indice];
      if (support > EPSILON) 
		result += (noeuds_w[indice+degre] - t)/support*bspline_w(t,indice,degre-1);
      return result;
    }
return 0;
}



static double *debug_pts;//[42875]={

#endif
