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
-- Name:            ARRAY.C
--
-- Description:     This file in a part of HyperFun 1.0 interpreter. It
--                  implements internal array storages.
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
#include <stdio.h>
#include <assert.h>
#include "general.h"



void HF_Double_Array_Init(Double_Array_T* ar)
{
  /*
    -- Function description:
    --  Function resets the internal data of Double_Array_T
    --  into the initial state. No deallocation takes place.
*/
  ar->data = NULL;
  ar->amount = 0;
  ar->size   = 0;
}


int HF_Double_Array_Make(Double_Array_T* ar, int default_size)
     /* returns 0, if memory is unavailable */
{
  /*
    -- Function description:
    --  Function allocates memory for storage of "default_zise" amount of 
    items and
    --  initializes the "amount" and "size" fields of "ar".
    --
  */
  if(default_size <= 0)
    return 0;
  
  ar->amount = 0;
  ar->data = malloc(sizeof(double) * default_size);
  if(ar->data != NULL)
    {
      ar->size = default_size;
      return 1;
    }
  return 0;
}


void HF_Delete_Double_Array(Double_Array_T* ar)
{
  /*
    -- Function description:
    --  Function deallocates allocated memory for array and resets the internal
    --  data fields.
  */
  if (ar->data!=NULL) 
    free(ar->data);
  HF_Double_Array_Init(ar);
}


int HF_Add_To_Double_Array(Double_Array_T* ar, double item)
     /* returns 0, if memory is unavailable */
{
  /*
    -- Function description:
    --  Function adds new item into the array. Memory reallocation takes place 
    if
    --  needed.
  */
  double* temp;
  if(ar->amount >= ar->size)
    {
      ar->size *= 2;
      temp = realloc(ar->data, (sizeof(double))*ar->size);
      if(temp != NULL)
	ar->data = temp;
      else
	return 0;
    }
  *(ar->data+ar->amount) = item;
  ar->amount++;
  return 1;
}

int  HF_Copy_Double_Array(Double_Array_T* source, Double_Array_T* dest)
{
  int i;
  
#ifdef DEBUG    
  assert(dest!=NULL);
  assert(source!=NULL);
#endif /* DEBUG */  
  //  if (dest->data!=NULL) HFP_FREE(dest->data);
if (dest->data != NULL) free(dest->data);
  dest->data = malloc(sizeof(double)*(source->size));
  
#ifdef DEBUG  
  assert (dest->data!=NULL);
#endif /* DEBUG */
  dest->size = source->size;
  dest->amount = source->amount;
#ifdef DEBUG
  assert(source->amount <= source->size);
  assert (dest->amount <= dest->size);
#endif /* DEBUG */
  for(i=0;i<source->amount;i++)
    dest->data[i] = source->data[i];
  return 1;
}

/***************************************************************************/
void HF_Int_Array_Init(Int_Array_T* ar)
{
  /*
    -- Function description:
    --  Function resets the internal data of Int_Array_T
    --  into the initial state. No deallocation takes place.
  */
  ar->data = NULL;
  ar->amount = 0;
  ar->size   = 0;
}


int HF_Int_Array_Make(Int_Array_T* ar, int default_size)
     /* returns 0, if memory is unavailable */
{
  /*
    -- Function description:
    --  Function allocates memory for storage of "default_zise" amount of 
    items and
    --  initializes the "amount" and "size" fields of "ar".
  */
#ifdef DEBUG  
  assert (default_size > 0);
#endif /* DEBUG */  
  ar->amount = 0;
  ar->data = malloc(sizeof(int) * default_size);
  if(ar->data != NULL)
    {
      ar->size = default_size;
      return 1;
    }
  return 0;
}


void HF_Delete_Int_Array(Int_Array_T* ar)
{
  /*
    -- Function description:
    --  Function deallocates allocated memory for array and resets the internal
    --  data fields.
  */
  if (ar->data!=NULL)
    free(ar->data);
  HF_Int_Array_Init(ar);
}


