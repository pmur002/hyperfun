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

Description: Convert a HSV color to an RGB color
		

Definition: 

Call: hfA_HSVToRGB(input_HSV_Color,output_RGB_Color);

Parameters:
 input_HSV_Color -  array of 3 values containing an HSV color
 output_RGB_Color -  array of 3 values containing the resulting RGB color

Test file: hfA_rgb_hsv.hf

C-parameters:

        f_a[0]   corresponds to array output (RGB)
		f_a[1]   corresponds to array input  (HSV)
		

Code by: Schmitt
Last revised: 08.01.2004
*/ 

#include "pattern.h"
#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

double hfA_HSVToRGB(double* f_a, String_Array_T* str_param)
{

	double *hsv;
	double *rgb;

	double h,s,v;
	double r,g,b;
	
	int array_index, dim;

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &hsv, &dim); 

array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &rgb, &dim); 


h = hsv[0];
s = hsv[1];
v = hsv[2];

   while (h < 0.0)
      h += 360.0;
   while (h > 360.0)
       h -= 360.0;

   if (h < 120.0) {
      r = (120.0 - h) / 60.0;
      g = h / 60.0;
      b = 0;
   } else if (h < 240.0) {
      r = 0;
      g = (240.0 - h) / 60.0;
      b = (h - 120.0) / 60.0;
   } else {
      r = (h - 240.0) / 60.0;
      g = 0;
      b = (360.0 - h) / 60.0;
   }
   r = (r<1)?r:1;
   g = (g<1)?g:1;
   b = (b<1)?b:1;

   rgb[0] = (1.0 - s + s * r) * v;
   rgb[1] = (1.0 - s + s * g) * v;
   rgb[2] = (1.0 - s + s * b) * v;



return 1.0;

}



 
