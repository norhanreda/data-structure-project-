#pragma once
#ifndef __PriorityQueue_H_
#define __PriorityQueue_H_

#include <iostream>
#include "PriorityNode.h"

using namespace std;

template <typename T>
class PriorityQueue
{

	PriorityNode<T>* front;		//	Pointer to the first node in the queue
	int count;					//	Count number of nodes in queue

public:

	// Default constructor for PriorityQueue

	PriorityQueue()
	{
		front = nullptr;	//	Initializes the front pointer to nullptr to show an empty queue
		count = 0;			//	Queue is empty so count of nodes should equal 0
	}

	~PriorityQueue()
	{
		// Priority Queue destructor
		count = 0;
		T destructor;
		while (front)
			dequeue(destructor);
	}

	// Bool function to check for an empty queue

	bool isEmpty()
	{
		if (front)				// checks if the front doesn't point to null
			return false;		// if front is not null it returns true

		return true;			// return true if front is assigned to nullptr
	}

	// enqueue function to add new items to queue and sort them according to priority
	// returns true to show that the new node has been added

	bool enqueue(const T& newEntry, int priority)
	{
		// Construct new node containing the given entry and priority

		PriorityNode<T>* NewNodePtr = new PriorityNode<T>(newEntry, priority);

		// Special case handling if the queue is empty

		if (isEmpty())
		{
			front = NewNodePtr;		//	assign the new node as the first node in queue
			count++;				//	Increment number of nodes in queue
			return true;			//	node added to queue terminate with true
		}


		//	 Check if new node's priority is bigger than the front's priority

		if (front->getPriority() < priority)
		{
			NewNodePtr->setNext(front);		//	sets the front of the queue to the new node's next pointer
			front = NewNodePtr;				//	sets the new node as the new front
			count++;						//	Increment number of nodes in queue
			return true;					//	node added to queue terminate with true
		}
		else
		{
			//	two temporary traversing pointer to sort the queue according to priority

			PriorityNode<T>* temp = front;
			PriorityNode<T>* temp2 = front->getNext();

			//	while loop to keep running as long as the second node doesn't point to null
			//	aswell as the priority of the node is more than the new node's priority

			while (temp2 && temp2->getPriority() >= priority)
			{
				temp = temp->getNext();
				temp2 = temp2->getNext();
			}

			NewNodePtr->setNext(temp2);			//	Sets the new node pointer between temp 1 and temp 2
			temp->setNext(NewNodePtr);
			count++;							//	Increment number of nodes in queue
			return true;						//	node added to queue terminate with true
		}
	}

	// dequeue function returns the item in the front node
	// takes input as a pass by ref. 
	// returns output true if it was able to return a value and false if the queue is empty

	bool dequeue(T& frontEntry)
	{
		// Check if list is empty return false boolean as it cannot dequeue

		if (isEmpty())
			return false;

		PriorityNode<T>* tempDel = front;			// temporary pointer for delete action
		frontEntry = front->getItem();				// allocates the item in the front node to the passed argument
		front = front->getNext();					// moves the front of the queue to the second highest priority
		
		delete tempDel;								// delete action
		count--;									// decrement number of nodes in queue
		return true;								// Confirms dequeuing action and free-ing memory
	}

	// peek function that takes an argument passed by ref. and stores the item in the front node in it
	// returns false if the queue is empty else returns true

	bool peekFront(T& frontEntry)
	{
		// Check if list is empty return false boolean as it cannot peek

		if (isEmpty())
			return false;

		frontEntry = front->getItem();			// stores the front's item inside the passed argument
		return true;							// returns true to show that queue isn't empty and front's item is stored
	}
	
	// getter for count to know the number of nodes in queue

	int getCount()
	{
		return count;
	}

	T* toArray(int& counter)
	{
		counter = 0;

		if (!front)
			return nullptr;

		//counting the no. of items in the Queue

		PriorityNode<T>* p = front;
		while (p)
		{
			counter++;
			p = p->getNext();
		}


		T* Arr = new T[counter];
		p = front;
		for (int i = 0; i < counter; i++)
		{
			Arr[i] = p->getItem();
			p = p->getNext();
		}
		return Arr;
	}
}; // end queue
#endif