int HF_Add_To_Int_Array(Int_Array_T* ar, int item)
     /* returns 0, if memory is unavailable */
{
  /*
    -- Function description:
    --  Function adds new item into the array. Memory reallocation takes place 
    if
    --  needed.
  */
  int* temp;
  if(ar->amount >= ar->size)
    {
      ar->size *= 2;
      temp = realloc(ar->data, ar->size*sizeof(int));
      if(temp != NULL)
	ar->data = temp;
      else
	return 0;
    }
  *(ar->data+ar->amount) = item;
  ar->amount++;
  return 1;
}


int  HF_Copy_Int_Array(Int_Array_T* source, Int_Array_T* dest)
{
  int i;

#ifdef DEBUG 
  assert (dest!=NULL);
  assert (source!=NULL);
#endif /* DEBUG */
  // if (dest->data!=NULL) HFP_FREE(dest->data);
if (dest->data != NULL)  free(dest->data);
  dest->data = malloc(sizeof(int)*(source->size));
  
#ifdef DEBUG  
  assert (dest->data!=NULL);
#endif /* DEBUG */  
  dest->size = source->size;
  dest->amount = source->amount;
#ifdef DEBUG
  assert (source->amount <= source->size);
  assert (dest->amount <= dest->size);
#endif /* DEBUG */
  for(i=0;i<source->amount;i++)
    dest->data[i] = source->data[i];
  return 1;
}


/***************************************************************************/
void HF_String_Array_Init(String_Array_T* ar)
{
  /*
    -- Function description:
    --  Function resets the internal data of String_Array_T
    --  into the initial state. No deallocation takes place.
  */
  ar->amount  = 0;
  ar->size    = 0;
  ar->data    = NULL;
}


void HF_Delete_String_Array(String_Array_T* ar)
{
  /*
    -- Function description:
    --  Function deallocates allocated memory for array and resets the internal
    --  data fields.
  */
  if (ar->data!=NULL)
    free(ar->data);
  HF_String_Array_Init(ar);
    
}


int HF_Add_To_String_Array(String_Array_T* ar, char* item)
{
  /*
    -- Function description:
    --  Function adds new string item into the array. Memory reallocation takes
    --  place if needed.
  */
  char* temp;
  int new_size = ar->size + strlen(item)+1;
  
  if(ar->amount == 0)
    {
      ar->data = malloc(new_size);
      if(ar->data != NULL)
	{
	  ar->amount++;
	  ar->size = new_size;
	  strncpy(ar->data, item, strlen(item)+1);
	  return 1;
	}
      return 0;
    }
  else
    {
      temp = realloc(ar->data, new_size);
      if(temp != NULL)
	{
	  ar->data = temp;
	  ar->amount++;
	  strncpy(ar->data+ar->size, item, strlen(item)+1);
	  ar->size = new_size;
	  return 1;
	}
      else
	return 0;
    }
}


char* HF_Get_By_Index(String_Array_T* ar, int index)
{
  /*
    -- Function description:
    --  Function retrieves the string from the array by the position number.
    --  NULL is returned if no data available.
  */
  int count=0, i=0;
  char* str = ar->data;
  while((str!=NULL) && (count <= ar->amount) && (i<ar->size))
    {
      if(count>=index)
	{
	  return (char*)(str+i);
	}
      else
	{
	  for(;(i<ar->size) && (str[i] != '\0');i++);
	  i++;
	  count++;
	}
    }
  return NULL;
}


int HF_Copy_String_Array(String_Array_T* source, String_Array_T* dest)
{
  
#ifdef DEBUG
  assert(dest != NULL);
  assert(source != NULL);
  //  assert(source->size!=0);
#endif /* DEBUG */  
  
  
  /*
    Modif. Pierre-Alain
    9/2001
    Let me explain ..
    In the case where you do not use HF_S
    e.g you do not use a string as argument in the call of 
    a function in HyperFun, the size of the source can be 0
    which means that for the source data = ""
  */
  if (source->size == 0)
    {
      source->size = 2;
      source->amount++;
      source->data = calloc(source->size, sizeof(char));
      strncpy(source->data, "t", source->size);
    }
  

  dest->data = calloc(source->size, sizeof(char));
  
  dest->size = source->size;
  
  dest->amount = source->amount;
  
#ifdef DEBUG  
  assert (dest->amount <= dest->size);
  assert (source->amount <= source->size);
#endif /* DEBUG */  
  //for(i=0;i<source->amount;i++)
  //  dest->data[i] = source->data[i];
  strncpy (dest->data, source->data, (source->size));
      
  return 1;
}

