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

Primitive: Gardner's solid noise

Definition: Series(x)*Series(y)*Series(z)

Call: hfNoiseG(x,amp,freq,phase);

Parameters:
 x - point coordinates array 
 amp - amplitude 
 freq - frequency
 phase - phase for sin() in the series 

Test file: noiseg.hf

C-parameters:
        f_a[0]   corresponds to phase
        f_a[1]   corresponds to freq
        f_a[2]   corresponds to amp
        f_a[3]   corresponds to array x 

Code by: Pasko
Last revised: 15.11.99 
*/ 

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern const double EPS;

double hfNoiseG(double* f_a, String_Array_T* str_param)
{
/*
  -- Following is Gardner series, good parameters:
  -- low noise sphere (sx+sx2): amp = 2.5, freq = 0.7, phase = 1.4
*/
  double xt, yt, zt, a2x, a2y, a2z, sx, sy, sz, sx2, sy2, sz2, a1d;
  double Serx, Sery, Serz;
  double a1, a2, phase, SS; 

double* x;
int array_index, dim;

phase = f_a[0]; 
a2 = f_a[1];
a1 = f_a[2];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &x, &dim); 

if (dim != 3) 
   return -1111111111111.0;

xt=x[0];
yt=x[1];
zt=x[2];

        a2x=a2*xt;
        a2y=a2*yt;
        a2z=a2*zt;
        sx=sin(a2x);
        sy=sin(a2y);
        sz=sin(a2z);
        a1d=a1/1.17;
        sx2=a1d*sin(a2x/1.35+phase*sz);
        sy2=a1d*sin(a2y/1.35+phase*sx);
        sz2=a1d*sin(a2z/1.35+phase*sy);
        Serx=a1*sx+sx2;
        Sery=a1*sy+sy2;
        Serz=a1*sz+sz2;
        SS=Serx*Sery*Serz;

return SS;
}



