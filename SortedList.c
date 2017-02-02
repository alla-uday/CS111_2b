#define _GNU_SOURCE
#include "SortedList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
int opt_yield=0;

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
  SortedList_t *curr=list, *check=NULL;
  element->next=NULL;
  element->prev=NULL;
  // if(curr!=NULL){
    if(curr->key==NULL)
      curr=curr->next;
    if(curr==NULL){
      list->next=element;
      element->prev=list;
    }
    else{
      while(curr!=NULL && strcmp(curr->key, element->key)<0){
	check=curr;
	curr=curr->next;
      }
      if(curr==NULL){
	check->next=element;
	/* yield */
	if(opt_yield & INSERT_YIELD)
	  pthread_yield();
	element->prev=check;
      }
      else if(check==NULL){
	element->next=list->next;
	list->next=element;
	/* yield */
	if(opt_yield & INSERT_YIELD)
	  pthread_yield();
	element->prev=list;
	curr->prev=element;
      }
      else{
	check->next=element;
	element->next=curr;
	/* yield */
	if(opt_yield & INSERT_YIELD)
	  pthread_yield();
	curr->prev=element;
	element->prev=check;
      }
    }
    //}
}

int SortedList_delete( SortedListElement_t *element)
{
  // if(element==NULL){
  //return -1;
  //}
  SortedListElement_t *previous=element->prev;
  SortedListElement_t *after = element->next;
  if(after==NULL && previous==NULL){
    return -1;
  }
  else if(previous==NULL){
    if(after->prev!=element){
      return -1;
    }
    else{
      after->prev=NULL;
      return 0;
    }
  }
  else if(after==NULL){
    if(previous->next!=element){
      return -1;
    }
    else{
      previous->next=NULL;
      return 0;
    }
  }
  else{
    if(previous->next!=element || after->prev!=element){
      return -1;
    }
  }
  previous->next=after;
  /* yield */
  if(opt_yield & DELETE_YIELD){
    pthread_yield();
    //    printf("del");
  }
  after->prev=previous;
  return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
  //SortedList_t *curr=list;  
  SortedList_t *curr=list->next;
  while(curr!=NULL){
    //if(curr->key!=NULL && curr->key==key){
    if(curr->key==key){
      /* yield */
      if(opt_yield & SEARCH_YIELD){
	pthread_yield();
	//	printf("search");
      }
      return curr;
    }
    //if(opt_yield==SEARCH_YIELD)
    //pthread_yield();
    curr=curr->next;
  }
  return NULL;
}

int SortedList_length(SortedList_t *list)
{
  SortedList_t *curr=list;
  if(curr==NULL){
     return -1;
  }
  if(curr!=NULL && curr->key!=NULL){
    return -1;
  }
  int count=0;
  curr=curr->next;
  while(curr!=NULL){
    SortedListElement_t *previous=curr->prev;
    //if(opt_yield==SEARCH_YIELD)
    //pthread_yield();
    SortedListElement_t *after = curr->next;
    if(after==NULL && previous==NULL){
      return -1;
    }
    else if(previous==NULL){
      if(after->prev!=curr){
	return -1;
      }
    }
    else if(after==NULL){
      if(previous->next!=curr){
	return -1;
      }
    }
    else{
      if(previous->next!=curr || after->prev!=curr){
	return -1;
      }
    }
    count++;
    /* yield */
    if(opt_yield & SEARCH_YIELD)
      pthread_yield();
    curr=curr->next;
  }
  
  return count;
}
