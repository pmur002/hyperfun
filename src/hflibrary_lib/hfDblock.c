
/*
Call: hfBlock(x,vertex,dx,dy,dz);

Parameters:
 x - point coordinates array
 vertex - block vertex coordinates array
 dx,dy,dz - edge lengths along x,y,z

C-parameters:
        f_a[0]   corresponds to dz
        f_a[1]   corresponds to dy
        f_a[2]   corresponds to dx
        f_a[3]   corresponds to array vertex
        f_a[4]   corresponds to array x

note the maximal radius for the sardef intersection
is hard coded here, maybe the syntax of block should be modified
to control this parameter
*/

#include "general.h"
#include <math.h>
#include "sardf.h"

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

double hfDBlock(double* f_a, String_Array_T* str_param)
{
 double dx,dy,dz;
 double xt1,xt2;
 double yt1,yt2;
 double zt1,zt2;
 double xy,temp,xy2,temp2,block;
 double r,alpha;
 double* x;
 double* vertex;
 int array_index, dim;

 dz = f_a[0];
 dy = f_a[1];
 dx = f_a[2];

 array_index = (int)f_a[3];
 HF_Get_Runtime_Array(array_index, &vertex, &dim);

 array_index = (int)f_a[4];
 HF_Get_Runtime_Array(array_index, &x, &dim);

 if (dim != 3 || (dx<0.) || (dy<0.) || (dz<0.))
    return -1111111111111.0;

 r = 0.2;
 alpha=0.0;

 xt1 = x[0] - vertex[0];
 xt2 = vertex[0] + dx - x[0];
 yt1 = x[1] - vertex[1];
 yt2 = vertex[1] + dy - x[1];
 zt1 = x[2] - vertex[2];
 zt2 = vertex[2] + dz - x[2];

 xy = int_sardef_1(xt2,zt1,r,alpha);
 temp = int_sardef_1(xy,yt1,r,alpha);

 xy2 = int_sardef_1(xt1,zt2,r,alpha);
 temp2 = int_sardef_1(xy2,yt2,r,alpha);

 block = int_sardef_1(temp,temp2,r,alpha);

 return block;
}
