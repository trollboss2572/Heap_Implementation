/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"
#include <math.h>
#include <stdlib.h>

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/
int priorityAt(MinHeap* heap, int nodeIndex)
{
  HeapNode *array = heap->arr;
  return array[nodeIndex].priority;
}

bool isValidIndex(MinHeap* heap, int maybeIdx)
{
  if (heap->size < maybeIdx || maybeIdx < 1)
  {
    return false;
  }
  return true;
}

int parentIdx(MinHeap* heap, int nodeIndex)
{
  if (nodeIndex == 1)
  {
    printf("Root of heap has no parent.\n");
    return NOTHING;
  }
  return floor(nodeIndex / 2);
}

int leftIdx(MinHeap* heap, int nodeIndex)
{
  int left_child_i = nodeIndex * 2;
  if (left_child_i > heap->size)
  {
    return NOTHING;
  }
  return left_child_i;
}

int rightIdx(MinHeap* heap, int nodeIndex)
{
  int right_ind = leftIdx(heap, nodeIndex) + 1;
  if (right_ind == 0)
  {
    return NOTHING;
  }
  if (right_ind > heap->size)
  {
    printf("Left child of node with index %d does not exist", nodeIndex);
    return NOTHING;
  }
  return right_ind;
}

void swap(MinHeap* heap, int index1, int index2)
{
  if (isValidIndex(heap, index1) && isValidIndex(heap, index2))
  {
    HeapNode *array = heap->arr;
    HeapNode i1 =  array[index1];
    HeapNode i2 = array[index2];
    heap->indexMap[i1.id] = index2;
    heap->indexMap[i2.id] = index1; 
    heap->arr[index1] = i2;
    heap->arr[index2] = i1;
    return;
  }
  printf("One of your indices is out of range. No change to the array\n");
}
/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex)
{
  if (nodeIndex == ROOT_INDEX)
  {
    return;
  }
  HeapNode *array = heap->arr;
  int up_ind = parentIdx(heap, nodeIndex);
  if (array[up_ind].priority > array[nodeIndex].priority)
  {
    swap(heap, up_ind, nodeIndex);
    bubbleUp(heap, up_ind);
  }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap* heap)
{
  int root_node = ROOT_INDEX;
  int root_left = leftIdx(heap, root_node);
  int root_right = rightIdx(heap, root_node);
  int root_priority = priorityAt(heap, root_node);
  int left_priority = priorityAt(heap, root_left);
  int right_priority = priorityAt(heap, root_right);
  while (root_priority > left_priority || root_priority > right_priority)
  {
    if(root_left == -1 || root_right == -1)
    {
      break;
    }
    if (right_priority > left_priority)
    {
      swap(heap, root_node, root_left);
      root_node = root_left;
      root_left = leftIdx(heap, root_node);
      root_right = rightIdx(heap, root_node);
      root_priority = priorityAt(heap, root_node);
      left_priority = priorityAt(heap, root_left);
      right_priority = priorityAt(heap, root_right);
    }
    else if (left_priority >= right_priority)
    {
      swap(heap, root_node, root_right);
      root_node = root_right;
      root_left = leftIdx(heap, root_node);
      root_right = rightIdx(heap, root_node);
      root_priority = priorityAt(heap, root_node);
      left_priority = priorityAt(heap, root_left);
      right_priority = priorityAt(heap, root_right);
    }
  }
}

/* Doubles the capacity of minheap 'heap'.
 */
void doubleCapacity(MinHeap* heap)
{
  int new_capacity = heap->capacity * 2;
  heap->arr = realloc(heap->arr, sizeof(HeapNode) * (new_capacity + 1));
  heap->indexMap = realloc(heap->indexMap, sizeof(int) * (new_capacity  + 1));
  heap->capacity = new_capacity;
}

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap *heap)
{
  HeapNode *array = heap->arr;
  return (array[ROOT_INDEX]);
}

HeapNode extractMin(MinHeap* heap)
{
  HeapNode min_node = getMin(heap);
  heap->indexMap[min_node.id] = NOTHING;
  heap->arr[ROOT_INDEX] = heap->arr[heap->size];
  bubbleDown(heap);
  heap->size = heap->size - 1;
  return min_node;
}

void insert(MinHeap* heap, int priority, void* value)
{
  HeapNode new_node;
  new_node.priority = priority;
  new_node.value = value;
  if (heap->size == heap->capacity)
  {
    doubleCapacity(heap);
  }
  heap->size = heap->size + 1;
  new_node.id = heap->size;
  heap->arr[heap->size] = new_node;
  heap->indexMap[heap->size] =  heap->size;
  bubbleUp(heap, heap->size); 
}

bool decreasePriority(MinHeap* heap, int id, int newPriority)
{
  int node_ind = heap->indexMap[id];
  if (heap->arr[node_ind].priority > newPriority)
  {
    heap->arr[node_ind].priority = newPriority;
    bubbleUp(heap, node_ind);
    return true;
  }
  return false;
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printHeap(MinHeap* heap) {
  printf("MinHip with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = ROOT_INDEX; i <= heap->size; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, heap->arr[i].priority, heap->arr[i].id,
           i, heap->indexMap[i]);
  for (int i = heap->size + 1; i <= heap->capacity; i++)
    printf("\t\t\t%d: %d\n", i, heap->indexMap[i]);
  printf("\n\n");
}

/*********************************************************************
 ** Helper functions not provided in the starter code
 *********************************************************************/

MinHeap* newHeap(int capacity)
{
  MinHeap *heap = malloc(sizeof(MinHeap));
  heap->size = 0;
  heap->capacity = capacity;
  HeapNode *node_arr = malloc(sizeof(HeapNode) * (capacity+1));
  for (int i = 1; i <= capacity; i++)
  {
    node_arr[i].priority = 0;
    node_arr[i].id = 0;
  }
  heap->arr = node_arr;
  int *Index_map_ins = malloc(sizeof(int) * (capacity + 1));
  for (int i = 0; i <= capacity; i++)
  {
    Index_map_ins[i] = NOTHING;
  }
  heap->indexMap = Index_map_ins;
  return heap;
}

void deleteHeap(MinHeap *heap)
{
  free(heap);
}