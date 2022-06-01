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

Description : CheckerBoard pattern (3D)

Definition: 

Call: hfA_CheckerBoard(x,block_size,gap_size,output,input_block,input_gap);

Parameters:
 x - point coordinates array 
 block_size - array containing the width, height and depth of the blocks
 gap_size - array containing the size of the space between the blocks
			along the x,z,y axis.
 output - output array containing the attributes
 input_block : array of attributes corresponging to the block
 input_gap   : array of attributes corresponging to the gaps

	
Test file: hfA_CheckerBoard.hf

C-parameters:

		f_a[0]   corresponds to array space color 
		f_a[1]   corresponds to array cube color 
		f_a[2]   corresponds to array resulting color 
       	f_a[3]   corresponds to array dx,dy,dz (space between)
    	f_a[4]   corresponds to array width, height, depth (cube size)
		f_a[5]   corresponds to array x 

Code by: Schmitt
Last revised: 10 June 2003 
*/ 

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

	
double hfA_CheckerBoard(double* f_a, String_Array_T* str_param)
{

	double	*x;
	double	*d;
	double	*cube;
	int		array_index, dim;
	int dim1,dim2;

	double delta[3];
	double center[3];
	int		i;
	double *c;
	double *c_cube;
	double *c_space;
	double lmin[3];
	double lmax[3];



array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &c_space, &dim2); 

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &c_cube, &dim1); 

array_index = (int)f_a[2];
HF_Get_Runtime_Array(array_index, &c, &dim); 


array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &d, &dim); 

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &cube, &dim); 

array_index = (int)f_a[5];
HF_Get_Runtime_Array(array_index, &x, &dim); 

for(i=0;i<3;i++){
	
	delta[i] =floor((double)(x[i])/(double)(d[i]+cube[i]));
	center[i] = (double)(d[i]+cube[i])*delta[i];
}


	  /*Best Solution : intersection with a cube : 
	  But due to appoximation, results gives "spots", i.e not nice at all*/
	/*
	xt=(x[0]-center[0])*(center[0]+cube[0]-x[0]);
	yt=(x[1]-center[1])*(center[1]+cube[1]-x[1]);
	zt=(x[2]-center[2])*(center[2]+cube[2]-x[2]);

	f=xt+yt-sqrt(xt*xt+yt*yt);
	   f=f+zt-sqrt(f*f+zt*zt);

	   if(f+1.0>0.0) {
			for(i=0;i<3;i++) c[i] = c_cube[i];
	   }
	   else {
			for(i=0;i<3;i++) c[i] = c_space[i];
	   }*/

/*Other Solution : not nice :(...Check the space coordinates manually..*/

lmin[0] = center[0];
lmin[1] = center[1];
lmin[2] = center[2];

lmax[0] = center[0]+cube[0];
lmax[1] = center[1]+cube[1];
lmax[2] = center[2]+cube[2];


if(dim1==dim2){
	if((x[0]>lmin[0])&&(x[0]<lmax[0])
		&&(x[1]>lmin[1])&&(x[1]<lmax[1])
		&&(x[2]>lmin[2])&&(x[2]<lmax[2])
		){
		for(i=0;i<dim1;i++) c[i] = c_cube[i];
	}
	else for(i=0;i<dim1;i++) c[i] = c_space[i];
}
return 1.0;

}



