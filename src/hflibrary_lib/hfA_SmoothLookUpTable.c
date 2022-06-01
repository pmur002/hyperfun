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

Description: Use a value as an entry of a table to apply a set of attributes,with a linar interpolation between the 2 sets
	

Definition: Attibutes: {a1,a2,a3}
			Intervals: [0,i,j,1]
			Input value: p
				 if(p belongs to [0,i]) then the set of attributes a1 is set as the output
			else if(p belongs to [i,j]) then the set of attributes resulting from a linear interpolation of a2 and a3 is set as the output
			else if(p belongs to [j,1]) then the set of attributes a3 is set as the output

Call: hfA_SmoothLookUpTable(t,output,input_attr,input_map);

Parameters:
 t - entry of the table
 output - output array of attributes
 input_attr : input array of set of attributes.
 input_map : input array for the mapping

Test file: 

C-parameters:

        f_a[0]   corresponds to array mapping array
		f_a[1]   corresponds to array attributes array
		f_a[2]   corresponds to array output attributes
		f_a[3]   corresponds to array t

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





double hfA_SmoothLookUpTable(double* f_a, String_Array_T* str_param)
{

	double t,l;
	double *colormap;
	double *entry;
	double *output;

	int array_index, dim,dim1;
	int i;
	int n_entries;
	int j;

	int filled=0;

array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &entry, &n_entries); 

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &colormap, &dim); 

array_index = (int)f_a[2];
HF_Get_Runtime_Array(array_index, &output, &dim1); 

t = (double)f_a[3];
if(t<entry[0])
	for(j=0;j<dim1;j++) output[j] = colormap[j];
else
		for(i=0;i<n_entries-1;i++){

			if((t>=entry[i])&&(t<entry[i+1])){
				filled=1;
				l=(t-entry[i])/(entry[i+1]-entry[i]);
				for(j=0;j<dim1;j++) 
					output[j] = (1-l)*colormap[dim1*i+j]+(l)*colormap[dim1*(i+1)+j];
			break;
			}
	}

if(filled == 0)
	for(j=0;j<dim1;j++) output[j] = colormap[dim1*(n_entries-1)+j];



return 1.0;

}



