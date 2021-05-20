/*--- kianavangProgramAsgLQueue.h ----------------------------------------------
    This header file defines a Queue data type.

    Basic operations:
    constructor:  Constructs an empty queue
    empty:        Checks if a queue is empty
    enqueue:      Modifies a queue by adding a value at the back
    front:        Accesses the top queue value; leaves queue unchanged
    dequeue:      Modifies queue by removing the value at the front
    display:      Displays all the queue elements
    Note: Execution terminates if memory isn't available for a queue element.

  Programmer:	Kiana Vang
  Date:			April 30, 2019
  Class:		CSCI 1107
  Assignment:	Final Programming Assignment
------------------------------------------------------------------------------*/

#include <iostream>
using namespace std;

#ifndef LQUEUE
#define LQUEUE

template <typename T>
class Queue
{
public:
    Queue();
    /*-----------------------------------------------------------------------
      Construct a Queue object.

      Precondition:  None.
      Postcondition: An empty Queue object has been constructed.
          (myFront and myBack are initialized to null pointers).
    -----------------------------------------------------------------------*/

    Queue(const Queue<T>& original);
    /*-----------------------------------------------------------------------
      Copy Constructor

      Precondition:  original is the queue to be copied and is received
          as a const reference parameter.
      Postcondition: A copy of original has been constructed.
    -----------------------------------------------------------------------*/

    /***** Destructor *****/
    ~Queue();
    /*-----------------------------------------------------------------------
      Class destructor

      Precondition:  None.
      Postcondition: The linked list in the queue has been deallocated.
    -----------------------------------------------------------------------*/

    /***** Assignment *****/
    const Queue<T>& operator= (const Queue<T>& rightHandSide);
    /*-----------------------------------------------------------------------
      Assignment Operator

      Precondition:  rightHandSide is the queue to be assigned and is
          received as a const reference parameter.
      Postcondition: The current queue becomes a copy of rightHandSide
          and a reference to it is returned.
    -----------------------------------------------------------------------*/

    bool empty() const;
    /*-----------------------------------------------------------------------
      Check if queue is empty.

      Precondition:  None.
      Postcondition: Returns true if queue is empty and false otherwise.
    -----------------------------------------------------------------------*/

    void enqueue(const T& value);
    /*-----------------------------------------------------------------------
      Add a value to a queue.

      Precondition:  value is to be added to this queue.
      Postcondition: value is added at back of queue.
    -----------------------------------------------------------------------*/

    void display(ostream& out) const;
    /*-----------------------------------------------------------------------
      Display values stored in the queue.

      Precondition:  ostream out is open.
      Postcondition: Queue's contents, from front to back, have been
          output to out.
    -----------------------------------------------------------------------*/

    T front() const;
    /*-----------------------------------------------------------------------
      Retrieve value at front of queue (if any).

      Precondition:  Queue is nonempty.
      Postcondition: Value at front of queue is returned, unless the queue
          is empty; in that case, an error message is displayed and a
          "garbage value" is returned.
    -----------------------------------------------------------------------*/

    void dequeue();
    /*-----------------------------------------------------------------------
      Remove value at front of queue (if any).

      Precondition:  Queue is nonempty.
      Postcondition: Value at front of queue has been removed, unless
          queue is empty; in that case, an error message is displayed
          and execution allowed to proceed.
    -----------------------------------------------------------------------*/

private:
    /*** Node class ***/
    class Node
    {
    public:
        T data;
        Node* next;
        //--- Node constructor
        Node(T value, Node* link = NULL)
            /*-------------------------------------------------------------------
              Precondition:  value and link are received
              Postcondition: A Node has been constructed with value in its
                  data part and its next part set to link (default 0).
             ------------------------------------------------------------------*/
        {
            data = value; next = link;
        }
    };

    typedef Node* NodePointer;

    /***** Data Members *****/
    NodePointer myBack;      // pointer to last node of circular linked list

}; // end of class declaration

// Overloaded operator<<
template <typename T>
ostream& operator<<(ostream& out, const Queue<T>& q)
{
    q.display(out);
    return out;
}

//--- Definition of Queue constructor
template <typename T>
Queue<T>::Queue()
/*------------------------------------------------------------------------------
    Construct a Queue object.

    Precondition:  None.
    Postcondition: An empty Queue object has been constructed.
        (myFront and myBack are initialized to null pointers).
  ----------------------------------------------------------------------------*/
    : myBack(NULL)
{}

//--- Definition of Queue copy constructor
template <typename T>
Queue<T>::Queue(const Queue<T>& original)
/*------------------------------------------------------------------------------
   Copy Constructor

   Precondition:  original is the queue to be copied and is received
       as a const reference parameter.
   Postcondition: A copy of original has been constructed.
 -----------------------------------------------------------------------------*/
{
    // Queue::NodePointer myFront = myBack->next;
    myBack = NULL; // empty list if NUll

    if (!original.empty())
    {
        // Copy first node
        myBack = new Queue::Node(original.myBack->next->data);
        myBack->next = myBack; 

        // Set pointer to run through original's linked list
        Queue::NodePointer origPtr = original.myBack->next->next;

        while (origPtr != original.myBack->next) // != myBack->next to avoid
                                                 // duplication
        {
            Queue::NodePointer front = myBack->next;
            myBack->next = new Queue::Node(origPtr->data);
            myBack = myBack->next;
            myBack->next = front; // set this back to front
            origPtr = origPtr->next;
        }
    }
}

