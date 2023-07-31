#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <algorithm> // sort algorithm
#include "types.h"
#include "iterator.h"
#include "keynode.h"
#include "xtrait.h"
using namespace std;

template <typename Container>
class array_forward_iterator 
     : public general_iterator<Container,  class array_forward_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, array_forward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef array_forward_iterator<Container>  myself;

  public:
    array_forward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    array_forward_iterator(myself &other)  : Parent (other) {}
    array_forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    array_forward_iterator operator++() { Parent::m_pNode++;  
                                          return *this;
                                        }
};

template <typename Container>
class array_backward_iterator 
     : public general_iterator<Container,  class array_backward_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, array_backward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef array_backward_iterator<Container>  myself;

  public:
    array_backward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    array_backward_iterator(myself &other)  : Parent (other) {}
    array_backward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    array_backward_iterator operator++() { Parent::m_pNode--;
                                          return *this;
                                        }
};

using TraitArrayFloatString = XTrait<float, string>;
using TraitArrayIntInt      = XTrait<TX  , int   , std::greater<KeyNode<TX  , int > &>>;
using TraitFloatLong        = XTrait<float, long  , std::greater<KeyNode<float, long> &>>;

// Created by: @ecuadros
template <typename Traits>
class CArray{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node      = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself    = CArray<Traits>;
    using iterator  = array_forward_iterator<myself>;
    using riterator  = array_backward_iterator<myself>; //riterator means reverse_iterator
private:
    Node     *m_pVect = nullptr;
    size_t    m_vcount = 0, m_vmax = 0;
    string    m_name = "Empty";
public:
    CArray(): m_name("Empty"){}
    CArray(string name) : m_name(name) {}
    ~CArray(){
        cout << "Destroying " << m_name << "..." << endl;
        destroy();
    }
    void insert(const value_type &key, LinkedValueType value){
        if(m_vcount == m_vmax) // Array is already full?
            resize();
        m_pVect[m_vcount++] = Node(key, value);
        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }

    // TODO: returns element as value
    Node back(){
        return m_pVect[m_vcount - 1];
    }

    // TODO: remove the last element only
    void pop_back(){
        m_vcount = m_vcount - 1; //I got a question in here!
    }

    void resize       ();
    void destroy(){
        delete [] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }
    
    void print        (ostream &os){
        // os << "Printing: " << m_name << endl;
        os << m_vcount << " " << m_vmax << endl;
        // sort(m_pVect, m_pVect+m_vcount, CompareFn() );
        for(size_t i = 1; i < m_vcount ; ++i )
            os << m_pVect[i].getData() << "\t: " << m_pVect[i].getValue() << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void read(istream &is){
        //freeing up space if it was already assigned
        destroy();
        // read here By Edson Caceres
        size_t vcount;
        is>>vcount>>m_vmax;
        Node *pTemp = new Node[m_vmax];
        //inserting values from .txt file
        value_type value;
        while(is >> value && size() != vcount){ //keeping in mind the m_vmax
            this->insert(value);
        }
    }

    size_t size()
    {  return m_vcount;    }

    Node &operator[](size_t pos)
    {   return m_pVect[pos];    }

    iterator begin() { iterator iter(this, m_pVect);    return iter;    }
    iterator end()   { iterator iter(this, m_pVect+m_vcount);    return iter;    }
    riterator rbegin() { riterator iter(this, m_pVect+m_vcount-1);     return iter;    }
    riterator rend()   { riterator iter(this, m_pVect-1);   return iter;    }
};

template <typename Traits>
void CArray<Traits>::resize(){
    Node *pTemp = new Node[m_vmax+10];
    for(size_t i = 0 ; i < m_vcount ; ++i)
        pTemp[i]   = m_pVect[i];
        // *(pTemp+i) = m_pVect[i];
        // pTemp[i]   = *(m_pVect+i);
        // *(pTemp+i) = *(m_pVect+i);
        // *(i+pTemp) = m_pVect[i];
        // i[pTemp]   = m_pVect[i];
    delete [] m_pVect;
    m_pVect = pTemp;
    m_vmax +=10;
    // cout << "Vector resized m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj){
    obj.print(os);
    return os;
}

// TODO
template <typename T>
istream & operator>>(istream &is, CArray<T> &obj){
    // TODO
    return is;
}


#endif // __ARRAY_H__
