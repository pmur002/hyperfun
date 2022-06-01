/*==============================================================================

 Copyright 1998, 1999 Valery Adzhiev, Anatoly Ossipov
 Copyright 2000-2004 Valery Adzhiev
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
  -- Name:            SCANNER.C
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  implements the lexical scanner.
--
-- Author:          Anatoliy P. Osipov
--
-- Last revised at: 01/11/98
--
-- Functions: HF_get_token_internal, HF_get_token
*/


#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "general.h"

#ifdef WIN32
# define strcasecmp stricmp
#endif


extern function_info_T functions_info[];



int HF_get_token_internal(char*                p_str, /* program ASCIIZ string */
			  ScanParamStruct_T*   sps)
{
  /* Returns token code. See Scanner.h */
  /* Function description:
   --
   -- "HF_get_token_internal" is intended to retrieve the next lexical element
   -- from the "p_str" string. All fields of structure "sps" is filled according
   -- to this lexical element.
*/
  
/***************************************************************************/
  int  Value_To_Return = -1;
   
  int l_e_pos = sps->lexem_end_position;
  int l_e_line = sps->lexem_end_line;
   
  int* i = &(sps->str_position);
   
  int count;
  int found;
  double temp;
  int error;
   
  int is_numeric = 0, has_real = 0;
   
  /* Skip Delimeters */
  while((p_str[*i] == ' ') || (p_str[*i] == '\n') || (p_str[*i] == '\t'))
    {
      if(p_str[*i] == '\n')
	{
	  l_e_pos = 1;
	  l_e_line = l_e_line + 1;
	}
      else
	l_e_pos = l_e_pos + 1;
      *i = *i + 1;
    };
   
  sps->lexem_start_position = l_e_pos;
  sps->lexem_start_line = l_e_line;
   
  switch(p_str[*i])
    {
    case '\0':
      Value_To_Return = EndOfParseToken; break;
      /*-------------------------------------------------------------------------*/
    case '^':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = PowerToken; break;
      /*-------------------------------------------------------------------------*/
    case '+':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = PlusToken;  break;
      /*-------------------------------------------------------------------------*/
    case '-':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
       
      if(p_str[*i] == '-') /* Comment token */
	{
	  l_e_pos = l_e_pos+1;
	  *i = *i + 1;
	  for(/*1*/count = 0;
		   /*2*/(p_str[*i] != '\n') && (p_str[*i] != '\0')&&
		     (count<MAX_LENGTH_OF_STRING);
		   /*3*/count++)
	    {
	      sps->str_buffer[count] = p_str[*i];
	      l_e_pos = l_e_pos+1;
	      *i = *i + 1;
	    }
	  sps->str_buf_len = count;
	  if((p_str[*i] != '\n') && (p_str[*i] != '\0'))
	    {
	      sps->error_no = Very_Long_Lexem;
	      Value_To_Return = IllegalToken;
	    }
	  else
	    Value_To_Return = CommentToken;
	}
      else
	Value_To_Return = MinusToken;
      break;
      /*-------------------------------------------------------------------------*/
    case '~':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = TildaToken;  break;
      /*-------------------------------------------------------------------------*/
    case '/':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      if(p_str[*i] == '=')
	{
	  l_e_pos = l_e_pos+1;
	  *i = *i + 1;
	  Value_To_Return = NotEqualToken;
	}
      else
	{
	  Value_To_Return = SlashToken;
	}
      break;
      /*-------------------------------------------------------------------------*/
    case '*':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = MultToken; break;
      /*-------------------------------------------------------------------------*/
    case '&':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = AmpersandToken; break;
      /*-------------------------------------------------------------------------*/
    case '|':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = UnionToken; break;
      /*-------------------------------------------------------------------------*/
    case '\\':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = NegToken; break;
      /*-------------------------------------------------------------------------*/
    case '@':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = CartProductToken; break;
      /*-------------------------------------------------------------------------*/
    case ',':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = CommaToken; break;
      /*-------------------------------------------------------------------------*/
    case ';':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = SemicolonToken; break;
      /*-------------------------------------------------------------------------*/
    case '=':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = AssignToken; break;
      /*-------------------------------------------------------------------------*/
    case '(':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = LeftParenToken; break;
      /*-------------------------------------------------------------------------*/
    case ')':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = RightParenToken; break;
      /*-------------------------------------------------------------------------*/
    case '[':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = LeftBracketToken; break;
      /*-------------------------------------------------------------------------*/
    case ']':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = RightBracketToken; break;
      /*-------------------------------------------------------------------------*/
    case '{':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = LeftBraceToken; break;
      /*-------------------------------------------------------------------------*/
    case '}':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      Value_To_Return = RightBraceToken; break;
      /*-------------------------------------------------------------------------*/
    case '<':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      if(p_str[*i] == '=')
	{
	  l_e_pos = l_e_pos+1;
	  *i = *i + 1;
	  Value_To_Return = NotGreaterToken;
	}
      else
	{
	  Value_To_Return = LessToken;
	}
      break;
      /*-------------------------------------------------------------------------*/
    case '>':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      if(p_str[*i] == '=')
	{
	  l_e_pos = l_e_pos+1;
	  *i = *i + 1;
	  Value_To_Return = NotLessToken;
	}
      else
	{
	  Value_To_Return = GreaterToken;
	}
      break;
      /*-------------------------------------------------------------------------*/
    case '"':
      l_e_pos = l_e_pos+1;
      *i = *i + 1;
      for(/*1*/ count=0;
		/*2*/ (p_str[*i] != '"') &&
		  (p_str[*i] != '\0') &&
		  (p_str[*i] != '\n') &&
		  (count < MAX_LENGTH_OF_STRING);
		/*3*/ count++)
	{
	  if(p_str[*i] == '\n')
	    {
	      l_e_pos = 1;
	      l_e_line = l_e_line + 1;
	    }
	  else
	    {
	      l_e_pos = l_e_pos + 1;
	    }
	  sps->str_buffer[count] = p_str[*i];
	  *i = *i + 1;
	}
      sps->str_buf_len = count;
      sps->str_buffer[count] = '\0';
      if((p_str[*i] == '\0') ||
	 (p_str[*i] == '\n') ||
	 (count >= MAX_LENGTH_OF_STRING))
	{
	  sps->error_no = Second_QuatationMark_Absent;
	  Value_To_Return = IllegalToken;
	}
      else
	{
	  l_e_pos = l_e_pos + 1;
	  *i = *i + 1;
	  Value_To_Return = ConstantStringToken;
	}
      break;
    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '.':
      sps->real_value = 0.0;
      error = 0;
       
      if(p_str[*i] == '.')
	goto Label_Point;
      is_numeric = 1;
      /* Integer[.Integer][(E/e)[+/-]Integer] */
      sps->real_value = (p_str[*i] - '0');
       
      l_e_pos = l_e_pos + 1;
      *i = *i + 1;
       
      if((p_str[*i-1] == '0') && isdigit(p_str[*i])){error = 1;}
      if(!error)
	{
	  while(isdigit(p_str[*i]))
	    {
	      sps->real_value = sps->real_value*10.0 + (p_str[*i] - '0');
	      l_e_pos = l_e_pos + 1;
	      *i = *i + 1;
	    }
	   
	   
	Label_Point:if((p_str[*i] == '.')) /* After '.' may only be digit or E/e */
	  {
	    l_e_pos = l_e_pos + 1;
	    *i = *i + 1;
	     
	    if( (isdigit(p_str[*i])))
	      {
		has_real = 1;
		temp = 10.0;
		while(isdigit(p_str[*i]))
		  {
		    sps->real_value = sps->real_value + (p_str[*i]-'0')/temp;
		    temp = temp*10.0;
		    l_e_pos = l_e_pos + 1;
		    *i = *i + 1;
		  }
	      }
	    else
	      {
		if(!is_numeric)
		  error = 1;
	      }
	  }
	  if(!error)
	    {
	      if((p_str[*i] == 'E') || (p_str[*i] == 'e'))
		{
		  l_e_pos = l_e_pos + 1;
		  *i = *i + 1;
		   
		  temp = 1;
		  if(p_str[*i] == '-')
		    {
		      l_e_pos = l_e_pos + 1;
		      *i = *i + 1;
		      temp = -1;
		    }
		  else
		    {
		      if(p_str[*i] == '+')
			{
			  l_e_pos = l_e_pos + 1;
			  *i = *i + 1;
			}
		    }
		   
		  if(p_str[*i] == '0'){error = 1;}
		  if(!error)
		    {
		      if(!isdigit(p_str[*i])){error = 1;}
		      if(!error)
			{
			  temp = temp * (p_str[*i]-'0');
			  l_e_pos = l_e_pos + 1;
			  *i = *i + 1;
			  while(isdigit(p_str[*i]))
			    {
			      temp = temp*10.0 + (p_str[*i]-'0');
			      l_e_pos = l_e_pos + 1;
			      *i = *i + 1;
			    }
			   
			  sps->real_value = sps->real_value*exp(temp*log(10));/*exp(temp*log(10));*/
			}
		    }
		}
	    }
	}
      sps->error_no = Wrong_Number_Format;
      Value_To_Return = (error)?IllegalToken:RealValueToken;
      break;
    default:
      if(isalpha(p_str[*i]))
	{
	  for(/*1*/count = 0;
		   /*2*/(isdigit(p_str[*i]) || isalpha(p_str[*i]) || (p_str[*i]=='_'))&&
		     (count<MAX_LENGTH_OF_STRING);
		   /*3*/count++)
	    {
	      sps->str_buffer[count] = tolower(p_str[*i]);
	      l_e_pos = l_e_pos + 1;
	      *i = *i + 1;
	    }
	  sps->str_buf_len = count;
	  sps->str_buffer[count] = '\0';
	   
	  if(isdigit(p_str[*i]) || isalpha(p_str[*i]) || (p_str[*i]=='_'))
	    {
	      sps->error_no = Very_Long_Identificator;
	      Value_To_Return = IllegalToken;
	    }
	  else
	    {
	      if(!strcmp(sps->str_buffer, "a"))
		{  Value_To_Return = AIdToken;      }
		 else if(!strcmp(sps->str_buffer, "s"))   /* VAVAVA */
               {  Value_To_Return = SIdToken;      }
	      else if(!strcmp(sps->str_buffer, "x"))
		{  Value_To_Return = XIdToken;      }
	      else if(!strcmp(sps->str_buffer, "and"))
		{  Value_To_Return = AndToken;      }
	      else if(!strcmp(sps->str_buffer, "or"))
		{  Value_To_Return = OrToken;      }
	      else if(!strcmp(sps->str_buffer, "not"))
		{  Value_To_Return = NotToken;      }
	      else if(!strcmp(sps->str_buffer, "xor"))
		{  Value_To_Return = XorToken;      }
	      else if(!strcmp(sps->str_buffer, "array"))
		{  Value_To_Return = ArrayToken;    }
	      else if(!strcmp(sps->str_buffer, "if"))
		{  Value_To_Return = IfToken;      }
	      else if(!strcmp(sps->str_buffer, "then"))
		{  Value_To_Return = ThenToken;      }
	      else if(!strcmp(sps->str_buffer, "else"))
		{  Value_To_Return = ElseToken;      }
	      else if(!strcmp(sps->str_buffer, "endif"))
		{  Value_To_Return = EndIfToken;      }
	      else if(!strcmp(sps->str_buffer, "while"))
		{  Value_To_Return = WhileToken;      }
	      else if(!strcmp(sps->str_buffer, "loop"))
		{  Value_To_Return = LoopToken;      }
	      else if(!strcmp(sps->str_buffer, "endloop"))
		{  Value_To_Return = EndLoopToken;      }
	      else if(!strcmp(sps->str_buffer, "loop"))
		{  Value_To_Return = LoopToken;      }
	      else
		{
		  /*Here we should check for function */
		  found = 0;
		  for(count = 0; !found && (functions_info[count].f_name != NULL); count++)
		    {
		      found = !strcasecmp(functions_info[count].f_name, sps->str_buffer);
			}
		  if(found)
		    {
		      sps->function_index = count-1;
		      Value_To_Return = FunctionIdToken;
		    }
		  else
		    Value_To_Return = IdentifierToken;
		  
		}
	    }
	}
      else
	{
	  /* Changed */
	  l_e_pos = l_e_pos+1;
	  *i = *i + 1;
	  /* End of Changed section */
	   
	  sps->error_no = Unknown_Lexem;
	  Value_To_Return = IllegalToken;
	}
    }
   
  sps->lexem_end_position = l_e_pos;
  sps->lexem_end_line     = l_e_line;
   
  sps->lexem_code = Value_To_Return;
  return Value_To_Return;
}

/***************************************************************************/
int HF_get_token(char*                p_str, /* program ASCIIZ string */
		 ScanParamStruct_T*   sps)
{
  /* Function description:
     --
     -- "HF_get_token" is intended to skip comments from program
  */
  int r;
  
  do
    {
      r = HF_get_token_internal(p_str, sps);
    }while(r == CommentToken);
  
  return r;
}
