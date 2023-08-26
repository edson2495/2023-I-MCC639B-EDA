#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

// using XTraitFloatStringDesc = XTraitTrait<float, string, std::less<KeyNode<int, int > &>>;>;
// using XTraitIntIntAsc       = XTraitTrait<int  , int   , std::greater<KeyNode<int, int > &>>;

// Created by: @ecuadros
template <typename Traits>
class CGraph{
public:
    // using value_type      = typename Traits::value_type;
    // using KeyType         = typename Traits::value_type;
    // using LinkedValueType = typename Traits::LinkedValueType;
    // using Node            = typename Traits::Node;
    // using CompareFn       = typename Traits::CompareFn;
    // using myself          = CGraph<Traits>;
private:
    
    string    m_name = "Empty";
    // TODO definir colección de nodes
    // map<string, NodeType>
public:
    CGraph(string name)  : m_name(name){ destroy();  }
    CGraph()                           { destroy();  }
    virtual ~CGraph(){
        cerr << "Destroying " << m_name << "..." << endl;
        reset();
    }
    void destroy(){
        
    }

    // CGraph operator=(CGraph &other){
    //     destroy();
        
    // }
    
    // TODO: Replantear
    void insert(const value_type &key, LinkedValueType value){
    }

    void print        (ostream &os){
        
    }
    void read(istream &is){
        
    }

    size_t size()
    {  return 0;    }

    // TODO : agregar el operator value_type &
    // value_type &operator[](size_t pos)
    // {   return m_heap[pos].getDataRef();    }
};

#endif
