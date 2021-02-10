#include "my402list.h"
#include "stdlib.h"
#include "stdio.h"

 int  My402ListLength(My402List* my402List)
 {
     return my402List->num_members;
 }

 int  My402ListEmpty(My402List* my402List)
 {
     if((my402List->anchor).next==&(my402List->anchor) && (my402List->anchor).prev==&(my402List->anchor))
     {
         return TRUE;
     }
     
     return FALSE;
 }

 My402ListElem *My402ListLast(My402List* my402List)
 {
     if((my402List->anchor).next==&(my402List->anchor) && (my402List->anchor).prev==&(my402List->anchor))
     {
         return NULL;
     }

     return (my402List->anchor).prev;
 }

 int  My402ListAppend(My402List* my402List, void* obj)
 {
     My402ListElem* my402ListElemNew;
     my402ListElemNew=(My402ListElem*)malloc(sizeof(My402ListElem));
     
     if(my402ListElemNew==NULL)
     {
         fprintf(stderr,"Dynamic memory allocation through malloc failed.\n");
         exit(1);
     }

     my402ListElemNew->obj=obj;
     //List is empty
     if((my402List->anchor).next==&(my402List->anchor) && (my402List->anchor).prev==&(my402List->anchor))
     {
         my402ListElemNew->next=&(my402List->anchor);
         my402ListElemNew->prev=&(my402List->anchor);
         (my402List->anchor).next=my402ListElemNew;
         (my402List->anchor).prev=my402ListElemNew;
         my402List->num_members+=1;
     }
     else
     {
         My402ListElem* my402ListElemLast=My402ListLast(my402List);
         my402ListElemLast->next=my402ListElemNew;
         my402ListElemNew->prev=my402ListElemLast;
         my402ListElemNew->next=&(my402List->anchor);
         (my402List->anchor).prev=my402ListElemNew;
         my402List->num_members+=1;
     }
     return TRUE;
 }

 My402ListElem *My402ListFirst(My402List* my402List)
 {
     if((my402List->anchor).next==&(my402List->anchor) && (my402List->anchor).prev==&(my402List->anchor))
     {
         return NULL;
     }

     return (my402List->anchor).next;
 }

 int  My402ListPrepend(My402List* my402List, void* obj)
 {
     My402ListElem* my402ListElemNew;
     my402ListElemNew=(My402ListElem*)malloc(sizeof(My402ListElem));
     
     if(my402ListElemNew==NULL)
     {
         fprintf(stderr,"Dynamic memory allocation through malloc failed.\n");
         exit(1);
     }

     my402ListElemNew->obj=obj;
     //List is empty
     if((my402List->anchor).next==&(my402List->anchor) && (my402List->anchor).prev==&(my402List->anchor))
     {
         my402ListElemNew->next=&(my402List->anchor);
         my402ListElemNew->prev=&(my402List->anchor);
         (my402List->anchor).next=my402ListElemNew;
         (my402List->anchor).prev=my402ListElemNew;
         my402List->num_members+=1;
     }

     else
     {
         My402ListElem* my402ListElemFirst=My402ListFirst(my402List);
         my402ListElemFirst->prev=my402ListElemNew;
         my402ListElemNew->next=my402ListElemFirst;
         my402ListElemNew->prev=&(my402List->anchor);
         (my402List->anchor).next=my402ListElemNew;
         my402List->num_members+=1;
     }
     return TRUE;
 }

 void My402ListUnlink(My402List* my402List, My402ListElem* elem)
 {
     (elem->prev)->next=elem->next;
     (elem->next)->prev=elem->prev;
     free(elem);
     my402List->num_members-=1;
 }

 void My402ListUnlinkAll(My402List* my402List)
 {
     My402ListElem* my402ListElem=(my402List->anchor).next;

    (my402List->anchor).next=&(my402List->anchor);
     while(my402ListElem!=&(my402List->anchor))
     {
         My402ListElem* temp=my402ListElem;
         my402ListElem=my402ListElem->next;
         my402ListElem->prev=&(my402List->anchor);
         free(temp);
         my402List->num_members-=1;
     }
 }

 int  My402ListInsertAfter(My402List* my402List, void* obj, My402ListElem* elem)
 {
     if(elem==NULL)
     {
         return My402ListAppend(my402List,obj);
     }

     My402ListElem* my402ListElemNew;
     my402ListElemNew=(My402ListElem*)malloc(sizeof(My402ListElem));
     
     if(my402ListElemNew==NULL)
     {
         fprintf(stderr,"Dynamic memory allocation through malloc failed.\n");
         exit(1);
     }

     my402ListElemNew->obj=obj;

     my402ListElemNew->prev=elem;
     my402ListElemNew->next=elem->next;
     (elem->next)->prev=my402ListElemNew;
     elem->next=my402ListElemNew;

     my402List->num_members+=1;

     return TRUE;
 }

 int  My402ListInsertBefore(My402List* my402List, void* obj, My402ListElem* elem)
 {
     if(elem==NULL)
     {
         return My402ListPrepend(my402List,obj);
     }

     My402ListElem* my402ListElemNew;
     my402ListElemNew=(My402ListElem*)malloc(sizeof(My402ListElem));
     
     if(my402ListElemNew==NULL)
     {
         fprintf(stderr,"Dynamic memory allocation through malloc failed.\n");
         exit(1);
     }

     my402ListElemNew->obj=obj;

     my402ListElemNew->prev=elem->prev;
     my402ListElemNew->next=elem;
     (elem->prev)->next=my402ListElemNew;
     elem->prev=my402ListElemNew;

     my402List->num_members=my402List->num_members+1;

     return TRUE;
 }

 
 
 My402ListElem *My402ListNext(My402List* my402List, My402ListElem* elem)
 {
     if(elem==My402ListLast(my402List))
     {
         return NULL;
     }

     return elem->next;
 }

 My402ListElem *My402ListPrev(My402List* my402List, My402ListElem* elem)
 {
     if(elem==My402ListFirst(my402List))
     {
         return NULL;
     }

     return elem->prev;
 }

 My402ListElem *My402ListFind(My402List* my402List, void* obj)
 {
     My402ListElem* my402ListElem=(my402List->anchor).next;

     while(my402ListElem!=&(my402List->anchor))
     {
         if(my402ListElem->obj==obj)
         {
             return my402ListElem;
         }
         my402ListElem=my402ListElem->next;
     }

     return NULL;
 }

 int My402ListInit(My402List* my402List)
 {
     my402List->num_members=0;
     (my402List->anchor).next=&(my402List->anchor);
     (my402List->anchor).prev=&(my402List->anchor);
     (my402List->anchor).obj=NULL;

     return TRUE;
 }