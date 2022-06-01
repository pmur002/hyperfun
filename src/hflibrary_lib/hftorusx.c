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

Primitive: Torus with X-axis

Definition: r0^2-(x-x0)^2-(y-y0)^2-(z-z0)^2-R^2+2*R*sqrt((z-z0)^2+(y-y0)^2)

Call: hfTorusX(x,center,R,r0);

Parameters:
 x - point coordinates array
 center - center array
 R - radius of revolution
 r0 - disk radius

Test file: torus.hpf

C-parameters:
        f_a[0]   corresponds to r0
        f_a[1]   corresponds to R
        f_a[2]   corresponds to array center
        f_a[3]   corresponds to array x

Code by: Pasko, Adzhiev
Last revised: 4.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfTorusX(double* f_a, String_Array_T* str_param)
{
double r0,R,xt,yt,zt,z2,y2,f;
double* x;
double* center;
int array_index, dim;

r0 = f_a[0];
R = f_a[1];

array_index = (int)f_a[2];
HF_Get_Runtime_Array(array_index, &center, &dim);

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &x, &dim);

if (dim != 3)
   return -1111111111111.0;

xt=x[0]-center[0];
yt=x[1]-center[1];
zt=x[2]-center[2];
z2=zt*zt;
y2=yt*yt;
f = r0*r0-xt*xt-y2-z2-R*R+2*R*sqrt(z2+y2);

return f;
}
