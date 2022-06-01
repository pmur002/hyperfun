/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Benjamin Schmitt
 Copyright 2007-2008 Oleg Fryazinov

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



void CMainApp::vrmlOut(){

  // ********** Write FILE *************
  //Open file
	std::ofstream toFile(itsCL.itsWRLName.c_str());
  if(!toFile){
    std::cout << "\n File: " << itsCL.itsWRLName << " could not be opened.\n";
    exit(0);
  }
	
  //Write file

  //COMMENTS
  toFile << "#VRML V2.0 utf8\n";
  toFile << "\n";
  toFile << "#Produced by HyperFun Polygonal Viewer\n";
  toFile << "\n";
  toFile << "#From Model: " << itsCL.itsModel << "  Object: " << itsCL.itsObject << "\n";
  toFile << "\n";
  //CAMERA
  toFile << "DEF Start Viewpoint {\n";
  toFile << "  position " << Pos_[0] << " " << Pos_[1] << " " << 2*Pos_[2] << "\n";
  toFile << "  orientation " << -Pos_[0] << " " << -Pos_[1] << " " << -Pos_[2] << " 0\n";
  toFile << "  fieldOfView " << (60/360)*PI << "\n";
  toFile << "  description \"Start\"\n";
  toFile << "  }\n";
  //OBJECT
  toFile << "DEF " << itsCL.itsObject << " Transform {\n";
  toFile << "  translation 0 0 0\n";
  toFile << "  children [\n";
  toFile << "    Shape {\n";
  toFile << "      appearance Appearance {\n";
  toFile << "        material Material {\n";
  toFile << "          diffuseColor " << itsCL.itsFaceColor[0] << " " << itsCL.itsFaceColor[1] << " " << itsCL.itsFaceColor[2] << "\n";
  toFile << "          specularColor 1 1 1\n";
  toFile << "          shininess 0.45\n";
  toFile << "          }\n";
  toFile << "        }\n";
  toFile << "      geometry DEF " << itsCL.itsObject << "-FACES IndexedFaceSet {\n";
  toFile << "        ccw TRUE\n";
  toFile << "        solid FALSE\n";


  //Vertices
  toFile << "        coord DEF " << itsCL.itsObject << "-COORD Coordinate {\n";
  toFile << "          point [\n";
  for(int A=0; A<itsPolyMesh_->VertexNum(); A++){
    toFile << "            ";
    toFile << itsPolyMesh_->itsData.getVertex(A, 0) << " ";
    toFile << itsPolyMesh_->itsData.getVertex(A, 1) << " ";
    toFile << itsPolyMesh_->itsData.getVertex(A, 2) << "\n";
  }
  toFile << "            ]\n";
  toFile << "          }\n";
  //Normals
  toFile << "        normal Normal {\n";
  toFile << "          vector [\n";
  for(int B=0; B<itsPolyMesh_->VertexNum(); B++){
    toFile << "            ";
    toFile << itsPolyMesh_->itsData.getVNormal(B, 0) << " ";
    toFile << itsPolyMesh_->itsData.getVNormal(B, 1) << " ";
    toFile << itsPolyMesh_->itsData.getVNormal(B, 2) << "\n";
  }
  toFile << "            ]\n";
  toFile << "          }\n";
  toFile << "        normalPerVertex TRUE\n";
  //Vert Index
  toFile << "        coordIndex [\n";
  for(int C=0; C<itsPolyMesh_->TriangleNum(); C++){
    toFile << "          ";
    toFile << itsPolyMesh_->itsData.getTriangle(C, 0) << ", ";
    toFile << itsPolyMesh_->itsData.getTriangle(C, 1) << ", ";
    toFile << itsPolyMesh_->itsData.getTriangle(C, 2) << ", -1, \n";
  }
  toFile << "          ]\n";
  //Norm Index
  toFile << "        normalIndex [\n";
  for(int D=0; D<itsPolyMesh_->TriangleNum(); D++){
    toFile << "          ";
    toFile << itsPolyMesh_->itsData.getTriangle(D, 0) << ", ";
    toFile << itsPolyMesh_->itsData.getTriangle(D, 1) << ", ";
    toFile << itsPolyMesh_->itsData.getTriangle(D, 2) << ", -1, \n";
  }
  toFile << "          ]\n";
  //End
  //Color Attribute
  
  if (itsCL.itsAttributes)
  {
  
	  toFile << "        colorPerVertex TRUE\n";
	  toFile << "        color Color { color [\n";//1 0 0, 0 1 0, 0 0 1, 1,1,0, 0,1,1]}
	  for(int E=0; E<itsPolyMesh_->VertexNum(); E++){
		  toFile << "          ";
		  toFile << itsPolyMesh_->itsData.getAttributes(E, 0) << ", ";
		  toFile << itsPolyMesh_->itsData.getAttributes(E, 1) << ", ";
		  toFile << itsPolyMesh_->itsData.getAttributes(E, 2) << ", \n";
	  }
	  toFile << "          ]}\n";
  }	  
	  
  toFile << "        }\n";
  toFile << "      }\n";
  toFile << "    ]\n";
  toFile << "  }\n";

  toFile << "\0";
  //Close fromFile
  toFile.close();
  // **********************************

}
