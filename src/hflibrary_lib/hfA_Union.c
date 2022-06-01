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

Description: Union between Attributes. Predefined operations (using the operator parameter):
			-Arbitrary 
			-Min/Max
			This function can be applied either to the whole array, or to a single
			attribute (using the index parameter)
			Returns the union valu : R_Uni(f1,f2);

Definition: 

Call: hfA_Union(f1,f2,output,input1,input2,operator)

Parameters:
 f1,f2  - two objects
 ouput - output array of attributes
 input1,input2 - output array of attributes
 operator : type of union
 index : index of the array where the union has to be applied
 

Test file: 

C-parameters:
        f_a[0]   corresponds to type of operator
        f_a[1]   corresponds to array color c2 of f2
        f_a[2]   corresponds to array color c1 of f1
        f_a[3]   corresponds to resulting array
        f_a[4]   corresponds to object f2
		f_a[5]   corresponds to object f1

Predifinned Operators (color of the intersection):
	0 : output= input1  
	1 : output = input2
	2 : Sum  : output[i]=input1[i]+input2[i]
	3:  Min of the components:  output[i] = min(input1[i],input2[i]);
	4:  Max of the components: output[i] = max(input1[i],input2[i]);
Code by: Schmitt
Last revised: 05 May 2003
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>


extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern const double EPS;

double hfA_Union(double* f_a, String_Array_T* str_param)
{
	double f1, f2;
	double *c1;
	double *c2;
	double *result;
	int op;
	int array_index, dim;
	int i;
	double f1_u_f2;

op  =(int)f_a[0];

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &c2, &dim); 

array_index = (int)f_a[2];
HF_Get_Runtime_Array(array_index, &c1, &dim); 

f2 = (double)f_a[4];
f1 = (double)f_a[5];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &result, &dim); 


f1_u_f2 = f1 + f2 + sqrt(f1*f1 + f2*f2);

if(f1_u_f2>-EPS) { 
		if((f1>=0.0)&&(f2<0.0)) {
				for(i=0;i<dim;i++)
					result[i] = c1[i];
		}
		else if((f2>=0.0)&&(f1<0.0)) {
			for(i=0;i<dim;i++)
				result[i] = c2[i];
		}
		else {
			switch(op){
			case 0 : {for(i=0;i<dim;i++) result[i] = c1[i];}break;
			case 1 : {for(i=0;i<dim;i++) result[i] = c2[i];}break;
			case 2 : {for(i=0;i<dim;i++) result[i] = c1[i]+c2[i];}break;
			case 3 :{
					for(i=0;i<dim;i++)
						if(c1[i]>c2[i]) result[i] = c2[i];
						else result[i] = c1[i];
					 }break;
			case 4 :{
					for(i=0;i<dim;i++)
						if(c1[i]>c2[i]) result[i] = c1[i];
						else result[i] = c2[i];
					 }break;

			}
		}
}


return f1_u_f2;
}
