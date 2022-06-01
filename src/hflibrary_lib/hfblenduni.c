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

Operation: Blending Union

Definition: R_uni(f1,f2) + disp(f1,f2,a0,a1,a2)
            disp(f1,f2,a0,a1,a2)=a0/(1+(f1/a1)^2(f2/a2)^2)

Call: hfBlendUni(f1,f2,a0,a1,a2);

Parameters:
 f1,f2  - two objects
 a0,a1,a2 - blend parameters

Test file: blend.hpf

C-parameters:
        f_a[0]   corresponds to a2
        f_a[1]   corresponds to a1
        f_a[2]   corresponds to a0
        f_a[3]   corresponds to object f2
        f_a[4]   corresponds to object f1

Code by: Pasko, Osipov, Adzhiev
Last revised: 7.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfBlendUni(double* f_a, String_Array_T* str_param)
{
	double d, f;

	d = f_a[2]/(1+(f_a[4]/f_a[1])*(f_a[4]/f_a[1]) + (f_a[3]/f_a[0])*(f_a[3]/f_a[0]));

	f = f_a[3] + f_a[4] + sqrt(f_a[4]*f_a[4] + f_a[3]*f_a[3]) + d;
	return f;
}

int R_hfBlendUni(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index)
{

	if((fabs(f_a[0]) < EPS) || (fabs(f_a[1]) < EPS))
	{
           HF_Fill_Error(p_pn, Blend_2_3_Error, error_info, code_index);
           return 0; /* Error */
	}
	return 1;
}




