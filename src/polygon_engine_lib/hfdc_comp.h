// OMP.h: COMP
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OMP_H__E943427C_792E_4677_BE22_28A628CF8161__INCLUDED_)
#define AFX_OMP_H__E943427C_792E_4677_BE22_28A628CF8161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

static double dmaxarg1,dmaxarg2;
#define DMAX(a,b) (dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ?\
        (dmaxarg1) : (dmaxarg2))

static double dminarg1,dminarg2;
#define DMIN(a,b) (dminarg1=(a),dminarg2=(b),(dminarg1) < (dminarg2) ?\
        (dminarg1) : (dminarg2))

static float maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))

static float minarg1,minarg2;
#define FMIN(a,b) (minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

static long lmaxarg1,lmaxarg2;
#define LMAX(a,b) (lmaxarg1=(a),lmaxarg2=(b),(lmaxarg1) > (lmaxarg2) ?\
        (lmaxarg1) : (lmaxarg2))

static long lminarg1,lminarg2;
#define LMIN(a,b) (lminarg1=(a),lminarg2=(b),(lminarg1) < (lminarg2) ?\
        (lminarg1) : (lminarg2))

static int imaxarg1,imaxarg2;
#define IMAX(a,b) (imaxarg1=(a),imaxarg2=(b),(imaxarg1) > (imaxarg2) ?\
        (imaxarg1) : (imaxarg2))

static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

class COMP  
{
public:
	static void svbksb(float **u, float w[], float **v, int m, int n, float b[], float x[]);
	static void svdcmp(float **a, int m, int n, float w[], float **v);
	COMP();
	virtual ~COMP();


	static inline double LENGTH(float v[3]){
		return sqrt((double)v[0]*(double)v[0] + (double)v[1]*(double)v[1] + (double)v[2]*(double)v[2]);
	}

	static inline double LENGTH(double v[3]){
		return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	}

	static inline void VEC(float v[3], float p1[3], float p2[3]){
		v[0] = p2[0]-p1[0];
		v[1] = p2[1]-p1[1];
		v[2] = p2[2]-p1[2];
	}

	static inline void VEC(double v[3], double p1[3], double p2[3]){
		v[0] = p2[0]-p1[0];
		v[1] = p2[1]-p1[1];
		v[2] = p2[2]-p1[2];
	}

	static inline void VEC(double v[3], double p1[3], float p2[3]){
		v[0] = (double)p2[0]-p1[0];
		v[1] = (double)p2[1]-p1[1];
		v[2] = (double)p2[2]-p1[2];
	}

	static inline void VEC(double v[3], float p1[3], double p2[3]){
		v[0] = p2[0]-(double)p1[0];
		v[1] = p2[1]-(double)p1[1];
		v[2] = p2[2]-(double)p1[2];
	}
	
	static inline void TIMES(float kv[3], float k, float v[3]){
		kv[0] = k*v[0];
		kv[1] = k*v[1];
		kv[2] = k*v[2];
	}

	static inline double DIST(float p1[3], float p2[3]){
		float v[3];
		VEC(v,p1,p2);
		return LENGTH(v);
	}

	static inline double DIST(double p1[3], double p2[3]){
		double v[3];
		VEC(v,p1,p2);
		return LENGTH(v);
	}

	static inline double DIST2(float p1[3], float p2[3]){
		float v[3];
		VEC(v,p1,p2);
		return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	}

	static inline double DIST2(double p1[3], double p2[3]){
		double v[3];
		VEC(v,p1,p2);
		return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	}

	static inline double DIST2(double p1[3], float p2[3]){
		double v[3];
		VEC(v,p1,p2);
		return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	}

	static inline double DOT(float v1[3], float v2[3]){
		return (double)v1[0]*(double)v2[0] + (double)v1[1]*(double)v2[1] + (double)v1[2]*(double)v2[2];
	}

	static inline double DOT(double v1[3], float v2[3]){
		return v1[0]*(double)v2[0] + v1[1]*(double)v2[1] + v1[2]*(double)v2[2];
	}

	static inline double DOT(float v1[3], double v2[3]){
		return (double)v1[0]*v2[0] + (double)v1[1]*v2[1] + (double)v1[2]*v2[2];
	}

	static inline double DOT(double v1[3], double v2[3]){
		return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	}

