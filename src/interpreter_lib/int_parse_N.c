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
  -- Name:            PARSE.C
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  implements parse stage of interpreter.
  --
  -- Author:          Anatoliy P. Osipov
  --
  -- Last revised at: 09/11/2001
  --				30 septembre 2003
  --
  -- Modifications: 
					* added parsing for s array attributes;
					* added possibility to parse both definition of functions with
					or without s array; e.g. my_model(x[],a[]) and
					my_model(x[],a[],s[]) are both valid;
					* when an external call to a function is parsed
					and when one of the array passed is not x, and not a, and not s
					but a local variable on the stack
					the index computed was wrong; it was computed as:
					index += p_pns->X_dim + p_pns->A_dim
					whereas it should have been: index += ..... + p_pns->S_dim;                  
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "general.h"


extern int CODE_First_Data_Index;
extern function_info_T functions_info[];

PN_List_T* HF_library;
PN_List_T* HF_local_library;

PN_T* HF_Copy_PNS(PN_T* p_pns); /* preliminary function declaration */

extern int HF_FUNCTIONS_AMOUNT;


int HF_Copy_PNS_List(PN_List_T* source, PN_List_T* dest)
{
  PN_NODE_T*	current;
  PN_T*		temp_data;
  int status = 1;

	if (source == NULL || dest == NULL)
		return 0;

  current = source->first;

  while((current != NULL) && status)
    {
      temp_data = HF_Copy_PNS(current->pn_info);
      if(temp_data != NULL)
	status = HF_Add_To_PN_List(dest, temp_data);
      else
	status = 0;
      current = current->next;
    }
  return status;
}


int HF_GEN_CODE(PN_T* p_pns, int code, int line, int pos)
{
  /*
    -- Function description:
    --  Function inserts the code into the code array for body and
    --  makes the referrence(line/position) to the source text program.
  */
  int memory_status;
  memory_status = HF_Add_To_Int_Array(&p_pns->code_list, code);

	if (!memory_status)
		return memory_status;

  memory_status = HF_Add_To_Int_Array(&p_pns->index_to_lexem, line);

	if (!memory_status)
		return memory_status;

  memory_status = HF_Add_To_Int_Array(&p_pns->index_to_lexem, pos);

  return memory_status;
}

void HF_Skip_To_Semicolon(char* p_program, ScanParamStruct_T* p_sps)
{
  /*
    -- Function description:
    --  Function skips lexems up to the found semicoln mark.
  */
  HF_get_token(p_program, p_sps);
  while((p_sps->lexem_code != EndOfParseToken) &&
	(p_sps->lexem_code != SemicolonToken))
    {
      HF_get_token(p_program, p_sps);
    }
}

extern char* Error_Strings[];

int HF_Get_Expression(char*              p_program,
                      PN_T*              p_pns,
                      ER_List_T*         p_error_list,
                      ScanParamStruct_T* p_sps,
                      Name_Array_T*      NAMES,
                      int*               p_er);
int HF_Get_Bool_Expr(char*              p_program,
                     PN_T*              p_pns,
                     ER_List_T*         p_error_list,
                     ScanParamStruct_T* p_sps,
                     Name_Array_T*      NAMES,
                     int*               p_er);
int HF_Parse_Operators(char*              p_program,
                       PN_T*              p_pns,
                       ER_List_T*         p_error_list,
                       ScanParamStruct_T* p_sps,
                       Name_Array_T*      NAMES,
                       int*               p_er);

int HF_round(double d)
{
  /*
    -- Function description:
    --  Function rounds double value to the integer type.
  */
  double c = ceil(d);
  return (int)((c-d>0.5)?(c-1):c);
}

int HF_Was_Error(ER_List_T* p_error_list)
{
  /*
    -- Function description:
    --  Function checks whether error was or not.
  */
  return (p_error_list->first != NULL);
}

