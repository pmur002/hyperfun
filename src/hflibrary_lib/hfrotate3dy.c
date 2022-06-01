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
Core HyperFun Library

Operation: Rotation in 3D around y-axis

Definition: inverse mapping
            z'=z*cos(theta)+x*sin(theta)
            x'=-z*sin(theta)+z*cos(theta)

Call: hfRotate3DY(xt,theta);

Parameters:
 xt - point coordinates array to be changed
 theta - rotation angle in radians

Test file: rotate.hpf

C-parameters:
	f_a[0]   corresponds to theta
	f_a[1]   corresponds to xt

Code by: Pasko, Adzhiev
Last revised: 10.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfRotate3DY(double* f_a, String_Array_T* str_param)
{

double theta,ct,st,zr,xr;
double* xt;
int array_index, num;

theta=f_a[0];

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &xt, &num);

ct=cos(theta);
st=sin(theta);

zr=xt[2]*ct + xt[0]*st;
xr=-xt[2]*st + xt[0]*ct;

xt[0]=xr;
xt[2]=zr;

return 1.;
}

int R_hfRotate3DY(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index)
{

/*	if((fabs(f_a[0]) < EPS) || (fabs(f_a[1]) < EPS) || (fabs(f_a[2]) < EPS))
	{
           HF_Fill_Error(p_pn, Blend_2_3_Error, error_info, code_index);
           return 0;
	}
*/
	return 1;
}