	static inline void CROSS(float n[3], float v1[3], float v2[3]){
		n[0] = v1[1]*v2[2] - v1[2]*v2[1];
		n[1] = v1[2]*v2[0] - v1[0]*v2[2];
		n[2] = v1[0]*v2[1] - v1[1]*v2[0];
	}

	static inline void CROSS(double n[3], double v1[3], double v2[3]){
		n[0] = v1[1]*v2[2] - v1[2]*v2[1];
		n[1] = v1[2]*v2[0] - v1[0]*v2[2];
		n[2] = v1[0]*v2[1] - v1[1]*v2[0];
	}

	static inline void CROSS(double n[3], float v1[3], float v2[3]){
		n[0] = (double)v1[1]*(double)v2[2] - (double)v1[2]*(double)v2[1];
		n[1] = (double)v1[2]*(double)v2[0] - (double)v1[0]*(double)v2[2];
		n[2] = (double)v1[0]*(double)v2[1] - (double)v1[1]*(double)v2[0];
	}

	static inline void CROSS(double n[3], double v1[3], float v2[3]){
		n[0] = v1[1]*(double)v2[2] - v1[2]*(double)v2[1];
		n[1] = v1[2]*(double)v2[0] - v1[0]*(double)v2[2];
		n[2] = v1[0]*(double)v2[1] - v1[1]*(double)v2[0];
	}

	static inline void CROSS(double n[3], float v1[3], double v2[3]){
		n[0] = (double)v1[1]*v2[2] - (double)v1[2]*v2[1];
		n[1] = (double)v1[2]*v2[0] - (double)v1[0]*v2[2];
		n[2] = (double)v1[0]*v2[1] - (double)v1[1]*v2[0];
	}

	static inline void CROSS(float n[3], double v1[3], double v2[3]){
		n[0] = (float)(v1[1]*v2[2] - v1[2]*v2[1]);
		n[1] = (float)(v1[2]*v2[0] - v1[0]*v2[2]);
		n[2] = (float)(v1[0]*v2[1] - v1[1]*v2[0]);
	}

	static inline void CROSS(float n[3], double v1[3], float v2[3]){
		n[0] = (float)(v1[1]*v2[2] - v1[2]*v2[1]);
		n[1] = (float)(v1[2]*v2[0] - v1[0]*v2[2]);
		n[2] = (float)(v1[0]*v2[1] - v1[1]*v2[0]);
	}

	static inline void CROSS(float n[3], float v1[3], double v2[3]){
		n[0] = (float)(v1[1]*v2[2] - v1[2]*v2[1]);
		n[1] = (float)(v1[2]*v2[0] - v1[0]*v2[2]);
		n[2] = (float)(v1[0]*v2[1] - v1[1]*v2[0]);
	}

	static inline double AREA(float p1[3], float p2[3], float p3[3])
	{
		double n[3];
		float v1[3], v2[3];
		VEC(v1, p2, p1);
		VEC(v2, p3, p1);
		CROSS(n, v1, v2);

		return 0.5*LENGTH(n);
	}

	static inline double AREA(double p1[3], double p2[3], double p3[3])
	{
		double n[3];
		double v1[3], v2[3];
		VEC(v1, p2, p1);
		VEC(v2, p3, p1);
		CROSS(n, v1, v2);

		return 0.5*LENGTH(n);
	}

	static inline double AREA(float p1[3], float p2[3], double p3[3])
	{
		double n[3];
		float v1[3];
		double v2[3];
		VEC(v1, p2, p1);
		VEC(v2, p3, p1);
		CROSS(n, v1, v2);

		return 0.5*LENGTH(n);
	}

		static inline void MAT_VEC(double y[3], double A[3][3], double x[3]){
		y[0] = A[0][0]*x[0] + A[0][1]*x[1] + A[0][2]*x[2];
		y[1] = A[1][0]*x[0] + A[1][1]*x[1] + A[1][2]*x[2];
		y[2] = A[2][0]*x[0] + A[2][1]*x[1] + A[2][2]*x[2];
	}

