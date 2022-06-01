#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "hfbsp.h"

#include "sardf.h"


/*
#define SBSP_NAIVE
*/

#define SBSP_NORMALIZEPLANE

double	sbspMinPlaneDistance(int nIndex, BSPPlane* plane);
int		sbspSplitPolygon(int nIndex, BSPPlane* planeSplit, int* nFront, int* nBack);
int		sbspClassifyPolygon(int nIndex, BSPPlane* plane);
void	sbspCleanBSPTree(BSPTree* pTree);
void	sbspProcessTree(BSPTree* pTree, int nPolygonsSize, int* pPolygons);

int		g_iSplitCount = 0;
double	g_lengthTol = 1e-7;
double	g_distTol = 1e-7;
double	g_angleTol = 1e-7;
int	g_stoScheme = sto_r;

int	g_nMeshDataSize = 0;
SoupMeshData * g_pMeshData = NULL;

double det3(double a, double b, double c, 
			double d, double e, double f,
			double g, double h, double i)
{
	return a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g;
}

BSPPlane* copyPlane(BSPPlane * other)
{
	BSPPlane* plane = (BSPPlane*)malloc(sizeof(BSPPlane));
	plane->a = other->a;
	plane->b = other->b;
	plane->c = other->c;
	plane->d = other->d;
	return plane;
}

BSPPlane* buildPlane(double x1, double y1, double z1, 
					 double x2, double y2, double z2, 
					 double x3, double y3, double z3)
{
	BSPPlane* plane = (BSPPlane*)malloc(sizeof(BSPPlane));
	double dx1 = x2 - x1, dy1 = y2 - y1, dz1 = z2 - z1;
	double dx2 = x3 - x1, dy2 = y3 - y1, dz2 = z3 - z1;
	double nx = dy1*dz2 - dy2*dz1, 
		ny = dz1*dx2 - dz2*dx1, 
		nz = dx1*dy2 - dx2*dy1;
#ifdef SBSP_NORMALIZEPLANE
	double nabs = sqrt(nx*nx+ny*ny+nz*nz);
	if (nabs != 0.0)
	{
		nx /= nabs; ny /= nabs; nz /= nabs;
	}
#endif
	plane->a = nx; plane->b = ny; plane->c = nz;
	plane->d = - (nx*x1 + ny*y1 + nz*z1);
	assert ((plane->a*plane->a + plane->b*plane->b + plane->c*plane->c) != 0.0);
	return plane;
}

double distancePlane(double px, double py, double pz, BSPPlane* plane)
{
#ifdef SBSP_NORMALIZEPLANE
	return -(plane->a*px + plane->b*py + plane->c*pz + plane->d);
#else
	return -(plane->a*px + plane->b*py + plane->c*pz + plane->d)/sqrt(plane->a*plane->a+plane->b*plane->b+plane->c*plane->c);
#endif
}

int intersectLinePlane(double x1, double y1, double z1, double x2, double y2, double z2, BSPPlane* plane, double* ot)
{
	double t;
	double det = plane->a*(x2-x1) + plane->b*(y2-y1) + plane->c*(z2-z1);
	if (det == 0.0)
		return 0;
	t = (- plane->d - plane->a*x1 - plane->b*y1 - plane->c*z1)/det;
	if (t >= 0.0 && t <= 1.0)
	{
		*ot = t;
		return 1;
	}
	return 0;
}

double anglePlane(BSPPlane* plane1, BSPPlane* plane2)
{
#ifdef SBSP_NORMALIZEPLANE
	return plane1->a*plane2->a + plane1->b*plane2->b + plane1->c*plane2->c;
#else
	return ((plane1->a*plane2->a + plane1->b*plane2->b + plane1->c*plane2->c)/sqrt(plane1->a*plane1->a+plane1->b*plane1->b+plane1->c*plane1->c))/sqrt(plane2->a*plane2->a + plane2->b*plane2->b + plane2->c*plane2->c);
#endif
}

