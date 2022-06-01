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
  -- Name:            SERIAL.c
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  contains procedures to store/load body's code.
  --
  -- Author:          Anatoliy P. Osipov
  --
  -- Last revised at: 
  -- 
  -- Modifications:   
  --                  
*/


#include <stdlib.h>
#include <string.h>
#include "general.h"



/* FORWARD DECLARATIONS: */
int HF_Get_Body_Size (PN_T* p_body);
void HF_Store_Body (PN_T* p_body,
		    unsigned char* p_buffer);
int  HF_Load_Body ( PN_T*           p_body,
                    unsigned char*  p_buffer);


int HF_Get_Buffer_Size(PN_List_T* p_Library)
{
  PN_NODE_T* current;
  PN_T*      body;
  int size = sizeof (int); /* integer for body counter */
  
  for(current = p_Library->first;current != NULL;current = current->next)
    {
      int cur_size = 0;
      body = current->pn_info;
      cur_size = HF_Get_Body_Size (body);
      size += cur_size;
    }
  return size;
}

int HF_Get_Body_Size (PN_T* p_body)
{
  int size = sizeof (int); /* for size of the object */
  /*
    1   char* body_name;
  */
  size += strlen (p_body->body_name) + 1;
  
  /*
    2   int X_dim;
  */
  size += sizeof (int);
  
  /*
    3   int A_dim;
  */
  size += sizeof (int);

  /*
    4   int stack_size;            ]
  */
  size += sizeof (int);

  /*
    5   Int_Array_T      code_list;
  */
  size += HF_Get_Size_Of_Int_Array (&p_body->code_list);
  
  /*
    6   Int_Array_T      index_to_lexem;
  */
  size += HF_Get_Size_Of_Int_Array (&p_body->index_to_lexem);

  /*
    7   Double_Array_T   constant_values;
  */
  size += HF_Get_Size_Of_Double_Array (&p_body->constant_values);

  /*
    8   String_Array_T   str_param;
  */
  size += HF_Get_Size_Of_String_Array (&p_body->str_param);
  
  /*
    9   PN_List_T        other_body_code_list;
  */
  size += HF_Get_Buffer_Size (&p_body->other_body_code_list);
  
  /*
    10   int				ad_size;
  */
  size += sizeof (int);
  
  return size;
}

int HF_Get_Size_Of_String_Array (String_Array_T* p_ar)
{
  int size = 0;
  
  /*
   *  int    amount;
   */
  size += sizeof (int);
  
  /*
   *  int    size;
   */
  size += sizeof (int);
  
  /*
   *   char*  data;
   */
  size += p_ar->size;
  
  return size;
}

int HF_Get_Size_Of_Double_Array (Double_Array_T* p_ar)
{
  int size = 0;
  
  /*
   *  int     amount;
   */
  size += sizeof (int);
  
  /*
   *  int     size;
   */
  size += sizeof (int);
  
  /*
   *  double* data;
   */
  size = size + p_ar->amount * sizeof (double);
  
  return size;
}

int HF_Get_Size_Of_Int_Array (Int_Array_T* p_ar)
{
  int size = 0;
  
  /*
   *  int     amount;
   */
  size += sizeof (int);
  
  /*
   *  int     size;
   */
  size += sizeof (int);
  
  /*
   *  int*    data;
   */
  size = size + p_ar->amount * sizeof (int);
  
  return size;
}

void HF_Store_Library ( PN_List_T*      p_Library,
                        unsigned char*  p_buffer)
{
  int size = sizeof(int);
  int counter = 0; /* for the amount of bodies */
  
  PN_NODE_T* current;
  PN_T*      body;
  
  for(current = p_Library->first;current != NULL;current = current->next)
    {
      int cur_size = 0;
      body = current->pn_info;
      cur_size = HF_Get_Body_Size (body);
      HF_Store_Body(body, p_buffer+size);
      size += cur_size;
      counter++;
    }
  *(int*)p_buffer = counter;
}