/***************************************************************************/
void HF_Name_Array_Init(Name_Array_T* ar)
{
  /*
    -- Function description: Function resets the internal data of Name_Array_T
    -- into the initial state. No deallocation takes place.
  */
  ar->amount  = 0;
  ar->size    = 0;
  
  ar->string_data    = NULL;
  ar->int_data       = NULL;
}


void HF_Delete_Name_Array(Name_Array_T* ar)
{
  /*
    -- Function description:
    --  Function deallocates allocated memory for array and resets the internal
    --  data fields.
  */
  if(ar != NULL)
    {
      if(ar->string_data != NULL)
	free(ar->string_data);
      
	
      if(ar->int_data != NULL)
	free(ar->int_data);
    }
  /* Shall I make a free of ar here ?? Let's try */
  /* 
     Yes I have to but not here ! Because ar ie NAMES (in at least one case) 
     has been  
     declared as a local variable thus allocated in the stack!
  */
  HF_Name_Array_Init(ar);
  
}


int  HF_Add_To_Name_Array(Name_Array_T* ar, char* item, int i, int c, int f)
{
  /*
    -- Function description:
    --  Function adds new string item and corresponding flags into the array . 
    Memory reallocation takes
    --  place if needed. The flags are: "i" is the index to the stack position
    --                                  "c" is amount of stack cells for named 
    entity
    --                                  "f" is the kind of entity:
    --                                     f = Variable or f = Array (See 
    "GENERAL.H" for more description).
  */
  char* temp;
  int*   int_temp;
  int new_size = ar->size + strlen(item)+1;
  
  if(ar->amount == 0)
    {
      ar->string_data = malloc(new_size);
      if(ar->string_data != NULL)
	{
	  ar->amount++;
	  ar->size = new_size;
	  strncpy(ar->string_data, item, strlen(item)+1);
	  
	  ar->int_data = malloc(3*sizeof(int));
	  if(ar->int_data == NULL)
	    {
	      if (ar->string_data != NULL)
		{
		  free(ar->string_data);
		  HF_Name_Array_Init(ar);
		}
	      return 0;
	    }
	  ar->int_data[0] = i;
	  ar->int_data[1] = c;
	  ar->int_data[2] = f;
	  return 1;
	}
      return 0;
    }
  else
    {
      temp = realloc(ar->string_data, new_size);
      if(temp != NULL)
	{
	  ar->string_data = temp;
	  ar->amount++;
	  strncpy(ar->string_data + ar->size, item, strlen(item)+1);
	  ar->size = new_size;
	  
	  int_temp = realloc(ar->int_data, sizeof(int) * ar->amount * 3);
	  if(int_temp != NULL)
	    {
	      ar->int_data = int_temp;
	      ar->int_data[(ar->amount-1)*3]     = i;
	      ar->int_data[(ar->amount-1)*3 + 1] = c;
	      ar->int_data[(ar->amount-1)*3 + 2] = f;
	      return 1;
	    }
	}
      return 0;
    }
  
}


int  HF_Find_In_Name_Array(Name_Array_T* ar, char* item, int* i, int* c, int* 
			   f)
{
  /*
    -- Function description:
    --  Function makes the search in the array by the "item" key. If
    --  this key is present in the array function returns 1 and
    --  three output flags i, c, f assosiated with the key entity.
    --  Otherwise, function returns 0.
  */
  
  int index=0;
  int ii;
  for(ii=0;(ii<ar->amount) && (strcmp(item, 
				      ar->string_data+index));ii++)
    {
      index = index + strlen(ar->string_data+index) + 1;
    }
  if(ii<ar->amount)
    {
      *i = ar->int_data[3*ii];
      *c = ar->int_data[3*ii+1];
      *f = ar->int_data[3*ii+2];
      return 1;
    }
  return 0;
}

