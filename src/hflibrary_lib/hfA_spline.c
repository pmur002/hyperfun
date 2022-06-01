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


/*
HyperFun Attributes Library

  Spline interpolation between components of an array

  Call: hfA_Spline(s,List_s1,x,xmin,xmax);

definition :	
	
 
Parameters:
 s  : output array
 size : number of different knots / color. Minimum is 4 
 List_s1  :array of colors (actually, it is a big array containing different colors; i.e. array of array of colors)
 x :    real value used for interpolation
 xmin : lower boundary 
 xmax : upper boundary 


Test file: 

C-parameters:
		f_a[0]   corresponds to xmax
		f_a[1]   corresponds to xmin
		f_a[2]   corresponds to x
		f_a[3]   corresponds to List_s1
		f_a[4]   corresponds to s


Code by: Schmitt
Last revised: 15.04.2003
*/ 

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);



#define CR00 -0.5
#define CR01 1.5
#define CR02 -1.5
#define CR03 0.5
#define CR10 1.0
#define CR11 -2.5
#define CR12 2.0
#define CR13 -0.5
#define CR20 -0.5
#define CR21 0.0
#define CR22 0.5
#define CR23 0.0
#define CR30 0.0
#define CR31 1.0
#define CR32 0.0
#define CR33 0.0
 

double hfA_Spline(double* f_a, String_Array_T* str_param){
double xmin;
double xmax;
double x;
int size;
int i;
int N;

double c0,c1,c2,c3;

int span,nspans;

double *s;
double *knot;

int array_index;


xmax = f_a[0];
xmin = f_a[1];
 x   = f_a[2];



array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &knot, &size); 

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &s, &N); 


size = size/N;

x=(x-xmin)/(xmax-xmin);

if(x<0.0)
	for(i=0;i<N;i++) s[i]=knot[i];
else if(x>1.0)
	for(i=0;i<N;i++) s[i]=knot[(size-1	)*N+i];

else {

	nspans = size-3;
	x=x*nspans;
	span = (int)x;

	if(span>=nspans) span = nspans-3;


	x-=span;
	
	for(i=0;i<N;i++){
			c3 = CR00*knot[(span+0)*N+i] + CR01*knot[(span+1)*N+i] + CR02*knot[(span+2)*N+i]+ CR03*knot[(span+3)*N+i];
			c2 = CR10*knot[(span+0)*N+i] + CR11*knot[(span+1)*N+i] + CR12*knot[(span+2)*N+i]+ CR13*knot[(span+3)*N+i];
			c1 = CR20*knot[(span+0)*N+i] + CR21*knot[(span+1)*N+i] + CR22*knot[(span+2)*N+i]+ CR23*knot[(span+3)*N+i];
			c0 = CR30*knot[(span+0)*N+i] + CR31*knot[(span+1)*N+i] + CR32*knot[(span+2)*N+i]+ CR33*knot[(span+3)*N+i];
	
	
			s[i] = ((c3*x+c2)*x+c1)*x+c0;

	}


	}



return 1;
}

