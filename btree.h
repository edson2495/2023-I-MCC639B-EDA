#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include <bits/stdc++.h>

#include "btreepage.h"
#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 

template <typename _K, typename _V>
struct BTreeTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
};

template <typename Trait>
class BTree // this is the full version of the BTree
{
       typedef typename Trait::value_type    value_type;
       typedef typename Trait::LinkedValueType    LinkedValueType;
       
       typedef CBTreePage <Trait> BTNode;// useful shorthand

public:
       
       typedef typename BTNode::ObjectInfo      ObjectInfo;

public:
       BTree(size_t order = DEFAULT_BTREE_ORDER, bool unique = true)
              : m_Order(order),
                m_Root(2 * order  + 1, unique),
                m_Unique(unique),
                m_NumKeys(0)
       {
              m_Root.SetMaxKeysForChilds(order);
              m_Height = 1;
       }
       ~BTree() {}

       //my work
       //https://en.cppreference.com/w/cpp/thread/mutex
       //https://en.cppreference.com/w/cpp/thread/thread/thread
       template <typename F, typename C, typename... Args> //created by Edson Cáceres
       //void execFuncInConcurrencyControl(F&& func, C& object, Args&&... args){
       void execFuncInConcurrencyControl(F&& func, C& object, Args&&... args){
              thread th(func,&object,args...);

              //thread th(&funcInConcurrencyControl<F,Args&&...>,&object,func,args...);

              th.join();
       }

       /*template <typename F, typename... Args> //created by Edson Cáceres
       void funcInConcurrencyControl(F&& func, Args&&... args){
       //void funcInConcurrencyControl(F&& func, const value_type key, const LinkedValueType value){
              mtx.lock();//written by Edson Cáceres
              invoke(func, forward<Args>(args)...);
              //invoke(func, key, value);
              mtx.unlock();//written by Edson Cáceres
       }*/


       bool            Insert (const value_type key, const LinkedValueType value);
       bool            Remove (const value_type key, const LinkedValueType value);
       LinkedValueType       Search (const value_type key)
       {      LinkedValueType value = -1;
              m_Root.Search(key, value);
              return value;
       }
       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }

       void read(istream &is){ //created by Edson Cáceres
              size_t file_size;
              value_type key;
              LinkedValueType value;
              is>>file_size;

              string line;
              getline(is, line); //to avoid 20
              
              while(getline(is, line) && this->m_NumKeys != file_size){ //keeping in mind the file_size
                     
                     stringstream ss(line);
                     string word;

                     ss >> key;
                     ss >> word;
                     ss >> value;
                     Insert(key,value);
              }
       }

       void            Print (ostream &os)
       {               m_Root.Print(os);                              }

       template <typename F, typename... Args> //created by Edson Cáceres
       void ForEach(F lpfn, Args&&... args){
              m_Root.ForEach(lpfn, 0, args...);   }

       template <typename F, typename... Args> //created by Edson Cáceres
       void FirstThat(F lpfn, Args&&... args){
              m_Root.FirstThat(lpfn, 0, args...);
       }

protected:
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       bool            m_Unique;  // Accept the elements only once ?

       mutex mtx;           // mutex for critical section ; written by Edson Cáceres

};     

//my work
// TODO change value by LinkedValueType value
template <typename Trait>
bool BTree<Trait>::Insert(const value_type key, const LinkedValueType value){
       mtx.lock();//written by Edson Cáceres
       bt_ErrorCode error = m_Root.Insert(key, value);
       if( error == bt_duplicate )
              return false;
       m_NumKeys++;
       if( error == bt_overflow )
       {
               m_Root.SplitRoot();
               m_Height++;
       }
       mtx.unlock();//written by Edson Cáceres
       return true;
}

//my work
template <typename Trait>
bool BTree<Trait>::Remove (const value_type key, const LinkedValueType value){
       mtx.lock();//written by Edson Cáceres
       bt_ErrorCode error = m_Root.Remove(key, value);
       if( error == bt_duplicate || error == bt_nofound )
              return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
              m_Height--;
       mtx.unlock();//written by Edson Cáceres
       return true;
}

//my work
// TODO Add operator<<
template <typename T> //created by Edson Cáceres
ostream &operator<<(ostream &os, BTree<T> &obj){
    obj.Print(cout);
    return os;
}

//my work
// TODO Add operator>>
template <typename T> //created by Edson Cáceres
istream &operator>>(istream &is, BTree<T> &obj){
    obj.read(is);
    return is;
}

#endif