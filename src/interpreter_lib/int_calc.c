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
  -- Name:            CALC.C
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  implements calculation stage of interpreter.
  --
  -- Author:          Anatoliy P. Osipov (v. 1.0)
--                  Valery Adzhiev in January, 2001
--                    produced version 1.2 to incorporate "S" array
--                  These additions are marked by "VAVAVA"
--
-- Last revised at: 25 April 2003
					30 septembre 2003

	modifications:
					* added the computation for s array attributes;
					* added the save and restore of s[] in the stack when
					call to an external function, so that s[] of a called
					function can be modified in the called function, and will be 
					available in the caller with the modified values, and not the initial
					as it was the case before;
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "general.h"

/* The variable BELOW gives current instance number for function. */
/* It is valid only inside the function definition or its resriction. */
int F_instance_No; 

/* Temporal pointers to storage */
int* HF_global_code;
double* HF_global_data;

#ifdef HF_DEBUG
#define INC(ic) if(HF_inc_ic(p_pn, &ic, error_info)) return 0/*Error*/
#else
#define INC(ic) ic++
#endif

extern int HF_FUNCTIONS_AMOUNT;


int HF_Get_Runtime_Array(int index, double** p, int *size)
{
  /*
    -- Function retrieves the information about the stored array
    -- by index
  */
  if((HF_global_code == NULL) || (HF_global_data == NULL) || (index < 2))
    return 0;
  if(HF_global_code[index--] != I_ARRAY_AS_ARG)
    return 0;
  *p    = &HF_global_data[HF_global_code[index--]];
  *size = HF_global_code[index];
	
  if((*size) < 1)
    return 0;
	
  return 1;
}

int HF_min_int(int a, int b)
{
  /*
    -- Function description:
    --  Function returns the min value from two arguments.
  */
  return (a>b)?b:a;
}

int   CODE_First_Data_Index;

extern double ALPHA;
extern double EPS;
extern function_info_T functions_info[];
extern char* Error_Strings[];

extern int HF_round(double);

void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic)
{
  /*
    -- Function description:
    --  Function fills the Error_Info_T (EI) structure with the next information:
    --  error description string message, line number in the source program text,
    --  position number in the line.
  */
  EI->error_text = Error_Strings[er_index];
	
  if(ic*2+1 < p_pn->index_to_lexem.amount)
    {
      EI->line       = p_pn->index_to_lexem.data[ic*2];
      EI->position   = p_pn->index_to_lexem.data[ic*2+1];
    }
  else
    {
      EI->line       = 0;
      EI->position   = 0;
    }
}

int HF_inc_ic(PN_T* p_pn, int* ic, Error_Info_T* error_info)
{
  /*
    -- Function description:
    --  Function increments internal instruction counter and check the validity of
    --  the new value.
  */
#ifdef HF_DEBUG
  if((*ic + 1) < p_pn->code_list.amount)
    {
#endif
      (*ic)++;
      return 0; /* Ok */
#ifdef HF_DEBUG
    }
  else
    {
      HF_Fill_Error(p_pn, Bad_Code, error_info, *ic);
      return 1; /* Error */
    }
#endif
}
/****************************************************************************/
/****************************************************************************/
int HF_Calc_Expr(PN_T*         p_pn,
                 Error_Info_T* error_info,
                 int           start_index,
                 int*          last_index,
                 double*       stack,
                 int*          stack_usage,
                 double*       value,
				 double*       svalue)   /* VAVAVA !!!! */
				 /* Returns 1 if no errors and expression is calculated;
					0 - otherwise
				  */
{  /* This is recursive function:                                          */
  /* When last_index == start_index we'got an error                       */
	
  /*
    -- Function description:
    --  Function for computation expressions.
  */
  int code;
  int code_index;
  double f_args[ARGS_LIMIT];
  int n, i;
  int instance_No;
  double f_temp;
	
    
#ifdef HF_DEBUG
  if(*last_index == start_index)
    {
      HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
      return 0; /* Error */
    }
#endif
  code_index = *last_index;
  code = p_pn->code_list.data[code_index];
  /*
    searching for array out of bound
  */
#ifdef DEBUG
  assert ((p_pn->code_list).size >= code_index);
#endif /* DEBUG */  
  /* The case of external body call: */
  if(code == I_EXTERN_CALL)
    {
      int code_index;
      int arg_1, amount_1;
      int arg_2, amount_2;
	  int arg_3, amount_3;   /* VAVAVA */
      PN_T*	code;
      int* temp_code = HF_global_code;
      double* temp_data = HF_global_data;
      int to_return;
	  double *temp_S_value;
		
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      code_index = p_pn->code_list.data[*last_index];
      /*
	searching for array out of bound
      */
#ifdef DEBUG
      assert ((p_pn->code_list).size >= *last_index);
#endif /* DEBUG */  		
      
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      arg_1 = p_pn->code_list.data[*last_index];
      /*
	searching for array out of bound
      */
#ifdef DEBUG
      assert ((p_pn->code_list).size >= *last_index);
#endif /* DEBUG */  		

      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      arg_2 = p_pn->code_list.data[*last_index];
      /*
	searching for array out of bound
      */
#ifdef DEBUG
      assert ((p_pn->code_list).size >= *last_index);
#endif /* DEBUG */      

	/* VAVAVA 3rd argument! */
				(*last_index)--;
	#ifdef HF_DEBUG
		if(*last_index == start_index)
			{
			HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
			return 0; /* Error */
			}
	#endif
		arg_3 = p_pn->code_list.data[*last_index];
      /* VAVAVA end of getting 3rd argument */

      /* Here we've got all the data and now we proceed: */
      code = HF_Body_By_Index(&p_pn->other_body_code_list, code_index);
#ifdef HF_DEBUG
      if(code == NULL)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index+2);
	  return 0; /* Error */
	}
