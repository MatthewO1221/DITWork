#include "rangelist.h"
#include <stdlib.h> /* NULL/malloc/free */
#include <stdio.h>  /* printf */

#define xDEBUG

/* helper function, prototyped in the implementation file, so NOT available in the main */
void dump_node(struct RangeNode *p);
static struct RangeNode *makeNode(int position, int value);
/* your implementations: insert_value, get, delete */

/*
Function: insert_value
Returns:    int - Whether the function was successful or not
Inputs:     ppHead - A pointer to the first node in the list
            position - The position of the node to insert
            value - The value of the node to insert
Purpose: insert_value inserts a rangenode at the given position with
         the given value
*/
int insert_value(struct RangeNode **ppHead, int position, int value)
{
  /*If the list is not empty*/
  if (*ppHead)
  {
    /*If the value is not 0*/
    if (value != 0)
    {
      /*Create a new node*/
      struct RangeNode *newNode = makeNode(position, value);

      if (newNode)
      {
        /*Get access to the first node in the list*/
        struct RangeNode *buffer = *ppHead;
        /*Storage for previous node*/
        struct RangeNode *prev = NULL;

        /*If we're inserting a node before the first node*/
        if (position < buffer->begin)
        {
          /*Set the newNode's next to the current first node*/
          newNode->next = buffer;
          /*Make newNode the first node*/
          *ppHead = newNode;

          return 0;
        }

        
        /*Loop through the list until you reach the end 
        or pass the needed position*/
        while (buffer->next != NULL && buffer->begin < position)
        {
          /*Save the current node and move to the next one*/
          prev = buffer;
          buffer = buffer->next;
        }

        /*If the current node is past the one we need*/
        if (buffer->begin > position)
        {
          /*Insert the new node in between prev and buffer*/
          prev->next = newNode;
          newNode->next = buffer;
        }
        /*If the current node is the one we need*/
        else if (buffer->begin == position)
        {
          /*Change the current node's value*/
          buffer->value = value;
          free(newNode);
        }
        /*If the current node is before the one we need*/
        else if (buffer->begin < position)
        {
          /*Set new node to the end of the list*/
          buffer->next = newNode;
        }
      }
    }
    /*If value is 0*/
    else if (value == 0)
    {
      struct RangeNode *buffer = *ppHead, *prev = NULL;
      /*Loop through the list until we find the right node*/
      while (buffer->next != NULL && buffer->begin != position)
      {
        prev = buffer;
        buffer = buffer->next;
      }
      
      /*If it's the first node*/
      if (prev == NULL && buffer->begin == position)
      {
        /*Delete the first node and make sure the next one is
        the new head*/
        *ppHead = buffer->next;
        free(buffer);
      }
      /*If the current node is the right one*/
      else if (buffer->begin == position)
      {
        /*Skip the current node and free it*/
        prev->next = buffer->next;
        free(buffer);
      }
    }
  }
  /*If the list is empty*/
  else
  {

    struct RangeNode *newNode = makeNode(position, value);
    if (newNode)
    {
      /*Make a new node and make it the head of the list*/
      *ppHead = newNode;
      return 0;
    }
  }

  return 0;
}

/*
Function: delete
Returns:    none
Inputs:     p - The first node in the list
Purpose: delete frees all the nodes in the list
*/
void delete (struct RangeNode *p)
{
  if (p)
  {
    struct RangeNode *buffer = p;
    while (p != NULL)
    {
      /*Loop through the list, 
      freeing everything along the way*/
      buffer = p->next;
      free(p);
      p = buffer;
    }
  }
}

/*
Function: get
Returns:    int - The value of the node at the given position
Inputs:     p - The first node in the list
            position - The position of the node to find
Purpose: get finds the given node and returns its value
*/
int get(struct RangeNode const *p, int position)
{
  if (p)
  {
    /*Loop through the list until we find the end
    or the correct node*/
    while (p->next != NULL && p->begin != position)
    {
      p = p->next;
    }
    /*If we found the correct node return its value*/
    if (p->begin == position)
    {
      return p->value;
    }
  }
  /*If the node did not exist then its value is 0*/
  return 0;
}

/*
Function: makeNode
Returns:    RangeNode* - The created node
Inputs:     position - The position of the node to create
            value - The value of the node to create
Purpose: makeNode makes and returns a new node using the given parameters
*/
static struct RangeNode *makeNode(int position, int value)
{
  struct RangeNode *newNode = malloc(sizeof(struct RangeNode));
  if (newNode)
  {
    /*Make a new node and set all its variables*/
    newNode->begin = position;
    newNode->end = position + 1;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
  }

  return NULL;
}
/* implementations - DO NOT MODIFY*/
void dump_node(struct RangeNode *p)
{
  printf("+--------------------------\n");
  printf("| %p | %i [ %i, %i ) --> %p\n", (void *)p, p->value, p->begin, p->end, (void *)p->next);
  printf("+--------------------------\n");
}

void printf_elements(struct RangeNode const *p, char *format, int dimension, int do_sanity_check)
{
  int i, last_pos = 0;
  typedef int boolean;
  boolean zeroes = 0;
  boolean non_merged = 0;
  boolean overlapping_ranges = 0;
  /* indices */
  for (i = 0; i < dimension; ++i)
  {
    printf(format, i);
  }
  printf("\n");
  /* data */
  while (p)
  {
    for (i = last_pos; i < p->begin; ++i)
    {
      printf(format, 0);
    } /* print leading zeros */
    if (p->value == 0)
    {
      zeroes = 1;
    } /* sanity check */
    for (i = p->begin; i < p->end; ++i)
    {
      printf(format, p->value);
    } /* print value */
    last_pos = p->end;
    if (p->next && p->end > p->next->begin)
    {
      overlapping_ranges = 1;
    } /* sanity check */
    if (p->next && p->end == p->next->begin && p->value == p->next->value)
    {
      non_merged = 1;
    } /* sanity check */
    p = p->next;
  }
  for (i = last_pos; i < dimension; ++i)
  {
    printf(format, 0);
  }

  /* sanity reporting */
  if (do_sanity_check && zeroes)
  {
    printf("zero values should not be stored\n");
  }
  if (do_sanity_check && overlapping_ranges)
  {
    printf("overlapping ranges\n");
  }
  if (do_sanity_check && non_merged)
  {
    printf("ranges with the same value and next to each other - should be merged\n");
  }
}

void print_raw(struct RangeNode const *p)
{
  while (p)
  {
    printf("%i [ %i, %i ) --> ", p->value, p->begin, p->end);
    p = p->next;
  }
  printf("NULL");
}

int insert_nonoverlapping_range(struct RangeNode **ppHead, int begin, int end, int value)
{
  struct RangeNode *new_node = (struct RangeNode *)malloc(sizeof(struct RangeNode));
  if (new_node == NULL)
  {
    return 1;
  }

  new_node->begin = begin;
  new_node->end = end;
  new_node->value = value;

  /* main logic */
  if (!*ppHead || end < (*ppHead)->begin)
  { /* empty or in-front (short circuit) */
    new_node->next = *ppHead;
    *ppHead = new_node;
    return 0;
  }
  else
  { /* insert in the middle or tail */
    struct RangeNode *p = *ppHead;
    while (p->next && p->next->end <= begin)
    { /* find the right place */
      p = p->next;
    }

    if (!p->next)
    { /* insert in the end */
      new_node->next = NULL;
      p->next = new_node; /* make tail */
      return 0;
    }
    else
    { /* insert between p and p->next */
      new_node->next = p->next;
      p->next = new_node;
    }
  }
  return 0;
}