void sbspCleanMeshData()
{
	int i;
	for (i = 0; i < g_nMeshDataSize; i++){
		free(g_pMeshData[i].m_plBase);
		free(g_pMeshData[i].m_points);
	}
	if (g_pMeshData != NULL)
		free(g_pMeshData);
	g_pMeshData = NULL;
	g_nMeshDataSize = 0;
}

void sbspCleanBSP(BSPTree* pTree)
{
	if (pTree != NULL)
		sbspCleanBSPTree(pTree);
	sbspCleanMeshData();
}

void sbspCleanBSPTree(BSPTree* pTree)
{
	free(pTree->plPartition);
	if (pTree->pFront != NULL)
		sbspCleanBSPTree(pTree->pFront);
	if (pTree->pBack != NULL)
		sbspCleanBSPTree(pTree->pBack);
	free(pTree);
}

double r_int(double x, double y, int operation)
{
	if (operation == sto_r)
		return x + y - sqrt(x*x+y*y);
	if (operation == sto_sardf)
		return int_sardef_2(x, y, 0.2, 0.2);

	return fmin(x,y);
}

double r_uni(double x, double y, int operation)
{
	if (operation == sto_r)
		return x + y + sqrt(x*x+y*y);
	if (operation == sto_sardf)
		return uni_sardef_2(x, y, 0.2, 0.2);

	return fmax(x,y);
}

double sbspGetValue(double x, double y, double z, BSPTree* pTree)
{
	double fDistance, fFront, fBack, fResult;
	if (pTree == NULL)
		return 0.0;
	fDistance = distancePlane(x, y, z, pTree->plPartition);
	fResult = fDistance;
	if (pTree->pFront)
	{
		fFront = sbspGetValue(x, y, z, pTree->pFront);
		fResult = r_int(fDistance, fFront, g_stoScheme);
	}
	if (pTree->pBack)
	{
		fBack = sbspGetValue(x, y, z, pTree->pBack);
		fResult = r_uni(fDistance, fBack, g_stoScheme);
	}
	if (pTree->pFront && pTree->pBack)
	{
		fResult = r_uni(r_int(r_uni(fDistance, fBack, g_stoScheme), fFront, g_stoScheme), r_int(-fDistance, fBack, g_stoScheme), g_stoScheme);
	}
	return fResult;
}

/*
 build from triangle mesh data
 pVertexData size SHOULD be nVertexNum*3
 pFaceData size should be nFacesNum*3
*/
BSPTree* sbspBuildBSP(int nVertexNum, double* pVertexData, int nFacesNum, int* pFaceData)
{
	int iFace, iIndex;
	int nPolygonsSize;
	int idx[3];
	int * pPolygons;
	BSPTree* pTree;

	g_nMeshDataSize = nFacesNum;
	g_pMeshData = (SoupMeshData*)malloc(sizeof(SoupMeshData)*nFacesNum);
	pPolygons = (int*)malloc(sizeof(int)*nFacesNum);
	nPolygonsSize = nFacesNum;

	for (iFace = 0; iFace < nFacesNum; iFace++)
	{
		pPolygons[iFace] = iFace;
		g_pMeshData[iFace].m_nPlaneID = iFace;
		g_pMeshData[iFace].m_pointsSize = 3;
		g_pMeshData[iFace].m_points = (vec3d*)malloc(sizeof(vec3d)*3);
		g_pMeshData[iFace].m_bSplit = 0;

		for (iIndex = 0; iIndex < 3; iIndex++)
			idx[iIndex] = pFaceData[iFace*3 + iIndex];

		for (iIndex = 0; iIndex < 3; iIndex++){
			g_pMeshData[iFace].m_points[iIndex].x = pVertexData[idx[iIndex]*3+0];
			g_pMeshData[iFace].m_points[iIndex].y = pVertexData[idx[iIndex]*3+1];
			g_pMeshData[iFace].m_points[iIndex].z = pVertexData[idx[iIndex]*3+2];
		}
		g_pMeshData[iFace].m_plBase = buildPlane(
			g_pMeshData[iFace].m_points[0].x, 
			g_pMeshData[iFace].m_points[0].y, 
			g_pMeshData[iFace].m_points[0].z, 
			g_pMeshData[iFace].m_points[1].x, 
			g_pMeshData[iFace].m_points[1].y, 
			g_pMeshData[iFace].m_points[1].z, 
			g_pMeshData[iFace].m_points[2].x, 
			g_pMeshData[iFace].m_points[2].y, 
			g_pMeshData[iFace].m_points[2].z); 
	}
	pTree = (BSPTree*)malloc(sizeof(BSPTree));
	pTree->pFront = NULL;
	pTree->pBack = NULL;
	sbspProcessTree(pTree, nPolygonsSize, pPolygons);
	printf("Split %d times", g_iSplitCount);
	free(pPolygons);
	return pTree;
}

