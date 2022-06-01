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
  -- Name:            GENERAL.H
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  contains main data structures used in program.
--
-- Author:          Anatoliy P. Osipov
--
-- Last revised at: 08/18/98
*/


#ifndef MAX_LENGTH_OF_STRING
#define MAX_LENGTH_OF_STRING 256
#endif

#ifndef __GENERAL_H__
#define __GENERAL_H__
//#define HF_DEBUG 1
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

//*****Added 10/07/01

static int Flag_Initialisation=0;
//*******************

struct PN_NODE_T;
typedef struct PN_List_T
{
  struct PN_NODE_T* first;
  struct PN_NODE_T* last;
}PN_List_T;

enum
{
  HF_R, HF_S, HF_A
};

enum
{
  Initialized,
  Not_Initialized
};

/* ARRAYS: */
typedef struct
{
  int     amount;
  int     size;
  double* data;
} Double_Array_T;

void HF_Double_Array_Init(Double_Array_T* ar);
int  HF_Double_Array_Make(Double_Array_T* ar, int default_size);
void HF_Delete_Double_Array(Double_Array_T* ar);
int  HF_Add_To_Double_Array(Double_Array_T* ar, double item);
int  HF_Copy_Double_Array(Double_Array_T* source, Double_Array_T* dest);
int  HF_Get_Size_Of_Double_Array (Double_Array_T* p_ar); /* to calculate the size for storing operation */
void HF_Store_Double_Array (Double_Array_T* p_ar, unsigned char* p_buf);
int  HF_Load_Double_Array (Double_Array_T* p_ar, unsigned char* p_buf);
/***************************************************************************/
typedef struct
{
  int  amount;
  int  size;
  int* data;
} Int_Array_T;

void HF_Int_Array_Init(Int_Array_T* ar);
int  HF_Int_Array_Make(Int_Array_T* ar, int default_size);
void HF_Delete_Int_Array(Int_Array_T* ar);
int  HF_Add_To_Int_Array(Int_Array_T* ar, int item);
int  HF_Copy_Int_Array(Int_Array_T* source, Int_Array_T* dest);
int  HF_Get_Size_Of_Int_Array (Int_Array_T* p_ar); /* to calculate the size for storing operation */
void HF_Store_Int_Array (Int_Array_T* p_ar, unsigned char* p_buf);
int  HF_Load_Int_Array (Int_Array_T* p_ar, unsigned char* p_buf);
/***************************************************************************/
typedef struct
{
  int    amount;
  int    size;
  char*  data;
} String_Array_T;

void    HF_String_Array_Init(String_Array_T* ar);
void    HF_Delete_String_Array(String_Array_T* ar);
int     HF_Add_To_String_Array(String_Array_T* ar, char* item);
char*   HF_Get_By_Index(String_Array_T* ar, int index);
int     HF_Copy_String_Array(String_Array_T* source, String_Array_T* dest);
int     HF_Get_Size_Of_String_Array (String_Array_T* p_ar); /* to calculate the size for storing operation */
void    HF_Store_String_Array (String_Array_T* p_ar, unsigned char* p_buf);
int     HF_Load_String_Array (String_Array_T* p_ar, unsigned char* p_buf);
/***************************************************************************/
typedef struct
{
  int    amount;
  int    size;
  
  char*  string_data;
  int*   int_data;/* index, amount, ar_flag = 1 if array, else = 0 */
} Name_Array_T;

void HF_Name_Array_Init(Name_Array_T* ar);
void HF_Delete_Name_Array(Name_Array_T* ar);
int  HF_Add_To_Name_Array(Name_Array_T* ar, char* item, int i, int c, int f);
int  HF_Find_In_Name_Array(Name_Array_T* ar, char* item, int* i, int* c, int* f);
int  HF_Get_Runtime_Array(int index, double **p, int *size);
/* End of arrays */
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
enum
{
  CODE_String_Index          =  0,
    
