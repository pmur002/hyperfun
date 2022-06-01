#ifndef DCGENERATOR_H
#define DCGENERATOR_H

#include "hfdc_comp.h"
#include "hfdc_quadmesh.h"
#include "hfpolymesh.h"
#include "hfinterpreter.h"

class DcGenerator{
public:
	DcGenerator();
	~DcGenerator();
	HFPolyMesh* generate();
	void setGridN(int x, int y, int z);
	void setBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
	void setFunc(HFInterpreter* func, double iso);
	void setParameters(std::vector<double>& params);
	void sampleValue();  
	void generateQuadMesh();
	void createHfMesh();

private:
	int gX, gY, gZ;
	double minX, maxX, minY, maxY, minZ, maxZ;
	double ***value;
	double h;
	double eps;
	double iso;
	int IMAX;
	QuadMesh* quadMesh;
	HFPolyMesh* hf_mesh;
	HFInterpreter* func;
	std::vector<double> p;
	double *pTmpS;

	double angle(const std::vector<double>& p0, const std::vector<double>& p1, const std::vector<double>& p2);
	double evalFunc(double x, double y, double z, double* s);
	void nablaFuncC(std::vector<double>& g, double x, double y, double z);
	double nablaFuncF(std::vector<double>& g, double x, double y, double z);
	double convergeToZero(std::vector<double>& p, double x1, double y1, double z1, double v1, 
		double x2, double y2, double z2, double v2);

	template <typename DST_T, typename SRC_T>
	inline void copyVector(DST_T* pN, const SRC_T* pSrcN) const {
		pN[0] = (DST_T)pSrcN[0];
		pN[1] = (DST_T)pSrcN[1];
		pN[2] = (DST_T)pSrcN[2];
	}
};

#endif
