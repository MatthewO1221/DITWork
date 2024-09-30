/***
 * Filename: rangelist.cpp
 * Author: Matthew Oliver
 * DP email matthew.oliver@digipen.edu
 * Course: CS225
 * Section: A
 * Assignment: 3
 * Due date: 10/18/22
 *
 * Description:
 * Logic for a linked list with merging of nodes
***/
#include "rangelist.h"
#include <cstdlib>   /* new/delete */
#include <cstddef>   /* NULL */
#include <cstdio>    /* printf */
#include <algorithm> /* std::swap */
#include <iostream>

#define xDEBUG

//All functions have doxygen style comments in the header file, they should show up on hover


//RANGELIST FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

RangeList::RangeList()
    : pHead(NULL)
{
}

RangeList::RangeList(const RangeList &other)
    : pHead(NULL)
{

  // This code just inserts each node from other into this
  RangeNode *temp = other.pHead, *thisTemp;
  while (temp)
  {
    if (!pHead)
    {
      pHead = new RangeNode(temp->begin, temp->end, temp->value);
      thisTemp = pHead;
    }
    else
    {
      thisTemp->next = new RangeNode(temp->begin, temp->end, temp->value);
      thisTemp = thisTemp->next;
    }
    temp = temp->next;
  }
}

RangeList::~RangeList()
{
  Clear();
}

void RangeList::Clear()
{
  // Delete every node in list
  RangeNode *temp = pHead;

  while (pHead)
  {
    temp = pHead->next;
    delete(pHead);
    pHead = temp;
  }
}

int RangeList::operator*(RangeList &other)
{
  int result = 0;
  int length; // For use as for loop stop

  // Get the longer of the two lists
  if (this->ListLength() < other.ListLength())
  {
    length = this->ListLength();
  }
  else
  {
    length = other.ListLength();
  }

  // Iterate through the list and essentially do dot product
  for (int i = 0; i < length; i++)
  {
    result += this->get(i) * other.get(i);
  }

  return result;
}

int RangeList::ListLength()
{
  RangeNode *temp = pHead;

  // Iterate until last node
  while (temp->next)
  {
    temp = temp->next;
  }

  // Last node's end is the length of the list
  return temp->end;
}

RangeList RangeList::operator+(RangeList &other)
{
  // Make a copy of this
  RangeList newList = *this;

  

  int length = 0;
  
  //Set length to longer of two lists
  if (this->ListLength() < other.ListLength())
  {
    length = this->ListLength();
  }
  else
  {
    length = other.ListLength();
  }

  //Iterate through both lists and set all values as sum
  for (int i = 0; i < length; i++)
  {
    newList[i] = newList.get(i) + other.get(i);
  }

  return newList;
}

void RangeList::RangeNode::dump_node() const
{
  printf("+--------------------------\n");
  printf("| %p | %i [ %i, %i ) --> %p\n", reinterpret_cast<void const *>(this), value, begin, end, reinterpret_cast<void const *>(next));
  printf("+--------------------------\n");
}

int RangeList::insert_nonoverlapping_range(int begin, int end, int value)
{
  RangeNode **ppHead = &pHead;

  /* main logic */
  if (!*ppHead || end < (*ppHead)->begin)
  { /* empty or in-front (short circuit) */
    // front next to old head same value
    if (*ppHead && (*ppHead)->value == value && end == (*ppHead)->begin)
    {
      (*ppHead)->begin = begin;
    }
    else
    {
      RangeNode *old_head = *ppHead;
      *ppHead = new RangeNode(begin, end, value);
      (*ppHead)->next = old_head;
    }
    return 0;
  }
  else
  { /* insert in the middle or tail */
    RangeNode *p = *ppHead;
    while (p->next && p->next->end <= begin)
    { /* find the right place */
      p = p->next;
    }

    if (!p->next)
    { /* insert in the end */
      if (p->value == value && p->end == begin)
      {               // if inserting same value next to exising node
        p->end = end; // just change the end
      }
      else
      {
        p->next = new RangeNode(begin, end, value); /* make tail */
      }
      return 0;
    }
    else
    { /* insert between p and p->next */
      RangeNode *old_p_next = p->next;
      p->next = new RangeNode(begin, end, value);
      p->next->next = old_p_next;
    }
  }
  return 0;
}

