/*--- kianavangProgramAsg.h ----------------------------------------------------
  This header file contains the class template BST and Token. It includes the 
  separate header file for circular linked list class Queue.

  Written by:   Larry R. Nyhoff
  Written for:  Lab Manual for ADTs, Data Structures, and Problem
                Solving with C++, 2E

                Project #10.1 Version Two
                                                          
  Programmer:	Kiana Vang
  Date:			April 30, 2019
  Class:		CSCI 1107
  Assignment:	Final Programming Assignment
------------------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <new>
#include <cctype>
#include "kianavangProgramAsgLQueue.h"

using namespace std;

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

// Global Variables
const int ARRAY_SIZE = 26;

/***** Token class *****/
class Token
{
public:
    string word;
    int lineNum;

    // Token constructors
    // Default -- word part is empty string and lineNum is set to 0.
    Token()
        : word(" "), lineNum(0)
    {}

    // Explicit Value -- word is set to string s and lineNum is set to int n.
    Token(string s, int n)
        : word(s), lineNum(n)
    {}
};

/***** BST class *****/
class BST
{
 public:
  BST();
  /*----------------------------------------------------------------------------
    Construct a BST object.

    Precondition:  None.
    Postcondition: An empty BST has been constructed.
   ---------------------------------------------------------------------------*/

  bool empty() const;
  /*----------------------------------------------------------------------------
    Check if BST is empty.

    Precondition:  None.
    Postcondition: Returns true if BST is empty and false otherwise.
   ---------------------------------------------------------------------------*/

  void insert(const Token & item);
  /*----------------------------------------------------------------------------
    Insert item into BST.

    Precondition:  None.
    Postcondition: BST has been modified with item inserted at proper
                   position to maintain BST property.
  ----------------------------------------------------------------------------*/
  
  void inorder(ostream & out) const;
  /*----------------------------------------------------------------------------
    Inorder traversal of BST.

    Precondition:  ostream out is open.
    Postcondition: BST has been inorder traversed and values in nodes
                   have been output to out.
    Note: inorder uses private auxiliary function inorderAux().
 -----------------------------------------------------------------------------*/
  
  ~BST();
  /*----------------------------------------------------------------------------
    BST Destructor

    Precondition:  None.
    Postcondition: myRoot is passed to recursive function destroy() that does a
                   recursive postorder transversal of the subtree whose root is
                   passed to it. It deletes that root when it is the only node
                   in the subtree.
   ---------------------------------------------------------------------------*/

  BST(const BST & original);
  /*----------------------------------------------------------------------------
    The BST copy constructor

    Precondition:  An original BST object is passed by const reference.
    Postcondition: original's root and myRoot are passed to a recursive function
                   copyTree() that will do the traversal and the node copying,
                   passing the root of this copy back to myRoot.
   ---------------------------------------------------------------------------*/

  const BST & operator=(const BST & rhs);
  /*----------------------------------------------------------------------------
    The BST assignment operator

    Precondition:  the right-hand side BST object is passed as const reference.
    Postcondition: First checks for self assignment. Returns *this if it is the
                   same. If not, the value of the current object is destroyed to
                   prevent memory leak before copying rhs to the current object.

                   Finally, *this is returned.
   ---------------------------------------------------------------------------*/

 private:
  /***** Node class *****/
  class BinNode 
  {
   public:
    Queue<int> list;
    Token data;
    BinNode * left;
    BinNode * right;

    // BinNode constructors
    // Default -- data part is default T value; both links are null.
    BinNode()
    : left(NULL), right(NULL)
    {}

    // Explicit Value -- data part contains item; both links are null.
    BinNode(Token item)
    : data(item), left(NULL), right(NULL)
    {}

  };// end of class BinNode declaration

  typedef BinNode * BinNodePointer; 
  
  /***** Private Function Members *****/
  void inorderAux(ostream & out, 
                  BinNodePointer subtreePtr) const;
  /*----------------------------------------------------------------------------
    Inorder traversal auxiliary function.

    Precondition:  ostream out is open; subtreePtr points to a subtree of this
                   BST.
    Postcondition: Subtree with root pointed to by subtreePtr has been output to
                   out.
  ----------------------------------------------------------------------------*/

  void destroy(BinNodePointer subtreeRoot);
  /*----------------------------------------------------------------------------
    A recursive function that does a recursive postorder transversal of the
    subtree whose root is passed to it, deleting that root when it is the only
    node in the subtree.

    Precondition:  subtreeRoot is passed by value.
    Postcondition: If subtreeRoot is not equal to NULL, the left subtree is
                   recursively destroyed. The right subtree is done similarly.
                   Then the subtreeRoot is deleted.
  ----------------------------------------------------------------------------*/
  
