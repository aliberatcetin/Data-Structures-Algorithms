"""
When data is growing over time, keeping the data sorted for getting median for every new element requires O(n).
In this heap implementation of running medians, adding new element requires O(logn), then getting the new median costs O(1).
"""

import sys
import math
import time


class MinHeap:
    def __init__(self):
        self.Heap=[]
    def size(self):
        return len(self.Heap)
    
    def get_top(self):
        return self.Heap[0] if len(self.Heap) > 0 else None

    def print_heap(self):
        print(self.Heap)
    
    def parent(self,position):
        return (position-1) // 2

    def swap(self,pos1,pos2):
        temp = self.Heap[pos1]
        self.Heap[pos1] = self.Heap[pos2]
        self.Heap[pos2] = temp
    
    def left_child(self,pos):
        return (pos*2) +1 

    def right_child(self,pos):
        return (pos*2)+2

    def find_pos(self,element):
        for i in range(len(self.Heap)):
            if self.Heap[i] == element:
                return i

    def delete_element(self,element):
        for i in range(len(self.Heap)):
            if self.Heap[i] == element:
                return self.delete_spesific_position(i)


    def inv_heapify(self,position):
        heap=self.Heap
        child=position
        while child > 0:
            parent = self.parent(child)
            if( heap[parent] <= heap[child]):
                return
                
            heap[parent], heap[child] = heap[child], heap[parent]
            child = parent


    def heapify(self,position):
        if(len(self.Heap) == 1):
            return
        #starts from root
        parent = position
        heap = self.Heap
        size = len(heap)
        while parent * 2 < size:
            child = parent * 2 +1
            if(child + 1) < size and heap[child] > heap[child+1]:
                child+=1
            
            if(child >= size):
                return 
            
            if(heap[parent] <= heap[child]):
                return
            heap[parent], heap[child] = heap[child], heap[parent]
            parent = child


    def insert(self,element):
        self.Heap.append(element)
        self.inv_heapify(len(self.Heap)-1)

    def delete_top(self):
        heap = self.Heap
        last_element = heap.pop()
        if not heap:
            return last_element
        item = heap[0]
        heap[0] = last_element
        self.heapify(0)
        return item

    
    def delete_spesific_position(self,pos):
        
        
        if(pos==0):
            return self.delete_top()
        if(pos>=len(self.Heap)-1):
            return self.Heap.pop()


        heap = self.Heap
        last_element = heap.pop()
        if not heap:
            return last_element
        item = heap[pos]
        heap[pos] = last_element
        self.heapify(pos)
        return item



class MaxHeap(MinHeap):
    def inv_heapify(self,position):
        heap=self.Heap
        child=position
        while child > 0:
            parent = self.parent(child)
            if( heap[parent] >= heap[child]):
                return
                
            heap[parent], heap[child] = heap[child], heap[parent]
            child = parent

    def heapify(self,position):
        if(len(self.Heap) == 1):
            return
        #starts from root
        parent = position
        heap = self.Heap
        size = len(heap)
        while parent * 2 < size:
            child = parent * 2 +1
            if(child + 1) < size and heap[child] < heap[child+1]:
                child+=1
            
            if(child >= size):
                return 
            
            if(heap[parent] >= heap[child]):
                return
            heap[parent], heap[child] = heap[child], heap[parent]
            parent = child

    def delete_top(self):
        heap = self.Heap
        last_element = heap.pop()
        if not heap:
            return last_element
        item = heap[0]
        heap[0] = last_element
        self.heapify(0)
        return item

    def delete_spesific_position(self,pos):
        if(pos==0):
            return self.delete_top()
        if(pos>=len(self.Heap)-1):
            return self.Heap.pop()

        heap = self.Heap
        last_element = heap.pop()
        if not heap:
            return last_element
        item = heap[pos]
        heap[pos] = last_element
        self.heapify(pos)
        return item

def rebalance(min_heap,max_heap):
    bigger = max_heap if max_heap.size() > min_heap.size() else min_heap
    smaller= min_heap if max_heap.size() > min_heap.size() else max_heap
    
    
    if(bigger.size() - smaller.size() >=2 ):
        smaller.insert(bigger.delete_top())
    

def process_running(median,delete,add,min_heap,max_heap):
    remove_element(median,delete,min_heap,max_heap)
    add_running(median,add,min_heap,max_heap)
    rebalance(min_heap,max_heap)

def add_running(median,new,min_heap,max_heap):
    if new < median:
        max_heap.insert(new)
    else:
        min_heap.insert(new)

def get_median(min_heap,max_heap):
    bigger = max_heap if max_heap.size() > min_heap.size() else min_heap
    smaller= min_heap if max_heap.size() > min_heap.size() else max_heap
    if bigger.size() == smaller.size():
        median = (bigger.get_top() + smaller.get_top()) /2
    else:
        median = bigger.get_top()

    return median

def total_size(min_heap,max_heap):
    return min_heap.size() + max_heap.size()

def add_element(element,min_heap,max_heap):

    if max_heap.size() == 0 or (min_heap.get_top() and element < min_heap.get_top()):
            max_heap.insert(element)
    else:
        min_heap.insert(element)
    rebalance(min_heap,max_heap)
    



def remove_element(current,element,min_heap,max_heap):
    delete_flag=0
    if element < current:
        delete_flag=1
        start_size = max_heap.size()
        max_heap.delete_element(element)
        end_size = max_heap.size()
        if start_size == end_size:
            print("d not")
            
    else:
        delete_flag=1
        start_size = min_heap.size()
        min_heap.delete_element(element)
        end_size = min_heap.size()
        if start_size == end_size:
            print("d not")
    rebalance(min_heap,max_heap)
    
    
    
    
def size_diff(min_heap,max_heap):
    return abs(min_heap.size() - max_heap.size())

def getMedianFromData(data,min_heap,max_heap):
    
    notifications=0
    count=0    
    for i in data:
        add_element(i,min_heap,max_heap)
    median=get_median(min_heap,max_heap)
    return median

data=[1,2,3,4,5,6,7,8,9,10,11]

min_heap = MinHeap()
max_heap = MaxHeap()
median=getMedianFromData(data,min_heap,max_heap)
print(median)

