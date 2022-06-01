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

Description: 2 steps in this function:
		1- Caculate a psuedo random value
		2- use this value as a table entry for another set of attributes.

		Different type are available. SITLL UNDER DEBUGGING/CONSTRUCTION.

Definition: Attibutes: {a1,a2,a3}
			Intervals: [0,i,j,1]
			Random value: p
				 if(p belongs to [0,i]) then the set of attributes a1 is set as the output
			else if(p belongs to [i,j]) then the set of attributes a2 is set as the output
			else if(p belongs to [j,1]) then the set of attributes a3 is set as the output

Call: hfColorPattern(x,type,output,freq,input_attr,input_map);

Parameters:
 x - point coordinates array 
 type - type of pattern.
 output - output array of attributes
 freq - frequency
 input_attr : input array of set of attributes.
 input_map : input array for the mapping

Test file: 

C-parameters:

        f_a[0]   corresponds to array mapping array
		f_a[1]   corresponds to array attributes array
		f_a[2]   corresponds to frequency
		f_a[3]   corresponds to array output attributes
		f_a[4]   corresponds to array x

Code by: Schmitt
Last revised: 24.04.2001
*/ 

#include "pattern.h"
#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);


extern const double EPS_COLOR;



double hfA_Crackles(double* f_a, String_Array_T* str_param)
{

	double *x;
	double *color;
	double *prob;
	double *rgb;

	int array_index, dim,dim1;
	int i;
	int n_prob;
	double xt,yt,zt;
	double noise;
	double freq,phase;
	double delta = 0.1;
	int j;

array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &prob, &n_prob); 



array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &color, &dim); 

	   freq = (double)f_a[2];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &rgb, &dim1); 

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &x, &dim); 

xt = x[0];
yt = x[1];
zt = x[2];


phase = (sqrt(xt*xt)+sqrt(yt*yt)+sqrt(zt*zt))/3.0;

noise = Crackles(xt,yt,zt,phase,freq);


color[n_prob] = 1.0+0.1;		
for(i=0;i<n_prob-1;i++){

	if((noise>=prob[i])&&(noise<prob[i+1]))
		for(j=0;j<dim1;j++) rgb[j] = color[dim1*i+j];
		
		
	
}



return 1.0;

}



