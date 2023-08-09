#ifndef __LINKEDLIST_H__  
#define __LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"
#include <bits/stdc++.h>
using namespace std;

// TODO remove general_iterator
template <typename Container>
class linkedlist_forward_iterator{
    
  public: 
    typedef typename Container::Node           Node; //
    typedef typename Node::Type         Type;
    typedef linkedlist_forward_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;

  public:

    linkedlist_forward_iterator(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {}
    linkedlist_forward_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    linkedlist_forward_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }


    //Type &operator*()                    { return m_pNode->getDataRef();   }
    Node &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        m_pNode = (Node *)m_pNode->getpNext();  
        return *this;
    }

};

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}

template <typename K, typename V>
class NodeLinkedList : public KeyNode<K,V>{

  public:
    using Type = K;
    using value_type   = K;
    using LinkedValueType = V;
    using Parent = class KeyNode<K,V>;
  private:
    using Node = NodeLinkedList<K,V>; //myself
  public:
      Node   *m_pNext;//
  public:

    NodeLinkedList(value_type key, Node *pNext = nullptr){// θɪŋk ðɪs ɪz juːst baɪ kəmˈper
      Parent::m_key = key;
      m_pNext = pNext;
    }
    
    NodeLinkedList(value_type key, LinkedValueType value, Node *pNext = nullptr) : Parent(key,value){
      m_pNext = pNext;
    }

    void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }

};

template <typename _K,
          typename _V,
          typename _CompareFn = std::less< NodeLinkedList<_K, _V> & >>
struct LLTrait
{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeLinkedList<_K,_V>;
    using  CompareFn = _CompareFn;
};

template <typename Traits>
class LinkedList
{
  public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    
    typedef typename Traits::CompareFn  CompareFn;
    typedef LinkedList<Traits>          myself;
    typedef linkedlist_forward_iterator<myself>    iterator;
    
  protected:
    Node    *m_pHead = nullptr, 
            *m_pTail = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }

  public:
    LinkedList() {}
    // TODO add LinkedValueType value
    void    insert(value_type &key, LinkedValueType value) { insert_forward(key,value);  }
    value_type &operator[](size_t pos)
    {
      assert(pos <= size());
      Node *pTmp = m_pHead;
      for(auto i= 0 ; i < pos ; i++)
        pTmp = pTmp->getpNext();
      return pTmp->getDataRef();
    }
    iterator begin() { iterator iter(this, m_pHead);    return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }

    void    push_front(value_type elem)
    {
        Node *pNew = CreateNode(elem);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if(m_size == 1)
          m_pTail = pNew;
    } 
    void    push_back(value_type elem)
    {   Node *pNew = CreateNode(elem, nullptr);
        if(m_pTail)
          m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if(!m_pHead)
          m_pHead = pNew;
        m_size++;
    }

  protected:
    Node **findPrev(value_type &elem) {   return findPrev(m_pHead, elem);   }
    Node **findPrev(Node *&rpPrev, value_type &elem)
    {
      if(!rpPrev || Compfn(elem, rpPrev->getData()) )
        return &rpPrev; // Retorna la direccion del puntero que me apunta
      return findPrev((Node *&)rpPrev->getpNextRef(), elem);
    }

    Node *CreateNode(value_type &key, LinkedValueType value, Node *pNext=nullptr){
      return new Node(key,value,pNext);
    }
    Node **insert_forward(value_type &key, LinkedValueType value)
    {
        Node **pParent = findPrev(key);
        Node *pNew = CreateNode(key,value);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if( !pNew->getpNext() )
          m_pTail = pNew;
        m_size++;
        return pParent;
    }
  public:
    value_type PopHead()
    {
        if(m_pHead)
        {
            Node *pNode = m_pHead;
            value_type data = pNode->getData();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if(!m_size) m_pTail = nullptr;
            return data;
        }
        throw "hola excepcion"; // Create custom exception pending
    }

    // TODO add print
    void print(ostream &os){
      foreach(begin(),end(), printLine<Node>);
    }

    void reset(){
      while(m_pHead){
        PopHead();
      }
    }
    
    void read(istream &is){
      
      reset();

      size_t file_size;
      value_type key;
      LinkedValueType value;
      is>>file_size;

      string abc;
      getline(is, abc); //to avoid 20
      
      while(getline(is, abc) && m_size != file_size){ //keeping in mind the file_size
        
        stringstream ss(abc);
        string word;

        ss >> key;
        ss >> word;
        ss >> value;
        insert_forward(key,value);
      }
    }



};

// TODO add operator<<
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &obj){
    obj.print(os);
    return os;
}

// TODO add operator>>
template <typename T>
istream & operator>>(istream &is, LinkedList<T> &obj){
    obj.read(is);
    return is;
}

#endif