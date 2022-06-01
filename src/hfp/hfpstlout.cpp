/*==============================================================================

Copyright 2003-2004 Shogo Onozuka
Copyright 2009 Oleg Fryazinov

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



#include "mainapp.h"
#include <math.h>

using namespace std;

void CMainApp::stlOut(){

	int coord1,coord2,coord3;
	double a1, a2, a3, b1, b2, b3, c1, c2, c3, N1, N2, N3, L;
//	double d1, d2, d3;
	double n1p, n2p, n3p, nn;
	bool bReverse = false;

	ofstream toFile(itsCL.itsSTLName.c_str());
	if(!toFile){
		std::cout << "\n File: " << itsCL.itsSTLName << " could not be opened.\n";    	
		exit(0);
	}

	toFile << "solid " <<itsCL.itsSTLName<<endl;

	for(int C=0; C<itsPolyMesh_->TriangleNum(); C++){

		bReverse = false;
		coord1 = itsPolyMesh_->itsData.getTriangle(C, 0);
		coord2 = itsPolyMesh_->itsData.getTriangle(C, 1);
		coord3 = itsPolyMesh_->itsData.getTriangle(C, 2);

		a1 = itsPolyMesh_->itsData.getVertex(coord1, 0);
		a2 = itsPolyMesh_->itsData.getVertex(coord1, 1);
		a3 = itsPolyMesh_->itsData.getVertex(coord1, 2);

		b1 = itsPolyMesh_->itsData.getVertex(coord2, 0);
		b2 = itsPolyMesh_->itsData.getVertex(coord2, 1);
		b3 = itsPolyMesh_->itsData.getVertex(coord2, 2);

		c1 = itsPolyMesh_->itsData.getVertex(coord3, 0);
		c2 = itsPolyMesh_->itsData.getVertex(coord3, 1);
		c3 = itsPolyMesh_->itsData.getVertex(coord3, 2);

/*
		N1 = (b2-a2)*(c3-b3) - (b3-a3)*(c2-b2);
		N2 = (b3-a3)*(c1-b1) - (b1-a1)*(c3-b3);
		N3 = (b1-a1)*(c2-b2) - (b2-a2)*(c1-b1);
		L = sqrt(N1*N1 + N2*N2 + N3*N3);
		if (L != 0.0)
		{
			N1 /= L;
			N2 /= L;
			N3 /= L;
		}
*/
		n1p = itsPolyMesh_->itsData.getTNormal(C, 0);
		n2p = itsPolyMesh_->itsData.getTNormal(C, 1);
		n3p = itsPolyMesh_->itsData.getTNormal(C, 2);
/*
		nn = n1p*N1 + n2p*N2 + n3p*N3;
		if (nn < 0)
			bReverse = true;
		
		d1 = (a1+b1+c1)/3;
		d2 = (a2+b2+c2)/3;
		d3 = (a3+b3+c3)/3;
		itsPolyMesh_->CalcVNormal(d1,d2,d3,n1p,n2p,n3p);
		n1p /= 3; n2p /= 3; n3p /= 3;
		nn = n1p*N1 + n2p*N2 + n3p*N3;
		if (nn < 0)
			bReverse = true;
		n1p = itsPolyMesh_->itsData.getTNormal(C, 0);
		n2p = itsPolyMesh_->itsData.getTNormal(C, 1);
		n3p = itsPolyMesh_->itsData.getTNormal(C, 2);
*/
		toFile << " facet normal " << n1p << " " << n2p << " " << n3p << "\n";
		toFile << "  outer loop\n";
		toFile << "   vertex "<< a1 <<" "<< a2 <<" "<< a3 <<" \n";

		if (bReverse)
		{
			toFile << "   vertex "<< c1 <<" "<< c2 <<" "<< c3 <<" \n";	
			toFile << "   vertex "<< b1 <<" "<< b2 <<" "<< b3 <<" \n";
		}
		else
		{
			toFile << "   vertex "<< b1 <<" "<< b2 <<" "<< b3 <<" \n";
			toFile << "   vertex "<< c1 <<" "<< c2 <<" "<< c3 <<" \n";	
		}
		toFile << "  endloop\n";
		toFile << " endfacet\n";
	}
	toFile << "endsolid "<<itsCL.itsSTLName<<endl;
	toFile.close();
}

