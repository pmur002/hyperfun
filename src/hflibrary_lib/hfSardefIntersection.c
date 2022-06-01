/*
call: hfSardefInt1(f1,f2,r,alpha);
hfSardefInt2(f1,f2,r1,r2);
hfSardefIntG1(f1,f2,r,alpha,g1,g2,g);
hfSardefIntG2(f1,f2,r1,r2,g1,g2,g);
*/

#include <math.h>
#include <stdio.h>
#include "general.h"
#include "sardf.h"

int HF_Get_Runtime_Array(int index, double **array, int *size);
void HF_Fill_Error(PN_T *p_pn, int er_index, Error_Info_T *EI, int ic);


double hfSardefInt1(double *f_a, String_Array_T *str_param)
{
	double f1,f2,r,alpha;
	double d;

	alpha=f_a[0];
	r=f_a[1];
	f2=f_a[2];
	f1=f_a[3];

	d = int_sardef_1(f1,f2,r,alpha);
	return d;
}

double hfSardefInt2(double *f_a, String_Array_T *str_param)
{
	double f1,f2,r1,r2;
	double d;

	r2=f_a[0];
	r1=f_a[1];
	f2=f_a[2];
	f1=f_a[3];

	d = int_sardef_2(f1,f2,r1,r2);
	return d;
}

double hfSardefIntG1(double *f_a, String_Array_T *str_param)
{
	int array_index, dim;
	double f1,f2,r,alpha;
	double *gf1, *gf2, *g;
	double d;

	array_index=(int)f_a[0];
	HF_Get_Runtime_Array(array_index,&g,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	array_index=(int)f_a[1];
	HF_Get_Runtime_Array(array_index,&gf2,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	array_index=(int)f_a[2];
	HF_Get_Runtime_Array(array_index,&gf1,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	alpha=f_a[3];
	r=f_a[4];
	f2=f_a[5];
	f1=f_a[6];

	d = int_sardef_and_gradient_1(f1,f2,r,alpha,gf1,gf2,g);
	return d;
}

double hfSardefIntG2(double *f_a, String_Array_T *str_param)
{
	int array_index, dim;
	double f1,f2,r1,r2;
	double *gf1, *gf2, *g;
	double d;

	array_index=(int)f_a[0];
	HF_Get_Runtime_Array(array_index,&g,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	array_index=(int)f_a[1];
	HF_Get_Runtime_Array(array_index,&gf2,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	array_index=(int)f_a[2];
	HF_Get_Runtime_Array(array_index,&gf1,&dim);
	if(dim!=3)
		printf("wrong size of array\n");


	r2=f_a[3];
	r1=f_a[4];
	f2=f_a[5];
	f1=f_a[6];

	d = int_sardef_and_gradient_1(f1,f2,r1,r2,gf1,gf2,g);
	return d;
}

double hfDInt(double *f_a, String_Array_T *str_param)
{
	double f1,f2,r,alpha;
	double d;

	alpha=f_a[0];
	r=f_a[1];
	f2=f_a[2];
	f1=f_a[3];

	d = int_sardef_1(f1,f2,r,alpha);
	return d;
}