void RangeList::printf_elements(char const *format, int dimension, bool do_sanity_check, bool indices) const
{
  RangeNode const *p = pHead;
  int i, last_pos = 0;
  bool zeroes = 0;
  bool non_merged = 0;
  bool overlapping_ranges = 0;
  /* indices */
  if (indices)
  {
    for (i = 0; i < dimension; ++i)
    {
      printf(format, i);
    }
    printf("\n");
  }
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

void RangeList::print_raw() const
{
  RangeNode const *p = pHead;
  while (p)
  {
    printf("%i [ %i, %i ) --> ", p->value, p->begin, p->end);
    p = p->next;
  }
  printf("NULL");
}

int RangeList::get(int position) const
{

  RangeNode *temp = pHead;

  while (temp)
  {
    // Iterate through the list and return the value of the node
    // containing position
    if (temp->WithinRange(position))
    {
      return temp->value;
    }
    temp = temp->next;
  }

  return 0;
}

int RangeList::insert_value(int position, int value)
{
  /*
  Not 0 Cases:
    -Insert at beginning Done
    -Insert in between 2 nodes
    -Merge with existing node
      -Can cause more merges
    -Split an existing node
    -Insert at end

  0 Cases:
    -Do nothing
    -Delete node
    -Split node
    -Delete at beginning or end of node



    Functions Needed:
    InsertNode
      Merger
      MakeSplitter

    DeleteNode
      DeleteSplitter
  */

  // If the list is not empty
  if (pHead)
  {
    // If we are inserting a value
    if (value != 0)
    {
      InsertNode(position, value);
    }
    // We are deleting a value
    else
    {
      DeleteNode(position);
    }
  }
  // List is empty
  else if (!pHead)
  {
    // Make a node and make it the head of the list
    RangeNode *newNode = new RangeNode(position, position + 1, value);
    pHead = newNode;
    return 0;
  }

  return 0;
}

int RangeList::InsertNode(int position, int value)
{
  RangeNode *temp = pHead, *prev = NULL;

  // Iterate through list
  while (temp)
  {

    // Inserting node at beginning
    if (position < temp->begin && !prev)
    {
      // Make a node and make it the head
      RangeNode *newNode = new RangeNode(position, position + 1, value);
      newNode->next = temp;
      pHead = newNode;

      // Call this after inserting in case of merges
      Merger();

      return 0;
    }
    // Inserting node at end
    else if (position >= temp->end && !temp->next)
    {
      // Make a new node and add it to the end
      RangeNode *newNode = new RangeNode(position, position + 1, value);
      temp->next = newNode;
      Merger();
      return 0;
    }
    // Inserting between two nodes
    else if (prev && position >= prev->end && position < temp->begin)
    {
      RangeNode *newNode = new RangeNode(position, position + 1, value);
      prev->next = newNode;
      newNode->next = temp;
      Merger();
      return 0;
    }
    // If position falls within existing node that has a range of more than 1
    else if (temp->WithinRange(position) && temp->value != value && temp->Length() != 1)
    {
      // Special insertion logic
      InsertSplit(position, value);
      Merger();
      return 0;
    }
    // If the position falls within existing node that has a range of 1
    else if (temp->WithinRange(position) && temp->value != value && temp->Length() == 1)
    {
      // Just change the value
      temp->value = value;
    }

    prev = temp;
    temp = temp->next;
  }
  return -1;
}

void RangeList::Merger()
{

  RangeNode *temp = pHead, *prev = NULL;

  // Until final node
  while (temp->next)
  {
    // If the value of the current node and the next node are the same
    if (temp->value == temp->next->value)
    {
      // If the nodes are next to each other
      if (temp->end == temp->next->begin)
      {
        // Make a new node that encompasses both ranges
        RangeNode *newNode = new RangeNode(temp->begin, temp->next->end, temp->value);

        // New node needs to skip the next node
        newNode->next = temp->next->next;

        if (prev)
        {
          // If not the first node then insert this into list
          prev->next = newNode;
        }
        else
        {
          // Make this the first node
          pHead = newNode;
        }

        delete temp->next;
        delete temp;
        temp = newNode;
      }
    }

    prev = temp;
    temp = temp->next;
  }
}

int RangeList::InsertSplit(int position, int value)
{

  // First step
  SplitNode(position);

  RangeNode *temp = pHead, *prev = NULL;

  while (temp)
  {
    // If this is not the first node and in between the correct nodes
    if (prev && prev->end <= position && temp->begin > position)
    {
      // Insert node in between the two correct nodes
      RangeNode *newNode = new RangeNode(position, position + 1, value);
      prev->next = newNode;
      newNode->next = temp;
      return 0;
    }
    // If this is the first node and is the correct position
    else if (!prev && temp->begin > position)
    {
      // Make a new head of the list
      RangeNode *newNode = new RangeNode(position, position + 1, value);
      pHead = newNode;
      newNode->next = temp;
      return 0;
    }

    prev = temp;
    temp = temp->next;
  }

  return -1;
}

int RangeList::DeleteNode(int position)
{
  RangeNode *temp = pHead, *prev = NULL;
  while (temp)
  {
    // If the node contains position
    if (temp->WithinRange(position))
    {
      // If it is the first node and is length 1
      if (!prev && temp->Length() == 1)
      {
        // Make the next node the first one
        pHead = temp->next;
        delete temp;
        return 0;
      }
      // If this is the last node and is length 1
      else if (temp->next == NULL && temp->Length() == 1)
      {
        // Make the previous node the last one
        prev->next = NULL;
        delete temp;
        return 0;
      }
      // If this is a middle node and is length 1
      else if (temp->Length() == 1)
      {
        prev->next = temp->next;
        delete temp;
        return 0;
      }
      else
      {
        // We can use split node to insert a 0 at the right position
        SplitNode(position);
      }
    }

    prev = temp;
    temp = temp->next;
  }

  return -1;
}

void RangeList::SplitNode(int position)
{
  RangeNode *temp = pHead, *prev = NULL;

  while (temp)
  {
    // If the current node contains position
    if (temp->WithinRange(position))
    {

      // Splitting at beginning of node
      if (temp->begin == position)
      {
        // Make a new node that starts at position + 1
        RangeNode *newNode = new RangeNode(position + 1, temp->end, temp->value);

        if (prev)
        {
          prev->next = newNode;
        }
        else
        {
          pHead = newNode;
        }

        newNode->next = temp->next;
        delete temp;
        return;
      }
      // Splitting at end of node
      else if (temp->end - 1 == position)
      {
        // Make a new node that ends at position
        RangeNode *newNode = new RangeNode(temp->begin, position, temp->value);

        if (prev)
        {
          prev->next = newNode;
        }
        else
        {
          pHead = newNode;
        }

        newNode->next = temp->next;
        delete temp;
        return;
      }
      // Splitting in the middle of a node
      else
      {
        // Make two nodes that surround the given position
        RangeNode *newNode1 = new RangeNode(temp->begin, position, temp->value);
        RangeNode *newNode2 = new RangeNode(position + 1, temp->end, temp->value);

        if (prev)
        {
          prev->next = newNode1;
        }
        else
        {
          pHead = newNode1;
        }

        // Pointer logic to put them in list
        newNode1->next = newNode2;
        newNode2->next = temp->next;
        delete temp;
        return;
      }
    }

    prev = temp;
    temp = temp->next;
  }
}

NodeProxy RangeList::operator[](unsigned int position)
{
  return NodeProxy(this, position);
}

NodeProxy RangeList::operator[](unsigned int position) const
{
  return NodeProxy(this, position);
}

RangeList &RangeList::operator=(RangeList other)
{
  // Just swap the heads
  std::swap(this->pHead, other.pHead);
  return *this;
}

//RANGENODE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

RangeList::RangeNode::RangeNode(int b, int e, int v)
    : begin(b), end(e), value(v), next(NULL)
{
}

int RangeList::RangeNode::Length()
{
  // The length of the node
  return this->end - this->begin;
}

bool RangeList::RangeNode::WithinRange(int position)
{
  // Whether the given position is within the node
  return position >= this->begin && position < this->end;
}

//NODEPROXY FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////

NodeProxy::NodeProxy(const RangeList *ref, int position)
    : listReference(NULL), constReference(ref), position(position)
{
}

NodeProxy::NodeProxy(RangeList *ref, int position)
    : listReference(ref), constReference(ref), position(position)
{
}

NodeProxy &NodeProxy::operator=(int value)
{
  //Just insert the given value
  listReference->insert_value(this->position, value);

  return *this;
}

NodeProxy::operator int() const
{
  //Return int version of proxy
  return listReference->get(this->position);
}

RangeList *NodeProxy::operator*=(int value)
{
  //Insert product of value and proxy
  listReference->insert_value(this->position, listReference->get(this->position) * value);
  return listReference;
}

RangeList *NodeProxy::operator+=(int value)
{
  //Insert sum of product and proxy
  listReference->insert_value(this->position, listReference->get(this->position) + value);
  return listReference;
}