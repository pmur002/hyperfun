/*==============================================================================

 Copyright 1998, 1999 Valery Adzhiev, Anatoly Ossipov
 Copyright 2000-2004 Valery Adzhiev
 Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt

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

/*
-- Name:            GLOBAL.C
--
-- Description:     This file in a part of HyperFun 1.0 interpreter. It
--                  contains error string interpreter messages and
--                  implements build-in function support.
--
-- Author:          Anatoliy P. Osipov
--
-- Last revised at: 14/01/01 by V.Adzhiev
-- All those modifications are marked by "-- VAVAVA"
-- last revised at: 29/12/03 by pa fayolle
-- All those modifications are marked by "pa"
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hflib.h"

double EPS = 0.1e-5;
double EPS_COLOR = 1e-5;
double ALPHA = 0.0;

void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);
extern int HF_round(double);
extern int HF_Get_Runtime_Array(int index, double** array, int* size);

int ZERO_position;

/* ERRORS SECTION **********************************************************/
char* Error_Strings[] =
{
   "Second quotation mark expected",
   "Wrong format of number",
   "Unknown lexem found",
   "Lexem is very long",
   "Very long identifier",
   "Body name expected",
   "Body name is not unique",
   "Left parenthes expected",
   "\"X\" expected",
   "Left bracket expected",
   "Real number expected",
   "Array size must be greater zero",
   "Right bracket expected",
   "Comma expected",
   "\"A\" expected",
	 "\"S\" expected",
   "Right parenthes expected",
   "Left brace expected",
   "Assign mark expected",
   "Semicolon expected",
   "Second unary plus is not allowed",
   "Second unary minus is not allowed",
   "Second unary tilda is not allowed",
   "Error in argument in function call",
   "Wrong number of items in array",
   "Only array name expected",
   "Reserved word \"then\" expected",
   "Reserved word \"endif\" expected",
   "Reserved words \"else\" or \"endif\" expected",
   "Reserved word \"loop\" expected",
   "Reserved word \"endloop\" expected",
   "Bad code is generated for body",
   "Not initialized variable",
   "Division by zero",
   "Negative value for power",
   "Value of ALPHA should be in range 0.0 .. 1.0",
   "Negative value for square root",
   "Negative for log function",
   "Function not implemented yet",
   "Identifier expected",
   "Right brace expected",
   "Unassigned identificator used in expression",
   "Some items of array are unassigned",
   "Expression expected",
   "Left bracket or assign mark expected",
   "Array index out of range",
   "Identifier, \"if\" or \"while\"  expected",
   "Argument must be in range -1.0 .. +1.0",
   "Cannot compute tan for this argument",
   "Zero value in negative or zero power",
   "Invalid parameters for blend",
   "Stretch: scale parameter is zero"
};
/* END OF ERRORS SECTION ***************************************************/

/* Functions definition section:********************************************/
double TEST(double* f_a, String_Array_T* str_param)
{/* ARRAY PASSING DEMO FUNCTION */
/*
-- In the language the call to function is:
--
--    test(real_number, array_name, string);
--
-- Array name is the second parameter and array "f_a" contains arrguments in
-- reverse order, so information about the array is in the 1 item of "f_a".
-- Information about the array is the integer index. Next step is to pass
-- this index to function "HF_Get_Runtime_Array". This function returns
-- the pointer to array and size of it.
-- DON'T modify the array items! These items are the actual ones in the program.
-- These items is for read-only purpose.
-- DON't take the items of array beyond the range: 0, 1, 2, ..., size-1.
--
-- Initialization of array items is checked automaticaly.
*/
   int array_index = (int)f_a[1];
   int     amount;
   double* p_array;

   int a_param;

   HF_Get_Runtime_Array(array_index, &p_array, &amount);

   a_param = HF_round(f_a[2]);
   if((-1<a_param) && (a_param<amount))
      return p_array[a_param]; /* Returns array item indexed by first argument */
   return -1111111111111.0; /* This situation may also be checked by R_TEST fucntion
                               it depeneds on the fucntion semantic.
                            */
}

int R_TEST(double* f_a,
	   String_Array_T* str_param,
	   Error_Info_T* error_info,
	   PN_T* p_pn,
	   int code_index)
{
   int array_index = (int)f_a[1];
   int     amount;
   double* p_array;

   if(HF_Get_Runtime_Array(array_index, &p_array, &amount))
      return 1;

   HF_Fill_Error(p_pn, Bad_Code, error_info, code_index);
   return 0; /* Error */
/* See above TEST function for example of array passing as an argument */
}