//--- Definition of Queue destructor
template <typename T>
Queue<T>::~Queue()
/*------------------------------------------------------------------------------
    Class destructor

    Precondition:  None.
    Postcondition: The linked list in the queue has been deallocated.
  ----------------------------------------------------------------------------*/
{
    if (!empty()) {
        // Set pointer to run through the queue
        Queue::NodePointer prev = myBack->next, ptr;

        myBack->next = NULL; // Need this in order to break out of loop later

        while (prev != NULL)
        {
            ptr = prev->next; //ptr is assigned prev->next, which is myBack->next
            delete prev;
            prev = ptr;
        }
    }
}
//--- Definition of assignment operator
template <typename T>
const Queue<T>& Queue<T>::operator=(const Queue<T>& rightHandSide)
/*------------------------------------------------------------------------------
   Assignment Operator

   Precondition:  rightHandSide is the queue to be assigned and is
       received as a const reference parameter.
   Postcondition: The current queue becomes a copy of rightHandSide
       and a reference to it is returned.
 -----------------------------------------------------------------------------*/
{
    if (this != &rightHandSide)         // check that not q = q
    {
        this->~Queue();                  // destroy current linked list
        if (rightHandSide.empty())       // empty queue
            myBack = NULL;
        else
        {                                // copy rightHandSide's list
           // Copy first node
            myBack = new Queue::Node(rightHandSide.myBack->next->data);
            myBack->next = myBack;

            // Set pointer to run through rightHandSide's linked list
            Queue::NodePointer rhsPtr = rightHandSide.myBack->next->next;

            while (rhsPtr != rightHandSide.myBack->next)
            {
                Queue::NodePointer front = myBack->next;
                myBack->next = new Queue::Node(rhsPtr->data);
                myBack = myBack->next;
                myBack->next = front;
                rhsPtr = rhsPtr->next; // increment to copy next node
            }
        }
    }
    return *this; // return this object
}

//--- Definition of empty()
template <typename T>
bool Queue<T>::empty() const
/*------------------------------------------------------------------------------
   Check if queue is empty.

   Precondition:  None.
   Postcondition: Returns true if queue is empty and false otherwise.
 -----------------------------------------------------------------------------*/
{
    return (myBack == NULL);
}

//--- Definition of enqueue()
template <typename T>
void Queue<T>::enqueue(const T & value)
/*------------------------------------------------------------------------------
    Add a value to a queue.

    Precondition:  value is to be added to this queue.
    Postcondition: value is added at back of queue.
  ----------------------------------------------------------------------------*/
{
    Queue::NodePointer newptr = new Queue::Node(value);
    if (empty()) {
        myBack = newptr;
        newptr->next = newptr;
    }
    else
    {
        Queue::NodePointer temp = myBack->next;
        myBack->next = newptr;
        myBack = newptr;
        newptr->next = temp;
    }
}

//--- Definition of display()
template <typename T>
void Queue<T>::display(ostream & out) const
/*------------------------------------------------------------------------------
   Display values stored in the queue.

   Precondition:  ostream out is open.
   Postcondition: Queue's contents, from front to back, have been
       output to out.
 -----------------------------------------------------------------------------*/
{
    if (empty())
        return;

    Queue::NodePointer ptr;

    //myBack->next points to first node in list
    Queue::NodePointer front = myBack->next; //save the front of the list
    ptr = front;

    do {

        out << ptr->data << " ";
        ptr = ptr->next;

    } while (ptr != front);
}

//--- Definition of front()
template <typename T>
T Queue<T>::front() const
/*------------------------------------------------------------------------------
    Retrieve value at front of queue (if any).

    Precondition:  Queue is nonempty.
    Postcondition: Value at front of queue is returned, unless the queue
        is empty; in that case, an error message is displayed and a
        "garbage value" is returned.
  ----------------------------------------------------------------------------*/
{
    if (!empty())
        return (myBack->next->data);
    else
    {
        cerr << "*** Queue is empty  -- returning garbage ***";
        T* temp = new(T);
        T garbage = *temp;     // "Garbage" value
        delete temp;
        return garbage;
    }
}

//--- Definition of dequeue()
template <typename T>
void Queue<T>::dequeue()
/*------------------------------------------------------------------------------
    Remove value at front of queue (if any).

    Precondition:  Queue is nonempty.
    Postcondition: Value at front of queue has been removed, unless
        queue is empty; in that case, an error message is displayed
        and execution allowed to proceed.
  ----------------------------------------------------------------------------*/
{
    if (!empty())
    {
        if (myBack == myBack->next) {
            delete myBack;
            myBack = NULL;
        }
        else {
            Queue::NodePointer ptr = myBack->next; // ptr pointing to 2
            myBack->next = myBack->next->next; // now 6 points to 4
            delete ptr; // delete 2
        }

        if (myBack == NULL)     // queue is now empty
            myBack = NULL;
    }
    else
        cerr << "*** Queue is empty -- can't remove a value ***";
}
#endif