	static inline void MAT_TIMES(double C[3][3], double A[3][3], double B[3][3]){
		C[0][0] = A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0];
		C[0][1] = A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1];
		C[0][2] = A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2];

		C[1][0] = A[1][0]*B[0][0] + A[1][1]*B[1][0] + A[1][2]*B[2][0];
		C[1][1] = A[1][0]*B[0][1] + A[1][1]*B[1][1] + A[1][2]*B[2][1];
		C[1][2] = A[1][0]*B[0][2] + A[1][1]*B[1][2] + A[1][2]*B[2][2];

		C[2][0] = A[2][0]*B[0][0] + A[2][1]*B[1][0] + A[2][2]*B[2][0];
		C[2][1] = A[2][0]*B[0][1] + A[2][1]*B[1][1] + A[2][2]*B[2][1];
		C[2][2] = A[2][0]*B[0][2] + A[2][1]*B[1][2] + A[2][2]*B[2][2];
	}

	static inline void GENERATE_MAT(double R[3][3], double angle, double v[3]){
		double l = sqrt(v[0]*v[0] + v[1]*v[1]);
		double a;
		if(l == 0)
			a = 0;
		else
			a = acos(v[1]/l);
		if(v[0] < 0)
			a = -a;
		double b = acos(v[2]/COMP::LENGTH(v));

		double Rz[3][3], Rx[3][3], Rt[3][3]; 
		Rz[0][0] = Rz[1][1] = (float)cos(a);
		Rz[0][1] = sin(a);
		Rz[1][0] = -Rz[0][1];
		Rz[2][0] = Rz[2][1] = Rz[0][2] = Rz[1][2] = 0;
		Rz[2][2] = 1;

		Rt[0][0] = Rt[1][1] = cos(angle);
		Rt[0][1] = -sin(angle);
		Rt[1][0] = -Rt[0][1];
		Rt[2][0] = Rt[2][1] = Rt[0][2] = Rt[1][2] = 0;
		Rt[2][2] = 1;

		Rx[0][0] = 1;
		Rx[0][1] = Rx[0][2] = Rx[1][0] = Rx[2][0] = 0;
		Rx[1][1] = Rx[2][2] = cos(b);
		Rx[1][2] = sin(b);
		Rx[2][1] = -Rx[1][2];

		double tmp[3][3];
		MAT_TIMES(tmp, Rz, Rx);
		MAT_TIMES(R, tmp, Rt);
		Rz[1][0] *= -1;
		Rz[0][1] *= -1;
		Rx[1][2] *= -1;
		Rx[2][1] *= -1;
		MAT_TIMES(tmp, R, Rx);
		MAT_TIMES(R, tmp, Rz);

		MAT_TIMES(tmp, Rx, Rz);
		double x[3];
		MAT_VEC(x, tmp, v);
	}

	static inline void GENERATE_MAT(double R[3][3], double angle, float v1[3]){
		double v[3];
		v[0] = v1[0];
		v[1] = v1[1];
		v[2] = v1[2];

		double l = sqrt(v[0]*v[0] + v[1]*v[1]);
		double a;
		if(l == 0)
			a = 0;
		else
			a = acos(v[1]/l);
		if(v[0] < 0)
			a = -a;
		double b = acos(v[2]/COMP::LENGTH(v));

		double Rz[3][3], Rx[3][3], Rt[3][3]; 
		Rz[0][0] = Rz[1][1] = (float)cos(a);
		Rz[0][1] = sin(a);
		Rz[1][0] = -Rz[0][1];
		Rz[2][0] = Rz[2][1] = Rz[0][2] = Rz[1][2] = 0;
		Rz[2][2] = 1;

		Rt[0][0] = Rt[1][1] = cos(angle);
		Rt[0][1] = -sin(angle);
		Rt[1][0] = -Rt[0][1];
		Rt[2][0] = Rt[2][1] = Rt[0][2] = Rt[1][2] = 0;
		Rt[2][2] = 1;

		Rx[0][0] = 1;
		Rx[0][1] = Rx[0][2] = Rx[1][0] = Rx[2][0] = 0;
		Rx[1][1] = Rx[2][2] = cos(b);
		Rx[1][2] = sin(b);
		Rx[2][1] = -Rx[1][2];

		double tmp[3][3];
		MAT_TIMES(tmp, Rz, Rx);
		MAT_TIMES(R, tmp, Rt);
		Rz[1][0] *= -1;
		Rz[0][1] *= -1;
		Rx[1][2] *= -1;
		Rx[2][1] *= -1;
		MAT_TIMES(tmp, R, Rx);
		MAT_TIMES(R, tmp, Rz);

		MAT_TIMES(tmp, Rx, Rz);
		double x[3];
		MAT_VEC(x, tmp, v);
	}

	
	static inline void MATRIX(double A[10], double n[3], double d){
		A[0] = n[0]*n[0];
		A[1] = n[0]*n[1];
		A[2] = n[0]*n[2];
		A[3] = n[1]*n[1];
		A[4] = n[1]*n[2];
		A[5] = n[2]*n[2];
		A[6] = d*n[0];
		A[7] = d*n[1];
		A[8] = d*n[2];
		A[9] = d*d;
	}

	static inline void MAT_TIMES(double A[10], double k){
		A[0] *= k;
		A[1] *= k;
		A[2] *= k;
		A[3] *= k;
		A[4] *= k;
		A[5] *= k;
		A[6] *= k;
		A[7] *= k;
		A[8] *= k;
		A[9] *= k;
	}

	static inline void MAT_SUM(double B[6], double A[6]){
		B[0] += A[0];
		B[1] += A[1];
		B[2] += A[2];
		B[3] += A[3];
		B[4] += A[4];
		B[5] += A[5];
		B[6] += A[6];
		B[7] += A[7];
		B[8] += A[8];
		B[9] += A[9];
	}

	static inline void MAT_BY_VEC(double v[3], double A[10], double b[3]){
		v[0] = A[0]*b[0] + A[1]*b[1] + A[2]*b[2];
		v[1] = A[1]*b[0] + A[3]*b[1] + A[4]*b[2];
		v[2] = A[2]*b[0] + A[4]*b[1] + A[5]*b[2];
	}

	static inline void MAT_INIT(double A[10]){
		A[0] = A[1] = A[2] = A[3] = A[4] = A[5] = A[6] = A[7] = A[8] = A[9] = 0;
	}

	static inline void MAT_PLUS(double C[10], double A[10], double B[10]){
		C[0] = A[0] + B[0];
		C[1] = A[1] + B[1];
		C[2] = A[2] + B[2];
		C[3] = A[3] + B[3];
		C[4] = A[4] + B[4];
		C[5] = A[5] + B[5];
		C[6] = A[6] + B[6];
		C[7] = A[7] + B[7];
		C[8] = A[8] + B[8];
		C[9] = A[9] + B[9];
	}

	static inline void MAT_COPY(double A[10], double B[10]){
		A[0] = B[0];
		A[1] = B[1];
		A[2] = B[2];
		A[3] = B[3];
		A[4] = B[4];
		A[5] = B[5];
		A[6] = B[6];
		A[7] = B[7];
		A[8] = B[8];
		A[9] = B[9];
	}

	static inline double Q_ERR(double A[10], float v[3]){
		return v[0]*(A[0]*v[0] + A[1]*v[1] + A[2]*v[2]) +
			   v[1]*(A[1]*v[0] + A[3]*v[1] + A[4]*v[2]) +
			   v[2]*(A[2]*v[0] + A[4]*v[1] + A[5]*v[2]) +
			   2.0*(A[6]*v[0] + A[7]*v[1] + A[8]*v[2]) + 
			   A[9];
	}

	static inline double Q_ERR(double A[10], double v[3]){
		return v[0]*(A[0]*v[0] + A[1]*v[1] + A[2]*v[2]) +
			   v[1]*(A[1]*v[0] + A[3]*v[1] + A[4]*v[2]) +
			   v[2]*(A[2]*v[0] + A[4]*v[1] + A[5]*v[2]) +
			   2.0*(A[6]*v[0] + A[7]*v[1] + A[8]*v[2]) + 
			   A[9];
	}

	#define EPSILON2 0.000001
	static inline bool INVERSE(double B[10], double A[10]){
		double d = DET(A);
		if(fabs(d) < EPSILON2)
			return false;
		B[0] = (A[3]*A[5] - A[4]*A[4])/d;
		B[1] = (A[2]*A[4] - A[1]*A[5])/d;
		B[2] = (A[1]*A[4] - A[2]*A[3])/d;
		B[3] = (A[0]*A[5] - A[2]*A[2])/d;
		B[4] = (A[1]*A[2] - A[0]*A[4])/d;
		B[5] = (A[0]*A[3] - A[1]*A[1])/d;
		return true;
	}

	static inline double DET(double A[10]){
		return A[0]*A[3]*A[5] + 2.0*A[1]*A[4]*A[2] 
			-A[2]*A[2]*A[3] - A[1]*A[1]*A[5] - A[4]*A[4]*A[0];
	}

	static inline void upheap(float *a, int N, int k, int *p, int *q){
		int v;
		v = p[k];
		while(k > 1 && a[p[k/2]] >= a[v]){
			p[k] = p[k/2]; q[p[k/2]] = k; k = k/2;
		}
		p[k] = v; q[v] = k;
	}

	static inline void downheap(float *a, int N, int k, int *p, int *q){
		int j, v;
		v = p[k];
		while(k <= N/2){
			j = k+k;
			if(j < N && a[p[j]] > a[p[j+1]]) j++;
			if(a[v] <= a[p[j]]) break;
			p[k] = p[j]; q[p[j]] = k; k = j;
		}
		p[k] = v; q[v] = k;
	}

	static inline float SHAPE(float p1[3], float p2[3], float p3[3]){
		double l1 = (p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])+(p1[2]-p2[2])*(p1[2]-p2[2]);
		double l2 = (p2[0]-p3[0])*(p2[0]-p3[0])+(p2[1]-p3[1])*(p2[1]-p3[1])+(p2[2]-p3[2])*(p2[2]-p3[2]);
		double l3 = (p3[0]-p1[0])*(p3[0]-p1[0])+(p3[1]-p1[1])*(p3[1]-p1[1])+(p3[2]-p1[2])*(p3[2]-p1[2]);
		double area = AREA(p1, p2, p3);
		return (float)(4.0*sqrt(3.0)*area/(l1 + l2 + l3));
	}
