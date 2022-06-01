#include "general.h"
#include <math.h>

int HF_Get_Runtime_Array(int index, double **array, int *size);
void HF_Fill_Error(PN_T *p_pn, int er_index, Error_Info_T *EI,int ic);

/*
 call hfBlendUniSardef(f1,f2,r)
 */

double hfDBlendUni(double *f_a, String_Array_T *str_param)
{
 double f1,f2,r;
 double xt,yt,d;
 
 r = f_a[0];
 f2 = f_a[1];
 f1 = f_a[2];

 xt=f1,yt=f2,d=0;

 if ((yt<=xt+r)&&(yt>=xt-r)){
  double aa,b,c,Ds;

  aa = 2.0;
  b = -2.0*xt - 4.0*r - 2.0*yt;
  c = xt*xt + yt*yt + r*r + 2.0*xt*r + 2.0*yt*r;

 Ds = b*b - 4.0*aa*c;
 if (Ds>=0)
   d = (-b - sqrt(Ds))/(2.0*aa);
 }
 else {
  if (yt < xt - r)
    d = xt;
  if (yt > xt + r)
    d = yt;
 }

 return d;
}