void sbspProcessTree(BSPTree* pTree, int nPolygonsSize, int* pPolygons)
{
	int * pFront;
	int * pBack;
	int nFrontSize, nBackSize, nToFront, nToBack, nToSpan;
	double fRatio, fRatioOpt, fDistance, fAngle;
	int nPoly, nItOpt, iIndex, iSubIndex, iSplitResult;
	int cl;

	assert(nPolygonsSize != 0);
	fRatioOpt = 0.0;
	nItOpt = -1;
#ifdef SBSP_NAIVE
	/* in naive case we just take first polygon as the root of bsp tree */
	nItOpt = 0;
#else
	/* in ordinary case we have to calculate it */
	for (iIndex = 0; iIndex < nPolygonsSize; iIndex++)
	{
		nPoly = pPolygons[iIndex];
		nToSpan = 0; nToFront = 0; nToBack = 0;
		fDistance = FLT_MAX;
		fAngle = -1;
		if (g_pMeshData[nPoly].m_bSplit)
			continue;
		/* collect all the information for candidate, such as: 
		   number of front/back/span faces, minimal distance after possible split and
		   minimal angle between planes */
		for (iSubIndex = 0; iSubIndex < nPolygonsSize; iSubIndex++)
		{
			if (iIndex == iSubIndex)
				continue;
			cl = sbspClassifyPolygon(pPolygons[iSubIndex], g_pMeshData[nPoly].m_plBase);
			if (cl == polygon_back)
				nToBack++;
			if (cl == polygon_front)
				nToFront++;
			if (cl == polygon_spanning)
			{
				nToSpan++;
				fDistance = fmin(fDistance, sbspMinPlaneDistance(pPolygons[iSubIndex], g_pMeshData[nPoly].m_plBase));
				fAngle = fmax(fAngle, fabs(anglePlane(g_pMeshData[nPoly].m_plBase, g_pMeshData[pPolygons[iSubIndex]].m_plBase)));
			}
		}
		if (nToSpan == 0)
		{
			nItOpt = iIndex;
			break;
		}
		/* set criteria here */
		if (nToFront != 0 && nToBack != 0)
		{

			if (nToFront > nToBack)
				fRatio = (double)nToBack/(double)nToFront;
			else
				fRatio = (double)nToFront/(double)nToBack;
		}
		else 
			fRatio = 0.0;
		fRatio *= fDistance;
		fRatio *= acos(fAngle);
		if (fRatio > fRatioOpt)
		{
			nItOpt = iIndex;
			fRatioOpt = fRatio;
		}
	}
	if (nItOpt == -1)
	{
		/* something went wrong */
		nItOpt = 0;
	}
#endif
	pTree->firstPolygon = pPolygons[nItOpt];
	pTree->plPartition = copyPlane(g_pMeshData[pTree->firstPolygon].m_plBase);
	nToFront = 0; nToBack = 0;
	
	/* first pass: calculate size for arrays */
	for (iIndex = 0; iIndex < nPolygonsSize; iIndex++)
	{
		if (iIndex == nItOpt)
			continue;
		cl = sbspClassifyPolygon(pPolygons[iIndex], pTree->plPartition);
		switch (cl)
		{
		case polygon_front:
			nToFront++;
			break;
		case polygon_back:
			nToBack++;
			break;
		case polygon_spanning:
			nToFront++;
			nToBack++;
			break;
		}
	}
	/* second pass - fill the arrays and repeat recursively */
	if (nToFront != 0)
		pFront = (int*)malloc(sizeof(int)*nToFront);
	if (nToBack != 0)
		pBack = (int*)malloc(sizeof(int)*nToBack);
	nFrontSize = 0; nBackSize = 0;
	for (iIndex = 0; iIndex < nPolygonsSize; iIndex++)
	{
		if (iIndex == nItOpt)
			continue;
		cl = sbspClassifyPolygon(pPolygons[iIndex], pTree->plPartition);
		switch(cl){
		case polygon_front:
			pFront[nFrontSize] = pPolygons[iIndex];
			nFrontSize++;
			break;
		case polygon_back:
			pBack[nBackSize] = pPolygons[iIndex];
			nBackSize++;
			break;
		case polygon_spanning:
			iSplitResult = sbspSplitPolygon(pPolygons[iIndex], pTree->plPartition, &nToFront, &nToBack);
			assert(iSplitResult != 0);
			pFront[nFrontSize] = nToFront;
			nFrontSize++;
			pBack[nBackSize] = nToBack;
			nBackSize++;
			break;
		}
	}
	if (nFrontSize != 0)
	{
		pTree->pFront = (BSPTree*)malloc(sizeof(BSPTree));
		sbspProcessTree(pTree->pFront, nFrontSize, pFront);
	}
	else
		pTree->pFront = NULL;
	if (nBackSize != 0)
	{
		pTree->pBack = (BSPTree*)malloc(sizeof(BSPTree));
		sbspProcessTree(pTree->pBack, nBackSize, pBack);
	}
	else
		pTree->pBack = NULL;
}

