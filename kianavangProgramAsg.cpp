/*--- kianavangProgramAsg.cpp --------------------------------------------------
  A program to construct a concordance for a document stored in a file. The
  words are stored in an array of 26 binary search trees. Part of each node of
  the BST will have a circular linked list Queue.

  Written by:   Larry R. Nyhoff
  Written for:  Lab Manual for ADTs, Data Structures, and Problem
                Solving with C++, 2E

                Project #10.1 Version 2

  Programmer:	Kiana Vang
  Date:			April 30, 2019
  Class:		CSCI 1107
  Assignment:	Final Programming Assignment
 -----------------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <new>
#include <cctype>
#include "kianavangProgramAsg.h"

using namespace std;

void insertToBST(string allCaps, int lineNum, BST concord[])
/*------------------------------------------------------------------------------
  Insert all caps word and line number into BST

  Precondition:  allCaps, lineNum, and concord[] are passed in by value
  Postcondition: allCaps and lineNum are inserted in the proper position of BST 
                 via BST class function member insert.
 -----------------------------------------------------------------------------*/
{
    // Insert allCaps word and line number into BST
    Token t(allCaps, lineNum);
    for (char c = 'A'; c <= 'Z'; c++) {
       //cout << allCaps.at(0) << endl;
       if (allCaps.front() == c) {
           concord[c - 'A'].insert(t);
       }    
    }
}

void displayConcord(BST concord[])
/*------------------------------------------------------------------------------
  Display array of 26 BST's.

  Precondition:  The BST array concord[] is passed by value.
  Postcondition: The BST array concord[] is displayed via BST class function 
                 member inorder, which does an inorder transversal display of 
                 BSTs.
 -----------------------------------------------------------------------------*/
{
    // Inorder traversal display of the text concordance 
    cout << "Below is the text concordance: " << endl;
    int i = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        cout << endl;
        cout << "concord[" << i << "]: "; // starts at concord[0]
        concord[c - 'A'].inorder(cout);
        cout << endl;
        i++; // increments up to concord[25]
    }
}

int main()
{
    BST concord[ARRAY_SIZE]; // size is 26 for the 26 alphabet letters
    fstream file;
    string input, word;
    int lineNum = 0; // Text lines start at line #1.

    file.open("MayaAngelouPoem.txt", ios::in);
    
    // If file opened successfully, do the following
    if (file.is_open()) {
        while (getline(file, input)) { // get entire sentence and for each, do
            lineNum++;                 // increment lineNum by 1
            istringstream iss(input);  // create iss object to parse sentences
            while (iss >> word) {      
                string allCaps = "";   // string variable to store word in CAPS
                for (int i = 0; i < word.length(); i++) {
                    unsigned char c = word.at(i);
                    if (isalpha(c)) { // if char at position i is alpha
                        allCaps += toupper(word.at(i)); // convert to uppercase
                    }
                }
                // Once converted to all caps, insert word and lineNum into BST
                insertToBST(allCaps, lineNum, concord);
            }
        }
        // After successful insertion of all words, display all 26 BST's
        displayConcord(concord);
        cout << endl;
    }
    // If file failed to open, do the following
    else {
        cerr << "Failed to open file. Exiting.";
        exit(1);
    }
} // end main()
