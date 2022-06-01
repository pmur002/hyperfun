/*==============================================================================

 Copyright 2003 Yutaka Ohtake
 Copyright 2008 Oleg Fryazinov


 This Work or file is part of the greater total Work, software or group of
 files named HyperFun Polygonizer.

 HyperFun Polygonizer can be redistributed and/or modified under the terms 
 of the CGPL, The Common Good Public License as published by and at CGPL.org
 (http://CGPL.org).  It is released under version 1.0 Beta of the License
 until the 1.0 version is released after which either version 1.0 of the
 License, or (at your option) any later version can be applied.

 THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
 CGPL, The Common Good Public License for more information.)

 You should have received a copy of the CGPL along with HyperFun Polygonizer;  
 if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/

#include "stdio.h"
#include <iostream>
#include "hfdc_quadmesh.h"

QuadMesh::QuadMesh()
{
	vertex_N = 0;
	face_N = 0;
}

QuadMesh::~QuadMesh()
{
}

void QuadMesh::setVertexN(int N)
{
	vertex_N = N;
	std::vector<double> vecEmpty;
	vecEmpty.push_back(0.0);
	vecEmpty.push_back(0.0);
	vecEmpty.push_back(0.0);
	for (int i = 0; i < N; i++)
	{
		vertex.push_back(vecEmpty);
		normal_v.push_back(vecEmpty);
		degree.push_back(0);
	}
}

void QuadMesh::setVertex(int i, double x, double y, double z)
{
	std::vector<double> vec;
	vec.push_back(x); vec.push_back(y); vec.push_back(z); 
	vertex[i] = vec;
}

void QuadMesh::addFace(int i0, int i1, int i2, int i3, const std::vector<double>& n)
{
	std::vector<int> vec;
	vec.push_back(i0); vec.push_back(i1); vec.push_back(i2); vec.push_back(i3);
	face.push_back(vec);
	normal_f.push_back(n);
	face_N = face.size();
}

