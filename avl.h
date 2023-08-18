#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
#include "binarytree.h"
#include <bits/stdc++.h>
using namespace std;

// template <typename T>
// class NodeAVL : public NodeBinaryTree
// {
// public:
//   typedef T         Type;
// private:
//   using Parent = NodeBinaryTree<T> Node;
//   public:
//     size_t m_depth; 
//   public:
//     NodeAVL(Node *pParent, T data, Node *p0 = nullptr, Node *p1 = nullptr) 
//         : Parent(pParent, p0, p1)
//     {}

// };

// TODO: AVL
template <typename Traits>
class CAVL: public BinaryTree<Traits>{
  
  public:
    typedef typename Traits::value_type      value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    
    //typedef typename Traits::CompareFn      CompareFn;
    typedef CAVL<Traits>                myself;
    // typedef binary_tree_iterator<myself>    iterator;

    using Parent = BinaryTree<Traits>;

  protected:

    size_t height(Node* pNode){
      return (pNode?pNode->getHeight():0);
    }

    int balanceFactor(Node* pNode){
      return height(pNode->getChild(0)) - height(pNode->getChild(1));
    }

    Node* leftRotation(Node* pCurrentRoot, Node* pParent){
      Node* pNewRoot = pCurrentRoot->getChild(1); //new root of the subtree
      pCurrentRoot->setpChild(pNewRoot->getChild(0),1);
      if(pNewRoot->getChild(0)){pNewRoot->getChild(0)->setParent(pCurrentRoot);}
      pNewRoot->setpChild(pCurrentRoot,0);
      pCurrentRoot->setParent(pNewRoot);
      
      pNewRoot->setParent(pParent);
      pCurrentRoot->setHeight( max( height(pCurrentRoot->getChild(0)) , height(pCurrentRoot->getChild(1)) ) + 1 );
      pNewRoot->setHeight( max( height(pNewRoot->getChild(0)) , height(pNewRoot->getChild(1)) ) + 1 );
      return pNewRoot;
    }

    Node* rightRotation(Node* pCurrentRoot, Node* pParent){
      Node* pNewRoot = pCurrentRoot->getChild(0); //new root of the subtree
      pCurrentRoot->setpChild(pNewRoot->getChild(1),0);
      if(pNewRoot->getChild(1)){pNewRoot->getChild(1)->setParent(pCurrentRoot);}
      pNewRoot->setpChild(pCurrentRoot,1);
      pCurrentRoot->setParent(pNewRoot);
      
      pNewRoot->setParent(pParent);
      pCurrentRoot->setHeight( max( height(pCurrentRoot->getChild(0)) , height(pCurrentRoot->getChild(1)) ) + 1 );
      pNewRoot->setHeight( max( height(pNewRoot->getChild(0)) , height(pNewRoot->getChild(1)) ) + 1 );
      return pNewRoot;
    }


    void balance(Node* pNode, Node *& pNewRoot){
      pNode->setHeight( max( height(pNode->getChild(0)) , height(pNode->getChild(1)) ) + 1 );
      int bF = balanceFactor(pNode);
      Node* pParent = pNode->getParent();

      if( bF > 1 || bF < -1 ){
        size_t numberChild;
        if(pParent){numberChild = (pParent->getChild(0)==pNode?0:1);}
        if(bF == -2){//left rotation cases
          if(balanceFactor(pNode->getChild(1)) == -1){//left roration
            pNewRoot = leftRotation(pNode,pParent);
          }else if(balanceFactor(pNode->getChild(1)) == 1){//right-left roration
            pNode->setpChild(rightRotation(pNode->getChild(1),pNode),1);
            pNewRoot = leftRotation(pNode,pParent);
          }
        }
        if(bF == 2){//right rotation cases
          if(balanceFactor(pNode->getChild(0)) == 1){//right roration
            pNewRoot = rightRotation(pNode,pParent);
          }else if(balanceFactor(pNode->getChild(0)) == -1){//left-right roration
            pNode->setpChild(leftRotation(pNode->getChild(0),pNode),0);
            pNewRoot = rightRotation(pNode,pParent);
          }
        }
        if(pParent){pParent->setpChild(pNewRoot,numberChild);}
        else{this->m_pRoot = pNewRoot;}
      }
      if(pParent){balance(pParent,pNewRoot);}
    }

  public: 
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void insert(value_type &key, LinkedValueType value){
      Node* pInsertedNode = this->internal_insert(key, value, nullptr, this->m_pRoot,0);
      Node* pNewRoot = nullptr;
      if(pInsertedNode->getParent()){balance(pInsertedNode->getParent(),pNewRoot);}
      if(pNewRoot != nullptr){
        this->preorder(pNewRoot,[](Node & node){
          node.setLevel( (node.getParent()?node.getParent()->getLevel():-1) + 1 );
        });
      }
    }

    void read(istream &is){
      size_t file_size;
      value_type key;
      LinkedValueType value;
      is>>file_size;

      string abc;
      getline(is, abc); //to avoid 20
      
      while(getline(is, abc) && this->m_size != file_size){ //keeping in mind the file_size
        
        stringstream ss(abc);
        string word;

        ss >> key;
        ss >> word;
        ss >> value;
        insert(key,value);
      }
    }
    
};

// TODO add operator>>
template <typename T>
istream & operator>>(istream &is, CAVL<T> &obj){
    obj.read(is);
    return is;
}

#endif