  CODE_array_assignment      =  1,
  CODE_assignment            =  2,
  CODE_array_item_assignment =  3,
  CODE_Sarray_assignment      =  4,  /* VAVAVA */
  CODE_Sarray_item_assignment =  5,  /* VAVAVA */ 

  CODE_goto                  =  6,
  CODE_if                    =  7,
  CODE_if_else               =  8,
  CODE_while                 =  9,
  CODE_body_assignment       =  10,
    
  I_ARRAY                    =  11,
  I_ARRAY_AS_ARG             = 12,
  I_EXTERN_CALL              = 13,
    
  FUNC_bool_and              = 14,
  FUNC_bool_or               = 15,
  FUNC_bool_less             = 16,
  FUNC_bool_greater          = 17,
  FUNC_bool_not_greater      = 18,
  FUNC_bool_not_less         = 19,
  FUNC_bool_equal            = 20,
  FUNC_bool_not_equal        = 21,
    
  FUNC_binary_plus           = 22,
  FUNC_binary_minus          = 23,
  FUNC_binary_mult           = 24,
  FUNC_binary_divide         = 25,
  FUNC_binary_power          = 26,
  FUNC_binary_cart           = 27,
  FUNC_binary_ampersand      = 28,
  FUNC_binary_union          = 29,
  FUNC_binary_neg            = 30,
    
  FUNC_unary_not             = 31,
    
  FUNC_unary_plus            = 32,
  FUNC_unary_minus           = 33,
  FUNC_unary_tilda           = 34,
  FUNC_sqrt  = 35,
  FUNC_exp   = 36,
  FUNC_log   = 37,
  FUNC_logd  = 38,
  FUNC_sin   = 39,
  FUNC_cos   = 40,
  FUNC_tan   = 41,
  FUNC_asin  = 42,
  FUNC_acos  = 43,
  FUNC_atan  = 44,
  FUNC_abs   = 45,
  FUNC_sinh  = 46,
  FUNC_cosh  = 47,
  FUNC_tanh  = 48,
  FUNC_sign  = 49,
  FUNC_mod   = 50,
  FUNC_max   = 51,
  FUNC_min   = 52,
  FUNC_atan2 = 53,
  FUNC_ceil = 54,
  FUNC_floor = 55,
  FUNC_int = 56,
  FUNC_frac = 57
};
/*
  For functions there is a range of
  FUNC_bool_and .. FUNC_unary_tilda +
  (amount_of_functions in functions_info)
*/

enum Error_Code
{
  Second_QuatationMark_Absent,
  Wrong_Number_Format,
  Unknown_Lexem,
  Very_Long_Lexem,
  Very_Long_Identificator,
  Body_Name_Expected,
  Not_Unique_Body_Name,
  Left_Parenthes_Expected,
  XIdExpected,
  Left_Bracket_Expected,
  Real_Number_Expected,
  Array_Size_Must_Be_Greater_Zero,
  Right_Bracket_Expected,
  Comma_Expected,
  AIdExpected,
  SIdExpected,             /* VAVAVA */
  Right_Parenthes_Expected,
  Left_Brace_Expected,
  Assign_Mark_Expected,
  Semicolon_Expected,
  Second_Unary_Plus,
  Second_Unary_Minus,
  Second_Unary_Tilda,
  Argument_Error,
  Wrong_Number_Of_Items_In_Array,
  Array_Name_Expected,
  Then_Expected,
  EndIf_Expected,
  Else_EndIf_Expected,
  Loop_Expected,
  EndLoop_Expected,
  Bad_Code,
  Value_Not_Initialized,
  Division_By_Zero,
  Negative_Value_For_Power,
  ALPHA_Range_Error,
  Negative_For_Root,
  Negative_For_Log,
  Function_Not_Implemented_Error,
  Identifier_Expected,
  Right_Brace_Expected,
  Unassigned_Identificator,
  Unassigned_Array,
  Expression_Expected,
  Left_Bracket_OR_Assign_Expected,
  Index_Out_Of_Range,
  Operator_Expected,
  Asin_Acos_Range_Error,
  Tan_Argument_Error,
  Zero_Value_In_Negative_Or_Zero_Power,
  Blend_2_3_Error,
  Stretch_Error
};

