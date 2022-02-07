#pragma once

#ifndef __PriorityNode_H_
#define __PriorityNode_H_

template <typename T>
class PriorityNode
{
private:
	T item;					//	A data item
	PriorityNode<T>* next;	//	Pointer to next node
	int Priority;			//	Integer to assign priority for the node
public:

	// Default priority node constructor 

	PriorityNode()
	{
		next = nullptr;		// initialize next pointer with null
		Priority = 0;		// initialize priority to zero
	}

	// Non default constructor without next pointer argument

	PriorityNode(const T& r_Item, int prio)	//passing by const ref.
	{
		item = r_Item;		// assignes the item to the argument passed by reference
		Priority = prio;	// initializes the priority data member with an integer argument
		next = nullptr;		// there is no next pointer so it is initialized to nullptr
	}

	// Non default constructor with all arguments 

	PriorityNode(const T& r_Item, PriorityNode<T>* nextNodePtr,int prio)
	{
		item = r_Item;			// assignes the item to the argument passed by reference
		Priority = prio;		// initializes the priority data member with an integer argument
		next = nextNodePtr;		// in this constructor there is a next pointer argument so it is initialized
	}

	// setter for item data member passed by const. ref.

	void setItem(const T& r_Item)
	{
		item = r_Item;
	}

	// setter for the next pointer

	void setNext(PriorityNode<T>* nextNodePtr)
	{
		next = nextNodePtr;
	}

	// Setter for priority data member and validates that priority is a positive integer

	void setPriority(int prio)
	{
		if (prio >= 0)
			Priority = prio;
	}

	// Getter for the item carried by the node

	T getItem() const
	{
		return item;
	}

	// Getter for the next node

	PriorityNode<T>* getNext() const
	{
		return next;
	}

	// Getter for the node's priority

	int getPriority() const
	{
		return Priority;
	}

}; // end Priority Node
#endif