#endif
      amount_1 = code->X_dim;
      amount_2 = code->A_dim;
		amount_3 = code->S_dim;  /*VAVAVA */

#ifdef HF_DEBUG
      if(	(arg_1<0) || (arg_1 >= p_pn->stack_size)||
		(arg_2<0) || (arg_2 >= p_pn->stack_size)||
		(arg_3<0) || (arg_3 >= p_pn->stack_size)||  /*VAVAVA */
		(arg_1+amount_1 > p_pn->stack_size)	||
		(arg_2+amount_2 > p_pn->stack_size)
		(arg_3+amount_3 > p_pn->stack_size))    /*VAVAVA */
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index+3);
	  return 0; /* Error */
	}
#endif
      
      /* Here we check for initialization of arrays */
      for(i=0;i<amount_1;i++)
	{
	  /*
	    searching for array out of bound
	  */
#ifdef DEBUG
	  assert((i+arg_1) < p_pn->stack_size);
#endif /* DEBUG */	  
	  if(stack_usage[i+arg_1] != Initialized)
	    {
	      HF_Fill_Error(p_pn, Unassigned_Array, error_info, *last_index+1);
	      return 0; /* Error */
	    }
	  stack[p_pn->stack_size + i] = stack[i+arg_1];
	  stack_usage[p_pn->stack_size + i] = stack_usage[i+arg_1];
	}
      for(i=0;i<amount_2;i++)
	{
	  if(stack_usage[i+arg_2] != Initialized)
	    {
	      HF_Fill_Error(p_pn, Unassigned_Array, error_info, *last_index+0);
	      return 0; /* Error */
	    }
	  stack[p_pn->stack_size + amount_1 + i] = stack[i+arg_2];
	  stack_usage[p_pn->stack_size + amount_1 + i] = stack_usage[i+arg_2];
	}

	for(i=0;i<amount_3;i++)    /*VAVAVA */
			{
			if(stack_usage[i+arg_3] != Initialized)
				{
				HF_Fill_Error(p_pn, Unassigned_Array, error_info, *last_index+0);
				return 0; /* Error */
				}
			stack[p_pn->stack_size + amount_1 + amount_2 + i] = stack[i+arg_3];
			stack_usage[p_pn->stack_size + amount_1 + amount_2 + i] = stack_usage[i+arg_3];  /*VAVAVA */
			}

      /* Very interesting case:
       * We do have to remember the global data and code for array access.
       * As far as, HF was developed as first without the call to
       * earlier defined sub-programs, it was no need to remember it.
       * The error case was(!): when we recursively call other function
       * code the variables global data and global code are replaced and when we try
       * again to reach the array defined in the native code
       * we never will be able to do that.
       * Sorry, for English...
       *
       * 27 August'98.
       */

	/* 	the size of s may be different between the objects.
		for example:
		obj1(x[3],a[1],s[5]){}
		my_object(x[3],a[1],s[3]){}
		Using the same pointer svalue is an error.
	*/
		temp_S_value = (double *)malloc(sizeof(double)*code->S_dim);
      to_return = HF_calc(code, stack+p_pn->stack_size, stack_usage+p_pn->stack_size, value,/*svalue*/ temp_S_value, error_info);


	  // suppose that in the called extern function the arg have been modified
	  // then we need to consider the modification here
	  
	  /*for (i=0; i<amount_1; i++)
		stack[i+arg_1] = stack[p_pn->stack_size + i];;	

	  for (i=0; i<amount_2; i++)
		  stack[i+arg_2] = stack[p_pn->stack_size + amount_1 + i];;
	  */
	  for (i=0; i<amount_3; i++)
		stack[i+arg_3]=stack[p_pn->stack_size + amount_1 + amount_2 + i];

      HF_global_code = temp_code;
      HF_global_data = temp_data;
      
	  free(temp_S_value);

      return to_return;
      
      /**********************************************************************/
      /***********************************************************************/
    }
	
  /* Simple case: when the expression is array function argument */
  if(code == I_ARRAY_AS_ARG)
    {
      int first  = p_pn->code_list.data[(*last_index) - 1];
      int amount = p_pn->code_list.data[(*last_index) - 2];
      int total  = first + amount;
		
      *value = (*last_index);
#ifdef HF_DEBUG
      if((((*last_index)-2) <= start_index) ||
	 (first<0)  ||
	 (amount<1) ||
	 (total> p_pn->stack_size)           )
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0;
	}
