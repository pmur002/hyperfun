#ifndef _HF_BSP_H_INCLUDED_
#define _HF_BSP_H_INCLUDED_

struct BSPTree;

#define polygon_coincident	0
#define polygon_front		1
#define polygon_back		2
#define polygon_spanning	3
#define polygon_undefined	4

#define sto_minmax			10
#define sto_r				11
#define sto_sardf			12

typedef struct 
{
	double a;
	double b;
	double c;
	double d;
}BSPPlane;

typedef struct 
{
	double x;
	double y;
	double z;
}vec3d;

typedef struct BSPTree
{
	BSPPlane* plPartition;
	int firstPolygon;
	struct BSPTree *pFront;
	struct BSPTree *pBack;
}BSPTree;

typedef struct
{
	vec3d* m_points;
	int m_pointsSize;
	BSPPlane* m_plBase;
	int m_nPlaneID;
	int m_bSplit;
}SoupMeshData;

void	sbspCleanBSP(BSPTree* pTree);
double	sbspGetValue(double x, double y, double z, BSPTree* pTree);
BSPTree* sbspBuildBSP(int nVertexNum, double* pVertexData, int nFacesNum, int* pFaceData);
void	sbspPrintTree(BSPTree* tree, int nStack);

#endif //_HF_BSP_H_INCLUDED_