double sbspMinPlaneDistance(int nIndex, BSPPlane* plane)
{
	int i;
	vec3d vecTemp;
	double fDistanceCur, fDistance;

	fDistance = FLT_MAX;
	for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
	{
		vecTemp = g_pMeshData[nIndex].m_points[i];		
		fDistanceCur = fabs(distancePlane(vecTemp.x, vecTemp.y, vecTemp.z, plane));
		if (fabs(fDistanceCur) > g_distTol)
			fDistance = fmin(fDistance, fDistanceCur);
	}
	return fDistance;
}

int sbspSplitPolygon(int nIndex, BSPPlane* planeSplit, int* oFront, int* oBack)
{
	int nRayIsec[2];
	vec3d tIsec[2];
	vec3d vecThis, vecNext, vecMiddle;
	double t, fDistance;
	int i, bSecond, nRayIsecIt, nVertex, cl;
	SoupMeshData * pNewData;

	nRayIsec[0] = nRayIsec[1] = -1;
	nRayIsecIt = 0;
	bSecond = 0;

	/* realloc and fill initial data */
	i = g_nMeshDataSize;
	pNewData = (SoupMeshData*)malloc(sizeof(SoupMeshData)*(g_nMeshDataSize+2));
	memcpy(pNewData, g_pMeshData, g_nMeshDataSize*sizeof(SoupMeshData));
//	g_pMeshData = realloc(g_pMeshData, g_nMeshDataSize);
	free(g_pMeshData);
	g_pMeshData = pNewData;
	g_nMeshDataSize = i+2;

	g_pMeshData[g_nMeshDataSize-2].m_plBase = copyPlane(g_pMeshData[nIndex].m_plBase);
	g_pMeshData[g_nMeshDataSize-1].m_plBase = copyPlane(g_pMeshData[nIndex].m_plBase);
	g_pMeshData[g_nMeshDataSize-2].m_nPlaneID = g_pMeshData[nIndex].m_nPlaneID;
	g_pMeshData[g_nMeshDataSize-1].m_nPlaneID = g_pMeshData[nIndex].m_nPlaneID;
	g_pMeshData[g_nMeshDataSize-2].m_bSplit = 1;
	g_pMeshData[g_nMeshDataSize-1].m_bSplit = 1;

	g_pMeshData[g_nMeshDataSize-2].m_pointsSize = 0;
	g_pMeshData[g_nMeshDataSize-2].m_points = (vec3d*)malloc((g_pMeshData[nIndex].m_pointsSize+2)*sizeof(vec3d));
	g_pMeshData[g_nMeshDataSize-1].m_pointsSize = 0;
	g_pMeshData[g_nMeshDataSize-1].m_points = (vec3d*)malloc((g_pMeshData[nIndex].m_pointsSize+2)*sizeof(vec3d));

	/* record all points in the polygons that lie on plane */
	for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
	{
		fDistance = distancePlane(g_pMeshData[nIndex].m_points[i].x, g_pMeshData[nIndex].m_points[i].y, g_pMeshData[nIndex].m_points[i].z, planeSplit);
		if (fabs(fDistance) < g_distTol)
		{
			if (nRayIsecIt >= 2)
			{
				printf("points/plane intersection\n");
				return 0;
			}
			nRayIsec[nRayIsecIt] = i;
			nRayIsecIt++;
		}
	}
	
	if (nRayIsec[0] != -1 && nRayIsec[1] != -1)
	{
		/* plane intersects the face over two vertices */
		for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
		{
			if (bSecond == 1 || i == nRayIsec[0] || i == nRayIsec[1])
			{
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
			}
			if (bSecond == 0 || i == nRayIsec[0] || i == nRayIsec[1])
			{
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
			}
			if (i == nRayIsec[0])
				bSecond = 1;
			if (i == nRayIsec[1])
				bSecond = 0;
		}
	}
	else if (nRayIsec[0] != -1 && nRayIsec[1] == -1)
	{
		/* plane intersects the face over one vertice */
		nVertex = nRayIsec[0];
		nRayIsec[0] = nRayIsec[1] = -1;
		nRayIsecIt = 0;
		for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
		{
			vecThis = g_pMeshData[nIndex].m_points[i];
			if (i == g_pMeshData[nIndex].m_pointsSize-1)
				vecNext = g_pMeshData[nIndex].m_points[0];
			else
				vecNext = g_pMeshData[nIndex].m_points[i+1];

			if (intersectLinePlane(vecThis.x, vecThis.y, vecThis.z, vecNext.x, vecNext.y, vecNext.z, planeSplit, &t) == 1)
			{
				if (t < (1.0-g_lengthTol) && t > g_lengthTol){
					nRayIsec[nRayIsecIt] = i;
					vecMiddle.x = vecThis.x + t*(vecNext.x - vecThis.x);
					vecMiddle.y = vecThis.y + t*(vecNext.y - vecThis.y);
					vecMiddle.z = vecThis.z + t*(vecNext.z - vecThis.z);
					tIsec[nRayIsecIt] = vecMiddle;
					nRayIsecIt++;
					if (nRayIsecIt == 2)
					{
						printf ("one point and edges\n");
						return 0;
					}
				}
			}
		}
		for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
		{
			if (bSecond == 1 || i == nVertex)
			{
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
			}
			if (!bSecond || i == nVertex)
			{
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
			}
			if (i == nRayIsec[0])
			{
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = tIsec[0];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = tIsec[0];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
				bSecond = 1 - bSecond;
			}
			if (i == nVertex)
				bSecond = 1 - bSecond;
		}
	}
	else
	{
		/* plane intersects the face over two edges */
		nRayIsec[0] = nRayIsec[1] = -1;
		nRayIsecIt = 0;
		for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
		{
			vecThis = g_pMeshData[nIndex].m_points[i];
			if (i == g_pMeshData[nIndex].m_pointsSize-1)
				vecNext = g_pMeshData[nIndex].m_points[0];
			else
				vecNext = g_pMeshData[nIndex].m_points[i+1];
			if (intersectLinePlane(vecThis.x, vecThis.y, vecThis.z, vecNext.x, vecNext.y, vecNext.z, planeSplit, &t) == 1)
			{
				if (t < 1.0 && t > 0.0){
					if (nRayIsecIt >= 2)
					{
						printf("ray intersects too much\n");
						return 0;
					}
					nRayIsec[nRayIsecIt] = i;
					vecMiddle.x = vecThis.x + t*(vecNext.x - vecThis.x);
					vecMiddle.y = vecThis.y + t*(vecNext.y - vecThis.y);
					vecMiddle.z = vecThis.z + t*(vecNext.z - vecThis.z);
					tIsec[nRayIsecIt] = vecMiddle;
					nRayIsecIt++;
				}
			}
		}
		for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
		{
			if (bSecond == 1)
			{
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
				if (i == nRayIsec[1])
					bSecond = 0;
			}
			else
			{
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = g_pMeshData[nIndex].m_points[i];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
				if (i == nRayIsec[0])
					bSecond = 1;
			}
			if (i == nRayIsec[0])
			{
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = tIsec[0];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = tIsec[0];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
			}
			if (i == nRayIsec[1])
			{
				g_pMeshData[g_nMeshDataSize-1].m_points[g_pMeshData[g_nMeshDataSize-1].m_pointsSize] = tIsec[1];
				g_pMeshData[g_nMeshDataSize-1].m_pointsSize++;
				g_pMeshData[g_nMeshDataSize-2].m_points[g_pMeshData[g_nMeshDataSize-2].m_pointsSize] = tIsec[1];
				g_pMeshData[g_nMeshDataSize-2].m_pointsSize++;
			}
		}
	}

	cl = sbspClassifyPolygon(g_nMeshDataSize-1, planeSplit);
	if (cl == polygon_spanning || cl == polygon_coincident)
	{
		printf ("polygon span or coincident\n");
		return 0;
	}
	if (cl == polygon_front)
	{
		*oFront = g_nMeshDataSize-1;
		*oBack = g_nMeshDataSize-2;
	}
	else
	{
		*oFront = g_nMeshDataSize-2;
		*oBack = g_nMeshDataSize-1;
	}
	return 1;
}


