#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"
using namespace std;

template <typename Data>
class BST {
   protected:
    /** Pointer to the root of this BST, or 0 if the BST is empty */
    BSTNode<Data> *root;

    /** Number of Data items stored in this BST. */
    unsigned int isize;

    /** Height of this BST. */
    unsigned int iheight;

   public:
    /** define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /** Default constructor.
        Initialize an empty BST.
     */
    BST() : root(0), isize(0), iheight(0) {}

    /** Default destructor.
        Delete every node in this BST.
     */
    virtual ~BST() {
        deleteAll(root);
    }


    /** Given a reference to a Data item, insert a copy of it in this BST.
     *  Return  true if the item was added to this BST
     *  as a result of this call to insert,
     *  false if an item equal to this one was already in this BST.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=)  For the reasoning
     *  behind this, see the assignment writeup.
     */
    virtual bool insert(const Data &item) {
	BSTNode<Data> *curr = root;
	unsigned int height=1;
	if(!root) {
	    root = new BSTNode<Data>(item);
	    isize++;
	    iheight = 1;
	    return 1;
        }
	else while (1) {
	    height++;
	    if (curr->data < item) {
                if(!curr->right) {
		    curr->right = new BSTNode<Data>(item);
		    curr->right->parent = curr;
	     	    isize++;
		    if(height>iheight) iheight=height;
	            return 1;
	        } 
		else 
		    curr = curr->right;
	    }
            else if (item < curr->data) {
 	    	if(!curr->left) {
		    curr->left = new BSTNode<Data>(item);
		    curr->left->parent = curr;
		    isize++;
		    if(height>iheight) iheight=height;
		    return 1;
		}
		else
		    curr = curr->left;
	    }
	    else
		return 0;
	}
    }

    /** Find a Data item in the BST.
     *  Return an iterator pointing to the item, or pointing past
     *  the last node in the BST if not found.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=).  For the reasoning
     *  be::::::hind this, see the assignment writeup.
     */
    virtual iterator find(const Data &item) const {
	auto curr = begin();
	if(curr == nullptr)
            return curr;
        for(; curr != end(); curr++) {
	    if(!(*curr < item) && !(item < *curr))
		break;
	}
	return curr;
    }

    /** Return the number of items currently in the BST.
     */
    unsigned int size() const { return isize; }

    /** Return the height of the BST.
     */
    unsigned int height() const { return iheight; }

    /** Return true if the BST is empty, else false.
     */
    bool empty() const { if(isize!=0)return 0; return 1;}

    /** Return an iterator pointing to the first item in the BST (not the root).
     */
    iterator begin() const {
	return typename BST<Data>::iterator(first(root));
    }

    /** Return an iterator pointing past the last item in the BST.
     */
    iterator end() const { return typename BST<Data>::iterator(0); }

    /** Perform an inorder traversal of this BST.
     */
    void inorder() const { inorder(root); }

   private:
    /** Recursive inorder traversal 'helper' function */

    /** Inorder traverse BST, print out the data of each node in ascending
       order. Implementing inorder and deleteAll base on the pseudo code is an
       easy way to get started.
     */
    void inorder(BSTNode<Data> *n) const {
        /* Pseudo Code:
          if current node is null: return;
          recursively traverse left sub-tree
          print current node data
          recursively traverse right sub-tree
        */
	if(!n)
	    return;
	if(n->left)
	    inorder(n->left);
	cout << n->data<<endl;
	if(n->right)
	    inorder(n->right);
    }

    /** Find the first element of the BST
     */
    static BSTNode<Data> *first(BSTNode<Data> *curr) {
	if (!curr) return 0;
        while (curr->left) curr = curr->left;
	return curr;
    }

    /** do a postorder traversal, deleting nodes
     */
    static void deleteAll(BSTNode<Data> *n) {
        /* Pseudo Code:
          if current node is null: return;
          recursively delete left sub-tree
          recursively delete right sub-tree
          delete current node
        */
        if(n == nullptr)
            return;
        if(n->left)
            deleteAll(n->left);
        if(n->right)
            deleteAll(n->right);
        delete n;

    }
};

#endif  // BST_HPP
