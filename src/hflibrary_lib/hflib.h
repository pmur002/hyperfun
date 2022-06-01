/*==============================================================================

 Copyright 1998, 1999 Valery Adzhiev, Alexander Pasko, Ken Yoshikawa 
 Copyright 2003-2004 Benjamin Schmitt

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
-- Name:            hfLib.h
--
-- Description:     This file in a part of HyperFun 1.0 interpreter. 
--                  It contains declarations of external functions
--                  of the core HyperFun library hfLib.    
--
-- Author:          Adzhiev V. Dzhelembievich
--
-- Last revised at: 4.8.98
*/
#include "general.h"
#ifndef HFLIB
#define HFLIB

double hfSphere(double* f_a, String_Array_T* str_param);

double hfEllipsoid(double* f_a, String_Array_T* str_param);

double hfTorusZ(double* f_a, String_Array_T* str_param);

double hfTorusX(double* f_a, String_Array_T* str_param);

double hfTorusY(double* f_a, String_Array_T* str_param);

double hfEllCylX(double* f_a, String_Array_T* str_param);

double hfEllCylY(double* f_a, String_Array_T* str_param);

double hfEllCylZ(double* f_a, String_Array_T* str_param);

double hfCylinderZ(double* f_a, String_Array_T* str_param);

double hfCylinderY(double* f_a, String_Array_T* str_param);

double hfCylinderX(double* f_a, String_Array_T* str_param);

double hfBlock(double* f_a, String_Array_T* str_param); 

double hfBlobby(double* f_a, String_Array_T* str_param); 
int R_hfBlobby(double* f_a, 
           String_Array_T* str_param, 
           Error_Info_T* error_info, 
           PN_T* p_pn, 
           int code_index); 

double hfMetaball(double* f_a, String_Array_T* str_param); 
int R_hfMetaball(double* f_a, 
           String_Array_T* str_param, 
           Error_Info_T* error_info, 
           PN_T* p_pn, 
           int code_index); 

double hfSoft(double* f_a, String_Array_T* str_param); 
int R_hfSoft(double* f_a, 
           String_Array_T* str_param, 
           Error_Info_T* error_info, 
           PN_T* p_pn, 
           int code_index); 


//Added 11/15/99
double hfBezierVolume(double* f_a, String_Array_T* str_param); 
int R_hfBezierVolume(double* f_a, 
           String_Array_T* str_param, 
           Error_Info_T* error_info, 
           PN_T* p_pn, 
           int code_index); 




double hfConvPoint(double* f_a, String_Array_T* str_param);

double hfConvLine(double* f_a, String_Array_T* str_param);

double hfConvArc(double* f_a, String_Array_T* str_param);

double hfConvTriangle(double* f_a, String_Array_T* str_param);

double hfConvLineR(double* f_a, String_Array_T* str_param);

double hfConvCurve(double* f_a, String_Array_T* str_param);

double hfConvMesh(double* f_a, String_Array_T* str_param);

double hfNoiseG(double* f_a, String_Array_T* str_param);

double hfSuperell(double* f_a, String_Array_T* str_param);

double hfConeX(double* f_a, String_Array_T* str_param);

double hfConeY(double* f_a, String_Array_T* str_param);

double hfConeZ(double* f_a, String_Array_T* str_param);

double hfEllConeX(double* f_a, String_Array_T* str_param);

double hfEllConeY(double* f_a, String_Array_T* str_param);

double hfEllConeZ(double* f_a, String_Array_T* str_param);

/****ADDED 11/11/00************/
double hfCubicBSpline(double* f_a, String_Array_T* str_param);
double hfDebugBSpline(double* f_a, String_Array_T* str_param);
double hfSpaceMapExp(double* f_a, String_Array_T* str_param);