void HF_Store_Body (PN_T* p_body, unsigned char* p_buffer)
{
  int index = sizeof (int); /* for size of the object */
  /*
    1   char* body_name;
  */
  strncpy(p_buffer+index, p_body->body_name, strlen(p_body->body_name)+1);
  index += strlen(p_body->body_name) + 1;
  
  /*
    2   int X_dim;
  */
  *(int *)(p_buffer+index) = p_body->X_dim;
  index += sizeof (int);
  
  /*
    3   int A_dim;
  */
  *(int *)(p_buffer+index) = p_body->A_dim;
  index += sizeof (int);
  
  /*
    4   int stack_size;            ]
  */
  *(int *)(p_buffer+index) = p_body->stack_size;
  index += sizeof (int);
  
  /*
    5   Int_Array_T      code_list;
  */
  HF_Store_Int_Array (&p_body->code_list, p_buffer+index);
  index += HF_Get_Size_Of_Int_Array (&p_body->code_list);
  
  /*
    6   Int_Array_T      index_to_lexem;
  */
  HF_Store_Int_Array (&p_body->index_to_lexem, p_buffer+index);
  index += HF_Get_Size_Of_Int_Array (&p_body->index_to_lexem);
  
  /*
    7   Double_Array_T   constant_values;
  */
  HF_Store_Double_Array (&p_body->constant_values, p_buffer+index);
  index += HF_Get_Size_Of_Double_Array (&p_body->constant_values);
  
  /*
    8   String_Array_T   str_param;
  */
  HF_Store_String_Array (&p_body->str_param, p_buffer+index);
  index += HF_Get_Size_Of_String_Array (&p_body->str_param);
  
  /*
    9   PN_List_T        other_body_code_list;
  */
  HF_Store_Library(&p_body->other_body_code_list, p_buffer+index);
  index += HF_Get_Buffer_Size (&p_body->other_body_code_list);
  
  /*
    10   int				ad_size;
  */
  *(int *)(p_buffer+index) = p_body->ad_size;
  index += sizeof (int);
  
  *(int *)p_buffer = index;
}

int HF_Load_Library (   PN_List_T*      p_Library,
                        unsigned char*  p_Buffer)
     /* Returns zero if loading failed */
{
  int index, amount, current = 0, status = 1;
  /* Library must be empty !!! */
  if((p_Library->first != NULL) || (p_Library->last != NULL))
    return 0;
  /*********************************************************/
  index = sizeof (int);
  amount  = *(int *)p_Buffer;
  while((current < amount) && status)
    {  /* there is something more in the buffer */
      PN_T* p_body;
      if(HF_CreateNewPNS(&p_body))
	{
          if(HF_Load_Body (p_body, p_Buffer+index))
	    {
	      index += HF_Get_Body_Size(p_body);
	      if(!HF_Add_To_PN_List(p_Library, p_body))
		{
		  HF_DeletePNS(&p_body);
		  status = 0;
		}
	      current++;
	    }
          else
	    status = 0;
	}
      else
	{
          status = 0; /* Exit loop, empty library, return error*/
	}
    }
  
  if(!status)
    {
      HF_Empty_PN_List (p_Library);
      return 0;
    }
  else
    return 1;
  /*********************************************************/
}

int  HF_Load_Body ( PN_T*           p_body,
                    unsigned char*  p_buffer)
     /* Returns ZERO if unsuccessful */
{
  int status = 1;
  int index = sizeof (int); /* for size of the object */
  /*
    1   char* body_name;
  */
  
  p_body->body_name = malloc(strlen(p_buffer+index) + 1);
  if(p_body->body_name == NULL)
    return 0;
  strncpy(p_body->body_name, p_buffer+index, strlen(p_buffer+index)+1);
  index += strlen(p_body->body_name) + 1;
  
  /*
    2   int X_dim;
  */
  p_body->X_dim = *(int *)(p_buffer+index);
  index += sizeof (int);
  
  /*
    3   int A_dim;
  */
  p_body->A_dim = *(int *)(p_buffer+index);
  index += sizeof (int);
  
  /*
    4   int stack_size;
  */
  p_body->stack_size = *(int *)(p_buffer+index);
  index += sizeof (int);
  
  /*
    5   Int_Array_T      code_list;
  */
  status = HF_Load_Int_Array (&p_body->code_list, p_buffer+index);
  index += HF_Get_Size_Of_Int_Array (&p_body->code_list);
  
  /*
    6   Int_Array_T      index_to_lexem;
  */
  if(status)
    {
      status = HF_Load_Int_Array (&p_body->index_to_lexem, p_buffer+index);
      index += HF_Get_Size_Of_Int_Array (&p_body->index_to_lexem);
    }
  
  /*
    7   Double_Array_T   constant_values;
  */
  if(status)
    {
      status = HF_Load_Double_Array (&p_body->constant_values, p_buffer+index);
      index += HF_Get_Size_Of_Double_Array (&p_body->constant_values);
    }
  
  /*
    8   String_Array_T   str_param;
  */
  if(status)
    {
      status = HF_Load_String_Array (&p_body->str_param, p_buffer+index);
      index += HF_Get_Size_Of_String_Array (&p_body->str_param);
    }
  
  /*
    9   PN_List_T        other_body_code_list;
  */
  if(status)
    {
      status = HF_Load_Library(&p_body->other_body_code_list, p_buffer+index);
      index += HF_Get_Buffer_Size (&p_body->other_body_code_list);
    }
  /*
    10   int				ad_size;
  */
  p_body->ad_size = *(int *)(p_buffer+index);
  
  if(status)
    return 1;
  /*  else: */
  HF_DeletePNS(&p_body);
  return 0;
}

