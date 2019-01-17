#ifndef BSTITERATOR_HPP
#define BSTITERATOR_HPP
#include <iterator>
#include <list>
#include "BSTNode.hpp"
using namespace std;

template <typename Data>
class BSTIterator : public iterator<input_iterator_tag, Data> {
   private:
    BSTNode<Data>* curr;

   public:
    /** Constructor.  Use the argument to initialize the current BSTNode
     *  in this BSTIterator.
     */
    BSTIterator(BSTNode<Data>* cur):curr(cur){}


    /** Dereference operator. */
    Data operator*() const { return curr->data; }

    /** Pre-increment operator. */
    BSTIterator<Data>& operator++() {
        curr = curr->successor();
        return *this;
    }

    /** Post-increment operator. */
    BSTIterator<Data> operator++(int) {
        BSTIterator before = BSTIterator(curr);
        ++(*this);
        return before;
    }

    /** Equality test operator. */ 
    bool operator==(BSTIterator<Data> const& other) const {
	if(!other.curr && !curr)
	    return 1;
        if(!other.curr || !curr)
	    return 0;
	if(other.curr->data < curr->data)
	    return 0;
	if(curr->data < other.curr->data)
	    return 0;
	return 1;
    }

    /** Inequality test operator. */ 
    bool operator!=(BSTIterator<Data> const& other) const {
	if(!other.curr && !curr)
	    return 0;
        if(!other.curr || !curr)
	    return 1;
	if(other.curr->data < curr->data)
	    return 1;
	if(curr->data < other.curr->data)
	    return 1;
	return 0;
    }
};

#endif  // BSTITERATOR_HPP
