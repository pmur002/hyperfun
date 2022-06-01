/*
 distance to a cone
 call : hfDConeX(x,center,R)
*/

#include "general.h"
#include <math.h>

int HF_Get_Runtime_Array(int index, double **array, int *size);
void HF_Fill_Error(PN_T *p_pn, int er_index, Error_Info_T *EI, int ic);

double hfDConeZ(double *f_a, String_Array_T *str_param){
 double R,t,xx,y,z,distance;
 double *x, *center;
 int array_index, dim;

 R = f_a[0];

 array_index = (int)f_a[1];
 HF_Get_Runtime_Array(array_index, &center, &dim);

 array_index = (int)f_a[2];
 HF_Get_Runtime_Array(array_index, &x, &dim);

 if (dim!=3)
  return -11111111111.0;

 t = atan(R);
 xx = x[0]-center[0];
 y = x[1] - center[1];
 z = x[2] - center[2];

 distance = sqrt(xx*xx+y*y)*cos(t) - fabs(z)*sin(t);
 return (-distance);
}
