#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linkedList.h"

/*********** Static functions declaration *********/
static char *stringClone(char *in);
static struct node *createNode(char *value);

List *list_create(void){
  List *lst = (List*)malloc(sizeof(List));
  if(lst == NULL){
    fprintf(stderr, "EROR : Can not allocate list\n");
  }
  lst->head.prev= &lst->head;
  lst->head.next= &lst->head;
  return lst;
}


void list_destroy(List *lst){
  while (!list_is_empty(lst)){
    list_remove(lst);
  }
  free(lst);
}

bool list_is_empty(const List *lst){
  if (lst->head.prev == &lst->head && lst->head.next == &lst->head){
    return true;
  }
  return false;
}

void list_insert(List *lst, char *value){

  ListPos pos = {
    .node = lst->head.next
  };

  struct node *node = createNode(value);

  struct node *before = pos.node->prev;
  struct node *after = pos.node;

  node->next = after;
  after->prev = node;

  node->prev = before;
  before->next = node;
}


void list_remove(List *lst){
  ListPos pos = {
    .node = lst->head.next
  };
  struct node* before = pos.node->prev;
  struct node* after = pos.node->next;

  if (before != NULL){
      before->next = after;
  }
  if (after != NULL){
      after->prev = before;
  }
  free(pos.node->value);
  free(pos.node);
}

char *list_inspect_first(List* lst){
  return lst->head.next->value;
}


/********************** Static functions ******************/

static char *stringClone(char *in){
    size_t len = strlen(in);
    char *out = calloc(len + 1, sizeof(char));
    strcpy(out, in);
    free(in);
    return out;
}

static struct node *createNode(char *value){
  struct node *NewNode = (struct node*)malloc(sizeof(struct node));
  if (NewNode == NULL){
    fprintf(stderr, "EROR : can't allocate memory\n");
  }
  NewNode->value = stringClone(value);
  NewNode->prev = NULL;
  NewNode->next = NULL;
  return NewNode;
}