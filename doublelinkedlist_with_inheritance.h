#ifndef __DOUBLE_LINKEDLIST_H__  
#define __DOUBLE_LINKEDLIST_H__ 

#include "linkedlist.h"

template <typename Container>
class linkedlist_backward_iterator{
    
  public: 
    typedef typename Container::Node           Node; //
    typedef typename Node::Type         Type;
    typedef linkedlist_backward_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;

  public:

    linkedlist_backward_iterator(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {}
    linkedlist_backward_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    linkedlist_backward_iterator(myself &&other) // Move constructor
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

    Node &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        m_pNode = ((Node *)m_pNode)->getpPrev();  
        return *this;
    }

};

// TODO Remove inheritance
template <typename K, typename V>
class NodeDLL : public NodeLinkedList<K,V>
{
public:
  using Type = K;
  using value_type   = K;
  using LinkedValueType = V;
  typedef class NodeLinkedList<K,V> Parent;
private:
  typedef NodeDLL<K,V> Node;
  public: 
    Node   *m_pPrev;//
  public:

    NodeDLL(value_type key,// θɪŋk ðɪs ɪz juːst baɪ kəmˈper
            Node *pNext = nullptr,
            Node *pPrev = nullptr)
            : Parent(key,pNext){
        m_pPrev = pPrev;
      }

    NodeDLL(value_type key, LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr) 
        : Parent(key, value, pNext), m_pPrev(pPrev)
    {}
   
    void      setpPrev(Node *pPrev)  {   m_pPrev = pPrev;  }
    Node     *getpPrev()             {   return getpPrevRef();   }
    Node    *&getpPrevRef()          {   return m_pPrev;   }
};

template <typename _K,
          typename _V,
          typename _CompareFn = std::less< NodeDLL<_K, _V> & >>
struct DLLTrait
{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeDLL<_K,_V>;
    using  CompareFn = _CompareFn;
};

// TODO remove inheritance
template <typename Traits>
class DoubleLinkedList : public LinkedList<Traits>
{
 public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    typedef typename Traits::CompareFn  CompareFn;
    typedef DoubleLinkedList<Traits>    myself;
    typedef LinkedList<Traits>          Parent;
    typedef linkedlist_forward_iterator<myself>    iterator;
    typedef linkedlist_backward_iterator<myself>   riterator;
public:
    DoubleLinkedList() {}
    void    insert(value_type &key, LinkedValueType value)
    {   
        Node *pPrevTail = Parent::m_pTail;
        Node *pNew = *Parent::insert_forward(key,value);
        if( pNew != Parent::m_pTail )
            ::CreateBridge( ((Node *)pNew->getpNext())->getpPrevRef(), pNew, &Node::m_pPrev);
        else
            pNew->setpPrev(pPrevTail);
    }
    riterator rbegin() { riterator iter(this, Parent::m_pTail);     return iter;    }
    riterator rend()   { riterator iter(this, nullptr);             return iter;    }
    void    push_front(value_type elem)
    {
        Parent::push_front(elem);
        if(Parent::size() > 1)
            ((Node *)Parent::m_pHead->m_pNext)->m_pPrev = Parent::m_pHead;
    }
    void    push_back(value_type elem)
    {   Node *pPrevTail = Parent::m_pTail;
        Parent::push_back(elem);
        Parent::m_pTail->setpPrev(pPrevTail);
    }
};

#endif