/*-------- Operations --------------------------------------------*/
double hfScale3D(double* f_a, String_Array_T* str_param);
int R_hfScale3D(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfShift3D(double* f_a, String_Array_T* str_param);
int R_hfShift3D(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfRotate3DZ(double* f_a, String_Array_T* str_param);
int R_hfRotate3DZ(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfRotate3DY(double* f_a, String_Array_T* str_param);
int R_hfRotate3DY(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfRotate3DX(double* f_a, String_Array_T* str_param);
int R_hfRotate3DX(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfBlendUni(double* f_a, String_Array_T* str_param);
int R_hfBlendUni(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfBlendInt(double* f_a, String_Array_T* str_param);
int R_hfBlendInt(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTwistZ(double* f_a, String_Array_T* str_param);
int R_hfTwistZ(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTwistY(double* f_a, String_Array_T* str_param);
int R_hfTwistY(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTwistX(double* f_a, String_Array_T* str_param);
int R_hfTwistX(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfStretch3D(double* f_a, String_Array_T* str_param);
int R_hfStretch3D(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTaperZ(double* f_a, String_Array_T* str_param);
int R_hfTaperZ(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTaperY(double* f_a, String_Array_T* str_param);
int R_hfTaperY(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

double hfTaperX(double* f_a, String_Array_T* str_param);
int R_hfTaperX(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);

/************Added 11/11/00*/
double hfSpaceMapCubic(double* f_a, String_Array_T* str_param);
int R_hfSpaceMapCubic(double* f_a,
            String_Array_T* str_param,
            Error_Info_T* error_info,
            PN_T* p_pn,
            int code_index);



/*********************************************/
//DEGUGGING
double hfVoxel(double* f_a, String_Array_T* str_param);

/****************ADDED FOR ATTRIBUTES********/






double hfA_CheckerBoard(double* f_a, String_Array_T* str_param);
double hfA_Circles(double* f_a, String_Array_T* str_param);
double hfA_Clamp(double* f_a, String_Array_T* str_param);
double hfA_Crackles(double* f_a, String_Array_T* str_param);
double hfA_Floor(double* f_a, String_Array_T* str_param);
double hfA_Lerp(double* f_a, String_Array_T* str_param);
double hfA_Cerp(double* f_a, String_Array_T* str_param);
double hfA_LookUpTable(double* f_a, String_Array_T* str_param);
double hfA_Modulo(double* f_a, String_Array_T* str_param);
double hfA_NoiseG(double* f_a, String_Array_T* str_param);
double hfA_NoiseP(double* f_a, String_Array_T* str_param);
double hfA_Pulse(double* f_a, String_Array_T* str_param);
double hfA_SetAttributes(double* f_a, String_Array_T* str_param);
double hfA_SetColor(double* f_a, String_Array_T* str_param);
double hfA_SmoothStep(double* f_a, String_Array_T* str_param);
double hfA_SmoothLookUpTable(double* f_a, String_Array_T* str_param);
double hfA_Spline(double* f_a, String_Array_T* str_param);
double hfA_Step(double* f_a, String_Array_T* str_param);
double hfA_Turbulence(double* f_a, String_Array_T* str_param);
double hfA_Union(double* f_a, String_Array_T* str_param);
double hfA_Wave(double* f_a, String_Array_T* str_param);

double hfA_HSVToRGB(double* f_a, String_Array_T* str_param);
double hfA_RGBToHSV(double* f_a, String_Array_T* str_param);


/* SARDF */
double hfDBlock (double *f_a, String_Array_T *str_param);
double hfDConeX (double *f_a, String_Array_T *str_param);
double hfDConeY (double *f_a, String_Array_T *str_param);
double hfDConeZ (double *f_a, String_Array_T *str_param);
double hfDTorusX (double *f_a, String_Array_T *str_param);
double hfDTorusY (double *f_a, String_Array_T *str_param);
double hfDTorusZ (double *f_a, String_Array_T *str_param);
double hfDCylinder (double *f_a, String_Array_T *str_param);
double hfDEllipsoid (double *f_a, String_Array_T *str_param);

double hfDUni(double *f_a, String_Array_T *str_param);
double hfDInt(double *f_a, String_Array_T *str_param);
double hfDSub(double *f_a, String_Array_T *str_param);
double hfDBlendUni(double *f_a, String_Array_T *str_param);
double hfDBlendInt(double *f_a, String_Array_T *str_param);

double hfSawtooth(double *f_a, String_Array_T *str_param);

#endif 