#endif
      (*last_index) -= 2;
      
      for(i=first;i<total;i++)
	//assert(stack_usage[i]==Initialized);
	
	if(stack_usage[i] != Initialized)
	  {
	    HF_Fill_Error(p_pn, Unassigned_Array, error_info, *last_index);
	    return 0;
	  }
	
			
      return 1;
    }
	
  /* Simple case: when the expression is array indexing                   */
  if(code == I_ARRAY)
    {
      int first;
      int global_i;
      int ar_item_amount;
      int er_status;
      double temp_value;
      int actual_i;
      int g_pos = *last_index;
		
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      first = p_pn->code_list.data[*last_index]; /* FIRST INDEX            */
#ifdef HF_DEBUG
      if((first<start_index) || (first>= *last_index-2))
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
		
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      global_i = p_pn->code_list.data[*last_index];
      /* GLOBAL INDEX IN THE STACK */
#ifdef HF_DEBUG
      if((global_i<0) || (global_i >= p_pn->stack_size))
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
		
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
      ar_item_amount = p_pn->code_list.data[*last_index];
      /* ARRAY ITEM AMOUNT */
#ifdef HF_DEBUG
      if((ar_item_amount<0) || (ar_item_amount>p_pn->stack_size-global_i))
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0; /* Error */
	}