void HF_Store_Double_Array (Double_Array_T* p_ar, unsigned char* p_buf)
{
  int size = 0, i;
  
  /*
   *  int     amount;
   */
  *(int *)p_buf = p_ar->amount;
  size += sizeof (int);
  
  /*
   *  int     size;
   */
  *(int *)(p_buf + size) = p_ar->size;
  size += sizeof (int);
  
  /*
   *  double* data;
   */
  for(i = 0;i<p_ar->amount;i++)
    {
      *(double *)(p_buf + size) = p_ar->data[i];
      size += sizeof (double);
    }
}

void HF_Store_Int_Array (Int_Array_T* p_ar, unsigned char* p_buf)
{
  int size = 0, i;
  
  /*
   *  int     amount;
   */
  *(int *)p_buf = p_ar->amount;
  size += sizeof (int);
  
  /*
   *  int     size;
   */
  *(int *)(p_buf + size) = p_ar->size;
  size += sizeof (int);
  
  /*
   *  double* data;
   */
  for(i = 0;i<p_ar->amount;i++)
    {
      *(int *)(p_buf + size) = p_ar->data[i];
      size += sizeof (int);
    }
}

void HF_Store_String_Array (String_Array_T* p_ar, unsigned char* p_buf)
{
  int size = 0;
  
  /*
   *  int    amount;
   */
  *(int *)p_buf = p_ar->amount;
  size += sizeof (int);
  
  /*
   *  int    size;
   */
  *(int *)(p_buf + size) = p_ar->size;
  size += sizeof (int);
  
  /*
   *   char*  data;
   */
  strncpy(p_buf + size, p_ar->data, p_ar->size);
}

int  HF_Load_Double_Array (Double_Array_T* p_ar, unsigned char* p_buf)
{
  int size, i;
  
  p_ar->data = malloc(sizeof(double)*(*(int *)(p_buf + sizeof(int))));
  if(p_ar->data == NULL)
    return 0;
  
  /*
   *  int     amount;
   */
  p_ar->amount = *(int *)p_buf;
  
  /*
   *  int     size;
   */
  p_ar->size = *(int *)(p_buf + sizeof(int));
  
  /*
   *  double* data;
   */
  
  size = sizeof(int)*2;
  for(i = 0;i<p_ar->amount;i++)
    {
      p_ar->data[i] = *(double *)(p_buf + size);
      size += sizeof (double);
    }
  
  return 1;
}

int  HF_Load_Int_Array (Int_Array_T* p_ar, unsigned char* p_buf)
{
  int size, i;
  
  p_ar->data = malloc(sizeof(int)*(*(int *)(p_buf + sizeof(int))));
  if(p_ar->data == NULL)
    return 0;
  
  /*
   *  int     amount;
   */
  p_ar->amount = *(int *)p_buf;
  
  /*
   *  int     size;
   */
  p_ar->size = *(int *)(p_buf + sizeof(int));
  
  /*
   *  double* data;
   */
  
  size = sizeof(int)*2;
  for(i = 0;i<p_ar->amount;i++)
    {
      p_ar->data[i] = *(int *)(p_buf + size);
      size += sizeof (int);
    }
  
  return 1;
}

int HF_Load_String_Array (String_Array_T* p_ar, unsigned char* p_buf)
{
  p_ar->data = malloc(*(int *)(p_buf + sizeof(int)));
  if(p_ar->data == NULL)
    return 0;
  /*
   *  int    amount;
   */
  p_ar->amount = *(int *)p_buf;
  
  /*
   *  int    size;
   */
  p_ar->size = *(int *)(p_buf + sizeof(int));
  
  /*
   *   char*  data;
   */
  strncpy(p_ar->data, p_buf + sizeof(int)*2, sizeof(p_buf)+2*sizeof(int));
  
  return 1;
}
