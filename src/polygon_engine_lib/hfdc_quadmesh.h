#ifndef POLYGONMESH_H
#define POLYGONMESH_H

#include <vector>

class QuadMesh  
{
public:
	std::vector<std::vector<double> > vertex; //3
	std::vector<std::vector<int> > face; //4
	std::vector<int> degree;
	std::vector<std::vector<double> > normal_v; //3
	std::vector<std::vector<double> > normal_f; //3

	int vertex_N;
	int face_N;

public:
	QuadMesh();
	~QuadMesh();
	void setVertexN(int N);
	void setVertex(int i, double x, double y, double z);
	void addFace(int i0, int i1, int i2, int i3, const std::vector<double>& n);
};

#endif