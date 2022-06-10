/*==============================================================================

 Copyright 2022 Paul Murrell

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
 * Call R to calculate value from an R function

  Call: hfR(x);

  Parameters:
  x - point coordinates array

  C-parameters:
        f_a[0]   corresponds to array x 

*/

#include <R.h>
#include <Rinternals.h>

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfR(double* f_a, String_Array_T* str_param)
{
    double* x;
    int array_index, dim;
    SEXP xyz, ans;
    double f;

    array_index = (int)f_a[0];
    HF_Get_Runtime_Array(array_index, &x, &dim);

    if (dim != 3) return -1111111111111.0;

    xyz = PROTECT(allocVector(REALSXP, 3));
    REAL(xyz)[0] = x[0];
    REAL(xyz)[1] = x[1];
    REAL(xyz)[2] = x[2];
    
    SEXP hfR = PROTECT(findFun(install("hfR"), R_GlobalEnv));
    SEXP R_fcall = PROTECT(lang2(hfR, xyz));
    ans = PROTECT(eval(R_fcall, R_GlobalEnv));
    
    if (!isReal(ans))
        error("Invalid return value");

    f = REAL(ans)[0];

    UNPROTECT(4);
    
    return f;
}
