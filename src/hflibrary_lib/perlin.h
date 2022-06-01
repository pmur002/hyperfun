

//Perlin noise form the book
//Texturing and Modelling: A procedural approach
//David S. Ebert, F. Kenton Musgrave, Darwin Peachey, Ken Perlin and Steven Worley
//Academic Press Professional
//ISBN : 0-12-228730-4


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifndef PERLIN_H
#define PERLIN_H

static double noise1(double arg);
static double noise2(double vec[]);
static double noise3(double vec[]);

static double noise(double vec[], int len)
{
	switch (len) {
	case 0:
		return 0.;
	case 1:
		return noise1(vec[0]);
	case 2:
		return noise2(vec);
	default:
		return noise3(vec);
	}
}



/* noise functions over 1, 2, and 3 dimensions */

#define PERLIN_B 0x100
#define PERLIN_BM 0xff

#define PERLIN_N 0x1000
#define PERLIN_NP 12   /* 2^N */
#define PERLIN_NM 0xfff

static int p[PERLIN_B + PERLIN_B + 2];
static double g3[PERLIN_B + PERLIN_B + 2][3];
static double g2[PERLIN_B + PERLIN_B + 2][2];
static double g1[PERLIN_B + PERLIN_B + 2];
static int Pstart = 1;

static void init(void);

#define s_curve(t) ( t * t * (3. - 2. * t) )

#define lerp(t, a, b) ( a + t * (b - a) )

static void setup_perlin(int i,int *b0,int *b1,double *r0,double *r1, double *vec){
	double t = vec[i] + PERLIN_N;
	*b0 = ((int)t) & PERLIN_BM;
	*b1 = ((*b0)+1) & PERLIN_BM;
	*r0 = t - (int)t;
	*r1 = *r0 - 1.;}

static double noise1(double arg)
{
	int bx0, bx1;
	double rx0, rx1, sx, u, v, vec[1];

	vec[0] = arg;
	if (Pstart ) {
		Pstart= 0;
		init();
	}

	setup_perlin(0, &bx0,&bx1, &rx0,&rx1,vec);

	sx = s_curve(rx0);

	u = rx0 * g1[ p[ bx0 ] ];
	v = rx1 * g1[ p[ bx1 ] ];

	return lerp(sx, u, v);
}

static double noise2(double vec[2])
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, u, v;
	register i, j;

	if (Pstart ) {
		Pstart  = 0;
		init();
	}

	setup_perlin(0, &bx0,&bx1, &rx0,&rx1,vec);
	setup_perlin(1, &by0,&by1, &ry0,&ry1,vec);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = g2[ b00 ] ; u = at2(rx0,ry0);
	q = g2[ b10 ] ; v = at2(rx1,ry0);
	a = lerp(sx, u, v);

	q = g2[ b01 ] ; u = at2(rx0,ry1);
	q = g2[ b11 ] ; v = at2(rx1,ry1);
	b = lerp(sx, u, v);

	return lerp(sy, a, b);
}

static double noise3(double vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register i, j;

	if (Pstart ) {
		Pstart  = 0;
		init();
	}

	setup_perlin(0, &bx0,&bx1, &rx0,&rx1,vec);
	setup_perlin(1, &by0,&by1, &ry0,&ry1,vec);
	setup_perlin(2, &bz0,&bz1, &rz0,&rz1,vec);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

static void normalize2(double v[2])
{
	double s;

	s = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
}

static void normalize3(double v[3])
{
	double s;

	s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}

static void init(void)
{
	int i, j, k;
srand( (unsigned)time( NULL ) );

	for (i = 0 ; i < PERLIN_B ; i++) {
		p[i] = i;

		g1[i] = (double)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;

		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (double)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;
		normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (double)((rand() % (PERLIN_B + PERLIN_B)) - PERLIN_B) / PERLIN_B;
		normalize3(g3[i]);
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = rand() % PERLIN_B];
		p[j] = k;
	}

	for (i = 0 ; i < PERLIN_B + 2 ; i++) {
		p[PERLIN_B + i] = p[i];
		g1[PERLIN_B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[PERLIN_B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[PERLIN_B + i][j] = g3[i][j];
	}
}


#endif /* PERLIN_H */
