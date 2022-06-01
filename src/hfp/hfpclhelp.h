/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
 Copyright 2007-2008 Oleg Fryazinov
 Copyright 2018-2019 Dmitry Popov

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





#if !defined(HFPCLHELP_H)
#define HFPCLHELP_H

#define HELP0 "Usage: hfp FileName [-a Parameters] [-b BoundingBox] [-cf FaceColor]\n [-cl LineColor] [-d DisplayMode] [-g GridDensity] [-gs SlicingGridStep] [-h] [-i IsoValue]\n [-o ObjectName] [-s Search] [-t] [-w WindowSize] [-wrl VRMLOut]\n [-stl STLOut] [-stlb STLOutBinary] [-pov POVOut] [-cli CLIOut]\n [-ct ConstructiveTexturing] [-x Mapping]\n"

#define HELP1 "\nIn the following definitions S=String, I=Integer, D=Double, C=Charecter,\n ....=elipsis, and /=or. Argument templates are given as <Arguments>.\n"

#define HELPFileName "\n FileName is the name of the model or hyperfun text file to use in the\n polygonization. This name must be specified, there is no default.\n"

#define HELPa "\n Parameters: <D,D,D....> (Default Value <0.0,0.0,0.0,....>)\n Given as a list of double values that become the parameter array for the object\n being polygonized. At least one double must be specified. Any double type\n value is acceptable.\n"

#define HELPb "\nBounding Box: <D> <D,D> <D,D,D> <D,D,D,D,D,D> (Default Value <10>)\n Given as a list of double values that become the Bounding Box for the object\n being polygonized. When one argument is given it's +/- values are used to\n define the Bounding Box about each axis. Two arguments give the minumum and\n maximum values around each axis. Three arguments specify the +/- values of\n the X, Y, and Z axis respectively. Six arguments specify the entire Bounding\n Box Xmin, Ymin, Zmin, Xmax, Ymax, and Zmax. Any double type value is\n acceptable. The values for max must be greater than the values for min.\n"

#define HELPcf "\nFace Color: <I> <I,I,I> (Default Value <50,200,80>)\n Given as a list of integer values between 0 and 255 that become the RGB values\n of the Face Color. When one argument is given that same value is used for Red,\n Green, and Blue values. When three arguments are given they are used for Red,\n Green, and Blue values respectively.\n"

#define HELPcl "\nLine Color: <I> <I,I,I> (Default Value <0>)\n Given as a list of integer values between 0 and 255 that become the RGB values\n of the Line Color in Wireframe mode. When one argument is given that same\n value is used for Red, Green, and Blue values. When three arguments are given\n they are used for Red, Green, and Blue values respectively.\n"

#define HELPg "\nGrid Density: <I> <I,I,I> (Default Value <30>)\n Given as a list of integer values between 2 and 250 that specify the Grid\n Density used for polygonization. When one argument is given that same\n value is used for the X, Y, and Z Grid Density values. When three arguments\n are given they are used for the X, Y, and Z Grid Density values respectively.\n"

#define HELPgs "\nSlicing Grid Step: <D> <D,D> <D,D,D> (Default Value <1>)\n Given as a list of double values between 0.001 and 10 that specify the Step of\n Slicing Grid used for getting CLI slices. When one argument is given that same\n value is used for the X, Y, and Z Slicing Grid Step values. When three arguments\n are given they are used for the X, Y, and Z Slicing Grid Step values respectively.\n When two arguments are given the first one is used for the X and Y, the second one - for Z.\n"

#define HELPh "\nHelp: No Arguments.\nThe -h option outputs this usage information.\n"

#define HELPi "\nIso Value: <D> (Default Value <0.0>)\n Given as a single double value. This is the value of the Iso-surface of the\n object being polygonized.\n"

#define HELPo "\nObject Name: <S> (Default Value <my_model>)\n Given as a single string, this is the name of the Object in the Model that is\n to be polygonized. If the given Object does not exist the first object in the\n model is used.\n"

#define HELPt "\nTime Report: No Arguments.\nThe -t option turns on the output of the timing information for the\n polygonization process.\n"

#define HELPx "\nMapping: <D/C,D/C,D/C....> (Default Value <X,Y,Z>)\nGiven as a list of double values or characters that define the mapping for\nobject polygonization. The charecters X, Y, and Z define the X, Y, and Z\nmapping for the object. Other variables will be held constant at the specified\ndouble values.\n"

#define HELPw "\nWindow Size: <I> <I,I> (Default Value <480,320>)\nGiven as a list of integer values that specify the width and height of the\ndisplay window in pixels. The range of values for width is 160 to 1600, and\nfor height is 120 to 1200. When one argument is given that same value is\nused for both the height and width. When two arguments are given they are\nused for the height and width of the window respectively.\n"

#define HELPs "\nSearch: <D> (Default Value <0.1>)\nThis option is used to create a mesh with greater accuracy. A search is done\nfor each vertex point. This search will find the vertex within the specified\nsearch number multiplied by the mesh size.\n"

#define HELPwrl "\nVRML Output: <S> (Default Value <out.wrl>)\nThis option is used to output the resulting mesh in VRML 2.0 format. The\nargument is the name by which the VRML file will be saved.\n"

#define HELPstl "\nSTL Output / STL Binary Output: <S> (Default Value <out.stl>)\nThis option is used to output the resulting mesh in STL format. The\nargument is the name by which the STL file will be saved.\n"

#define HELPpov "\nPOV Output: <S> (Default Value <out.pov>)\nThis option is used to output the resulting mesh in POV format. The\nargument is the name by which the POV file will be saved.\n"

#define HELPcli "\nCLI Output: <S> (Default Value <out.cli>)\nThis option is used to output the resulting mesh in CLI format. The\nargument is the name by which the CLI file will be saved.\n"

#define HELPct "\nConstructive Texturing: \nThis option is used to indicate that attributes should be used.\n"

#define HELPd "\nDisplay Mode: <I> (Default Value <5>)\nThis option is used to set the display type for the mesh. The integer argument\ncan be specified as follows:\n\t1 - Wireframe Only\n\t2 - Surface & Wireframe\n\t3 - Surface with Normals (Diffuse Lighting) \n\t4 - Surface & Wireframe with Normals (Diffuse Lighting) \n\t5 - Surface with Normals (Diffuse & Specular Lighting) \n\t6 - Surface & Wireframe with Normals (Diffuse & Specular Lighting)\n\t7 - Surface with Normals (Diffuse & Specular Lighting) & Normals\nDisplayed \nNote:  Normal calculation is not executed in Display Mode 1 or 2.\nThis will also affect wrl output.\n"

#endif //!defined(HFPCLHELP_H)