  void copyTree(BinNodePointer origRoot, BinNodePointer& subtreeRoot);
  /*----------------------------------------------------------------------------
    A recursive function that does a recursive preorder transversal of the
    original subtree whose root is passed to it, copying each node, and passing
    the root of this copy back to myRoot.

    Precondition:  origRoot is passed by value. subtreeRoot is passed by
                   reference.
    Postcondition: If subtreeRoot is not NULL, the left subtree is recursively
                   destroyed. The right subtree is destroyed similarly. Then the
                   subtreeRoot is deleted.
  ----------------------------------------------------------------------------*/
 /***** Data Members *****/
  BinNodePointer myRoot; 
 
}; // end of class template declaration

/* * * * * * * * * * * * * All Class Member Definitions * * * * * * * * * * * */

//--- Definition of constructor
inline BST::BST()
: myRoot(NULL)
{}

//--- Definition of empty()
inline bool BST::empty() const
{ return myRoot == NULL; }

//--- Definition of insert()
inline void BST::insert(const Token & item)
{
   BST::BinNodePointer 
        locptr = myRoot,   // locptr is a search pointer
        parent = NULL;     // pointer to parent of current node
   bool found = false;     // indicates if item already in BST
  
   while (!found && locptr != NULL)
   {
      parent = locptr;
      if (item.word < locptr->data.word)       // descend left
         locptr = locptr->left;
      else if (locptr->data.word < item.word)  // descend right
         locptr = locptr->right;
      else                           // item found
         found = true;
   }
   if (!found)
   {                                 // construct node containing item
       locptr = new(nothrow) BST::BinNode(item);
       if (locptr == NULL)
       {
           cerr << "*** Out of memory -- terminating program ***\n";
           exit(1);
       }

       if (parent == NULL) { // empty tree
           myRoot = locptr; // insert at root
           myRoot->list.enqueue(item.lineNum); // push line number into queue
       }
       else if (item.word < parent->data.word) { // insert to left of parent
           parent->left = locptr;
           parent->left->list.enqueue(item.lineNum); // push line num into queue
       }
       else { // insert to right of parent
           parent->right = locptr;
           parent->right->list.enqueue(item.lineNum); // push line num into queue
       }
   }
   else
       // Word is already in BST. Push line num into the queue.
       locptr->list.enqueue(item.lineNum);
}

//--- Definition of inorder()
inline void BST::inorder(ostream & out) const
{ 
   inorderAux(out, myRoot); 
}

//--- Definition of inorderAux()
void BST::inorderAux(ostream & out, 
                               BinNodePointer subtreeRoot) const
{
   if (subtreeRoot != NULL)
   {
      inorderAux(out, subtreeRoot->left);    // L operation
      out << endl << " " << right << setw(12) << subtreeRoot->data.word << ": ";// V operation
      while (!subtreeRoot->list.empty()) {
          out << " " << subtreeRoot->list.front() << " ";
          subtreeRoot->list.dequeue();
      }
      inorderAux(out, subtreeRoot->right);   // R operation
   }
}

//--- Definition of destructor
inline BST::~BST()
{
    destroy(myRoot);
}

//--- Definition of copy constructor
inline BST::BST(const BST& original)
{
    copyTree(original.myRoot, myRoot);
}

//--- Definition of assignment operator
const BST& BST::operator=(const BST& rhs)
{
    // Check for self-assignment. If they're the same, return *this
    if (this == &rhs) {
        return *this;
    }

    // If rhs is not the same as current BST, do the following.
    if (this != &rhs) {
        this->~BST(); // Destory current BST
        copyTree(rhs.myRoot, myRoot); // Call copyTree to make a copy of rhs.
    }
    return *this;
}

//--- Definition of destroy
inline void BST::destroy(BinNodePointer subtreeRoot)
{
    if (subtreeRoot != NULL) {
        destroy(subtreeRoot->left);    // recursively destroy left subtree
        destroy(subtreeRoot->right);   // recursively destroy right subtree
        delete subtreeRoot;            // delete subtreeRoot
        // cerr << "\nNode destroyed.\n";
    }
}

//--- Definition copyTree()
inline void BST::copyTree(BinNodePointer origRoot,
    BinNodePointer& subtreeRoot)
{
    if (origRoot == NULL) {
        subtreeRoot = NULL;
    }
    else {
        subtreeRoot = new(nothrow) BST::BinNode(origRoot->data);
        copyTree(origRoot->left, subtreeRoot->left);
        copyTree(origRoot->right, subtreeRoot->right);
    }
}

#endif