#endif
		
      (*last_index)--;
      er_status = HF_Calc_Expr(p_pn,
			       error_info,
			       first,
			       last_index,
			       stack,
			       stack_usage,
			       &temp_value,
				   svalue);      /* VAVAVA !!!! */
      if(!er_status)
	return 0; /* Error */
		
      actual_i = HF_round(temp_value);
      if((actual_i>0) && (actual_i <= ar_item_amount))
	{
	  if(stack_usage[global_i+actual_i-1] == Initialized)
	    {
	      *value = stack[global_i+actual_i -1];
	      return 1; /* No errors */
	    }
	  else
	    {
	      HF_Fill_Error(p_pn, Unassigned_Identificator, error_info, g_pos);
	      return 0; /* Error */
	    }
	}
      else
	{
	  HF_Fill_Error(p_pn, Index_Out_Of_Range, error_info, *last_index);
	  return 0; /* Error */
	}
    }
	
  /* Simple case: when the expression is string index                     */
  if(code == CODE_String_Index)
    {
      (*last_index)--;
#ifdef HF_DEBUG
      if(*last_index == start_index)
	{
	  HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
	  return 0;
	}
#endif
      *value = p_pn->code_list.data[*last_index]; /* String index          */
#ifdef HF_DEBUG
      if((int)*value < p_pn->str_param.amount)
#endif
	return 1;
#ifdef HF_DEBUG
      HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
      return 0;
#endif
    }
	
  /* Simple case: when the expression consists of constant data           */
  {
    int First_Data = CODE_First_Data_Index;
    int Last_Data  = CODE_First_Data_Index + p_pn->constant_values.amount-1;
    if((First_Data <= code)&&
       (code       <= Last_Data))
      {
	*value = p_pn->constant_values.data[code - First_Data];
	return 1; /* We've got the value                                  */
      }
  }
  /* Simple case: when the expression consists of local data              */
  {
    /* Code are negative, so invert it                                   */
    int correct_i = -(code + 1);
		
    if((0 <= correct_i) &&
       (correct_i < p_pn->stack_size))
      {
	if(stack_usage[p_pn->X_dim+p_pn->A_dim+correct_i+p_pn->S_dim] != Initialized) /*VAVAVA*/
	  {
	    HF_Fill_Error(p_pn, Value_Not_Initialized, error_info, *last_index);
	    return 0; /* Error */
	  }
	*value = stack[p_pn->X_dim+p_pn->A_dim+p_pn->S_dim+correct_i]; /*VAVAVA*/
	return 1; /* We've got the value                                  */
      }
  }
		
  /* Case when expression consists of function:                           */
  /* n - amount of arguments:                                             */
  switch(code)
    {
    case FUNC_bool_and:
    case FUNC_bool_or:
    case FUNC_bool_less:
    case FUNC_bool_greater:
    case FUNC_bool_not_greater:
    case FUNC_bool_not_less:
    case FUNC_bool_equal:
    case FUNC_bool_not_equal:
				
    case FUNC_binary_plus:
    case FUNC_binary_minus:
    case FUNC_binary_mult:
    case FUNC_binary_divide:
    case FUNC_binary_power:
    case FUNC_binary_cart:
    case FUNC_binary_ampersand:
    case FUNC_binary_union:
    case FUNC_binary_neg:
      n = 2;
      break;
    case FUNC_unary_not:
    case FUNC_unary_plus:
    case FUNC_unary_minus:
    case FUNC_unary_tilda:
      n = 1;
      break;
    default: /* Check for built-in functions:                            */
      if((FUNC_unary_tilda<code) && (code<CODE_First_Data_Index))
	{
	  n = functions_info[code-FUNC_unary_tilda-1].dim;
	  instance_No = p_pn->code_list.data[--(*last_index)];
	  break;
	}
#ifdef HF_DEBUG
      /* Case when it's not argument for function, no data ==> ERROR          */
      HF_Fill_Error(p_pn, Bad_Code, error_info, *last_index);
      return 0; /* Error */
#endif
    }
		
  /* Get arguments:                                                       */
  for(i = 0;i<n;i++)
    {
      (*last_index)--;
      if(!HF_Calc_Expr(p_pn, error_info, start_index, last_index,
		       stack, stack_usage, f_args+i, svalue))
	{
	  return 0;/* Error */
	}
    }
		
  /* Calculate function:                                                  */
  switch(code)
    {
    case FUNC_bool_and:
      if((fabs(f_args[0]) >= EPS) && (fabs(f_args[1]) >= EPS))
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_or:
      if((fabs(f_args[0]) < EPS) && (fabs(f_args[1]) < EPS))
	{
	  *value = 0.0;
	  return 1;
	}
      *value = 1.0;
      return 1;
				
    case FUNC_bool_less:
      if(f_args[1] < f_args[0])
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_greater:
      if(f_args[1] > f_args[0])
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_not_greater:
      if(f_args[1] <= f_args[0])
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_not_less:
      if(f_args[1] >= f_args[0])
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_equal:
      if(fabs(f_args[1] - f_args[0]) < EPS)
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_bool_not_equal:
      if(fabs(f_args[1] - f_args[0]) >= EPS)
	{
	  *value = 1.0;
	  return 1;
	}
      *value = 0.0;
      return 1;
				
    case FUNC_binary_plus:
      *value = f_args[1] + f_args[0];
      return 1;
				
    case FUNC_binary_minus:
      *value = f_args[1] - f_args[0];
      return 1;
				
    case FUNC_binary_mult:
      *value = f_args[1] * f_args[0];
      return 1;
				
    case FUNC_binary_divide:
      if(fabs(f_args[0])<EPS)
	{
	  HF_Fill_Error(p_pn, Division_By_Zero, error_info, code_index);
	  return 0; /* Error */
	}
      *value = f_args[1] / f_args[0];
      return 1;
				
    case FUNC_binary_power:
      if(fabs(f_args[1]) < EPS)
	{
	  if(f_args[0] <= EPS)
	    {
	      HF_Fill_Error(p_pn, Zero_Value_In_Negative_Or_Zero_Power, error_info, code_index);
	      return 0; /* Error */
	    }
	  *value = 0.0;
	  return 1;
	}
      if( (f_args[0] - floor(f_args[0])) < EPS )
	{/* Integer power */
	  int power = HF_round(fabs(f_args[0]));
	  *value = 1.0;
	  for(i=0;i<power;i++)
	    *value *= f_args[1];
	  if(f_args[0] < 0.0)
	    *value = 1/(*value);
	  return 1;
	}
      else
	{/* Real power */
	  if(f_args[1]<0.0)
	    {
	      HF_Fill_Error(p_pn, Negative_Value_For_Power, error_info, code_index);
	      return 0; /* Error */
	    }
	  *value = exp(f_args[0]*log(f_args[1]));
	  return 1;
	}
    case FUNC_binary_cart:
    case FUNC_binary_ampersand:
      if((ALPHA<0) || (ALPHA>1))
	{
	  HF_Fill_Error(p_pn, ALPHA_Range_Error, error_info, code_index);
	  return 0; /* Error */
	}


      *value = (1/(1+ALPHA))*(f_args[1] + f_args[0] -
			      sqrt(fabs(f_args[1]*f_args[1] + f_args[0]*f_args[0] -
					2*ALPHA*f_args[0]*f_args[1])));

//	  *value = min(f_args[1], f_args[0]);
      return 1;
				
    case FUNC_binary_union:
      if((ALPHA<0) || (ALPHA>1))
	{
	  HF_Fill_Error(p_pn, ALPHA_Range_Error, error_info, code_index);
	  return 0; /* Error */
	}
				
      *value = (1/(1+ALPHA))*(f_args[1] + f_args[0] +
			      sqrt(fabs(f_args[1]*f_args[1] + f_args[0]*f_args[0] -
					2*ALPHA*f_args[0]*f_args[1])));

//  	  *value = max(f_args[1], f_args[0]);

      return 1;
				
    case FUNC_binary_neg:
      if((ALPHA<0) || (ALPHA>1))
	{
	  HF_Fill_Error(p_pn, ALPHA_Range_Error, error_info, code_index);
	  return 0; /* Error */
	}
				
      f_args[0] = -f_args[0];
				
      *value = (1/(1+ALPHA))*(f_args[1] + f_args[0] -
			      sqrt(fabs(f_args[1]*f_args[1] + f_args[0]*f_args[0] -
					2*ALPHA*f_args[0]*f_args[1])));
      return 1;
				
    case FUNC_unary_not:
      *value = 0.0;
      if(fabs(f_args[0]) < EPS)
	*value = 1.0;
      return 1;
    case FUNC_unary_plus:
      *value = f_args[0];
      return 1;
				
    case FUNC_unary_minus:
      *value = -f_args[0];
      return 1;
				
    case FUNC_unary_tilda:
      *value = -f_args[0];
      return 1;
      /********************* BUILD-IN FUNCTIONS ************************/
    case FUNC_sqrt  :
      if(f_args[0] < 0.0)
	{
	  HF_Fill_Error(p_pn, Negative_For_Root, error_info, code_index);
	  return 0; /* Error */
	}
      *value = sqrt(f_args[0]);
      return 1;
				
    case FUNC_exp   :
      *value = exp(f_args[0]);
      return 1;
    case FUNC_log   :
      if(!(f_args[0]>0.0))
	{
	  HF_Fill_Error(p_pn, Negative_For_Log, error_info, code_index);
	  return 0; /* Error */
	}
      *value = log(f_args[0]);
      return 1;
    case FUNC_logd  :
      if(!(f_args[0]>0.0))
	{
	  HF_Fill_Error(p_pn, Negative_For_Log, error_info, code_index);
	  return 0; /* Error */
	}
      *value = log10(f_args[0]);
      return 1;
    case FUNC_sin   :
      *value = sin(f_args[0]);
      return 1;
    case FUNC_cos   :
      *value = cos(f_args[0]);
      return 1;
    case FUNC_tan   :
      if(fabs(cos(f_args[0]))<EPS)
	{
	  HF_Fill_Error(p_pn, Tan_Argument_Error, error_info, code_index);
	  return 0; /* Error */
	}
      *value = tan(f_args[0]);
      return 1;
    case FUNC_asin  :
      if(fabs(f_args[0]) > 1.0)
	{
	  HF_Fill_Error(p_pn, Asin_Acos_Range_Error, error_info, code_index);
	  return 0; /* Error */
	}
      *value = asin(f_args[0]);
      return 1;
    case FUNC_acos  :
      if(fabs(f_args[0]) > 1.0)
	{
	  HF_Fill_Error(p_pn, Asin_Acos_Range_Error, error_info, code_index);
	  return 0; /* Error */
	}
      *value = acos(f_args[0]);
      return 1;
    case FUNC_atan  :
      *value = atan(f_args[0]);
      return 1;
    case FUNC_abs   :
      *value = fabs(f_args[0]);
      return 1;
    case FUNC_sinh  :
      *value = sinh(f_args[0]);
      return 1;
    case FUNC_cosh  :
      *value = cosh(f_args[0]);
      return 1;
    case FUNC_tanh  :
      *value = tanh(f_args[0]);
      return 1;
    case FUNC_sign  :
      if(f_args[0] < 0.0)
	*value = -1.0;
      *value = 1.0;
      return 1;
    case FUNC_mod   :
//      *value = floor(HF_round(f_args[1]) / HF_round(f_args[0]));
		*value = fmod(HF_round(f_args[1]), HF_round(f_args[0]));
      return 1;
    case FUNC_max   :
      *value = (f_args[1] > f_args[0])?f_args[1]:f_args[0];
      return 1;
    case FUNC_min   :
      *value = (f_args[1] < f_args[0])?f_args[1]:f_args[0];
      return 1;

	case FUNC_ceil   :
      *value = ceil(f_args[0]);
      return 1;
    case FUNC_floor   :
      *value = floor(f_args[0]);
      return 1;
    case FUNC_int   :
      modf(f_args[0], value);
      return 1;
    case FUNC_frac   :
      *value = modf(f_args[0], &f_temp);
      return 1;

    case FUNC_atan2 :
      *value = atan2(f_args[1], f_args[0]);
      return 1;
      /********************* BUILD-IN FUNCTIONS ************************/
    default: /* Calc built-in function:                                  */
      F_instance_No = instance_No;
				
      if(functions_info[code-FUNC_unary_tilda-1].p_F_Restriction != NULL)
	{
	  if(!functions_info[code-FUNC_unary_tilda-1].p_F_Restriction(f_args,
								      &(p_pn->str_param),
								      error_info,
								      p_pn,
								      code_index))
						
	    {
	      return 0; /* Error */ /* error_info filled by previous call    */
	    }
	}
				
      if(functions_info[code-FUNC_unary_tilda-1].p_Function != NULL)
	{
	  *value = functions_info[code-FUNC_unary_tilda-1].p_Function(f_args, &(p_pn->str_param));
	  return 1; /* No errors */
	}
      HF_Fill_Error(p_pn, Function_Not_Implemented_Error, error_info, code_index);
      return 0; /* Error */
    }
};