private:
	static float pythag(float a, float b);
	
	
static void nrerror(const char* error_text)
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	//exit(1);
}

static float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;

	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl+NR_END;
}

unsigned char *cvector(long nl, long nh)
/* allocate an unsigned char vector with subscript range v[nl..nh] */
{
	unsigned char *v;

	v=(unsigned char *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned char)));
	if (!v) nrerror("allocation failure in cvector()");
	return v-nl+NR_END;
}

unsigned long *lvector(long nl, long nh)
/* allocate an unsigned long vector with subscript range v[nl..nh] */
{
	unsigned long *v;

	v=(unsigned long *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(long)));
	if (!v) nrerror("allocation failure in lvector()");
	return v-nl+NR_END;
}

double *dvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

double **dmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

int **imatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	int **m;

	/* allocate pointers to rows */
	m=(int **) malloc((size_t)((nrow+NR_END)*sizeof(int*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;


	/* allocate rows and set pointers to them */
	m[nrl]=(int *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(int)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **submatrix(float **a, long oldrl, long oldrh, long oldcl, long oldch,
	long newrl, long newcl)
/* point a submatrix [newrl..][newcl..] to a[oldrl..oldrh][oldcl..oldch] */
{
	long i,j,nrow=oldrh-oldrl+1,ncol=oldcl-newcl;
	float **m;

	/* allocate array of pointers to rows */
	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure in submatrix()");
	m += NR_END;
	m -= newrl;

	/* set pointers to rows */
	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **convert_matrix(float *a, long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix m[nrl..nrh][ncl..nch] that points to the matrix
declared in the standard C manner as a[nrow][ncol], where nrow=nrh-nrl+1
and ncol=nch-ncl+1. The routine should be called with the address
&a[0][0] as the first argument. */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure in convert_matrix()");
	m += NR_END;
	m -= nrl;

	/* set pointers to rows */
	m[nrl]=a-ncl;
	for(i=1,j=nrl+1;i<nrow;i++,j++) m[j]=m[j-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}

float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a float 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	float ***t;

	/* allocate pointers to pointers to rows */
	t=(float ***) malloc((size_t)((nrow+NR_END)*sizeof(float**)));
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(float **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(float *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}

static void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_cvector(unsigned char *v, long nl, long nh)
/* free an unsigned char vector allocated with cvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_lvector(unsigned long *v, long nl, long nh)
/* free an unsigned long vector allocated with lvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_dvector(double *v, long nl, long nh)
/* free a double vector allocated with dvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by dmatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_imatrix(int **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_submatrix(float **b, long nrl, long nrh, long ncl, long nch)
/* free a submatrix allocated by submatrix() */
{
	free((FREE_ARG) (b+nrl-NR_END));
}

void free_convert_matrix(float **b, long nrl, long nrh, long ncl, long nch)
/* free a matrix allocated by convert_matrix() */
{
	free((FREE_ARG) (b+nrl-NR_END));
}

void free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh)
/* free a float f3tensor allocated by f3tensor() */
{
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}
/* (C) Copr. 1986-92 Numerical Recipes Software 9z!+!1(t+%. */

};

#endif // !defined(AFX_OMP_H__E943427C_792E_4677_BE22_28A628CF8161__INCLUDED_)
