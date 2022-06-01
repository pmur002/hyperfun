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
  -- Name:            LIST.C
  --
  -- Description:     This file in a part of HyperFun 1.0 interpreter. It
  --                  implements internal list storages.
--
-- Author:          Anatoliy P. Osipov
--
-- Last revised at: 09/08/2001 by Pierre Alain
*/

#include <stdlib.h>
#include <assert.h>
#include "general.h"


/********************************************************************/
void HF_Init_ER_List(ER_List_T* head)
{
  head->first = NULL;
  head->last  = NULL;
}


void HF_Empty_ER_List(ER_List_T* head)
{
  ER_NODE_T* temp;
  while(head->first)
    {
      temp = head->first->next;
      free(head->first);
      head->first = temp;
    }
  head->last = NULL;
}


int HF_Add_To_ER_List(ER_List_T* head, char* text, int line, int pos)
{
  ER_NODE_T* new_node = malloc(sizeof(ER_NODE_T));
  if(new_node == NULL)
    return 0;
  new_node->next = NULL;
  new_node->er_info.error_text = text;
  new_node->er_info.line       = line;
  new_node->er_info.position   = pos;
  
  
  if(head->last == NULL)
    {
      head->first = new_node;
      head->last  = new_node;
    }
  else
    {
      head->last->next = new_node;
      head->last = new_node;
    }
  return 1;
}
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
void HF_Init_PN_List(PN_List_T* head)
{
  head->first = NULL;
  head->last  = NULL;
}

void HF_Empty_PN_List(PN_List_T* head)
{
  PN_NODE_T* temp;
  while(head->first)
    {
      temp = head->first->next;
      HF_DeletePNS(&(head->first->pn_info));
      free(head->first);
      
	  head->first = temp;
    }
  head->last = NULL;
}

int HF_Add_To_PN_List(PN_List_T* head, PN_T* data)
{
  PN_NODE_T* new_node = malloc(sizeof(PN_NODE_T));
#ifdef DEBUG
  assert (new_node != NULL);
#endif /* DEBUG */
  new_node->next = NULL;
  new_node->pn_info = data;
  
  if(head->last == NULL)
    {
      head->first = new_node;
      head->last  = new_node;
    }
  else
    {
      head->last->next = new_node;
      head->last = new_node;
    }
  return 1;
}
