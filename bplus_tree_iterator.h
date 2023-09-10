#ifndef __BPLUS_TREE_ITERATOR_H__
#define __BPLUS_TREE_ITERATOR_H__
//#include <unistd.h>
//usleep(microseconds);

template <typename Container,typename Page>
class bplus_tree_iterator{
    
  public: 
    typedef bplus_tree_iterator<Container,Page>  myself;

  private:

    Container *m_pContainer;
    Page      *m_pNode;

  public:

    bplus_tree_iterator(Container *pContainer, Page *pNode)
        : m_pContainer(pContainer), m_pNode(pNode){}
    bplus_tree_iterator(myself &other) 
          //: m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}//ˈʌð.ɚ.waɪz aɪ n't g ˈvæl.juː
    bplus_tree_iterator(myself &&other) // Move constructor
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

    Page &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        m_pNode = (Page *)m_pNode->getpNext();  
        return *this;
    }

};

#endif