#ifndef SARDF_H
#define SARDF_H

double uni_sardef_1(const double xt, const double yt, const double R, const double alpha);
double uni_sardef_2(double xt, double yt, double R1, double R2);
double uni_sardef_and_gradient_1(const double xt, const double yt, const double R, 
								 const double alpha, const double gxt[3], const double gyt[3], double grad[3]);
double uni_sardef_and_gradient_2(const double xt, const double yt, const double R1, const double R2,
								 const double gxt[3], const double gyt[3], double grad[3]);
double int_sardef_1 (double xt, double yt, double R, double alpha);
double int_sardef_2 (double xt, double yt, double R1, double R2);
double int_sardef_and_gradient_1 (const double xt, const double yt, const double R, 
								  const double alpha, const double gxt[3], const double gyt[3], double grad[3]);
double int_sardef_and_gradient_2 (const double xt, const double yt, const double R1, const double R2,
								  const double gxt[3], const double gyt[3], double grad[3]);

#endif
