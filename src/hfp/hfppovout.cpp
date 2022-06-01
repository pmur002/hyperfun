/*==============================================================================

 Copyright 2003-2004 Pierre-Alain Fayolle

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



#include <stdio.h>
#include <stdlib.h>

#include "mainapp.h"
#include "main.h"

void CMainApp::povrayMeshOut(){
	FILE *pov = fopen(itsCL.itsPOVName.c_str(),"w");

	fprintf(pov,"#declare Green = texture {\n");
	fprintf(pov,"\tpigment { color rgb<0.2, 0.8, 0.2> }\n");
	fprintf(pov,"\tfinish { ambient 0.2 diffuse 0.5 }\n");
	fprintf(pov,"}\n");

	// texture needs to be generated before mesh
	// see povray doc
	int i;
	double red, green, blue;

	double tr_n = itsPolyMesh_->TriangleNum();

	if (itsCL.itsAttributes){
		for (i=0;i<tr_n;++i){
				// attributes 
				// arithmetic sum of the colors of each vertex
				red =   (1.0f/3.0f)*(GetAttrFromTriangle(i,0,0) + GetAttrFromTriangle(i,1,0) + GetAttrFromTriangle(i,2,0));
				green = (1.0f/3.0f)*(GetAttrFromTriangle(i,0,1) + GetAttrFromTriangle(i,1,1) + GetAttrFromTriangle(i,2,1));
				blue =  (1.0f/3.0f)*(GetAttrFromTriangle(i,0,2) + GetAttrFromTriangle(i,1,2) + GetAttrFromTriangle(i,2,2));

				fprintf(pov,"#declare attributes_%d = texture { \n",i);
				fprintf(pov,"\tpigment{ color rgb<%lf, %lf, %lf> }\n",red, green, blue);
				fprintf(pov,"\tfinish{ ambient 0.2 diffuse 0.5 }\n");
				fprintf(pov, "}\n");
			}
	}

	fprintf(pov,"camera {\n");
	fprintf(pov,"\tlocation <%lf,%lf,%lf>\n",Pos_[0], Pos_[1], Pos_[2]);
	fprintf(pov,"\tlook_at <%lf,%lf,%lf>\n",Look_[0], Look_[1], Look_[2]);
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	//fprintf(pov,"\t<%lf,%lf,%lf>\n",Look_[0], Look_[1]+HF_MAX[2]*2, Look_[2]);
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Min_[0], Min_[1], Min_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Max_[0], Min_[1], Min_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");
	
	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Min_[0], Max_[1], Min_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Min_[0], Min_[1], Max_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Max_[0], Max_[1], Min_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");
	
	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Max_[0], Min_[1], Max_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Min_[0], Max_[1], Max_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"light_source {\n");
	fprintf(pov,"\t<%lf,%lf,%lf>\n",Max_[0], Max_[1], Max_[2]);
	fprintf(pov,"\tcolor 1\n");
	fprintf(pov,"}\n");

	fprintf(pov,"mesh {\n");

	if (!(itsCL.itsAttributes)){
		if (itsCL.itsNormals){
			for (i=0;i<tr_n;++i){
				//for (j=0;j<3;++j){
				fprintf(pov,"smooth_triangle {\n");

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,0,0), // corner 1
												GetVertexFromTriangle(i,0,1),
												GetVertexFromTriangle(i,0,2));
				fprintf(pov,"\t<%lf,%lf,%lf>,\n",	GetNormalFromTriangle(i,0,0), // normal 1
												GetNormalFromTriangle(i,0,1),
												GetNormalFromTriangle(i,0,2));
			
				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,1,0), // corner 1
												GetVertexFromTriangle(i,1,1),
												GetVertexFromTriangle(i,1,2));
				fprintf(pov,"\t<%lf,%lf,%lf>,\n",	GetNormalFromTriangle(i,1,0), // normal 1
												GetNormalFromTriangle(i,1,1),
												GetNormalFromTriangle(i,1,2));
			

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,2,0), // corner 1
												GetVertexFromTriangle(i,2,1),
												GetVertexFromTriangle(i,2,2));
				fprintf(pov,"\t<%lf,%lf,%lf>\n",	GetNormalFromTriangle(i,2,0), // normal 1
												GetNormalFromTriangle(i,2,1),
												GetNormalFromTriangle(i,2,2));
			
				
							// no attributes => now put green as a color
				// later put HF_FACE_COLOR
				fprintf(pov,"\ttexture { Green }\n");

				fprintf(pov,"}\n");
			}
		}
		else{ // if has no normals
			for (i=0;i<tr_n;++i){
				//for (j=0;j<3;++j){
				fprintf(pov,"triangle {\n");

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,0,0), // corner 1
												GetVertexFromTriangle(i,0,1),
												GetVertexFromTriangle(i,0,2));
				
				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,1,0), // corner 1
												GetVertexFromTriangle(i,1,1),
												GetVertexFromTriangle(i,1,2));
				

				fprintf(pov,"\t<%lf,%lf,%lf>\n",	GetVertexFromTriangle(i,2,0), // corner 1
												GetVertexFromTriangle(i,2,1),
												GetVertexFromTriangle(i,2,2));
				
				// no attributes => now put green as a color
				// later put HF_FACE_COLOR
				fprintf(pov,"\ttexture { Green }\n");

				fprintf(pov,"}\n");
			}
		}
	}
	else{ // if has attributes
		if (itsCL.itsNormals){
			for (i=0;i<tr_n;++i){
				//for (j=0;j<3;++j){
				fprintf(pov,"smooth_triangle {\n");

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,0,0), // corner 1
												GetVertexFromTriangle(i,0,1),
												GetVertexFromTriangle(i,0,2));
				fprintf(pov,"\t<%lf,%lf,%lf>,\n",	GetNormalFromTriangle(i,0,0), // normal 1
												GetNormalFromTriangle(i,0,1),
												GetNormalFromTriangle(i,0,2));
			
				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,1,0), // corner 1
												GetVertexFromTriangle(i,1,1),
												GetVertexFromTriangle(i,1,2));
				fprintf(pov,"\t<%lf,%lf,%lf>,\n",	GetNormalFromTriangle(i,1,0), // normal 1
												GetNormalFromTriangle(i,1,1),
												GetNormalFromTriangle(i,1,2));
			

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,2,0), // corner 1
												GetVertexFromTriangle(i,2,1),
												GetVertexFromTriangle(i,2,2));
				fprintf(pov,"\t<%lf,%lf,%lf>\n",	GetNormalFromTriangle(i,2,0), // normal 1
												GetNormalFromTriangle(i,2,1),
												GetNormalFromTriangle(i,2,2));
			
				
				fprintf(pov,"\ttexture { attributes_%d }\n",i);
				
				fprintf(pov,"}\n");
			}
		}
		else{ // if has no normals
			for (i=0;i<tr_n;++i){
				//for (j=0;j<3;++j){
				fprintf(pov,"triangle {\n");

				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,0,0), // corner 1
												GetVertexFromTriangle(i,0,1),
												GetVertexFromTriangle(i,0,2));
				
				fprintf(pov,"\t<%lf,%lf,%lf>,",	GetVertexFromTriangle(i,1,0), // corner 1
												GetVertexFromTriangle(i,1,1),
												GetVertexFromTriangle(i,1,2));
				

				fprintf(pov,"\t<%lf,%lf,%lf>\n",	GetVertexFromTriangle(i,2,0), // corner 1
												GetVertexFromTriangle(i,2,1),
												GetVertexFromTriangle(i,2,2));
			
				
				fprintf(pov,"\ttexture { attributed_%d }\n",i);
				
				fprintf(pov,"}\n");
			}
		}
	}


	fprintf(pov,"}\n");

	fflush(pov);

	fclose(pov);


}