int HF_For_Bool(char*              p_program,
                PN_T*              p_pns,
                ER_List_T*         p_error_list,
                ScanParamStruct_T* p_sps,
                Name_Array_T*      NAMES,
                int*               p_er)
{
  /*
    -- Function description:
    --  Low level function for parsing bool expressions tokens.
  */
  int memory_status = 1;
  int g_line, g_pos;
  *p_er = 0;

  switch(p_sps->lexem_code)
    {
    case NotToken:
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Bool(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			if (!memory_status || *p_er)
				return memory_status;

      /* GENERATING CODE:*********************************************************/
      memory_status = HF_GEN_CODE(p_pns, FUNC_unary_not, g_line, g_pos);

			if (!memory_status)
				return memory_status;

      /* GENERATING CODE END *****************************************************/
      break;
      
    case LeftParenToken:
      HF_get_token(p_program, p_sps);
      memory_status = HF_Get_Bool_Expr(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);



			
			if (!memory_status || *p_er)
				return memory_status;

      if(p_sps->lexem_code != RightParenToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Right_Parenthes_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      HF_get_token(p_program, p_sps);
      break;
    default:
      memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

		if (!memory_status || *p_er)
			return memory_status;
		}
  return memory_status;
}

int HF_For_Compare(char*              p_program,
                   PN_T*              p_pns,
                   ER_List_T*         p_error_list,
                   ScanParamStruct_T* p_sps,
                   Name_Array_T*      NAMES,
                   int*               p_er)
{
  /*
    -- Function description:
    --  Middle level function for parsing bool infix operators.
  */
  int memory_status = 1;
  int oper;
  int g_line, g_pos;

  memory_status = HF_For_Bool(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);


	

	if (!memory_status || *p_er)
		return memory_status;

  oper = p_sps->lexem_code;
  while((oper == NotEqualToken) || (oper == LessToken)
	|| (oper == NotGreaterToken)
	|| (oper == GreaterToken)
	|| (oper == NotLessToken)
	|| (oper == AssignToken))
    {
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Bool(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			if (!memory_status || *p_er)
				return memory_status;

      /* GENERATING CODE:*********************************************************/
      switch(oper)
	{
	case NotEqualToken:
	  oper = FUNC_bool_not_equal;
	  break;
	case LessToken:
	  oper = FUNC_bool_less;
	  break;
	case NotGreaterToken:
	  oper = FUNC_bool_not_greater;
	  break;
	case GreaterToken:
	  oper = FUNC_bool_greater;
	  break;
	case NotLessToken:
	  oper = FUNC_bool_not_less;
	  break;
	case AssignToken:
	  oper = FUNC_bool_equal;
	  break;
	}
      memory_status = HF_GEN_CODE(p_pns, oper, g_line, g_pos);

		

			if (!memory_status) return memory_status;

			/* GENERATING CODE END *****************************************************/
      oper = p_sps->lexem_code;
    }
  return memory_status;
}

int HF_Get_Bool_Expr(char*              p_program,
                     PN_T*              p_pns,
                     ER_List_T*         p_error_list,
                     ScanParamStruct_T* p_sps,
                     Name_Array_T*      NAMES,
                     int*               p_er)
{
  /*
    -- Function description:
    --  High level function for parsing bool expressions.
  */
  int memory_status = 1;
  int oper;
  int g_line, g_pos;

  memory_status = HF_For_Compare(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

	if (!memory_status || *p_er) return memory_status;

  oper = p_sps->lexem_code;
  while((oper == AndToken) || (oper == OrToken))
    {
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Compare(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			if (!memory_status || *p_er) return memory_status;

      /* GENERATING CODE:*********************************************************/
      if(oper == AndToken)
	memory_status = HF_GEN_CODE(p_pns, FUNC_bool_and, g_line, g_pos);
      else
	memory_status = HF_GEN_CODE(p_pns, FUNC_bool_or, g_line, g_pos);

			if (!memory_status) return memory_status;

      /* GENERATING CODE END *****************************************************/
      oper = p_sps->lexem_code;
    }
  return memory_status;
}
/***************************************************************************/

PN_T* HF_Find_body_in_library(PN_List_T* p_lib, char* name)
{
  /*
    -- Function description:
    --  Function tryes to find the body in the libarary by the body name.
    --  Pointer to found body is returned;
    --  NULL returned if no body is found.
  */
  if((p_lib != NULL) && (name != NULL))
    {
      PN_NODE_T* current = p_lib->first;
      while(current != NULL)
	{
	  if((current->pn_info) != NULL)
	    {
	      if(strncmp(current->pn_info->body_name, name, strlen(name)+1) == 0)
		return current->pn_info;
	    }
	  current = current->next;
	}
    }
  return NULL;
}

int HF_Get_bIndex(PN_List_T* p_lib, PN_T* p_pns)
{
  int i;
  PN_NODE_T* current;



	if ((p_lib == NULL) || (p_pns==NULL))
		return -1;

  current = p_lib->first;
  for(i=0;(current != NULL) && (current->pn_info != p_pns);i++)
    current = current->next;

  return (current == NULL)?-1:i;
}

PN_T* HF_Body_By_Index(PN_List_T* p_lib, int index)
{
  int i;
  PN_NODE_T* current;

	if (p_lib == NULL) return NULL;
  
	current = p_lib->first;
  for(i=0;(current != NULL) && (i < index);i++)
    current = current->next;

  return (current != NULL)?current->pn_info:NULL;
}

int HF_For_Power(char*              p_program,
                 PN_T*              p_pns,
                 ER_List_T*         p_error_list,
                 ScanParamStruct_T* p_sps,
                 Name_Array_T*      NAMES,
                 int*               p_er)
{
  /*
    -- Function description:
    --  Low level function for parsing real type expression tokens.
  */
  int memory_status = 1;
  int lexem;
  int found;
  int index, amount, flag;
  int i;
  int oper;
  int g_line, g_pos;

  PN_T* p_ibody;
  int index_to_body;

  int function_index;
  extern function_info_T functions_info[];

  *p_er = 0; /* No error */

  lexem = p_sps->lexem_code;
  switch(lexem)
    {
    case PlusToken: case MinusToken: case TildaToken:
      oper = lexem = p_sps->lexem_code;

      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Power(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			
			if (!memory_status || *p_er)
				return memory_status;


      /* 
	 After function return we process unary operation
      */
      /* GENERATING CODE:*********************************************************/
      switch(oper)
	{
	case PlusToken:
	  oper = FUNC_unary_plus;
	  break;
	case MinusToken:
	  oper = FUNC_unary_minus;
	  break;
	case TildaToken:
	  oper = FUNC_unary_tilda;
	  break;
	}
      memory_status = HF_GEN_CODE(p_pns, oper, g_line, g_pos);

			if (!memory_status)
				return memory_status;

      /* GENERATING CODE END *****************************************************/
      break;
    case RealValueToken:/* Ok */
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;
      /* GENERATING CODE:*********************************************************/
      memory_status = HF_Add_To_Double_Array(&p_pns->constant_values,
					     p_sps->real_value);

			if (!memory_status)
				return memory_status;

      memory_status = HF_GEN_CODE(p_pns,
				  CODE_First_Data_Index +
				  p_pns->constant_values.amount - 1,
				  g_line, g_pos);

			if (!memory_status)
				return memory_status;

      /* GENERATING CODE END *****************************************************/
      HF_get_token(p_program, p_sps);
      break;
    case LeftParenToken:
      HF_get_token(p_program, p_sps);
      memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);


			if (!memory_status || *p_er)
				return memory_status;

      if(p_sps->lexem_code != RightParenToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Right_Parenthes_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      HF_get_token(p_program, p_sps);
      break;
    case IdentifierToken:
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &index, &amount, &flag);
      if(found && (flag==Array))
	{
	  /* GENERATING CODE:*********************************************************/
	  int first = p_pns->code_list.amount-1;
	  /* GENERATING CODE END *****************************************************/

	  HF_get_token(p_program, p_sps);
	  if(p_sps->lexem_code != LeftBracketToken)
	    {
	      memory_status = HF_Add_To_ER_List(p_error_list,
						Error_Strings[Left_Bracket_Expected],
						p_sps->lexem_start_line,
						p_sps->lexem_start_position);
	      *p_er = 1;
	      return memory_status;
	    }
	  HF_get_token(p_program, p_sps);
	  memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);


		
		if (!memory_status || *p_er)
			return memory_status;

	  if(p_sps->lexem_code != RightBracketToken)
	    {
	      memory_status = HF_Add_To_ER_List(p_error_list,
						Error_Strings[Right_Bracket_Expected],
						p_sps->lexem_start_line,
						p_sps->lexem_start_position);
	      *p_er = 1;
	      return memory_status;
	    }
	  HF_get_token(p_program, p_sps);
	  /* GENERATING CODE:*********************************************************/
	  memory_status = HF_GEN_CODE(p_pns, amount, g_line, g_pos);


		if (!memory_status)
			return memory_status;

	  memory_status = HF_GEN_CODE(p_pns, index+p_pns->X_dim+p_pns->A_dim+p_pns->S_dim, g_line, g_pos);

		if (!memory_status)
			return memory_status;
	  
	  memory_status = HF_GEN_CODE(p_pns, first, g_line, g_pos);

		if (!memory_status)
			return memory_status;

	  memory_status = HF_GEN_CODE(p_pns, I_ARRAY, g_line, g_pos);
		
		if (!memory_status)
			return memory_status;

	  /* GENERATING CODE END *****************************************************/
	}
      else
	{
	  if(found)
	    {
	      /* GENERATING CODE:*********************************************************/
	      memory_status = HF_GEN_CODE(p_pns, -index-1, g_line, g_pos);

				
				if (!memory_status)
					return memory_status;

	      /* GENERATING CODE END *****************************************************/
	      HF_get_token(p_program, p_sps);
	    }
	  else
	    {
	      /************************************************************************/

	      /************************************************************************/
	      /* Here we try to recognize the identifier as an external body name     */
	      /************************************************************************/
	      int index_1 = -1, index_2 = -1, index_3=-1;
	      int g_line_1, g_pos_1;
	      int g_line_2, g_pos_2;
		  int g_line_3, g_pos_3;

	      g_line = p_sps->lexem_start_line;
	      g_pos  = p_sps->lexem_start_position;

	      p_ibody = HF_Find_body_in_library(&(p_pns->other_body_code_list), p_sps->str_buffer);
	      
	      if(p_ibody == NULL)
		{ 
		  
		  
		  p_ibody = HF_Find_body_in_library(HF_local_library, p_sps->str_buffer);
		  if(p_ibody != NULL)
		    {/* found that body is already defined in the same program */
		      p_ibody = HF_Copy_PNS(p_ibody);
		      if(p_ibody == NULL)
			{
			  memory_status = 0;
			  return memory_status;
			}
		      memory_status = HF_Add_To_PN_List(&(p_pns->other_body_code_list), p_ibody);

					if (!memory_status)
						return memory_status;
		    }
		  else
		    {
		      p_ibody = HF_Find_body_in_library(HF_library, p_sps->str_buffer);
		      if(p_ibody != NULL)
			{/* body is present in the global library */
			  p_ibody = HF_Copy_PNS(p_ibody); 
			  if(p_ibody == NULL)
			    {
			      memory_status = 0;
			      return memory_status;
			    }
			  memory_status = HF_Add_To_PN_List(&(p_pns->other_body_code_list), p_ibody);

				if (!memory_status)
					return memory_status;

			}
		      else
			{/* Unfortunately the identifier is not recognized */
			  /* By default, it means that no this identifier was not assigned a value */
			  memory_status = HF_Add_To_ER_List(p_error_list,
							    Error_Strings[Unassigned_Identificator],
							    p_sps->lexem_start_line,
							    p_sps->lexem_start_position);
			  *p_er = 1;
			  return memory_status;
			}
		    }
		}
	      index_to_body = HF_Get_bIndex(&(p_pns->other_body_code_list), p_ibody);
	      /* Continue to parse call to external body */

	      HF_get_token(p_program, p_sps);
	      if(p_sps->lexem_code != LeftParenToken)
		{
		  memory_status = HF_Add_To_ER_List(p_error_list,
						    Error_Strings[Left_Parenthes_Expected],
						    p_sps->lexem_start_line,
						    p_sps->lexem_start_position);
		  *p_er = 1;
		  return memory_status;
		}

	      HF_get_token(p_program, p_sps);
		  /* VAVAVA 's' can be one of arguments*/
	      if((p_sps->lexem_code == IdentifierToken) || (p_sps->lexem_code == XIdToken) || (p_sps->lexem_code == AIdToken) || (p_sps->lexem_code == SIdToken))
		/** First parameter: ******/
		{
		  g_line_1 = p_sps->lexem_start_line;
		  g_pos_1  = p_sps->lexem_start_position;

		  if(p_sps->lexem_code == XIdToken)
		    {
		      amount = p_pns->X_dim;
		      index  = 0;
		    }
		  else
		    {
		      if(p_sps->lexem_code == AIdToken)
			{
			  amount = p_pns->A_dim;
			  index  = p_pns->X_dim;
			}
		      else
			/* VAVAVA*/
			  {
				  if (p_sps->lexem_code == SIdToken)
				  {
					amount = p_pns->S_dim;
					index = p_pns->X_dim + p_pns->A_dim;
				  }
				  else
				  {


			  found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &index, &amount, &flag);
			  if(found && (flag==Array))
			    {
				  index  += p_pns->X_dim + p_pns->A_dim + p_pns->S_dim;
			    }
			  else
			    {
			      memory_status = HF_Add_To_ER_List(p_error_list,
								Error_Strings[Array_Name_Expected],
								p_sps->lexem_start_line,
								p_sps->lexem_start_position);
			      *p_er = 1;
			      return memory_status;
			    }
				} /* VAVAVA if(p_sps->lexem_code == SIdToken)*/
			} /* if(p_sps->lexem_code == AIdToken) */
		    } /* if(p_sps->lexem_code == XIdToken) */
		  /** Check for amount of arguments for the  first parameter: ********************/
		  if(amount != p_ibody->X_dim)
                    {
		      memory_status = HF_Add_To_ER_List(p_error_list,
							Error_Strings[Wrong_Number_Of_Items_In_Array],
							p_sps->lexem_start_line,
							p_sps->lexem_start_position);
		      *p_er = 1;
		      return memory_status;
		    }
		  index_1 = index;
		  /*******************************************************************************/
		}
	      /**************************/

	      HF_get_token(p_program, p_sps);
	      if(p_sps->lexem_code != CommaToken)
		{
		  memory_status = HF_Add_To_ER_List(p_error_list,
						    Error_Strings[Comma_Expected],
						    p_sps->lexem_start_line,
						    p_sps->lexem_start_position);
		  *p_er = 1;
		  return memory_status;
		}

	      HF_get_token(p_program, p_sps);
	      if((p_sps->lexem_code == IdentifierToken) || (p_sps->lexem_code == XIdToken) || (p_sps->lexem_code == AIdToken) || (p_sps->lexem_code == SIdToken)) /* VAVAVA */
		/** Second parameter: *****/
		{
		  g_line_2 = p_sps->lexem_start_line;
		  g_pos_2  = p_sps->lexem_start_position;

		  if(p_sps->lexem_code == XIdToken)
		    {
		      amount = p_pns->X_dim;
		      index  = 0;
		    }
		  else 
		    {
		      if(p_sps->lexem_code == AIdToken)
			{
			  amount = p_pns->A_dim;
			  index  = p_pns->X_dim;
			}
		      else
				  /*VAVAVA*/
			{
				if(p_sps->lexem_code == SIdToken)
					{
							amount = p_pns->S_dim;
							index  = p_pns->X_dim + p_pns->A_dim;
					} 
				else
				{
			  found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &index, &amount, &flag);
			  if(found && (flag==Array))
			    {
					
			      index  += p_pns->X_dim + p_pns->A_dim + p_pns->S_dim;
			    }
			  else
			    {
			      memory_status = HF_Add_To_ER_List(p_error_list,
								Error_Strings[Array_Name_Expected],
								p_sps->lexem_start_line,
								p_sps->lexem_start_position);
			      *p_er = 1;
			      return memory_status;
			    }
				} /* VAVAVA if(p_sps->lexem_code == SIdToken)*/
			} /* if(p_sps->lexem_code == AIdToken) */
		    } /* if(p_sps->lexem_code == XIdToken) */
		  /** Check for amount of arguments for second parameter: ************************/
		  if(amount != p_ibody->A_dim)
                    {
		      memory_status = HF_Add_To_ER_List(p_error_list,
							Error_Strings[Wrong_Number_Of_Items_In_Array],
							p_sps->lexem_start_line,
							p_sps->lexem_start_position);
		      *p_er = 1;
		      return memory_status;
		    }
		  index_2 = index;
		  /*******************************************************************************/
		}

	      HF_get_token(p_program, p_sps);

		  /* No s array */
		  if (p_sps->lexem_code == RightParenToken)
		  {
			  amount = 1;/*;p_pns->S_dim;*/

			if (amount != p_ibody->S_dim) 
			{
				memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Wrong_Number_Of_Items_In_Array],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
				*p_er = 1;
				return memory_status;
			}
					  
			  index = p_pns->X_dim + p_pns->A_dim;
g_line_3 = p_sps->lexem_start_line;
					g_pos_3  = p_sps->lexem_start_position;
					index_3 = index;
			  /*HF_get_token(p_program, p_sps);*/
			  goto NO_S_ARRAY;
		  }

	      if(p_sps->lexem_code != CommaToken)
		{
		  memory_status = HF_Add_To_ER_List(p_error_list,
						    Error_Strings[Comma_Expected],
						    p_sps->lexem_start_line,
						    p_sps->lexem_start_position);
		  *p_er = 1;
		  return memory_status;
		}

		HF_get_token(p_program, p_sps);

		

		if((p_sps->lexem_code == IdentifierToken) || (p_sps->lexem_code == XIdToken) 
					                              || (p_sps->lexem_code == AIdToken)
												  || (p_sps->lexem_code == SIdToken)) /* VAVAVA */

		/** The third parameter: *****/
				{
					g_line_3 = p_sps->lexem_start_line;
					g_pos_3  = p_sps->lexem_start_position;

					if(p_sps->lexem_code == XIdToken)
					{
						amount = p_pns->X_dim;
						index  = 0;
					}
					else
					{
						if(p_sps->lexem_code == AIdToken)
						{
							amount = p_pns->A_dim;
							index  = p_pns->X_dim;
						}
						else
              /* VAVAVA */
						{
							if(p_sps->lexem_code == SIdToken)
							{
								amount = p_pns->S_dim;
								index  = p_pns->X_dim + p_pns->A_dim;
							} else
							{
								found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &index, &amount, &flag);
								if(found && (flag==Array))
								{
									index  += p_pns->X_dim + p_pns->A_dim + p_pns->S_dim;
								}
								else
								{
									memory_status = HF_Add_To_ER_List(p_error_list,
																	Error_Strings[Array_Name_Expected],
																	p_sps->lexem_start_line,
																	p_sps->lexem_start_position);
									*p_er = 1;
									return memory_status;
								}
							} /* VAVAVA if(p_sps->lexem_code == SIdToken)*/
						} /* if(p_sps->lexem_code == AIdToken) */
					} /* if(p_sps->lexem_code == XIdToken) */
/** Check for amount of arguments for third parameter: ************************/
                    if(amount != p_ibody->S_dim)
                    {
                    	memory_status = HF_Add_To_ER_List(p_error_list,
														Error_Strings[Wrong_Number_Of_Items_In_Array],
														p_sps->lexem_start_line,
														p_sps->lexem_start_position);
						*p_er = 1;
						return memory_status;
					}
					index_3 = index;
/*******************************************************************************/
				}
				HF_get_token(p_program, p_sps);
				if(p_sps->lexem_code != RightParenToken)
				{
					memory_status = HF_Add_To_ER_List(p_error_list,
								                      Error_Strings[Right_Parenthes_Expected],
											          p_sps->lexem_start_line,
													  p_sps->lexem_start_position);
					*p_er = 1;
					return memory_status;
				}										  
		//
NO_S_ARRAY:	      HF_get_token(p_program, p_sps);
	      /************************************************************************/
	      if(p_pns->ad_size < p_ibody->stack_size + p_ibody->ad_size)
		p_pns->ad_size = p_ibody->stack_size + p_ibody->ad_size;
	      /* GENERATING CODE:*********************************************************/
		/* Third array index: */       /* VAVAVA */
				memory_status = HF_GEN_CODE(p_pns, index_3, g_line_3, g_pos_3);
				if(!memory_status)
					return memory_status;


	      /* Second array index: */
	      memory_status = HF_GEN_CODE(p_pns, index_2, g_line_2, g_pos_2);
				if (!memory_status)
					return memory_status;
	      /* First array index: */
	      memory_status = HF_GEN_CODE(p_pns, index_1, g_line_1, g_pos_1);
				if (!memory_status)
					return memory_status;
				/* body number: */
	      memory_status = HF_GEN_CODE(p_pns, index_to_body, g_line, g_pos);
				if (!memory_status)
					return memory_status;
				/* Action code: extern call */
	      memory_status = HF_GEN_CODE(p_pns, I_EXTERN_CALL, g_line, g_pos);
				if (!memory_status)
					return memory_status;

	      /* GENERATING CODE END *****************************************************/
	    }
	}
      break;
    case FunctionIdToken:
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      function_index = p_sps->function_index;

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != LeftParenToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Left_Parenthes_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      HF_get_token(p_program, p_sps);
      for(i = 0;i<functions_info[function_index].dim;i++)
	{
	  if(functions_info[function_index].function_args[i] == HF_R)
	    {
	      memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

				if (!memory_status || *p_er)
					return memory_status;
			}
	  else
	    {
	      if((functions_info[function_index].function_args[i] == HF_S) &&
		 (p_sps->lexem_code == ConstantStringToken))
		{
		  /* GENERATING CODE:*********************************************************/
                  memory_status = HF_Add_To_String_Array(&p_pns->str_param, p_sps->str_buffer);
									if (!memory_status)
										return memory_status;
									memory_status = HF_GEN_CODE(p_pns, p_pns->str_param.amount-1, p_sps->lexem_start_line, p_sps->lexem_start_position);
									if (!memory_status)
										return memory_status;
									memory_status = HF_GEN_CODE(p_pns, CODE_String_Index, p_sps->lexem_start_line, p_sps->lexem_start_position);
									if (!memory_status)
										return memory_status;

		  /* GENERATING CODE END *****************************************************/
                  HF_get_token(p_program, p_sps);
		}
	      else
		{
		  if ((functions_info[function_index].function_args[i] == HF_A) &&
                      ((p_sps->lexem_code == IdentifierToken) ||
                       (p_sps->lexem_code == XIdToken)        ||
                       (p_sps->lexem_code == AIdToken) ||
					   (p_sps->lexem_code == SIdToken) ))        /* VAVAVA */		   
		    {/* In this case we should interpret arg as ARRAY */
		      if(p_sps->lexem_code == XIdToken)
			{
			  amount = p_pns->X_dim;
			  index  = 0;
			}
		      else
			{
			  if(p_sps->lexem_code == AIdToken)
			    {
			      amount = p_pns->A_dim;
			      index  = p_pns->X_dim;
			    }
			  else
			    {                                     /* VAVAVA */
					if(p_sps->lexem_code == SIdToken)
					{
						amount = p_pns->S_dim;
						index  = p_pns->X_dim + p_pns->A_dim;
					}
                    else
					{
			      found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &index, &amount, &flag);
			      if(found && (flag==Array))
				{
				  index  += p_pns->X_dim + p_pns->A_dim + p_pns->S_dim; /* VAVAVA */
				}
			      else
				{
				  memory_status = HF_Add_To_ER_List(p_error_list,
								    Error_Strings[Array_Name_Expected],
								    p_sps->lexem_start_line,
								    p_sps->lexem_start_position);
				  *p_er = 1;
				  return memory_status;
				}
				} /* if(p_sps->lexem_code == SIdToken) */  /* VAVAVA */
			    } /* if(p_sps->lexem_code == AIdToken) */
			} /* if(p_sps->lexem_code == XIdToken) */
		      /* GENERATING CODE:*********************************************************/
		      memory_status = HF_GEN_CODE(p_pns, amount, p_sps->lexem_start_line, p_sps->lexem_start_position);

					if (!memory_status)
						return memory_status;

		      memory_status = HF_GEN_CODE(p_pns, index, p_sps->lexem_start_line, p_sps->lexem_start_position);
					
					if (!memory_status)
						return memory_status;

					memory_status = HF_GEN_CODE(p_pns, I_ARRAY_AS_ARG, p_sps->lexem_start_line, p_sps->lexem_start_position);

					if (!memory_status)
						return memory_status;

					/* GENERATING CODE END *****************************************************/
		      HF_get_token(p_program, p_sps);
		    }
                  else
		    {
		      memory_status = HF_Add_To_ER_List(p_error_list,
							Error_Strings[Argument_Error],
							p_sps->lexem_start_line,
							p_sps->lexem_start_position);

		      *p_er = 1;
		      return memory_status;
		    }
		}
	    }
	  if(i+1 < functions_info[function_index].dim)
	    {
	      if(p_sps->lexem_code != CommaToken)
		{
                  memory_status = HF_Add_To_ER_List(p_error_list,
						    Error_Strings[Comma_Expected],
						    p_sps->lexem_start_line,
						    p_sps->lexem_start_position);

		  *p_er = 1;
		  return memory_status;
		}
	      HF_get_token(p_program, p_sps);
	    }
	  else
	    {
	      break;
	    }
	}
      if(p_sps->lexem_code != RightParenToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Right_Parenthes_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      /* GENERATING CODE:*********************************************************/
      memory_status = HF_GEN_CODE(p_pns, ++(functions_info[function_index].F_instance_counter), g_line, g_pos);

			if (!memory_status)
				return memory_status;

			memory_status = HF_GEN_CODE(p_pns, FUNC_unary_tilda+1+function_index, g_line, g_pos);
	
			if (!memory_status)
				return memory_status;

			/* GENERATING CODE END *****************************************************/
      HF_get_token(p_program, p_sps);
      break;
    case XIdToken:
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != LeftBracketToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Left_Bracket_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      {
	/* GENERATING CODE:*********************************************************/
	int first = p_pns->code_list.amount-1;
	/* GENERATING CODE END *****************************************************/

	HF_get_token(p_program, p_sps);
	memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);


	if (!memory_status || *p_er)
		return memory_status;

	if(p_sps->lexem_code != RightBracketToken)
	  {
	    memory_status = HF_Add_To_ER_List(p_error_list,
					      Error_Strings[Right_Bracket_Expected],
					      p_sps->lexem_start_line,
					      p_sps->lexem_start_position);
	    *p_er = 1;
	    return memory_status;
	  }
	HF_get_token(p_program, p_sps);
	/* GENERATING CODE:*********************************************************/
	memory_status = HF_GEN_CODE(p_pns, p_pns->X_dim, g_line, g_pos);
	if (!memory_status)
		return memory_status;
	
	memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);
	if (!memory_status)
		return memory_status;

	memory_status = HF_GEN_CODE(p_pns, first, g_line, g_pos);
	if (!memory_status)
		return memory_status;

	memory_status = HF_GEN_CODE(p_pns, I_ARRAY, g_line, g_pos);

	if (!memory_status)
		return memory_status;

	/* GENERATING CODE END *****************************************************/
      }
      break;
    case AIdToken:
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != LeftBracketToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Left_Bracket_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      {
	/* GENERATING CODE:*********************************************************/
	int first = p_pns->code_list.amount-1;
	/* GENERATING CODE END *****************************************************/
	HF_get_token(p_program, p_sps);
	memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

	if (!memory_status || *p_er)
		return memory_status;

	if(p_sps->lexem_code != RightBracketToken)
	  {
	    memory_status = HF_Add_To_ER_List(p_error_list,
					      Error_Strings[Right_Bracket_Expected],
					      p_sps->lexem_start_line,
					      p_sps->lexem_start_position);
	    *p_er = 1;
	    return memory_status;
	  }
	HF_get_token(p_program, p_sps);
	/* GENERATING CODE:*********************************************************/
	memory_status = HF_GEN_CODE(p_pns, p_pns->A_dim, g_line, g_pos);
	if (!memory_status)
		return memory_status;

	memory_status = HF_GEN_CODE(p_pns, p_pns->X_dim, g_line, g_pos);
	if (!memory_status)
		return memory_status;

	memory_status = HF_GEN_CODE(p_pns, first, g_line, g_pos);

	if (!memory_status)
		return memory_status;

	memory_status = HF_GEN_CODE(p_pns, I_ARRAY, g_line, g_pos);
	
	if (!memory_status)
		return memory_status;

	/* GENERATING CODE END *****************************************************/
      }
      break;




	  /* VAVAVA Processing 's[...]' in RHS */
      case SIdToken:
	 g_line = p_sps->lexem_start_line;
	 g_pos  = p_sps->lexem_start_position;

         HF_get_token(p_program, p_sps);
	 if(p_sps->lexem_code != LeftBracketToken)
	 {
            memory_status = HF_Add_To_ER_List(p_error_list,
					   Error_Strings[Left_Bracket_Expected],
					   p_sps->lexem_start_line,
					   p_sps->lexem_start_position);
	    *p_er = 1;
	    return memory_status;
	 }
	 {
/* GENERATING CODE:*********************************************************/
	    int first = p_pns->code_list.amount-1; /* VAVAVA ? from where? */
/* GENERATING CODE END *****************************************************/
            HF_get_token(p_program, p_sps);
            memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
	    if(!memory_status || *p_er)
	       return memory_status;
	    if(p_sps->lexem_code != RightBracketToken)
	    {
               memory_status = HF_Add_To_ER_List(p_error_list,
					      Error_Strings[Right_Bracket_Expected],
					      p_sps->lexem_start_line,
					      p_sps->lexem_start_position);
	       *p_er = 1;
	       return memory_status;
	    }
            HF_get_token(p_program, p_sps);
/* GENERATING CODE:*********************************************************/
            memory_status = HF_GEN_CODE(p_pns, p_pns->S_dim, g_line, g_pos);
	    if(!memory_status)
	       return memory_status;
            memory_status = HF_GEN_CODE(p_pns, p_pns->X_dim + p_pns->A_dim, g_line, g_pos);
	    if(!memory_status)
	       return memory_status;
            memory_status = HF_GEN_CODE(p_pns, first, g_line, g_pos);
	    if(!memory_status)
	       return memory_status;
            memory_status = HF_GEN_CODE(p_pns, I_ARRAY, g_line, g_pos);
	    if(!memory_status)
	       return memory_status;
/* GENERATING CODE END *****************************************************/
	 }
	 break;

    default:
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Expression_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  return memory_status;
}