int sbspClassifyPolygon(int nIndex, BSPPlane* plane)
{
	int i;
	double fDistance, cosa;
	int ret = polygon_undefined;
	for (i = 0; i < g_pMeshData[nIndex].m_pointsSize; i++)
	{
		fDistance = distancePlane(g_pMeshData[nIndex].m_points[i].x, g_pMeshData[nIndex].m_points[i].y, g_pMeshData[nIndex].m_points[i].z, plane);
		if (fDistance > g_distTol)
		{
			if (ret == polygon_undefined || ret == polygon_coincident)
				ret = polygon_front;
			else if (ret == polygon_back)
				ret = polygon_spanning;
		}
		else if (fDistance < -g_distTol)
		{
			if (ret == polygon_undefined || ret == polygon_coincident)
				ret = polygon_back;
			else if (ret == polygon_front)
				ret = polygon_spanning;
		}
		else
		{
			if (ret == polygon_undefined)
				ret = polygon_coincident;
		}
	}

	cosa = anglePlane(plane, g_pMeshData[nIndex].m_plBase);
	if (fabs(cosa-1.0) < g_angleTol && ret == polygon_spanning)
		ret = polygon_coincident;

	assert (ret != polygon_undefined);

	return ret;
}

void	sbspPrintTree(BSPTree* tree, int nStack)
{
	int i;
	char indent[1024];
	if (nStack >= 1024)
	{
		printf("overflow!\n");
		return;
	}
	for (i = 0; i < nStack; i++)
	{
		indent[i] = ' ';
	}
	indent[nStack] = 0;
	printf("%spolygon: ",indent);
	printf("%d \n", tree->firstPolygon);

	if (tree->pFront)
	{
		printf("%sfront tree\n", indent);
		sbspPrintTree(tree->pFront, nStack+1);
	}
	else
	{
		printf("%sfront tree: NULL\n", indent);
	}
	if (tree->pBack)
	{
		printf("%sback tree\n", indent);
		sbspPrintTree(tree->pBack, nStack+1);
	}
	else
	{
		printf("%sback tree: NULL\n", indent);
	}
}
