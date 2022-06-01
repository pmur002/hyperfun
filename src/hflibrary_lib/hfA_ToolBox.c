/*==============================================================================

Copyright 1998, 1999 Valery Adzhiev, Alexander Pasko, Ken Yoshikawa 
Copyright 2003-2004 Benjamin Schmitt
Copyright 2008 Oleg Fryazinov

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

Description: Toolbox functions, from Ebert's book

Code by: Schmitt
Last revised: 15.04.2003
*/ 

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

//Clamping a value f in the interval [a,b]
/*Call: hfA_Clamp(f,a,b);

definition : min(max(f,a),b)

Parameters:
f  : real value (to be clamped)
a  : lower boundary of the interval
b  : upper boundary of the interval


Test file: 

C-parameters:
f_a[0]   corresponds to b
f_a[1]   corresponds to a
f_a[2]   corresponds to f


*/
double hfA_Clamp(double* f_a, String_Array_T* str_param){
	double f;
	double a;
	double b;


	f = f_a[2];
	a = f_a[1];
	b = f_a[0];

	if(f<a) return a;
	else if(f>b) return b;
	else return f;


}


//
/*Call: hfA_Pulse(f,a,b);

definition : step(f,a)-step(b,f)

Parameters:
f  : real value 
a  : real value 
b  : real value 


Test file: 

C-parameters:
f_a[0]   corresponds to a
f_a[1]   corresponds to b
f_a[2]   corresponds to f


*/
double hfA_Pulse(double* f_a, String_Array_T* str_param){
	double f;
	double a;
	double b;


	f = f_a[2];
	b = f_a[1];
	a = f_a[0];


	if(f<a) return 0.0;
	else if(f>b) return 0.0;
	else return 1.0;
}

//
/*Call: hfA_Step(f,a);

definition : if f<a return 0
else return 1

Parameters:
f  : real value 
a  : real value 


Test file: 

C-parameters:
f_a[0]   corresponds to a
f_a[1]   corresponds to f


*/
double hfA_Step(double* f_a, String_Array_T* str_param){
	double f;
	double a;


	f = f_a[1];
	a = f_a[0];


	if(f<a) return 0.0;
	else return 1.0;
}


//mapping a value x to [0,1], with a smooth interpolation between [a,b] 
/*Call: hfA_SmoothStep(f,a,b);

Parameters:
f  : real value to perform cubic interpolation
fmin,fmax : 

Test file: 

C-parameters:
f_a[0]   corresponds to fmax
f_a[1]   corresponds to fmin
f_a[2]   corresponds to f
f_a[3]   corresponds to s2
f_a[4]   corresponds to s1
f_a[5]   corresponds to s


*/
double hfA_SmoothStep(double* f_a, String_Array_T* str_param){
	double f;
	double a;
	double b;

	b = f_a[0];
	a = f_a[1];
	f = f_a[2];

	if (f > b)
		return 1.0;
	if (f < a)
		return 0.0;

	f = (f-a)/(b-a);
	f = (f*f*(3.0-2.0*f));

	return f;
}


//Modulo (and not remainder!)
/*Call: hfA_Modulo(a,b);

definition :	
a%b

Parameters:
a  : lower boundary of the interval
b  : upper boundary of the interval


Test file: 

C-parameters:
f_a[0]   corresponds to b
f_a[1]   corresponds to a


*/
double hfA_Modulo(double* f_a, String_Array_T* str_param){
	double a;
	double b;
	int n;

	a = f_a[1];
	b = f_a[0];

	n = (int)(a/b);

	a-=n*b;
	if(a<0.0) a+=b;

	return a;
}

//Floor (and not remainder!)
/*Call: hfA_Floor(a);

definition :	


Parameters:
a  : real value



Test file: 

C-parameters:
f_a[0]   corresponds to a


*/
double hfA_Floor(double* f_a, String_Array_T* str_param){
	int a;


	a = (int)f_a[0];


	return a;
}