enum Tokens
{
  IllegalToken       , /* Error      0 */
    
  ArrayToken         , /* massive    1 */
  IfToken            , /* if         2 */
  ThenToken          , /* then       3 */
  ElseToken          , /* else       4 */
  EndIfToken         , /* end if     5 */
  WhileToken         , /* while      6 */
  LoopToken          , /* loop       7 */
  EndLoopToken       , /* end while  8 */
  AssignToken        , /* =          9 */
    
  LeftParenToken     , /* (         10 */
  RightParenToken    , /* )         11 */
  LeftBracketToken   , /* [         12 */
  RightBracketToken  , /* ]         13 */
  LeftBraceToken     , /* {         14 */
  RightBraceToken    , /* }         15 */
    
  AndToken,            /* and       16 */
  NotToken           , /* not       17 */
  OrToken            , /* or        18 */
  XorToken           , /* xor       19 */
    
  NotEqualToken      , /* /=        20 */
  LessToken          , /* <         21 */
  NotGreaterToken    , /* <=        22 */
  GreaterToken       , /* >         23 */
  NotLessToken       , /* >=        24 */
    
  PowerToken         , /* ^         25 */
  PlusToken          , /* +         26 */
  MinusToken         , /* -         27 */
  TildaToken         , /* ~         28 */
  SlashToken         , /* /         29 */
  MultToken          , /* *         30 */
  AmpersandToken     , /* &         31 */
  UnionToken         , /* |         32 */
  NegToken           , /* \         33 */
  CartProductToken   , /* @         34 */
    
  CommaToken         , /* ,         35 */
  SemicolonToken     , /* ;         36 */
    
  RealValueToken     , /* 1.2e-4.5  37 */
  ConstantStringToken, /* "f.txt"   38 */
  CommentToken       , /* --        39 */
  XIdToken           , /* X array   40 */
  AIdToken           , /* A array   41 */
 /* VAVAVA Token for "S" array */
   SIdToken           , /* S array   42 */  
   FunctionIdToken    , /* COS       43 */
   IdentifierToken    , /* count     44 */

   EndOfParseToken      /*           45 */
};

enum
{
  Variable, Array
};

typedef struct                                       /* Error_Info_T */
{                                                    /* Error_Info_T */
  char* error_text;                                 /* Error_Info_T */
  int line, position;                               /* Error_Info_T */
} Error_Info_T;                                      /* Error_Info_T */

typedef struct                                         /* Pol_Not_T */
{                                                      /* Pol_Not_T */
 /* 1*/ char* body_name;                                /* Pol_Not_T */
/* 2*/ int X_dim;                                      /* Pol_Not_T */
/* 3*/ int A_dim;                                      /* Pol_Not_T */
/* 4*/ int S_dim;         /* VAVAVA */                  /* Pol_Not_T *
/* 5*/ int stack_size;           /* with A, X, S */     /* Pol_Not_T */
/* 6*/ Int_Array_T      code_list;                     /* Pol_Not_T */
/* 7*/ Int_Array_T      index_to_lexem; /* line, pos*/ /* Pol_Not_T */
/* 8*/ Double_Array_T   constant_values;               /* Pol_Not_T */
/* 9*/ String_Array_T   str_param;                     /* Pol_Not_T */
/*10*/ PN_List_T        other_body_code_list;		   /* Pol_Not_T */
/*11*/ int				ad_size;					   /* Pol_Not_T */
} PN_T;

