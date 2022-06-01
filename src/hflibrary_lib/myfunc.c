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
** HyperFun Library
**
** Primitive:
**
**     An object for test
**
** Definition:
**
**     S^2 - (x-x0)^2(y-y0)^2 - (y-y0)^2(z-z0)^2 - (z-z0)^2(x-x0)^2
**
** Call:
**
**     myFunc(x, center, s)
**
** Parameters:
**
**     x      - point coordinates array
**     center - center array
**     S      - radius of an object?
**
** C-parameters:
**
**     f_a[0] - corresponds to s
**     f_a[1] - corresponds to array center
**     f_a[2] - corresponds to array x
**
** Code by:
**
**     Yuichiro Goto
**
** Last revised:
**
**     2000/02/03
**  
*/

#include "general.h"
#include <math.h>

extern int HF_Get_Runtime_Array(int index, double **array, int *size);
extern void HF_Fill_Error(PN_T *p_pn, int er_index, Error_Info_T *EI, int ic);

extern const double EPS;

double
myFunc(double *f_a, String_Array_T *str_param)
{
  double xt, yt, zt;
  double *x, *center, S;
  int arrayIndex, dim;

  S = f_a[0];
 
  arrayIndex = (int) f_a[1];
  HF_Get_Runtime_Array(arrayIndex, &center, &dim); 

  arrayIndex = (int) f_a[2];
  HF_Get_Runtime_Array(arrayIndex, &x, &dim);

  if (dim != 3)
    return -1111111111111.0;

  xt = (x[0]-center[0]) * (x[0]-center[0]);
  yt = (x[1]-center[1]) * (x[1]-center[1]);
  zt = (x[2]-center[2]) * (x[2]-center[2]);

  return S*S - xt*yt - yt*zt - zt*xt;
}

#ifndef M_PI
#define M_PI   3.1415926535897932384626
#endif

// sawtooth
// syntax = t, n, a, f

double
hfSawtooth(double *f_a, String_Array_T *str_param)
{
	double f = f_a[0];
	double a = f_a[1];
	int n = (int) f_a[2];
	double t = f_a[3];
	int ni;
	double di;
	double res = 0.5;

	if (n <= 1)
		return 0.0;

	for (ni = 1; ni < n; ni++)
	{
		di = (double)ni;
		res -= sin(2*M_PI*di*f*t)/di;
	}
	res *= (2/(a*M_PI));
	return res;
}

