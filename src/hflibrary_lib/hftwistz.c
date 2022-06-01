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

Operation: Twisting around z-axis

Definition: inverse mapping
            t = (z-z1)/(z2-z1)
            theta = (1-t)*theta1 + t*theta2
            x'=x*cos(theta)+y*sin(theta)
            y'=-x*sin(theta)+y*cos(theta)

Call: hfTwistZ(xt,z1,z2,theta1,theta2);

Parameters:
 xt - point coordinates array to be changed
 z1, z2 - end points of z-interval
 theta1, theta2 - rotation angles in radians for end points

Test file: twist.hpf

C-parameters:
	f_a[0]   corresponds to theta2
	f_a[1]   corresponds to theta1
	f_a[2]   corresponds to z2
	f_a[3]   corresponds to z1
	f_a[4]   corresponds to xt

Code by: Pasko, Adzhiev
Last revised: 7.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfTwistZ(double* f_a, String_Array_T* str_param)
{

double theta1,theta2,theta,z1,z2,ct,st,xr,yr,t;
double* xt;
int array_index, num;

theta2=f_a[0];
theta1=f_a[1];
z2=f_a[2];
z1=f_a[3];

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &xt, &num);

t = (xt[2]-z1)/(z2-z1);
theta = (1-t)*theta1 + t*theta2;
ct=cos(theta);
st=sin(theta);

xr=xt[0]*ct + xt[1]*st;
yr=-xt[0]*st + xt[1]*ct;

xt[0]=xr;
xt[1]=yr;

return 1.;
}

int R_hfTwistZ(double* f_a,
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