typedef struct                                       /* ScanParamStruct_T */
{                                                    /* ScanParamStruct_T */
  int    lexem_code;                                /* ScanParamStruct_T */
  /* ScanParamStruct_T */
  char   str_buffer[MAX_LENGTH_OF_STRING];          /* ScanParamStruct_T */
  int    str_buf_len;                               /* ScanParamStruct_T */
  int    function_index;                            /* ScanParamStruct_T */
  double real_value;                                /* ScanParamStruct_T */
  /* ScanParamStruct_T */
  int    lexem_start_position;                      /* ScanParamStruct_T */
  int    lexem_end_position;      /*initially = 1*/ /* ScanParamStruct_T */
  /* ScanParamStruct_T */
  int    lexem_start_line;                          /* ScanParamStruct_T */
  int    lexem_end_line;          /*initially = 1*/ /* ScanParamStruct_T */
  /* ScanParamStruct_T */
  int    str_position;            /*initially = 0*/ /* ScanParamStruct_T */
  /* ScanParamStruct_T */
  int    error_no;                                  /* ScanParamStruct_T */
} ScanParamStruct_T;                                 /* ScanParamStruct_T */

#define ARGS_LIMIT 10

typedef double (*p_F_T)(double* f_a, String_Array_T* str_param);
typedef int (*p_FR_T)(double*         f_a,
		      String_Array_T* str_param,
		      Error_Info_T*   error_info,
                      PN_T*           p_pn,
		      int             code_index);

typedef struct                                         /* function_info_T */
{                                                      /* function_info_T */
  char*  f_name;                                      /* function_info_T */
  int    dim;                                         /* function_info_T */
  int    function_args[ARGS_LIMIT];                   /* function_info_T */
  p_F_T  p_Function;                                  /* function_info_T */
  p_FR_T p_F_Restriction;                             /* function_info_T */
  
  int    F_instance_counter;                          /* function_info_T */
} function_info_T;                                     /* function_info_T */
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* LIST: */
typedef struct ER_NODE_T
{
  struct ER_NODE_T* next;
  Error_Info_T      er_info;
}ER_NODE_T;


typedef struct ER_List_T
{
  ER_NODE_T* first;
  ER_NODE_T* last;
}ER_List_T;

/********************************************************************/
void           HF_Init_ER_List     (ER_List_T* head);
void           HF_Empty_ER_List    (ER_List_T* head);
int            HF_Add_To_ER_List   (ER_List_T* head, char* text, int line, int pos);
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
typedef struct PN_NODE_T
{
  struct PN_NODE_T* next;
  PN_T*             pn_info;
}PN_NODE_T;

/********************************************************************/
void			HF_Init_PN_List       (PN_List_T* head);
void			HF_Empty_PN_List      (PN_List_T* head);
int				HF_Add_To_PN_List     (PN_List_T* head, PN_T* data);
PN_T*			HF_Body_By_Index(PN_List_T* p_lib, int index);
/********************************************************************/
/* END OF LIST */
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
int  HF_CreateNewPNS(PN_T** p_p_pns);
void HF_DeletePNS(PN_T** p_p_pns);

int HF_get_token(char* p_str, ScanParamStruct_T* sps);

/* the memory allocated by HF_parse() needs to be freed by the client 
	for example with the polygonizer: 
	the memory allocated by HF_parse is freed inside the destructor 
	of the wrapper HFinterpreter.
 */
int HF_parse(char* p_program, PN_List_T* p_pn_list, ER_List_T* p_er_list, PN_List_T* p_Library);

/* VAVAVA double* svalue - values of "S" array returned */ 
int HF_calc(PN_T* p_pn, double* stack, int* stack_usage, double* value, double* svalue, Error_Info_T* error_info);

/* SERIALIZATION: */
int  HF_Get_Buffer_Size (   PN_List_T*  p_Library);
void HF_Store_Library   (   PN_List_T*  p_Library,
			    unsigned char*  pp_buffer);
int  HF_Load_Library    (   PN_List_T*      p_Library,
			    unsigned char*  pp_buffer);


#endif /* __GENERAL_H__ */
