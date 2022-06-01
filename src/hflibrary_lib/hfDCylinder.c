#include "general.h"
#include <math.h>

/*
 distance to a general cylinder
 from mathworld: http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html

 x0: input point
 x1: center
 x2: center + u
 x21: x2 - x1
 x10: x1 - x0
 cp: x21 * x10 (cross product of x21 and x10)
*/

int HF_Get_Runtime_Array(int index, double **array, int *size);
void HF_Fill_Error(PN_T *p_pn, int er_index, Error_Info_T *EI, int ic);

double hfDCylinder(double *f_a, String_Array_T *str_param){
 double R, d1, d2, d, f;
 double *x, *center, *u;
 double x0[3], x1[3], x2[3];
 double x21[3], x10[3], cp[3];

 int array_index, dim;

 R=f_a[0];
 
 array_index = (int)f_a[1];
 HF_Get_Runtime_Array(array_index, &u, &dim);

 array_index = (int)f_a[2];
 HF_Get_Runtime_Array(array_index, &center, &dim);

 array_index = (int)f_a[3];
 HF_Get_Runtime_Array(array_index, &x, &dim);
 
 if (dim != 3) return -11111111111.1;
 
 x0[0]=x[0],x0[1]=x[1],x0[2]=x[2];
 x1[0]=center[0],x1[1]=center[1],x1[2]=center[2];
 x2[0]=center[0]+u[0],x2[1]=center[1]+u[1],x2[2]=center[2]+u[2];

 
 x21[0]=x2[0]-x1[0];
 x21[1]=x2[1]-x1[1];
 x21[2]=x2[2]-x1[2];

 x10[0]=x1[0]-x0[0];
 x10[1]=x1[1]-x0[1];
 x10[2]=x1[2]-x0[2];

 cp[0]=x21[1]*x10[2] - x10[1]*x21[2];cp[1]=x21[2]*x10[0]-x21[0]*x10[2];
 cp [2]=x21[0]*x10[1] - x21[1]*x10[0];

 d1 = cp[0]*cp[0] + cp[1]*cp[1] + cp[2]*cp[2];
 d2 = x21[0]*x21[0] + x21[1]*x21[1] + x21[2]*x21[2];
 
 d=d1/d2;

 d = sqrt(d);

 f = R - d;
 return f;
}

