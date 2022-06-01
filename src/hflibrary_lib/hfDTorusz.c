/*
distance to a torus with z axis

call: hfDTorusZ(x,center,R,r0);

parameters:
 x: point coordinates array
 center: center array
 R: radius of revolution
 r0: disk radius

C-parameters:
        f_a[0]   corresponds to r0
        f_a[1]   corresponds to R
        f_a[2]   corresponds to array center
        f_a[3]   corresponds to array x

Pierre-Alain Fayolle
10.25.2004
*/

#include "general.h"
#include <math.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

double hfDTorusZ(double* f_a, String_Array_T* str_param)
{
double r0,R,xx,y,z;
double dxy,dxyz;
double* x;
double* center;
int array_index, dim;

r0 = f_a[0];
R = f_a[1];

array_index = (int)f_a[2];
HF_Get_Runtime_Array(array_index, &center, &dim);

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &x, &dim);

if (dim != 3)
   return -1111111111111.0;

xx=x[0]-center[0];
y=x[1]-center[1];
z=x[2]-center[2];

dxy = sqrt(xx*xx + y*y);
dxy = dxy - R;
dxyz = sqrt(dxy*dxy + z*z);
dxyz = dxyz - r0;

return (-dxyz);
}
