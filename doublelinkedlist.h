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

template <typename K, typename V>
class NodeDLL : public KeyNode<K,V>{
  public:
    using Type = K;
    using value_type   = K;
    using LinkedValueType = V;
    using Parent = class KeyNode<K,V>;
  private:
    using Node = NodeDLL<K,V>;
    public: 
      Node   *m_pPrev;
      Node   *m_pNext;
    public:

      NodeDLL(value_type key,// θɪŋk ðɪs ɪz juːst baɪ kəmˈper
              Node *pNext = nullptr,
              Node *pPrev = nullptr){
        Parent::m_key = key;
        m_pPrev = pPrev;
        m_pNext = pNext;
      }

      NodeDLL(value_type key,
              LinkedValueType value,
              Node *pNext = nullptr,
              Node *pPrev = nullptr)
              : Parent(key,value){
        m_pPrev = pPrev;
        m_pNext = pNext;
      }
    
      void      setpPrev(Node *pPrev)  {   m_pPrev = pPrev;  }
      Node     *getpPrev()             {   return getpPrevRef();   }
      Node    *&getpPrevRef()          {   return m_pPrev;   }
      void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
      Node     *getpNext()               {   return getpNextRef();   }
      Node    *&getpNextRef()            {   return m_pNext;   }
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
class DoubleLinkedList{
  public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    typedef typename Traits::CompareFn  CompareFn;
    typedef DoubleLinkedList<Traits>    myself;
    typedef linkedlist_forward_iterator<myself>    iterator;
    typedef linkedlist_backward_iterator<myself>   riterator;
  protected:
    Node    *m_pHead = nullptr, 
            *m_pTail = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public:
    DoubleLinkedList() {}
    void    insert(value_type &key, LinkedValueType value)
    {   
        Node *pPrevTail = m_pTail;
        Node *pNew = *insert_forward(key,value);
        if( pNew != m_pTail )
            ::CreateBridge( ((Node *)pNew->getpNext())->getpPrevRef(), pNew, &Node::m_pPrev);
        else
            pNew->setpPrev(pPrevTail);
    }
    iterator begin() { iterator iter(this, m_pHead);    return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }
    riterator rbegin() { riterator iter(this, m_pTail);     return iter;    }
    riterator rend()   { riterator iter(this, nullptr);             return iter;    }
    // void    push_front(value_type elem)
    // {
    //     Parent::push_front(elem);
    //     if(Parent::size() > 1)
    //         ((Node *)Parent::m_pHead->m_pNext)->m_pPrev = Parent::m_pHead;
    // }
    // void    push_back(value_type elem)
    // {   Node *pPrevTail = Parent::m_pTail;
    //     Parent::push_back(elem);
    //     Parent::m_pTail->setpPrev(pPrevTail);
    // }
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
      value_type PopHead(){
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
      foreach(begin(),end(), [&os](Node& node){
        os<<"{"<<node.getData()<<":"<<node.getValue()<<"}"<< ", "; 
      });
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
ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj){
    obj.print(os);
    return os;
}

// TODO add operator>>
template <typename T>
istream & operator>>(istream &is, DoubleLinkedList<T> &obj){
    obj.read(is);
    return is;
}

#endif