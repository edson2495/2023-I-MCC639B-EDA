#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
#include "binarytree.h"
#include <bits/stdc++.h>
using namespace std;

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

    void setChild(Node* ptrParent, Node* ptrChild, size_t numberChild){
      ptrParent->setpChild(ptrChild,numberChild);
      if(ptrChild){ptrChild->setParent(ptrParent);}//in case child is null
    }

  Node* rotation(Node* pCurrentRoot, Node* pParent, string type){//by Edson Cáceres
      Node* pNewRoot = pCurrentRoot->getChild(type=="left"?1:0); //new root of the subtree
      setChild(pCurrentRoot,pNewRoot->getChild(type=="left"?0:1),type=="left"?1:0);
      setChild(pNewRoot,pCurrentRoot,type=="left"?0:1);
      pNewRoot->setParent(pParent);
      pCurrentRoot->setHeight( max( height(pCurrentRoot->getChild(0)) , height(pCurrentRoot->getChild(1)) ) + 1 );
      pNewRoot->setHeight( max( height(pNewRoot->getChild(0)) , height(pNewRoot->getChild(1)) ) + 1 );
      return pNewRoot;
  }

  void balance(Node* pNode, Node *& pNewRoot){ //by Edson Cáceres
    pNode->setHeight( max( height(pNode->getChild(0)) , height(pNode->getChild(1)) ) + 1 );
    int bF = balanceFactor(pNode);
    Node* pParent = pNode->getParent();

    if( bF > 1 || bF < -1 ){
      size_t numberChild;
      if(pParent){numberChild = (pParent->getChild(0)==pNode?0:1);}
      if(bF == -2){//left rotation cases
        if(balanceFactor(pNode->getChild(1)) == -1){//left roration
          pNewRoot = rotation(pNode,pParent,"left");
        }else if(balanceFactor(pNode->getChild(1)) == 1){//right-left roration
          pNode->setpChild(rotation(pNode->getChild(1),pNode,"right"),1);
          pNewRoot = rotation(pNode,pParent,"left");
        }
      }
      if(bF == 2){//right rotation cases
        if(balanceFactor(pNode->getChild(0)) == 1){//right roration
          pNewRoot = rotation(pNode,pParent,"right");
        }else if(balanceFactor(pNode->getChild(0)) == -1){//left-right roration
          pNode->setpChild(rotation(pNode->getChild(0),pNode,"left"),0);
          pNewRoot = rotation(pNode,pParent,"right");
        }
      }
      if(pParent){pParent->setpChild(pNewRoot,numberChild);}
      else{this->m_pRoot = pNewRoot;}
    }
    if(pParent){balance(pParent,pNewRoot);}
  }

  public: 

    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    Node* insert(value_type &key, LinkedValueType value/*, Node*& pInsertedNode = nullptr*/){ //by Edson Cáceres
      Node* pInsertedNode = this->internal_insert(key, value, nullptr, this->m_pRoot,0);
      //pInsertedNode = this->internal_insert(key, value, nullptr, this->m_pRoot,0);
      Node* pNewRoot = nullptr;
      if(pInsertedNode->getParent()){balance(pInsertedNode->getParent(),pNewRoot);}
      if(pNewRoot != nullptr){
        this->preorder2(pNewRoot,[](Node & node){
          node.setLevel( (node.getParent()?node.getParent()->getLevel():-1) + 1 );
        });
      }
      return pInsertedNode;
    }

    void read(istream &is){ //created by Edson Cáceres
      size_t file_size;
      value_type key;
      LinkedValueType value;
      is>>file_size;

      string line;
      getline(is, line); //to avoid 20
      
      while(getline(is, line) && this->m_size != file_size){ //keeping in mind the file_size
        
        stringstream ss(line);
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