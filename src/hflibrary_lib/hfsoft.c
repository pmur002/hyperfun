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

Primitive: Soft object (Wyvill 1986)

Definition: Sum f(r)
            f(r) = 1 - 22r^2/9d^2 + 17r^4/9d^4 - 4r^6/9d^6

Call: hfSoft(x,x0,y0,z0,d,T);

Parameters:
 x - point coordinates
 x0,y0,z0 - arrays of blob centers
 d - array of d - radii of influence
 T - threshold value

Test file: soft.hpf

C-parameters:
        f_a[0]   corresponds to T threshold
        f_a[1]   corresponds to array d - radii of influence
        f_a[2]   corresponds to array z0 of -z coordinates of keypoints
        f_a[3]   corresponds to array y0 of -y coordinates of keypoints
        f_a[4]   corresponds to array x0 of -xcoordinates of keypoints
        f_a[5]   corresponds to array x of point coordinates

Code by: Pasko, Adzhiev
Last revised: 6.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

/*--------------------------------------------------------------*/
double hfSoft(double* f_a, String_Array_T* str_param)
{
   int array_index;
   int     num, dim, i;
   double T, soft, xt, yt, zt, r2, r4, d2, d4;
   double* d;
   double* z0;
   double* y0;
   double* x0;
   double* x;

   T = f_a[0];

   array_index = (int)f_a[1];
   HF_Get_Runtime_Array(array_index, &d, &num);

   array_index = (int)f_a[2];
   HF_Get_Runtime_Array(array_index, &z0, &num);

   array_index = (int)f_a[3];
   HF_Get_Runtime_Array(array_index, &y0, &num);

   array_index = (int)f_a[4];
   HF_Get_Runtime_Array(array_index, &x0, &num);

   array_index = (int)f_a[5];
   HF_Get_Runtime_Array(array_index, &x, &dim);

   if(num > 0)
      {
      soft = 0.;
      for(i=0; i<num; i++)
        {
         xt = x[0] - x0[i];
         yt = x[1] - y0[i];
         zt = x[2] - z0[i];
         r2 = xt*xt+yt*yt+zt*zt;
         r4=r2*r2;
         d2=d[i]*d[i];
         d4=d2*d2;
         soft =
         (sqrt(r2) <= d[i])? soft + 1-22.*r2/(9.*d2)+17.*r4/(9.*d4)-4.*r4*r2/(9.*d4*d2) : soft;
        }
      return (soft - T);
      }
   else return -1111111111111.0;
}

/*--------------------------------------------------------------*/
 int R_hfSoft(double* f_a,
           String_Array_T* str_param,
           Error_Info_T* error_info,
           PN_T* p_pn,
           int code_index)
{
   int array_index = (int)f_a[1];
   int     amount;
   double* p_array;

   if(HF_Get_Runtime_Array(array_index, &p_array, &amount))
      return 1;

   HF_Fill_Error(p_pn, Bad_Code, error_info, code_index);
   return 0; /* Error */
/* See above TEST function for example of array passing as an argument */
}

/*--------------------------------------------------------------*/