double STR_T(double* f_a, String_Array_T* str_param)
{
   if(!strcmp(HF_Get_By_Index(str_param, (int)f_a[0]), "Hey stub!"))
      return 10101010.0;
   return -1.0;
}

/* End of functions definition section *************************************/

function_info_T functions_info[] =
{
/* Unary functions */
/* DON'T MODIFY */  {"sqrt", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  0                              */
/* DON'T MODIFY */  {"exp",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  1                              */
/* DON'T MODIFY */  {"log",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  2                              */
/* DON'T MODIFY */  {"logd", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  3                              */
/* DON'T MODIFY */  {"sin",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  4                                */
/* DON'T MODIFY */  {"cos",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  5                                */
/* DON'T MODIFY */  {"tan",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  6                                */
/* DON'T MODIFY */  {"asin", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  7                                */
/* DON'T MODIFY */  {"acos", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /*  8                                */
/* DON'T MODIFY */  {"atan", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 13                               */
/* DON'T MODIFY */  {"abs",  1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 10                               */
/* DON'T MODIFY */  {"sinh", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 11                               */
/* DON'T MODIFY */  {"cosh", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 12                               */
/* DON'T MODIFY */  {"tanh", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 13                               */
/* DON'T MODIFY */  {"sign", 1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 14                               */
                     /* Binary functions */
/* DON'T MODIFY */  {"mod",  2, {HF_R,HF_R,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 15                               */
/* DON'T MODIFY */  {"max",  2, {HF_R,HF_R,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 16                               */
/* DON'T MODIFY */  {"min",  2, {HF_R,HF_R,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 17                               */
/* DON'T MODIFY */  {"atan2",2, {HF_R,HF_R,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 18                               */
/* DON'T MODIFY */  {"ceil",1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 19                               */
/* DON'T MODIFY */  {"floor",1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 20                               */
/* DON'T MODIFY */  {"int",1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 21                               */
/* DON'T MODIFY */  {"frac",1, {HF_R,0,0,0,0,0,0,0,0,0}, NULL, NULL, 0},    /* 22                               */

/* DON'T MODIFY THE LINES ABOVE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


           /* Next list can be modified:  */
/* Customized functions: */
/* 23 */  {"hfsphere",       3, { HF_A, HF_A, HF_R, 0,    0,    0,    0,    0, 0, 0 }, hfSphere,       NULL, 0          },
/* 24 */  {"hfellipsoid",    5, { HF_A, HF_A, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfEllipsoid,    NULL, 0          },
/* 25 */  {"hftorusz",       4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfTorusZ,       NULL, 0          },
/* 26 */  {"hftorusx",       4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfTorusX,       NULL, 0          },
/* 27 */  {"hftorusy",       4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfTorusY,       NULL, 0          },
/* 28 */  {"hfellcylx",      4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllCylX,      NULL, 0          },
/* 29 */  {"hfellcyly",      4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllCylY,      NULL, 0          },
/* 30 */  {"hfellcylz",      4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllCylZ,      NULL, 0          },
/* 31 */  {"hfcylinderx",    3, { HF_A, HF_A, HF_R, 0,    0,    0,    0,    0, 0, 0 }, hfCylinderX,    NULL, 0          },
/* 32 */  {"hfcylindery",    3, { HF_A, HF_A, HF_R, 0,    0,    0,    0,    0, 0, 0 }, hfCylinderY,    NULL, 0          },
/* 33 */  {"hfcylinderz",    3, { HF_A, HF_A, HF_R, 0,    0,    0,    0,    0, 0, 0 }, hfCylinderZ,    NULL, 0          },
/* 34 */  {"hfblock",        5, { HF_A, HF_A, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfBlock,        NULL, 0          },
/* 35 */  {"hfblobby",       7, { HF_A, HF_A, HF_A, HF_A, HF_A, HF_A, HF_R, 0, 0, 0 }, hfBlobby,       NULL, 0          },
/* 36 */  {"hfmetaball",     7, { HF_A, HF_A, HF_A, HF_A, HF_A, HF_A, HF_R, 0, 0, 0 }, hfMetaball,     NULL, 0          },
/* 37 */  {"hfsoft",         6, { HF_A, HF_A, HF_A, HF_A, HF_A, HF_R, 0,    0, 0, 0 }, hfSoft,         NULL, 0          },

/* 38 */  {"hfscale3d",      4, { HF_A, HF_R, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfScale3D,      NULL, 0          },
/* 39 */  {"hfshift3d",      4, { HF_A, HF_R, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfShift3D,      NULL, 0          },
/* 40 */  {"hfrotate3dz",    2, { HF_A, HF_R, 0,    0,    0,    0,    0,    0, 0, 0 }, hfRotate3DZ,    NULL, 0          },
/* 41 */  {"hfrotate3dy",    2, { HF_A, HF_R, 0,    0,    0,    0,    0,    0, 0, 0 }, hfRotate3DY,    NULL, 0          },
/* 42 */  {"hfrotate3dx",    2, { HF_A, HF_R, 0,    0,    0,    0,    0,    0, 0, 0 }, hfRotate3DX,    NULL, 0          },
/* 43 */  {"hfblenduni",     5, { HF_R, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfBlendUni,     NULL, 0          },
/* 44 */  {"hfblendint",     5, { HF_R, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfBlendInt,     NULL, 0          },
/* 45 */  {"hftwistz",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTwistZ,       NULL, 0          },
/* 46 */  {"hftwisty",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTwistY,       NULL, 0          },
/* 47 */  {"hftwistx",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTwistX,       NULL, 0          },
/* 48 */  {"hfstretch3d",    5, { HF_A, HF_A, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfStretch3D,    NULL, 0          },
/* 49 */  {"hftaperz",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTaperZ,       NULL, 0          },
/* 50 */  {"hftapery",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTaperY,       NULL, 0          },
/* 51 */  {"hftaperx",       5, { HF_A, HF_R, HF_R, HF_R, HF_R, 0,    0,    0, 0, 0 }, hfTaperX,       NULL, 0          },
//Addition 11/15/99
/* 49 */  {"hfconvpoint",    4, { HF_A, HF_A, HF_A, HF_R, 0,    0,    0,    0, 0, 0 }, hfConvPoint,    NULL, 0			},
/* 50 */  {"hfconvline",     5, { HF_A, HF_A, HF_A, HF_A, HF_R, 0,    0,    0, 0, 0 }, hfConvLine,     NULL, 0			},
/* 51 */  {"hfconvarc",      8, { HF_A, HF_A, HF_A, HF_A, HF_A, HF_A, HF_A, HF_R, 0,0},hfConvArc,      NULL, 0			},
/* 52 */  {"hfconvtriangle", 4, { HF_A, HF_A, HF_A, HF_R, 0,    0,    0,    0, 0, 0 }, hfConvTriangle, NULL, 0			},
/* 53 */  {"hfbeziervolume", 6, { HF_A, HF_R, HF_R, HF_R, HF_A, HF_A, 0,    0, 0, 0 }, hfBezierVolume, NULL, 0			},
/* 55 */  {"hfconvliner",    4, { HF_A, HF_A, HF_A, HF_R, 0,	0,    0,    0, 0, 0 }, hfConvLineR,	   NULL, 0			},
//Addition 03/11/00
/* 58 */  {"hfconvcurve",    4, { HF_A, HF_A, HF_A, HF_R, 0,    0,    0,    0, 0, 0 }, hfConvCurve,    NULL, 0          },
/* 59 */  {"hfconvmesh",     5, { HF_A, HF_A, HF_A, HF_A, HF_R, 0,    0,    0, 0, 0 }, hfConvMesh,     NULL, 0          },
/* 60 */  {"hfnoiseg",       4, { HF_A, HF_R, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfNoiseG,       NULL, 0          },
/* 61 */  {"hfsuperell",     7, { HF_A, HF_A, HF_R, HF_R, HF_R, HF_R, HF_R, 0, 0, 0 }, hfSuperell,     NULL, 0          },
/* 62 */  {"hfconex",        3, { HF_A, HF_A, HF_R,    0, 0,    0,    0,    0, 0, 0 }, hfConeX,        NULL, 0          },
/* 63 */  {"hfconey",        3, { HF_A, HF_A, HF_R,    0, 0,    0,    0,    0, 0, 0 }, hfConeY,        NULL, 0          },
/* 64 */  {"hfconez",        3, { HF_A, HF_A, HF_R,    0, 0,    0,    0,    0, 0, 0 }, hfConeZ,        NULL, 0          },
/* 65 */  {"hfellconex",     4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllConeX,     NULL, 0          },
/* 66 */  {"hfellconey",     4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllConeY,     NULL, 0          },
/* 67 */  {"hfellconez",     4, { HF_A, HF_A, HF_R, HF_R, 0,    0,    0,    0, 0, 0 }, hfEllConeZ,     NULL, 0          },
//Addition 11/11/00
/* 68 */  //{"hfcubicbspline", 6, { HF_A, HF_R, HF_R, HF_R,HF_A , HF_A,    0,    0, 0, 0 }, hfCubicBSpline,     NULL, 0          },
/* 69 */  {"hfspacemapcubic", 4, { HF_A, HF_A, HF_A, HF_A, 0,    0,    0,    0, 0, 0 }, hfSpaceMapCubic,     NULL, 0          },
/* 70 */  {"hfspacemapexp", 4, { HF_A, HF_A, HF_A, HF_A, 0,    0,    0,    0, 0, 0 }, hfSpaceMapExp,     NULL, 0          },
/* 71 */  //{"hfvoxel", 2, { HF_A, HF_R, 0, 0, 0,    0,    0,    0, 0, 0 }, hfVoxel,     NULL, 0          },
  
/* // Examples:
 *  {"test", 3, {HF_R,HF_A,HF_S,0,0,0,0,0,0,0}, TEST,  R_TEST, 0 },
 *  {"str_t",1, {HF_S,0,0,0,0,0,0,0,0,0}, STR_T, NULL, 0},
 */
/**********/

/* Attributes */  
/*A-01*/			{"hfA_Wave", 4, { HF_R, HF_R, HF_R, HF_R,		0,  0,	  0,	0, 0 },				hfA_Wave,     NULL, 0          },
/*A-02*/			{"hfA_CheckerBoard", 6, { HF_A, HF_A, HF_A,	HF_A,HF_A,	HF_A,	  0,	0, 0 },		hfA_CheckerBoard,     NULL, 0          },
/*A-03*/			{"hfA_Union", 6, { HF_R, HF_R, HF_A,HF_A,HF_A,HF_R, 0,	0, 0 },						hfA_Union,     NULL, 0          },
/*A-04*/			{"hfA_Crackles", 5, { HF_A,HF_A, HF_R, HF_A, HF_A, 0, 0,	0, 0 },					hfA_Crackles,     NULL, 0          },
/*A-05*/			{"hfA_Circles", 4, { HF_A,HF_A,  HF_A, HF_A, 0, 0,	0, 0 },							hfA_Circles,     NULL, 0          },
/*A-06*/			{"hfA_NoiseG", 3, { HF_A, HF_R, HF_R, 0, 0, 0,	  0,	0, 0 },						hfA_NoiseG,   NULL, 0          },
/*A-07*/			{"hfA_NoiseP", 1, { HF_A,0,  0, 0, 0, 0,	0, 0 },									hfA_NoiseP,     NULL, 0          },
/*A-08*/			{"hfA_Turbulence", 2, { HF_A,HF_R,  0, 0, 0, 0,	0, 0 },								hfA_Turbulence,     NULL, 0          },
/*A-09*/			{"hfA_SetColor", 5, { HF_R,HF_A,  HF_R, HF_R, HF_R, 0,	0, 0 },						hfA_SetColor,     NULL, 0          },
/*A-10*/			{"hfA_SetAttributes", 3, { HF_R,HF_A,  HF_A, 0, 0, 0,	0, 0 },						hfA_SetAttributes,     NULL, 0          },
/*A-11*/			{"hfA_Lerp", 6, { HF_A,HF_A,  HF_A, HF_R, HF_R, HF_R,	0, 0 },						hfA_Lerp,     NULL, 0          },
/*A-11*/			{"hfA_Cerp", 6, { HF_A,HF_A,  HF_A, HF_R, HF_R, HF_R,	0, 0 },						hfA_Lerp,     NULL, 0          },
/*A-12*/			{"hfA_Clamp", 3, { HF_R,HF_R,  HF_R, 0, 0, 0,	0, 0 },								hfA_Clamp,     NULL, 0          },
/*A-13*/			{"hfA_Floor", 1, { HF_R,0,  0, 0, 0, 0,	0, 0 },										hfA_Floor,     NULL, 0          },
/*A-14*/			{"hfA_Pulse", 3, { HF_R,HF_R,  HF_R, 0, 0, 0,	0, 0 },								hfA_Pulse,     NULL, 0          },
/*A-15*/			{"hfA_LookUpTable", 4, { HF_R,HF_A,  HF_A, HF_A, 0, 0,	0, 0 },						hfA_LookUpTable,     NULL, 0          },
/*A-16*/			{"hfA_SmoothLookUpTable", 4, { HF_R,HF_A,  HF_A, HF_A, 0, 0,	0, 0 },				hfA_SmoothLookUpTable,     NULL, 0          },
/*A-17*/			{"hfA_Step", 2, { HF_R,HF_R,  0, 0, 0, 0,	0, 0 },									hfA_Step,     NULL, 0          },
/*A-18*/			{"hfA_SmoothStep", 6, { HF_R, HF_R, HF_R,	0, 0 },									hfA_SmoothStep,     NULL, 0          },
/*A-19*/			{"hfA_Modulo", 2, { HF_R,HF_R,  0, 0, 0, 0,	0, 0 },									hfA_Modulo,     NULL, 0          },
/*A-20*/			{"hfA_Spline", 5, { HF_A,HF_A,  HF_R, HF_R, HF_R, 0,	0, 0 },						hfA_Spline,     NULL, 0          },
//Added 2004_01_13
/*A-21*/			{"hfA_RGBToHSV", 2, { HF_A,HF_A,  0, 0, 0, 0,	0, 0,0 },							hfA_RGBToHSV,     NULL, 0          },
/*A-22*/			{"hfA_HSVToRGB", 2, { HF_A,HF_A,  0, 0, 0, 0,	0, 0,0 },							hfA_HSVToRGB,     NULL, 0          },

/* sardef function and distance primitives */
//{"hfDBlock", 5, { HF_A, HF_A, HF_R, HF_R,		HF_R,  0,	  0,	0, 0 ,0},				hfDBlock,     NULL, 0          },
{"hfDConeX", 3, { HF_A, HF_A, HF_R, 0,		0,  0,	  0,	0, 0 ,0},				hfDConeX,     NULL, 0          },
{"hfDConeY", 3, { HF_A, HF_A, HF_R, 0,		0,  0,	  0,	0, 0 ,0},				hfDConeY,     NULL, 0          },
{"hfDConeZ", 3, { HF_A, HF_A, HF_R, 0,		0,  0,	  0,	0, 0 ,0},				hfDConeZ,     NULL, 0          },
{"hfDCylinder", 4, { HF_A, HF_A, HF_A, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDCylinder,     NULL, 0          },
{"hfDEllipsoid", 5, { HF_A, HF_A, HF_R, HF_R,		HF_R,  0,	  0,	0, 0 ,0},				hfDEllipsoid,     NULL, 0          },
{"hfDTorusX", 4, { HF_A, HF_A, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDTorusX,     NULL, 0          },
{"hfDTorusY", 4, { HF_A, HF_A, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDTorusY,     NULL, 0          },
{"hfDTorusZ", 4, { HF_A, HF_A, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDTorusZ,     NULL, 0          },

{"hfDUni", 4, { HF_R, HF_R, HF_R, HF_R,		0,  0,	  0,	0, 0,0 },				hfDUni,     NULL, 0          },
{"hfDInt", 4, { HF_R, HF_R, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDInt,     NULL, 0          },
{"hfDSub", 4, { HF_R, HF_R, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfDSub,     NULL, 0          },

{"hfDBlendUni", 3, { HF_R, HF_R, HF_R, 0,		0,  0,	  0,	0, 0 ,0},				hfDBlendUni,     NULL, 0          },
{"hfDBlendInt", 3, { HF_R, HF_R, HF_R, 0,		0,  0,	  0,	0, 0 ,0},				hfDBlendInt,     NULL, 0          },

{"hfSawtooth", 4, { HF_R, HF_R, HF_R, HF_R,		0,  0,	  0,	0, 0 ,0},				hfSawtooth,     NULL, 0          },

  {NULL,   0, {0,0,0,0,0,0,0,0,0,0}, NULL,  NULL, 0} /* This last string is REQUIRED for correct work !!!*/
};

int HF_FUNCTIONS_AMOUNT = (((sizeof (functions_info)) / sizeof(functions_info[0])) - 1);