void stlWriteInt(ofstream& ofs, int nValue)
{
	union
	{
		int niv;
		char civ[4];
	} in;
	
	union
	{
		int nov;
		char cov[4];
	} out;

	in.niv = nValue;
  
	out.nov  = in.civ[0] & 0xFF;
	out.nov |= (in.civ[1] & 0xFF) << 0x08;
	out.nov |= (in.civ[2] & 0xFF) << 0x10;
	out.nov |= (in.civ[3] & 0xFF) << 0x18;
	ofs.write(out.cov, 4);
}

void stlWriteFloat(ofstream& ofs, float fValue)
{
	union 
	{
		int nov;
		char cov[4];
	} out;
	union
	{
		float fiv;
		char civ[4];
	} in;

	in.fiv = fValue;

	out.nov  = in.civ[0] & 0xFF;
	out.nov |= (in.civ[1] & 0xFF) << 0x08;
	out.nov |= (in.civ[2] & 0xFF) << 0x10;
	out.nov |= (in.civ[3] & 0xFF) << 0x18;
	ofs.write(out.cov, 4);
}


void CMainApp::stlBinaryOut()
{
	double a1, a2, a3, b1, b2, b3, c1, c2, c3;
	double n1p, n2p, n3p;
	int coord1,coord2,coord3;

	ofstream toFile;
	toFile.open(itsCL.itsSTLName.c_str(), ios_base::binary);
	if(!toFile){
		std::cout << "\n File: " << itsCL.itsSTLName << " could not be opened.\n";    	
		exit(0);
	}

	char header[80];
	char empty[2] = {0,0};
	strcpy(header, itsCL.itsSTLName.c_str());
	unsigned int nFacets = itsPolyMesh_->TriangleNum();
	toFile.write(header, 80);
	stlWriteInt(toFile, itsPolyMesh_->TriangleNum());
	for(int C=0; C<itsPolyMesh_->TriangleNum(); C++){
		coord1 = itsPolyMesh_->itsData.getTriangle(C, 0);
		coord2 = itsPolyMesh_->itsData.getTriangle(C, 1);
		coord3 = itsPolyMesh_->itsData.getTriangle(C, 2);

		a1 = itsPolyMesh_->itsData.getVertex(coord1, 0);
		a2 = itsPolyMesh_->itsData.getVertex(coord1, 1);
		a3 = itsPolyMesh_->itsData.getVertex(coord1, 2);

		b1 = itsPolyMesh_->itsData.getVertex(coord2, 0);
		b2 = itsPolyMesh_->itsData.getVertex(coord2, 1);
		b3 = itsPolyMesh_->itsData.getVertex(coord2, 2);

		c1 = itsPolyMesh_->itsData.getVertex(coord3, 0);
		c2 = itsPolyMesh_->itsData.getVertex(coord3, 1);
		c3 = itsPolyMesh_->itsData.getVertex(coord3, 2);
	
		n1p = itsPolyMesh_->itsData.getTNormal(C, 0);
		n2p = itsPolyMesh_->itsData.getTNormal(C, 1);
		n3p = itsPolyMesh_->itsData.getTNormal(C, 2);

		stlWriteFloat(toFile, (float)n1p);
		stlWriteFloat(toFile, (float)n2p);
		stlWriteFloat(toFile, (float)n3p);
		stlWriteFloat(toFile, (float)a1);
		stlWriteFloat(toFile, (float)a2);
		stlWriteFloat(toFile, (float)a3);
		stlWriteFloat(toFile, (float)b1);
		stlWriteFloat(toFile, (float)b2);
		stlWriteFloat(toFile, (float)b3);
		stlWriteFloat(toFile, (float)c1);
		stlWriteFloat(toFile, (float)c2);
		stlWriteFloat(toFile, (float)c3);
		toFile.write(empty, 2); //attributes - do we need this?
	}

	toFile.close();
}