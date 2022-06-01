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

Description: Convert a RGB color to an HSV color
		

Definition: 

Call: hfA_HSVToRGB(input_RGB_Color,output_HSV_Color);

Parameters:
 input_RGB_Color -  array of 3 values containing an RGB color
 output_HSV_Color -  array of 3 values containing the resulting HSV color

Test file: hfA_rgb_hsv.hf

C-parameters:

        f_a[0]   corresponds to array output (HSV)
		f_a[1]   corresponds to array input  (RGB)
		

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

double LocalMIN(double a,double b){
	return (a<b)?a:b;
}

double LocalMAX(double a,double b){
	return (a>b)?a:b;
}


double hfA_RGBToHSV(double* f_a, String_Array_T* str_param)
{

	double *hsv;
	double *rgb;

	double h,s,v;
	double r,g,b;
    double themin,themax,delta;

	int array_index, dim;
array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &hsv, &dim); 



array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &rgb, &dim); 

r=rgb[0];
g=rgb[1];
b=rgb[2];


   themin = LocalMIN(r,LocalMIN(g,b));
   themax = LocalMAX(r,LocalMAX(g,b));
   delta = themax - themin;
   v = themax;
   s = 0;
   if (themax > 0)
      s = delta / themax;
   h = 0;
   if (delta > 0) {
      if (themax == r && themax != g)
         h += (g - b) / delta;
      if (themax == g && themax != b)
         h += (2 + (b - r) / delta);
      if (themax == b && themax != r)
         h += (4 + (r - g) / delta);
      h *= 60;
   }


    hsv[0] = h;
	hsv[1] = s;
	hsv[2] = v;


return 1.0;

}



 
