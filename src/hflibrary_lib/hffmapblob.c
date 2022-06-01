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

Operation: Functional Mapping Blobby type (pinching/pricking)

Definition: F - Sum F[i]*(1+(r/a)^2)

Call: hfFMapBlob(x,fobj,x0,y0,z0,fobj0,sigma);

Parameters:
 x - point coordinates
 fobj - function value in x[] point of the object to be deformed
 x0,y0,z0 - arrays of blob centers
 fobj0 - array of the object function values in blob centers
 sigma - array of blob width parameter

Test file: fmapblob.hpf

C-parameters:
        f_a[0]   corresponds to array sigma
        f_a[1]   corresponds to array fobj0
        f_a[2]   corresponds to array z0 of -z coordinates of keypoints
        f_a[3]   corresponds to array y0 of -y coordinates of keypoints
        f_a[4]   corresponds to array x0 of -x coordinates of keypoints
        f_a[5]   corresponds to fobj
        f_a[6]   corresponds to array x of point coordinates
Code by: Pasko, Adzhiev
Last revised: 21.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);
extern double EPS;

/*--------------------------------------------------------------*/
double hfFMapBlob(double* f_a, String_Array_T* str_param)
{
   int array_index;
   int     num, dim, i;
   double xt, yt, zt, fobj, d2;
   double* sigma;
   double* fobj0;
   double* z0;
   double* y0;
   double* x0;
   double* x;

   array_index = (int)f_a[0];
   HF_Get_Runtime_Array(array_index, &sigma, &num);

   array_index = (int)f_a[1];
   HF_Get_Runtime_Array(array_index, &fobj0, &num);

   array_index = (int)f_a[2];
   HF_Get_Runtime_Array(array_index, &z0, &num);

   array_index = (int)f_a[3];
   HF_Get_Runtime_Array(array_index, &y0, &num);

   array_index = (int)f_a[4];
   HF_Get_Runtime_Array(array_index, &x0, &num);

   fobj = f_a[5];

   array_index = (int)f_a[6];
   HF_Get_Runtime_Array(array_index, &x, &dim);

/*
printf("x= %f y=%f z=%f \n",x[0],x[1],x[2]);
*/

   if(num > 0)
      {
      for(i=0; i<num; i++)
        {
         xt = x[0] - x0[i];
         yt = x[1] - y0[i];
         zt = x[2] - z0[i];
           if(sigma[i]>EPS){
             d2 = (xt*xt+yt*yt+zt*zt)/(sigma[i]*sigma[i]);
             fobj = fobj - fobj0[i]/(1+d2);                /*exp(-a[i]*(xt*xt+yt*yt+zt*zt));*/
           }
        }
      return fobj;
      }
   else return -1111111111111.0;

return -1.;
}