int HF_calc(/*  in*/  PN_T*         p_pn,
            /*  in*/  double*       stack,
            /*  in*/  int*          stack_usage,
            /* out*/  double*       value, double *svalue,
            /* out*/  Error_Info_T* error_info)
{
  /*
    -- Function description:
    --  Function for computation the value for body:
    --  p_pn is taken after call to HF_parse
    --  memory for stack and stack_usage is allocated and
    --  stack contains actual parameters for body.
  */
	
	/*  VAVAVA 
	  NB: 'svalue' array is filled only all the calculation have been made 
	  in the very end of HF_calc; all processing of 's' array is performed 
	  with using stack's cells in the range stack[X_dim+Adim] to stack[X_dim+A_dim+S_dim-1]; 
	  initially, these stack's cells are filled from outside - exactly as for X and A but
	  then (in distinction to X and A can be assigned by new valus of s array.
	*/
  int i;  /* for cycle indeces                                            */
  int IC; /* instruction counter: -> p_pn->code_list   	           */
  int SL, SD;
 int SX, SA, SS;    /* VAVAVA */
  int* instr; /* Instructions array                                       */
  double* c_data;/* Constants array                                          */
  int SI;     /* Size of instruction array                                */
  /* No errors: yet                                                          */
   
  HF_global_code = p_pn->code_list.data;
  HF_global_data = stack;

  /* Compute CODE_First_Data_Index                                           */
  CODE_First_Data_Index = FUNC_unary_tilda + 1 + HF_FUNCTIONS_AMOUNT;
  
  /* Initialize stack_usage: X_dim + A_dim variables are meaningfull         */
  for(i=0;i<p_pn->X_dim+p_pn->A_dim+p_pn->S_dim;i++)   /* VAVAVA */
    stack_usage[i] = Initialized;
  for(;i<p_pn->stack_size;i++)
    stack_usage[i] = Not_Initialized;
  
  /* 
     Here we are at the first instruction, so ic = 0                         
  */
  IC = 0;
  SX = p_pn->X_dim;/* Amount X of variables in the stack                  */
  SA = p_pn->A_dim;/* Amount A of variables in the stack                  */
  SS = p_pn->S_dim;/* Amount S of variables in the stack                  */  /* VAVAVA */
	SL = p_pn->stack_size - (SX + SA +SS); /* Local data size               */  /* VAVAVA */
	
  SD = p_pn->constant_values.amount; /* Constant data amount              */
  SI = p_pn->code_list.amount;       /* Instructions amount               */
  instr = p_pn->code_list.data;      /* This renames Instruction array    */
  c_data = p_pn->constant_values.data;

  /* VAVAVA Initializing svalue  */
     for(i=0;i<SS;i++)   	
		svalue[i] = Initialized;

  /* This renames Constant Values array */
	
  while(IC < SI)
    {
      switch(instr[IC])
	{
	case CODE_array_assignment:
	  INC(IC);
	  {  /* Declare Block                                            */
	    int si, am, di;
	    am = instr[IC]; /* Items amount                             */
					
	    INC(IC);
	    si = instr[IC]; /* Local item first index                   */
					
	    INC(IC);
	    di = instr[IC]; /* Constant item first index                */
					
#ifdef HF_DEBUG
	    /* Check for correcteness                                   */
	    if(((SL-si)>0) && ((SD-di)>0) && (am <= HF_min_int(SL-si,SD-di))
	       && (am > 0) && (si>=0) && (di>=0))
	      { /* Ok */
#endif
		for(i = 0; i<am; i++)
		  {
		    int index = SX+SA+SS+si+i;
		    stack[index] = c_data[di+i];
		    stack_usage[index] = Initialized;
		  }
		INC(IC);
						
#ifdef HF_DEBUG
	      }
	    else
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
	  }
	  break;



/* VAVAVA New code: Beginning of processing 's=[...];' */
case CODE_Sarray_assignment:
				INC(IC);
					{  /* Declare Block                                            */
					int am, di;
					am = instr[IC]; /* 's' array's items amount                             */
		
					INC(IC);
					di = instr[IC]; /* Constant item first index                */
					
#ifdef HF_DEBUG
					/* Check for correcteness                                   */
					if( (am <= SS) &&
						((SD-di)>0) && (am <= HF_min_int(SL,SD-di))
						&& (am > 0) && (di>=0))
						{ /* Ok */
#endif
						for(i = 0; i<am; i++)
							{
			/* VAVAVA NB! 's[i]' values are put in the stack instead actual parameters of 's'! */
             /*  output array 's' is also completely modified*/
							int index = SX+SA+i;    
							stack[index] = c_data[di+i];
						/*	svalue[i] = stack[index]; */  
							stack_usage[index] = Initialized;  /* VAVAVA !!!! */
							}
						INC(IC);
						
#ifdef HF_DEBUG
						}
					else
						{
						HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
						return 0; /* Error */
						}
#endif
					}
					break;
/* VAVAVA End of processing 's=[...];' */
	case CODE_assignment:
	  INC(IC);
				
	  { /* Declare Block */
	    int si;
	    int expr_end_index;
	    double temp_value;
	    int er_status;
	    int index;
					
	    si = instr[IC]; /* He we've got the variable index          */
#ifdef HF_DEBUG
	    if((si<0) || !(si<SL))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
					
	    INC(IC);
					
	    expr_end_index = instr[IC];
					
#ifdef HF_DEBUG
	    if((expr_end_index<=IC) || (!(expr_end_index<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
					
	    { /* Declare block */
	      int last_index = expr_end_index;
	      er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
				       stack, stack_usage, &temp_value, svalue);
	    }
	    if(!er_status)
	      return 0; /* Error, error_info is filled */
						
	    index = SX+SA+si+SS;
	    stack[index] = temp_value;
	    stack_usage[index] = Initialized;
						
	    IC = expr_end_index;
	    INC(IC);
	  }
					
	  break;
	case CODE_array_item_assignment:
	  INC(IC);
				
	  { /* Declare Block */
	    int ar_start_index; /* First item of array is
				   SX+SA+ar_start_index
				*/
	    int ar_items_amount;/* Amount of items in array             */
	    int si;             /* Calculated item index
				   index = SX+SA+ar_start_index+si
				*/
	    int expr_end_index;
	    double temp_value;
	    int er_status;
										  
	    ar_start_index = instr[IC];/* Array starting index in SL    */
#ifdef HF_DEBUG
	    if((ar_start_index<0) || (!(ar_start_index<SL)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
										  
	    INC(IC);
	    ar_items_amount = instr[IC];/* Array items amount           */
										  
#ifdef HF_DEBUG
	    if((ar_items_amount<1) || (!(ar_items_amount + ar_start_index-1 < SL)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
										  
	    INC(IC);
										  
	    expr_end_index = instr[IC];
#ifdef HF_DEBUG
	    if((expr_end_index<=IC) || (!(expr_end_index<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
										  
	    {
	      /* Calculate expression for index                           */
	      int last_index = expr_end_index;
	      er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
				       stack, stack_usage, &temp_value, svalue);
	    }
	    if(!er_status)
	      return 0; /* Error, error_info is filled                 */
	    si = HF_round(temp_value);
											  
	    if((1 > si) || (si > ar_items_amount))
	      {
		HF_Fill_Error(p_pn, Index_Out_Of_Range, error_info, IC);
		return 0; /* Error */
	      }
											  
	    IC = expr_end_index;
	    INC(IC);
											  
	    expr_end_index = instr[IC];
#ifdef HF_DEBUG
	    if((expr_end_index<=IC) || (!(expr_end_index<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
											  
	    /* Calculate expression                                     */
	    {
	      int last_index = expr_end_index;
	      er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
				       stack, stack_usage, &temp_value, svalue);
	    }
	    if(!er_status)
	      return 0; /* Error, error_info is filled                 */
												  
		stack[SX+SA+SS+ar_start_index+si-1]       = temp_value;  /* VAVAVA */
					stack_usage[SX+SA+SS+ar_start_index+si-1] = Initialized; /* VAVAVA */
														  
	    IC = expr_end_index;
	    INC(IC);
	  }
	  break;









	  /* VAVAVA New code: Beginning of processing 's[...] = ...;' */
			case CODE_Sarray_item_assignment:
				INC(IC);
				
				{ /* Declare Block */
				 /* First item of array is
										  SX+SA
				 */
					int ar_items_amount;/* Amount of items in array             */
					int si;             /* Calculated item index
										  index = SX+SA+si
										 */
					int expr_end_index;
					double temp_value;
					int er_status;
										  
					ar_items_amount = instr[IC]; /* Array items amount           */
										  
#ifdef HF_DEBUG
					if((ar_items_amount<1) || ((ar_items_amount > SS)))
					{
							HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
					return 0; /* Error */
					}
#endif
										  
					INC(IC);
										  
					expr_end_index = instr[IC];
#ifdef HF_DEBUG
					if((expr_end_index<=IC) || (!(expr_end_index<SI)))
					{
							HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
					return 0; /* Error */
					}
#endif
										  
					{
						/* Calculate expression for index                           */
					int last_index = expr_end_index;
							er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
												  stack, stack_usage, &temp_value, svalue);
					}
					if(!er_status)
					return 0; /* Error, error_info is filled                 */
						si = HF_round(temp_value);
											  
					if((1 > si) || (si > ar_items_amount))
					{
							HF_Fill_Error(p_pn, Index_Out_Of_Range, error_info, IC);
				    return 0; /* Error */
					}
											  
					IC = expr_end_index;
					INC(IC);
											  
					expr_end_index = instr[IC];
#ifdef HF_DEBUG
					if((expr_end_index<=IC) || (!(expr_end_index<SI)))
					{
							HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
				    return 0; /* Error */
					}
#endif
											  
					/* Calculate expression                                     */
					{
						int last_index = expr_end_index;
							er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
													  stack, stack_usage, &temp_value, svalue);
					}
					if(!er_status)
					return 0; /* Error, error_info is filled                 */
												  
					stack[SX+SA+si-1]       = temp_value;
					stack_usage[SX+SA+si-1] = Initialized;
												  
					IC = expr_end_index;
					INC(IC);
				}
				break;
/* VAVAVA End of processing 's[...] = ...;' */

	case CODE_goto:
	  INC(IC);
				
	  { /* Declare Block */
	    int oi;
	    oi = instr[IC];
#ifdef HF_DEBUG
	    if( (!(oi<SI)) || (oi<0))
	      /* To be sure that we are inside operations array bounds    */
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
	    IC = oi; /* Reposition instruction counter                  */
	  }
	  break;
	case CODE_if:
	case CODE_if_else:
	case CODE_while:
	  INC(IC);
	  { /* Declare block */
	    int oi;
	    int expr_end_index;
	    double temp_value;
	    int er_status;
					
	    oi = instr[IC];
#ifdef HF_DEBUG
	    if((IC>=oi) || (!(oi<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
					
	    INC(IC);
	    expr_end_index = instr[IC];
#ifdef HF_DEBUG
	    if((expr_end_index<=IC) || (!(expr_end_index<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
	    { /* Declare block */
	      int last_index = expr_end_index;
	      er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
				       stack, stack_usage, &temp_value, svalue);
	    }
	    if(!er_status)
	      return 0; /* Error, error_info is filled                 */
						
	    if(fabs(temp_value) >= EPS)
	      {
		IC = expr_end_index;
		INC(IC);
	      }
	    else
	      IC = oi;
	  }
	  break;
	case CODE_body_assignment:
	  /* value = */
	  INC(IC);
				
	  { /* Declare Block */
	    int expr_end_index;
	    int er_status;
					
	    expr_end_index = instr[IC];
					
#ifdef HF_DEBUG
	    if((expr_end_index<=IC) || (!(expr_end_index<SI)))
	      {
		HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
		return 0; /* Error */
	      }
#endif
					
	    { /* Declare block */
	      int last_index = expr_end_index;
	      er_status = HF_Calc_Expr(p_pn, error_info, IC, &last_index,
				       stack, stack_usage, value, svalue);
	    }
	    if(!er_status)
	      return 0; /* Error, error_info is filled */
			
	/* VAVAVA Assigning items of svalue array */

						for(i = 0; i<SS; i++)    
						      svalue[i] = stack[SX+SA+i]; 
   /* Normal return */
						//****Added 10/07/01
						Flag_Initialisation = 1;
						//*******************			
	    return 1; /* No errors */
	  }
#ifdef HF_DEBUG
	default:
	  HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
	  return 0;/*Error*/
#endif
	}
    }
  /* If the following code is reached, we should indicate about the error    */
  HF_Fill_Error(p_pn, Bad_Code, error_info, IC);
  return 0;/*Error*/
}
