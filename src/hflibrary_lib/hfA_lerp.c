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


/*Core HyperFun Attributes Library 

Primitive: Linear interpolation of the components of 2 arrays 

Call: hfA_Lerp(s,s1,s2,f,fmin,fmax);

definition : s[i] = (1-f)*s1[i]+f*s2[i]

return     : f 

Parameters:
	s	 : resulting array
	s1	 : input array 1
	s2	 : input array 2
	f	 : real value to perform linear interpolation
	fmin : lower boundary used for linear interpolation
	fmax : upper boundary used for linear interpolation

	

Test file: 

C-parameters:
        f_a[0]   corresponds to fmax
        f_a[1]   corresponds to fmin
        f_a[2]   corresponds to f
		f_a[3]   corresponds to s2
        f_a[4]   corresponds to s1
        f_a[5]   corresponds to s

Code by: Schmitt
Last revised: 05 May 2003


*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "perlin.h"
extern int HF_Get_Runtime_Array(int index, double** array, int* size);

void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);


double hfA_Lerp(double* f_a, String_Array_T* str_param){
double *s1;
double *s2;
double *s;
double f;
double fmin;
double fmax;
int i;

int array_index, dim;

fmax = f_a[0];
fmin = f_a[1];
f    = f_a[2];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &s2, &dim); 

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &s1, &dim); 

array_index = (int)f_a[5];
HF_Get_Runtime_Array(array_index, &s, &dim); 

f=(f-fmin)/(fmax-fmin);

if(f<0.0) 
	for(i=0;i<dim;i++) s[i]=s1[i];
else if(f>1.0) 
	for(i=0;i<dim;i++) s[i]=s2[i];
else 
	for(i=0;i<dim;i++) s[i]=(1.0-f)*s1[i]+f*s2[i];

	
return f;
}

double hfA_Cerp(double* f_a, String_Array_T* str_param){
double *s1;
double *s2;
double *s;
double f;
double fmin;
double fmax;
int i;

int array_index, dim;

fmax = f_a[0];
fmin = f_a[1];
f    = f_a[2];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &s2, &dim); 

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &s1, &dim); 

array_index = (int)f_a[5];
HF_Get_Runtime_Array(array_index, &s, &dim); 

f=(f-fmin)/(fmax-fmin);

f = (f*f*(3.0-2.0*f));

if(f<0.0) 
	for(i=0;i<dim;i++) s[i]=s1[i];
else if(f>1.0) 
	for(i=0;i<dim;i++) s[i]=s2[i];
else 
	for(i=0;i<dim;i++) s[i]=(1.0-f)*s1[i]+f*s2[i];

return f;
}