int HF_For_Mult(char*              p_program,
                PN_T*              p_pns,
                ER_List_T*         p_error_list,
                ScanParamStruct_T* p_sps,
                Name_Array_T*      NAMES,
                int*               p_er)
{
  /*
    -- Function description:
    --  Middle level function for parsing real type expression infix operators.
  */
  int memory_status = 1;
  int oper;
  int g_line, g_pos;

  memory_status = HF_For_Power(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
  if(!memory_status || *p_er)
    return memory_status;

  oper = p_sps->lexem_code;
  while((oper == PowerToken))
    {
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Power(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			if (!memory_status || *p_er)
				return memory_status;

			/* GENERATING CODE:*********************************************************/
      memory_status = HF_GEN_CODE(p_pns, FUNC_binary_power, g_line, g_pos);
			if (!memory_status)
				return memory_status;

			/* GENERATING CODE END *****************************************************/

      oper = p_sps->lexem_code;
    }
  return memory_status;
}

int HF_For_Sum(char*              p_program,
               PN_T*              p_pns,
               ER_List_T*         p_error_list,
               ScanParamStruct_T* p_sps,
               Name_Array_T*      NAMES,
               int*               p_er)
{
  /*
    -- Function description:
    --  Middle level function for parsing real type expression infix operators.
  */
  int memory_status = 1;
  int oper;
  int g_line, g_pos;

  memory_status = HF_For_Mult(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
	if (!memory_status || *p_er)
		return memory_status;

  oper = p_sps->lexem_code;
  while((oper == SlashToken) || (oper == MultToken))
    {
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Mult(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
      if(!memory_status || *p_er)
	return memory_status;

      /* GENERATING CODE:*********************************************************/
      if(oper == SlashToken)
	memory_status = HF_GEN_CODE(p_pns, FUNC_binary_divide, g_line, g_pos);
      else
	memory_status = HF_GEN_CODE(p_pns, FUNC_binary_mult, g_line, g_pos);

			if (!memory_status)
				return memory_status;
			
			/* GENERATING CODE END *****************************************************/

      oper = p_sps->lexem_code;
    }
  return memory_status;
}


int HF_Get_Expression(char*              p_program,
                      PN_T*              p_pns,
                      ER_List_T*         p_error_list,
                      ScanParamStruct_T* p_sps,
                      Name_Array_T*      NAMES,
                      int*               p_er)
{
  /*
    -- Function description:
    --  High level function for parsing real type expressions.
  */
  int memory_status = 1;
  int oper;
  int g_line, g_pos;

  memory_status = HF_For_Sum(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
	if (!memory_status || *p_er)
		return memory_status;

	oper = p_sps->lexem_code;
  while((oper == PlusToken) || (oper == MinusToken)
	|| (oper == CartProductToken)
	|| (oper == AmpersandToken)
	|| (oper == UnionToken)
	|| (oper == NegToken))
    {
      int code;
      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      HF_get_token(p_program, p_sps);
      memory_status = HF_For_Sum(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
			if (!memory_status || *p_er)
				return memory_status;

			/* GENERATING CODE:*********************************************************/
      switch(oper)
	{
	case PlusToken:
	  code = FUNC_binary_plus;
	  break;
	case MinusToken:
	  code = FUNC_binary_minus;
	  break;
	case CartProductToken:
	  code = FUNC_binary_cart;
	  break;
	case AmpersandToken:
	  code = FUNC_binary_ampersand;
	  break;
	case UnionToken:
	  code = FUNC_binary_union;
	  break;
	case NegToken:
	  code = FUNC_binary_neg;
	  break;
	}
      memory_status = HF_GEN_CODE(p_pns, code, g_line, g_pos);
			if (!memory_status)
				return memory_status;

			/* GENERATING CODE END *****************************************************/

      oper = p_sps->lexem_code;
    }
  return memory_status;
}

/***************************************************************************/
int HF_Get_If_Operator(char*              p_program,
                       PN_T*              p_pns,
                       ER_List_T*         p_error_list,
                       ScanParamStruct_T* p_sps,
                       Name_Array_T*      NAMES,
                       int*               p_er)
{
  /*
    -- Function description:
    -- Function processes the if operator.
  */
  int memory_status = 1;
  int temp_if_index;
  int g_line, g_pos;

  *p_er = 0;

  /* GENERATING CODE:*********************************************************/
  temp_if_index  = p_pns->code_list.amount;

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;

  memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* Reserve for CODE_if        */
	if (!memory_status)
		return memory_status;
	memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* Reserve for goto oi1       */
	if (!memory_status)
		return memory_status;
	memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* Reserve for bool end index */
  
	if (!memory_status)
		return memory_status;

  /* GENERATING CODE END *****************************************************/
  HF_get_token(p_program, p_sps);
  memory_status = HF_Get_Bool_Expr(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
	if (!memory_status || *p_er)
		return memory_status;

	/* GENERATING CODE:*********************************************************/
  p_pns->code_list.data[temp_if_index+2] = p_pns->code_list.amount-1;
  /* GENERATING CODE END *****************************************************/
  if(p_sps->lexem_code != ThenToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Then_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  memory_status = HF_Parse_Operators(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);


	if (!memory_status || *p_er)
		return memory_status;


	if(p_sps->lexem_code == ElseToken)
    {
      /* GENERATING CODE:*********************************************************/
      int temp_index;

      g_line = p_sps->lexem_start_line;
      g_pos  = p_sps->lexem_start_position;

      memory_status = HF_GEN_CODE(p_pns, CODE_goto, g_line, g_pos);

			if (!memory_status)
				return memory_status;

			temp_index = p_pns->code_list.amount;
      memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos); /* reserved for goto oi2  */

			if (!memory_status)
				return memory_status;

      p_pns->code_list.data[temp_if_index]   = CODE_if_else;
      p_pns->code_list.data[temp_if_index+1] = p_pns->code_list.amount;
      /* oi1                    */
      /* GENERATING CODE END *****************************************************/

      HF_get_token(p_program, p_sps);
      memory_status = HF_Parse_Operators(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);

			if (!memory_status || *p_er)
				return memory_status;

			if(p_sps->lexem_code != EndIfToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[EndIf_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      /* GENERATING CODE:*********************************************************/
      p_pns->code_list.data[temp_index] = p_pns->code_list.amount;
      /* GENERATING CODE END *****************************************************/
    }
  else
    {
      if(p_sps->lexem_code != EndIfToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Else_EndIf_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      /* GENERATING CODE:*********************************************************/
      p_pns->code_list.data[temp_if_index]   = CODE_if;
      p_pns->code_list.data[temp_if_index+1] = p_pns->code_list.amount;
      /* GENERATING CODE END *****************************************************/
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != SemicolonToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  return memory_status;
}/*HF_Get_If_Operator*/

int HF_Get_While_Operator(char*              p_program,
                          PN_T*              p_pns,
                          ER_List_T*         p_error_list,
                          ScanParamStruct_T* p_sps,
                          Name_Array_T*      NAMES,
                          int*               p_er)
{
  /*
    -- Function description:
    -- Function processes the while operator.
  */
  int memory_status = 1;
  int temp_index;
  int g_line, g_pos;

  *p_er = 0;

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;

  /* GENERATING CODE:*********************************************************/
  memory_status = HF_GEN_CODE(p_pns, CODE_while, g_line, g_pos);
	if (!memory_status)
		return memory_status;

	temp_index = p_pns->code_list.amount;
  memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* reserved for goto oi       */
	if (!memory_status)
		return memory_status;
	memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* reserved for bool end expr */
	if (!memory_status)
		return memory_status;
	/* GENERATING CODE END *****************************************************/

  HF_get_token(p_program, p_sps);
  memory_status = HF_Get_Bool_Expr(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
	if (!memory_status || *p_er)
		return memory_status;

  if(p_sps->lexem_code != LoopToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Loop_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  /* GENERATING CODE:*********************************************************/
  p_pns->code_list.data[temp_index + 1] = p_pns->code_list.amount-1;
  /* GENERATING CODE END *****************************************************/
  HF_get_token(p_program, p_sps);
  memory_status = HF_Parse_Operators(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
if (!memory_status || *p_er)
return memory_status;
	g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;
  /* GENERATING CODE:*********************************************************/
  memory_status = HF_GEN_CODE(p_pns, CODE_goto, g_line, g_pos);
if (!memory_status)
return memory_status;

  memory_status = HF_GEN_CODE(p_pns, temp_index-1, g_line, g_pos);
if (!memory_status)
return memory_status;

  p_pns->code_list.data[temp_index] = p_pns->code_list.amount;
  /* GENERATING CODE END *****************************************************/

  if(p_sps->lexem_code != EndLoopToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[EndLoop_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != SemicolonToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  return memory_status;
}



int HF_Get_Whole_Array_Assign_Operator(char*              p_program,
                                       PN_T*              p_pns,
                                       ER_List_T*         p_error_list,
                                       ScanParamStruct_T* p_sps,
                                       int                index,
                                       int                amount,
                                       int*               p_er)
{
  /*
    -- Function description:
    -- Function processes the array assignment operator.
  */
  int memory_status = 1;
  int i;
  int g_line, g_pos;

  int sign_multiplier = 1;

  *p_er = 0;

  if(p_sps->lexem_code != LeftBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Left_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code == MinusToken)
    {
      sign_multiplier = -1;
      HF_get_token(p_program, p_sps);
    }
  else
    {
      if(p_sps->lexem_code == PlusToken)
	{
	  HF_get_token(p_program, p_sps);
	}
    }

  if(p_sps->lexem_code != RealValueToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Real_Number_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;
  /* GENERATING CODE:*********************************************************/
  memory_status = HF_GEN_CODE(p_pns, CODE_array_assignment, g_line, g_pos);
	if (!memory_status)
		return memory_status;
  memory_status = HF_GEN_CODE(p_pns, amount, g_line, g_pos);

	if (!memory_status)
		return memory_status;

  memory_status = HF_GEN_CODE(p_pns, index, g_line, g_pos);
	
	if (!memory_status)
		return memory_status;
	
	memory_status = HF_GEN_CODE(p_pns, p_pns->constant_values.amount, g_line, g_pos);

	
	if (!memory_status)
		return memory_status;


	/***************************************************************************/
  memory_status = HF_Add_To_Double_Array(&p_pns->constant_values, (p_sps->real_value)*sign_multiplier);

	if (!memory_status)
		return memory_status;

  /* GENERATING CODE END *****************************************************/

  HF_get_token(p_program, p_sps);
  for(i = 0; i<amount-1; i++)
    {
      if(p_sps->lexem_code != CommaToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Comma_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}
      sign_multiplier = 1;
      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code == MinusToken)
	{
	  sign_multiplier = -1;
	  HF_get_token(p_program, p_sps);
	}
      else
	{
	  if(p_sps->lexem_code == PlusToken)
	    {
	      HF_get_token(p_program, p_sps);
	    }
	}

      if(p_sps->lexem_code != RealValueToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Real_Number_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  *p_er = 1;
	  return memory_status;
	}

      /* GENERATING CODE:*********************************************************/
      memory_status = HF_Add_To_Double_Array(&p_pns->constant_values, (p_sps->real_value)*sign_multiplier);

			if (!memory_status)
				return memory_status;

			/* GENERATING CODE END *****************************************************/
      HF_get_token(p_program, p_sps);
    }

  if(p_sps->lexem_code != RightBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Right_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != SemicolonToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  return memory_status;
};



int HF_Get_Array_Assign_Operator(char*              p_program,
                                 PN_T*              p_pns,
                                 ER_List_T*         p_error_list,
                                 ScanParamStruct_T* p_sps,
                                 Name_Array_T*      NAMES,
                                 int                index,
                                 int                amount,
                                 int*               p_er)
{
  /*
    -- Function description:
    -- Function processes the array item assignment operator.
  */
  int temp_index;
  int memory_status;
  int g_line, g_pos;

  *p_er = 0;

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;
  /* GENERATING CODE:*********************************************************/
  memory_status = HF_GEN_CODE(p_pns, CODE_array_item_assignment, g_line, g_pos);
if (!memory_status)
return memory_status;

  memory_status = HF_GEN_CODE(p_pns, index, g_line, g_pos);
if (!memory_status)
return memory_status;

  memory_status = HF_GEN_CODE(p_pns, amount, g_line, g_pos);
if (!memory_status)
return memory_status;

	temp_index = p_pns->code_list.amount;

  memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
if (!memory_status)
return memory_status;

	/* GENERATING CODE END *****************************************************/
  memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES,p_er);
if (!memory_status || *p_er)
return memory_status;

	/* GENERATING CODE:*********************************************************/
  p_pns->code_list.data[temp_index] = p_pns->code_list.amount - 1;
  /* GENERATING CODE END *****************************************************/
  if(p_sps->lexem_code != RightBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Right_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != AssignToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Assign_Mark_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;
  /* GENERATING CODE:*********************************************************/
  temp_index = p_pns->code_list.amount;

  memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
if (!memory_status)
return memory_status;
	/* GENERATING CODE END *****************************************************/

  memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
if (!memory_status)
return memory_status;

	/* GENERATING CODE:*********************************************************/
  p_pns->code_list.data[temp_index] = p_pns->code_list.amount - 1;
  /* GENERATING CODE END *****************************************************/

  if(p_sps->lexem_code != SemicolonToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  return memory_status;
};


int HF_Get_SArray_Assign_Operator(char*              p_program,
                                 PN_T*              p_pns,
                                 ER_List_T*         p_error_list,
                                 ScanParamStruct_T* p_sps,
                                 Name_Array_T*      NAMES,
                                 int*               p_er)
{
/*
-- Function description:
-- Function processes the array item assignment operator.
*/
   int temp_index;
   int memory_status;
   int g_line, g_pos;
   int samount;  /* VAVAVA */

   *p_er = 0;

   g_line = p_sps->lexem_start_line;
   g_pos  = p_sps->lexem_start_position;
   samount = p_pns->S_dim;
/* GENERATING CODE:*********************************************************/
   memory_status = HF_GEN_CODE(p_pns, CODE_Sarray_item_assignment, g_line, g_pos); /* VAVAVA*/
   if(!memory_status)
      return memory_status;
  /* VAVAVA No need for index in NAMES for 's' array*/
 /*  memory_status = HF_GEN_CODE(p_pns, index, g_line, g_pos);
   if(!memory_status)
      return memory_status;
*/
   memory_status = HF_GEN_CODE(p_pns, samount, g_line, g_pos);  /* VAVAVA */
   if(!memory_status)
      return memory_status;

   temp_index = p_pns->code_list.amount;

   memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
   if(!memory_status)
      return memory_status;
/* GENERATING CODE END *****************************************************/
   memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES,p_er);
   if(!memory_status || *p_er)
      return memory_status;
/* GENERATING CODE:*********************************************************/
   p_pns->code_list.data[temp_index] = p_pns->code_list.amount - 1;
/* GENERATING CODE END *****************************************************/
   if(p_sps->lexem_code != RightBracketToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Right_Bracket_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }
   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != AssignToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Assign_Mark_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }
   HF_get_token(p_program, p_sps);

   g_line = p_sps->lexem_start_line;
   g_pos  = p_sps->lexem_start_position;
/* GENERATING CODE:*********************************************************/
   temp_index = p_pns->code_list.amount;

   memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
   if(!memory_status)
      return memory_status;
/* GENERATING CODE END *****************************************************/

   memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
   if(!memory_status || *p_er)
      return memory_status;
/* GENERATING CODE:*********************************************************/
   p_pns->code_list.data[temp_index] = p_pns->code_list.amount - 1;
/* GENERATING CODE END *****************************************************/

   if(p_sps->lexem_code != SemicolonToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Semicolon_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }
   HF_get_token(p_program, p_sps);
   return memory_status;
};

int HF_Get_Whole_SArray_Assign_Operator(char*              p_program,
                                       PN_T*              p_pns,
                                       ER_List_T*         p_error_list,
                                       ScanParamStruct_T* p_sps,
                                       int*               p_er)
{
/*
-- Function description:
-- Function processes the array assignment operator.
*/
   int memory_status = 1;
   int i;
   int g_line, g_pos;
   int samount;      /* VAVAVA */

   int sign_multiplier = 1;

   *p_er = 0;

   if(p_sps->lexem_code != LeftBracketToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Left_Bracket_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code == MinusToken)
   {
      sign_multiplier = -1;
      HF_get_token(p_program, p_sps);
   }
   else
   {
      if(p_sps->lexem_code == PlusToken)
      {
         HF_get_token(p_program, p_sps);
      }
   }

   if(p_sps->lexem_code != RealValueToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Real_Number_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }

   g_line = p_sps->lexem_start_line;
   g_pos  = p_sps->lexem_start_position;

   samount = p_pns->S_dim;   /* VAVAVA */
/* GENERATING CODE:*********************************************************/
   memory_status = HF_GEN_CODE(p_pns, CODE_Sarray_assignment, g_line, g_pos);
   if(!memory_status)
      return memory_status;

   memory_status = HF_GEN_CODE(p_pns, samount, g_line, g_pos);
   if(!memory_status)
      return memory_status;

     /* VAVAVA No need for index in NAMES! */
/*   memory_status = HF_GEN_CODE(p_pns, index, g_line, g_pos); 
   if(!memory_status)
      return memory_status;
*/

   memory_status = HF_GEN_CODE(p_pns, p_pns->constant_values.amount, g_line, g_pos);
   if(!memory_status)
      return memory_status;
/***************************************************************************/
   memory_status = HF_Add_To_Double_Array(&p_pns->constant_values, (p_sps->real_value)*sign_multiplier);
   if(!memory_status)
      return memory_status;
/* GENERATING CODE END *****************************************************/

   HF_get_token(p_program, p_sps);
   for(i = 0; i<samount-1; i++)   /* VAVAVA */
   {
      if(p_sps->lexem_code != CommaToken)
      {
         memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Comma_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
	 *p_er = 1;
	 return memory_status;
      }
      sign_multiplier = 1;
      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code == MinusToken)
      {
         sign_multiplier = -1;
         HF_get_token(p_program, p_sps);
      }
      else
      {
         if(p_sps->lexem_code == PlusToken)
         {
            HF_get_token(p_program, p_sps);
         }
      }

      if(p_sps->lexem_code != RealValueToken)
      {
         memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Real_Number_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
	 *p_er = 1;
	 return memory_status;
      }

/* GENERATING CODE:*********************************************************/
      memory_status = HF_Add_To_Double_Array(&p_pns->constant_values, (p_sps->real_value)*sign_multiplier);
      if(!memory_status)
	 return memory_status;
/* GENERATING CODE END *****************************************************/
      HF_get_token(p_program, p_sps);
   }

   if(p_sps->lexem_code != RightBracketToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Right_Bracket_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != SemicolonToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Semicolon_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
   }
   HF_get_token(p_program, p_sps);
   return memory_status;
};

int HF_Get_Assign_Operator(char*              p_program,
                           PN_T*              p_pns,
                           ER_List_T*         p_error_list,
                           ScanParamStruct_T* p_sps,
                           Name_Array_T*      NAMES,
                           int                index,
                           int*               p_er)
{
  /*
    -- Function description:
    -- Function processes the assignment operator.
  */
  int memory_status = 1;
  int temp_index;
  int g_line, g_pos;

  *p_er = 0;

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != AssignToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Assign_Mark_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }

  g_line = p_sps->lexem_start_line;
  g_pos  = p_sps->lexem_start_position;
  /* GENERATING CODE:*********************************************************/
  memory_status = HF_GEN_CODE(p_pns, CODE_assignment, g_line, g_pos);
if (!memory_status)
return memory_status;

	memory_status = HF_GEN_CODE(p_pns, index, g_line, g_pos);
if (!memory_status)
return memory_status;

	temp_index = p_pns->code_list.amount;

  memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
if (!memory_status)
return memory_status;
	/* GENERATING CODE END *****************************************************/

  HF_get_token(p_program, p_sps);
  memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, NAMES, p_er);
if (!memory_status || *p_er)
return memory_status;

  /* GENERATING CODE:*********************************************************/
  p_pns->code_list.data[temp_index] = p_pns->code_list.amount - 1;
  /* GENERATING CODE END *****************************************************/

  if(p_sps->lexem_code != SemicolonToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      *p_er = 1;
      return memory_status;
    }
  HF_get_token(p_program, p_sps);
  return memory_status;
};
/***************************************************************************/
int HF_Parse_Operators(char*              p_program,
                       PN_T*              p_pns,
                       ER_List_T*         p_error_list,
                       ScanParamStruct_T* p_sps,
                       Name_Array_T*      NAMES,
                       int*               p_er)
{
  /*
    -- Function description:
    -- High level function for retrieving program operators.
  */
  int memory_status = 1;
  int stop=0;
  int found;
  int i = 0;
  int a = 0;
  int flag = 0;
  int start_index;

  int er = 0;
  *p_er = 0;

  do /* For one operator */
    {
      switch(p_sps->lexem_code)
	{
	case IfToken:
	  memory_status = HF_Get_If_Operator(p_program, p_pns, p_error_list, p_sps, NAMES, &er);
		if (!memory_status)
			return memory_status;
	  break;
	case WhileToken:
	  memory_status = HF_Get_While_Operator(p_program, p_pns, p_error_list, p_sps, NAMES, &er);
		if (!memory_status)
			return memory_status;
		break;


/* VAVAVA Processing 's=[...,...,...];' or s[...]=...; */
     case SIdToken:
         HF_get_token(p_program, p_sps);
		 if(p_sps->lexem_code == LeftBracketToken)
	    /* 's[...]=...;'*/
		  {
                     HF_get_token(p_program, p_sps);
                     memory_status = HF_Get_SArray_Assign_Operator(p_program, p_pns, p_error_list, p_sps, NAMES, &er);
		     if(!memory_status)
			return memory_status;
		  }
		  else
        /* 's=[...];'*/
		  {
             if(p_sps->lexem_code == AssignToken)
		     {
                        HF_get_token(p_program, p_sps);
                        memory_status = HF_Get_Whole_SArray_Assign_Operator(p_program, p_pns, p_error_list, p_sps, &er);
			if(!memory_status)
			   return memory_status;
		     }
		     else
		     {
                        memory_status = HF_Add_To_ER_List(p_error_list,
						       Error_Strings[Left_Bracket_OR_Assign_Expected],
						       p_sps->lexem_start_line,
						       p_sps->lexem_start_position);
			er = 1;
			if(!memory_status)
			   return memory_status;
		     }
		  }
	    break;


	case IdentifierToken:
	  if(strncmp(p_sps->str_buffer, p_pns->body_name, strlen(p_sps->str_buffer)+1))
	    {
	      found = HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &i, &a, &flag);
	      if(found && (flag==Array))
		{
                  HF_get_token(p_program, p_sps);
		  if(p_sps->lexem_code == LeftBracketToken)
		    {
		      HF_get_token(p_program, p_sps);
		      memory_status = HF_Get_Array_Assign_Operator(p_program, p_pns, p_error_list, p_sps, NAMES, i, a, &er);
					if (!memory_status)
						return memory_status;
		    }
		  else
		    {
		      if(p_sps->lexem_code == AssignToken)
			{
			  HF_get_token(p_program, p_sps);
			  memory_status = HF_Get_Whole_Array_Assign_Operator(p_program, p_pns, p_error_list, p_sps, i, a, &er);
				if (!memory_status)
					return memory_status;
			}
		      else
			{
			  memory_status = HF_Add_To_ER_List(p_error_list,
							    Error_Strings[Left_Bracket_OR_Assign_Expected],
							    p_sps->lexem_start_line,
							    p_sps->lexem_start_position);
			  er = 1;
				if (!memory_status)
					return memory_status;
			}
		    }
		}
	      else
		{
		  if(!found)
		    {
		      if(NAMES->amount>0)
			{
			  found = 3*(NAMES->amount-1);
			  start_index = NAMES->int_data[found] + NAMES->int_data[found+1];
			}
		      else
			{
			  start_index = 0;
			}

		      memory_status = HF_Add_To_Name_Array(NAMES, p_sps->str_buffer, start_index, 1, Variable);
					if (!memory_status)
						return memory_status;
					/* STACK RESERVATION: ******************************************************/
		      p_pns->stack_size++; /* for real variable             */
		      /* STACK RESERVATION *******************************************************/
		    }

                  HF_Find_In_Name_Array(NAMES, p_sps->str_buffer, &i, &a, &flag);
                  memory_status = HF_Get_Assign_Operator(p_program, p_pns, p_error_list, p_sps, NAMES, i, &er);
									if (!memory_status)
										return memory_status;
		}
	    }
	  else
	    stop = 1;
	  break;
	default:
	  stop  = 1;
	}

      *p_er = *p_er || er;
      if(!stop)
	{
	  if(er)
	    {
	      HF_Skip_To_Semicolon(p_program, p_sps);
	      if(p_sps->lexem_code != SemicolonToken)
		stop = 1;
	      else
		HF_get_token(p_program, p_sps);
	    }
	}
    }while(!stop);
  return memory_status;
}

int HF_Get_Array_Decl(char*              p_program,
                      PN_T*              p_pns,
                      ER_List_T*         p_error_list,
                      ScanParamStruct_T* p_sps,
                      Name_Array_T*      NAMES,
                      int*               er)
{
  /*
    -- Function description:
    -- High level function for retrieving array declarations.
  */
  int memory_status = 1;
  int start_index;
  int temp;

  do
    {
      *er = 1;
      if(NAMES->amount>0)
	{
	  temp = 3*(NAMES->amount-1);
	  start_index = NAMES->int_data[temp] + NAMES->int_data[temp+1];
	}
      else
	{
	  start_index = 0;
	}

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != IdentifierToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Identifier_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
		if (!memory_status)
			return memory_status;
	  HF_Skip_To_Semicolon(p_program, p_sps);
	  break;
	}

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != LeftBracketToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Left_Bracket_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);




		if (!memory_status)
			return memory_status;
	  HF_Skip_To_Semicolon(p_program, p_sps);
	  break;
	}

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != RealValueToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Real_Number_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
		if (!memory_status)
			return memory_status;
	  HF_Skip_To_Semicolon(p_program, p_sps);
	  break;
	}

      if(HF_round(p_sps->real_value) < 1)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Array_Size_Must_Be_Greater_Zero],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);

		if (!memory_status)
			return memory_status;
	  HF_Skip_To_Semicolon(p_program, p_sps);
	  break;
	}

      HF_get_token(p_program, p_sps);
      if(p_sps->lexem_code != RightBracketToken)
	{
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Right_Bracket_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
		if (!memory_status)
			return memory_status;
	  HF_Skip_To_Semicolon(p_program, p_sps);
	  break;
	}

      memory_status = HF_Add_To_Name_Array(NAMES, p_sps->str_buffer, start_index, HF_round(p_sps->real_value), Array);
			
			
			if (!memory_status)
				return memory_status;

      /* STACK RESERVATION: ******************************************************/
      p_pns->stack_size += HF_round(p_sps->real_value);
      /* STACK RESERVATION *******************************************************/

      HF_get_token(p_program, p_sps);
      *er = 0;
    }while(p_sps->lexem_code == CommaToken);

  if(p_sps->lexem_code != SemicolonToken)
    {
      *er = 1;
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Semicolon_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      if(!memory_status)
	return memory_status;
      HF_Skip_To_Semicolon(p_program, p_sps);
    }
  HF_get_token(p_program, p_sps);

  return memory_status;
}

int HF_Get_Body_Def(char*              p_program,
                    PN_T*              p_pns,
                    ER_List_T*         p_error_list,
                    ScanParamStruct_T* p_sps,
                    int*               er)
{
  /*
    -- Function description:
    --  Function processes the sequence of array declaration and operator instructions.
  */
  int memory_status = 1;
  int g_line, g_pos;

  int er_array = 0;
  int er_operators = 0;

  Name_Array_T NAMES;
  HF_Name_Array_Init(&NAMES);


  HF_get_token(p_program, p_sps);
  while(p_sps->lexem_code == ArrayToken)
    {
      memory_status = HF_Get_Array_Decl(p_program, p_pns, p_error_list, p_sps, &NAMES, &er_array);/* Process array declarations */
	  
      if(!memory_status)
	goto label_exit;
    }
  memory_status = HF_Parse_Operators(p_program, p_pns, p_error_list, p_sps, &NAMES, &er_operators);
  if(!memory_status)
    goto label_exit;

  if(p_sps->lexem_code != IdentifierToken)
    {
      er_array = 1;
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Body_Name_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      if(!memory_status)
	goto label_exit;
    }
  else
    {
      if(strncmp(p_sps->str_buffer, p_pns->body_name, strlen(p_sps->str_buffer)+1))
	{
	  er_array = 1;
	  memory_status = HF_Add_To_ER_List(p_error_list,
					    Error_Strings[Body_Name_Expected],
					    p_sps->lexem_start_line,
					    p_sps->lexem_start_position);
	  if(!memory_status)
	    goto label_exit;
	}
      else
	{
	  HF_get_token(p_program, p_sps);
	  if(p_sps->lexem_code != AssignToken)
	    {
	      er_array = 1;
	      memory_status = HF_Add_To_ER_List(p_error_list,
						Error_Strings[Assign_Mark_Expected],
						p_sps->lexem_start_line,
						p_sps->lexem_start_position);
	      if(!memory_status)
		goto label_exit;
	    }
	  else
	    {
	      int temp_index;

	      g_line = p_sps->lexem_start_line;
	      g_pos  = p_sps->lexem_start_position;
	      /* GENERATING CODE:*********************************************************/
	      /* 1 */     memory_status = HF_GEN_CODE(p_pns, CODE_body_assignment, g_line, g_pos);
	      if(!memory_status)
		return memory_status;

	      temp_index = p_pns->code_list.amount;

	      /* 2 */     memory_status = HF_GEN_CODE(p_pns, 0, g_line, g_pos);/* RESERVATION PLACE */
	      if(!memory_status)
		return memory_status;
	      /* GENERATING CODE END *****************************************************/
	      HF_get_token(p_program, p_sps);
	      /* 3^ */    memory_status = HF_Get_Expression(p_program, p_pns, p_error_list, p_sps, &NAMES, er);
	      if(!memory_status || *er)
		goto label_exit;
	      /* GENERATING CODE:*********************************************************/
	      /* 2 */     p_pns->code_list.data[temp_index] = p_pns->code_list.amount-1;
	      /* GENERATING CODE END *****************************************************/
	      if(p_sps->lexem_code != SemicolonToken)
		{
		  er_array = 1;
		  memory_status = HF_Add_To_ER_List(p_error_list,
						    Error_Strings[Semicolon_Expected],
						    p_sps->lexem_start_line,
						    p_sps->lexem_start_position);
		  if(!memory_status)
		    goto label_exit;
		}
	      else
		{
		  HF_get_token(p_program, p_sps);
		  if(p_sps->lexem_code != RightBraceToken)
		    {
		      er_array = 1;
		      memory_status = HF_Add_To_ER_List(p_error_list,
							Error_Strings[Right_Brace_Expected],
							p_sps->lexem_start_line,
							p_sps->lexem_start_position);
		      if(!memory_status)
			goto label_exit;
		    }
		  HF_get_token(p_program, p_sps);
		}
	    }
	}
    }
  *er = er_array || er_operators;
 label_exit:
  HF_Delete_Name_Array(&NAMES);
  return memory_status;
}


int HF_Get_X_A(char*              p_program,
               PN_T*              p_pns,
               ER_List_T*         p_error_list,
               ScanParamStruct_T* p_sps,
               int*               er)
{
  /*
    -- Function description:
    --  Function retrieves the body description from program.
  */
/* VAVAVA_NEW 
     HF program argument list must now have either of two forms, e.g.:
	     my_model(x[4], a[2]) {  -- as in v.1.0
	and
		 my_model(x[4], a[2], s[3]) { -- as in v.1.2
 - NB: the function name remains the same: HF_Get_X_A (not HF_Get_X_A_S) !
*/
/* VAVAVA_NEW NB! 29.10.01: Now it is possible not to use obligotary s[1], 
	if there are no s array; however, it works as if s[1] is present */

  int memory_status = 1;

  *er = 1;

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != LeftParenToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Left_Parenthes_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != XIdToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[XIdExpected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != LeftBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Left_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != RealValueToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Real_Number_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }
  else
    {
      p_pns->X_dim = HF_round(p_sps->real_value);
      p_pns->stack_size += p_pns->X_dim;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != RightBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Right_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != CommaToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Comma_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != AIdToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[AIdExpected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != LeftBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Left_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != RealValueToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Real_Number_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }
  else
    {
      p_pns->A_dim = HF_round(p_sps->real_value);
      p_pns->stack_size += p_pns->A_dim;
    }

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != RightBracketToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Right_Bracket_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

/* VAVAVA Processing "S" array */

   HF_get_token(p_program, p_sps);

/* VAVAVA_NEW Possibly, there are no s array, so ')' expected */

	if(p_sps->lexem_code == RightParenToken)
		{	

/* VAVAVA_NEW We act as if 's[1]' is present */

			p_pns->S_dim = HF_round(1.0);
			p_pns->stack_size += p_pns->S_dim;
			
			goto HF_GET_X_A_End;
		}

   if(p_sps->lexem_code != CommaToken) /* VAVAVA : ","? ( ; ")" will be implemented later - it is implemented on 29.10.01! */
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Comma_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      return memory_status;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != SIdToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[SIdExpected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      return memory_status;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != LeftBracketToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Left_Bracket_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      return memory_status;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != RealValueToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Real_Number_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      return memory_status;
   }
   else
   {
      p_pns->S_dim = HF_round(p_sps->real_value);
      p_pns->stack_size += p_pns->S_dim;
   }

   HF_get_token(p_program, p_sps);
   if(p_sps->lexem_code != RightBracketToken)
   {
      memory_status = HF_Add_To_ER_List(p_error_list,
				     Error_Strings[Right_Bracket_Expected],
				     p_sps->lexem_start_line,
				     p_sps->lexem_start_position);
      return memory_status;
   }

/* The end of "s" array processing */

  HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != RightParenToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Right_Parenthes_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

/*VAVAVA_NEW Label for case when s array is absent */

HF_GET_X_A_End: HF_get_token(p_program, p_sps);
  if(p_sps->lexem_code != LeftBraceToken)
    {
      memory_status = HF_Add_To_ER_List(p_error_list,
					Error_Strings[Left_Brace_Expected],
					p_sps->lexem_start_line,
					p_sps->lexem_start_position);
      return memory_status;
    }

  *er = 0;
  return memory_status;
}
/*************************************/
/*************************************/
/*************************************/
/* Create, copy, delete PNS  implementation */
int HF_CreateNewPNS(PN_T** p_p_pns)
{
  /*
    -- Function description:
    --  Function makes the new initialized internal body representation structure.
  */

	int return_value;

  *p_p_pns = malloc(sizeof(PN_T));
#ifdef DEBUG
  assert (*p_p_pns != NULL);
#endif /* DEBUG */

  /* Init all fields */
  (*p_p_pns)->body_name = NULL;
  (*p_p_pns)->X_dim = 0;
  (*p_p_pns)->A_dim = 0;
  (*p_p_pns)->S_dim = 0;     /* VAVAVA */
  (*p_p_pns)->stack_size = 0;
  (*p_p_pns)->ad_size = 0;
  HF_Int_Array_Init(&((*p_p_pns)->code_list));
  HF_Int_Array_Init(&((*p_p_pns)->index_to_lexem));
  HF_Double_Array_Init(&((*p_p_pns)->constant_values));
  HF_String_Array_Init(&((*p_p_pns)->str_param));
  
  HF_Init_PN_List(&((*p_p_pns)->other_body_code_list));
  
  return_value = HF_Int_Array_Make(&((*p_p_pns)->code_list), 20);
  assert (return_value != 0 );
  

  return_value = HF_Int_Array_Make(&((*p_p_pns)->index_to_lexem), 40);
  assert (return_value != 0 );

  return_value = HF_Double_Array_Make(&((*p_p_pns)->constant_values), 20);
  assert (return_value != 0 );

  return 1;
}

PN_T* HF_Copy_PNS(PN_T* p_pns)
{
  /*
    -- Function description:
    --  Function returns copy of all data from p_pns
  */
  int status = 1, return_value;
  PN_T* result;
  //#ifdef DEBUG
  return_value = HF_CreateNewPNS(&result);
  if (!return_value)
		return NULL;

  //#endif /* DEBUG */  
  result->body_name = malloc(strlen(p_pns->body_name)+1);
  //  assert(0);
  if(result->body_name == NULL)
    {
      status = 0;
    }
  else
    {
      strncpy(result->body_name, p_pns->body_name, strlen(p_pns->body_name)+1);
      
      result->X_dim = p_pns->X_dim;
      result->A_dim = p_pns->A_dim;
	  result->S_dim = p_pns->S_dim;   /* VAVAVA */

      result->stack_size = p_pns->stack_size;
      result->ad_size = p_pns->ad_size;
      
      status = HF_Copy_Int_Array(&(p_pns->code_list), &(result->code_list));
      if(status)
		status = HF_Copy_Int_Array(&(p_pns->index_to_lexem), &(result->index_to_lexem));
      if(status)
		status = HF_Copy_Double_Array(&(p_pns->constant_values), &(result->constant_values));
      if( status )
		status = HF_Copy_String_Array(&(p_pns->str_param), &(result->str_param));
      
      if(status)
		status = HF_Copy_PNS_List(&(p_pns->other_body_code_list), &(result->other_body_code_list));
    }
if (!status)
HF_DeletePNS(&result);
  return (status)?result:NULL;
}


void HF_DeletePNS(PN_T** p_p_pns)
{
  /*
    -- Function description:
    --  Function deallocates all the memory used by internal body representation
    --  structure.
  */
  if(*p_p_pns != NULL)
    {
      /* Delete all fileds */
      if((*p_p_pns)->body_name != NULL)
	  { free((*p_p_pns)->body_name); }
      
	  (*p_p_pns)->body_name = NULL;


      HF_Delete_Int_Array(&((*p_p_pns)->code_list));

      HF_Delete_Int_Array(&((*p_p_pns)->index_to_lexem));

      HF_Delete_Double_Array(&((*p_p_pns)->constant_values));
      
	  if((*p_p_pns)->str_param.data != NULL)
	  { HF_Delete_String_Array(&((*p_p_pns)->str_param)); }
      
      HF_Empty_PN_List(&((*p_p_pns)->other_body_code_list));
      if (*p_p_pns!=NULL)
	  { free(*p_p_pns); }
      *p_p_pns = NULL;
    }
}
/* End of section for create, delete PNS implementation */
/********************************************************/
/********************************************************/
/********************************************************/
int HF_parse(char*        p_program,
             PN_List_T*   p_pn_list,
             ER_List_T*   p_er_list,
			 PN_List_T*   p_Library)
{
  /*
    -- Function description:
    --  Function parses the p_program into the list of internal body representation
    --  structure.
  */
  int i;
  /* Returns 0, if memory allocation fails;                     */
  ScanParamStruct_T sps;
  PN_T*   p_pns = NULL;
  
  int memory_status = 1;

  HF_local_library = p_pn_list;
  HF_library       = p_Library;

  /* Initialization for arguments */
  HF_Init_PN_List(p_pn_list);
  HF_Init_ER_List(p_er_list);

	/* p_library is never initialized nor used */
	/* it should be initialized here */
	/* otherwise a problem */
	/* occurs in HF_Find_body_in_library() when called with HF_library */
	/* because HF_Library is != NULL (class variable in HFInterpreter) */
	/* but first field points nowhere (0xcdcdcdcd) */
	HF_Init_PN_List(p_Library);

  /* Initillization for sps */
  sps.lexem_end_position = 1; /* logicaly: first symbol in line */
  sps.lexem_end_line     = 1; /* logicaly: first line in program */
  sps.str_position       = 0;

  for(i=0; i < HF_FUNCTIONS_AMOUNT; i++)
    functions_info[i].F_instance_counter = 0;

  /* Compute CODE_First_Data_Index */
  CODE_First_Data_Index = FUNC_unary_tilda + 1 + HF_FUNCTIONS_AMOUNT;

  /* START TO WORK */
  HF_get_token(p_program, &sps);
  
  while((sps.lexem_code != EndOfParseToken))
    {
      memory_status = HF_CreateNewPNS(&p_pns);
      
	  if(!memory_status)
		break;

      if(sps.lexem_code != IdentifierToken) /* Body name expected */
		{
			/* Grammar error */
			memory_status = HF_Add_To_ER_List(p_er_list,
											Error_Strings[Body_Name_Expected],
											sps.lexem_start_line,
											sps.lexem_start_position);

			break;
		}
      else /* Identifier found check for unique name */
		{
			PN_NODE_T* iterator = p_pn_list->first;
			int found = 0;

			while((iterator != NULL) && (!found))
			{
				found = !strncmp(iterator->pn_info->body_name, sps.str_buffer, strlen(sps.str_buffer)+1);
				iterator = iterator->next;
			}

			if(found)
			{/* Name already defined */
				memory_status = HF_Add_To_ER_List(p_er_list,
							Error_Strings[Not_Unique_Body_Name],
							sps.lexem_start_line,
							sps.lexem_start_position);
				break;
			}
			else
			{
				/* Ok */
				p_pns->body_name = calloc(strlen(sps.str_buffer)+1,sizeof(char));
				if(p_pns->body_name == NULL)
				{
					memory_status = 0;
					break;
				}
				else
				{
					int er;
					strncpy(p_pns->body_name, sps.str_buffer, strlen(sps.str_buffer)+1);
					memory_status = HF_Get_X_A(p_program, p_pns, p_er_list, &sps, &er);
					if(!memory_status || er)
						break;
					memory_status = HF_Get_Body_Def(p_program, p_pns, p_er_list, &sps, &er);
					if(!memory_status || er)
						break;
					memory_status = HF_Add_To_PN_List(p_pn_list, p_pns);
					if(!memory_status)
						break;
				}
			}
		}
    }

	if(!memory_status)
    {
		HF_DeletePNS(&p_pns); 
		HF_Empty_PN_List(p_pn_list);
		HF_Empty_ER_List(p_er_list);
     
    }
	else
	{
		if(HF_Was_Error(p_er_list))
		{
			HF_DeletePNS(&p_pns); 
			HF_Empty_PN_List(p_pn_list);
	 
		}
	}

  return memory_status;
} /* End of parse */
