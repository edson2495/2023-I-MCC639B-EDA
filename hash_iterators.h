#ifndef __HASH_ITERATOR_H__
#define __HASH_ITERATOR_H__

template <typename Container>
class hash_forward_iterator{
    
  public: 
    typedef typename Container::Node           Node;
    typedef hash_forward_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    size_t m_index;

  public:

    hash_forward_iterator(Container *pContainer, size_t index)
        : m_pContainer(pContainer), m_index(index){}
    hash_forward_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_index(other.m_index){}
    hash_forward_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_index      = move(other.m_index);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_index      = move(iter.m_index);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_index == iter.m_index; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node& operator*(){
        return *(m_pContainer->search(m_pContainer->getKey(m_index)));
    }

    myself operator++() {
        m_index++;
        return *this;
    }

};

template <typename Container>
class hash_backward_iterator{
    
  public: 
    typedef typename Container::Node           Node;
    typedef hash_backward_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    //Node      *m_pNode;
    size_t m_index;

  public:

    hash_backward_iterator(Container *pContainer, size_t index)
        : m_pContainer(pContainer), m_index(index){}
    hash_backward_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_index(other.m_index){}
    hash_backward_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_index      = move(other.m_index);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_index      = move(iter.m_index);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_index == iter.m_index; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node& operator*(){
        return *(m_pContainer->search(m_pContainer->getKey(m_index)));
    }

    myself operator++() {
        m_index--;
        return *this;
    }

};

#endif