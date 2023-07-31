#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

using XTraitFloatStringDesc = XTrait<float, string, std::less<KeyNode<int, int > &>>;
using XTraitIntIntAsc       = XTrait<int  , int   , std::greater<KeyNode<int, int > &>>;
using XTraitIntIntDesc       = XTrait<float  , int   , std::less<KeyNode<float, int > &>>;

// Created by: @ecuadros
template <typename Traits>
class CHeap{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    using CompareFn       = typename Traits::CompareFn;
    using myself          = CHeap<Traits>;
private:
    CArray<Traits>    m_heap;
    string    m_name = "Empty";
    CompareFn compareFn_;
public:
    CHeap(string name)  : m_name(name){ destroy();  }
    CHeap()                           { destroy();  }
    virtual ~CHeap(){
        cerr << "Destroying " << m_name << "..." << endl;
        destroy();
    }
    void destroy(){
        m_heap.destroy();
        m_heap.insert(0, 0);
    }
    
    void insert(const value_type &key, LinkedValueType value){
        m_heap.insert(key, value);
        //sort(); //if you want the array to keep it sorted always
    }

    size_t left(const size_t n){
        assert(n >= 1);
        return 2*n;
    }

    size_t right(const size_t n){
        assert(n >= 1);
        return 2*n + 1;
    }

    size_t size(){
        return m_heap.size() - 1;
    }

    void heapify(size_t n){ //just one function for both purposes
        auto l = left(n);
        auto r = right(n);
        auto smallestOrLargest = n;
        
        if( l <= size() && compareFn_( m_heap[l] , m_heap[n] ) ){
            smallestOrLargest = l;
        }
        if( r <= size() && compareFn_(m_heap[r],m_heap[smallestOrLargest]) ){
            smallestOrLargest = r;
        }
        if(smallestOrLargest != n){
            swap(m_heap[n], m_heap[smallestOrLargest]);
            heapify(smallestOrLargest);
        }        
    }

    void sort(){
        for(size_t i = size()/2;i >= 1;i--){
            heapify(i);
        }
    }

    Node pop(){
        assert(m_heap.size() > 1);
        Node ans = m_heap[1];
        swap(m_heap[1], m_heap[m_heap.size()-1]);
        m_heap.pop_back();
        heapify(1);
        return ans;
    }

    void print        (ostream &os){
        os << m_heap;
    }
    void read(istream &is){
        is >> m_heap;
    }

    value_type operator[](size_t pos)
    {   return m_heap[pos];    }

};

#endif
