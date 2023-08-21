#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <cassert>
#include "types.h"
#include "tree_iterators.h"
using namespace std;

string operator*(string text, size_t n)
{
    for(size_t i = 0;i<n;i++){
        text += text;
    }
    return text;
}

template <typename K, typename V>
class NodeBinaryTree : public KeyNode<K,V>{
public:
    using Type = K;
    using value_type   = K;
    using LinkedValueType = V;
    using Parent = class KeyNode<K,V>;
private:
  typedef NodeBinaryTree<K,V> Node;
  private : bool m_printed = 0;
  public:
    //T       m_data;
    Node *  m_pParent = nullptr;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t m_level;
  public:

    NodeBinaryTree(value_type key, // θɪŋk ðɪs ɪz juːst baɪ kəmˈper
                   Node *pParent= nullptr,                   
                   Node *p0 = nullptr,
                   Node *p1 = nullptr) 
        : m_pParent(pParent){
        Parent::m_key = key;
        m_pChild[0] = p0;   m_pChild[1] = p1;
    }

    NodeBinaryTree(Node *pParent,
                   value_type key,
                   LinkedValueType value,
                   size_t level,
                   Node *p0 = nullptr,
                   Node *p1 = nullptr) 
        : m_pParent(pParent), Parent(key,value){
        m_level = level;
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }
 
    void      setpChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }

    bool getPrinted(){return m_printed;}
    void setPrinted(bool printed){m_printed = printed;}
    size_t getLevel(){return m_level;}

};

template <typename _K,
          typename _V,
          typename _CompareFn = std::less< NodeBinaryTree<_K, _V> & >>
struct BinaryTreeTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeBinaryTree<_K,_V>;
    using  CompareFn = _CompareFn;
};

template <typename Traits>
class BinaryTree
{
  public:
    typedef typename Traits::value_type      value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    
    typedef typename Traits::CompareFn      CompareFn;
    typedef BinaryTree<Traits>              myself;
    typedef binary_tree_inorder_iterator<myself>    in_iterator; //inorder iterator
    typedef binary_tree_postorden_iterator<myself>    post_iterator; //postorden iterator
    typedef binary_tree_preorden_iterator<myself>    pre_iterator; //preorden iterator
    typedef binary_tree_print_iterator<myself>    print_iterator; //print as tree iterator

protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void    insert(value_type &key, LinkedValueType value) { internal_insert(key, value, nullptr, m_pRoot,0);  }

    in_iterator inbegin(bool printed = 0) { in_iterator iter(this, leftMost(m_pRoot),printed); return iter;    }
    in_iterator inend(bool printed = 0)   { in_iterator iter(this, rightMost(m_pRoot),printed);return iter;    }
    post_iterator postbegin(bool printed = 0) { post_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }
    post_iterator postend(bool printed = 0)   { post_iterator iter(this, m_pRoot,printed);    return iter;    }
    pre_iterator prebegin(bool printed = 0) { pre_iterator iter(this, m_pRoot,printed);    return iter;    }
    pre_iterator preend(bool printed = 0)   { pre_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    print_iterator printbegin(bool printed = 0) { print_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    print_iterator printend(bool printed = 0)   { print_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }

protected:
    Node *CreateNode(Node *pParent, value_type &key, LinkedValueType value, size_t level){ return new Node(pParent, key, value, level); }
    Node *internal_insert(value_type &key, LinkedValueType value, Node *pParent, Node *&rpOrigin, size_t level)
    {
        if( !rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent, key, value,level));
        }
        size_t branch = Compfn(key, rpOrigin->getDataRef() );
        return internal_insert(key, value, rpOrigin, rpOrigin->getChildRef(branch), level+1);
    }

    Node* leftMost(Node *pNode){
        assert(pNode != nullptr);
        while(pNode != nullptr && pNode->getChild(0) != nullptr)
			pNode = pNode->getChild(0);
        return pNode;
    }

    Node* rightMost(Node *pNode){
        assert(pNode != nullptr);
        while(pNode != nullptr && pNode->getChild(1) != nullptr)
			pNode = pNode->getChild(1);
        return pNode;
    }

public:

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in inorder
    template <typename F, typename... Args> //created by Edson Cáceres
    void inorder(F func, Args&&... args){
        foreach(inbegin(),inend(), func,args...);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    template <typename F, typename... Args> //created by Edson Cáceres
    void postorder(F func, Args&&... args){
        foreach(postbegin(),postend(), func,args...);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    template <typename F, typename... Args> //created by Edson Cáceres
    void preorder(F func, Args&&... args){
        foreach(prebegin(),preend(), func,args...);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }
    
    // TODO: generalize this function by using iterators and apply any function
    template <typename F, typename... Args> //created by Edson Cáceres
    void print(F func, Args&&... args){
        foreach(printbegin(),printend(), func, args...);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    //replaced by inorder with variadic inorder
    // // TODO: generalize this function by using iterators and apply any function
    // void inorder(void (*func) (Node& Node, LinkedValueType& value),LinkedValueType& value){ //created by Edson Cáceres
    //     foreach(inbegin(),inend(), func, value);
    //     foreach(inbegin(1),inend(1), [](Node& node){});
    // }
};

#endif