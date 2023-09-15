#ifndef __HASH_H__  
#define __HASH_H__
#include "avl.h"
#include "hash_iterators.h"

template <typename Traits>
class CHash{// created by Edson Cáceres

    public:

        using value_type = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
        using Node = typename Traits::Node;
        using myself = CHash<Traits>;
        using iterator = hash_forward_iterator<myself>;
        using riterator = hash_backward_iterator<myself>;

    protected:

        CAVL<Traits> m_avl;
        size_t m_size = 0;
        vector<value_type> m_keys;

    private:

        void sortKeys(){
            sort(m_keys.begin(), m_keys.end());
        }

        Node* insert(value_type& key, LinkedValueType value){
            Node* pNode = m_avl.insert(key,value);
            m_keys.push_back(key);
            m_size++;
            return pNode;
        }


    public:
        
        CHash(){}

        size_t getSize(){return m_size;}

        value_type getKey(size_t index){return m_keys[index];}

        iterator begin() { sortKeys(); iterator iter(this, 0); return iter;   }
        iterator end()   { iterator iter(this, m_size-1); return iter;   }
        riterator rbegin() { sortKeys(); riterator iter(this, m_size-1); return iter;   }
        riterator rend()   { riterator iter(this, 0); return iter;   }
    
        LinkedValueType& operator[](value_type key){// created by Edson Cáceres
            Node* pNode = m_avl.search(key);//in order to avoid duplicated insertion
            if(!pNode){
                LinkedValueType any;
                pNode = insert(key,any);
            }
            return pNode->getValueRef();   
        }

        Node* search(value_type key){
            return m_avl.search(key);
        }

        template <typename F, typename... Args> //created by Edson Cáceres
        void print(F func, Args&&... args){
            foreach(begin(),end(), func, args...);
        }

        void read(istream &is){ //created by Edson Cáceres
      
            size_t file_size;
            value_type key;
            LinkedValueType value;
            is>>file_size;

            string line;
            getline(is, line); //to avoid 20
            
            while(getline(is, line) && m_size != file_size){ //keeping in mind the file_size
                
                stringstream ss(line);
                string word;

                ss >> key;
                ss >> word;
                ss >> value;
                insert(key,value);
            }
        }

};

template <typename T>//created by Edson Cáceres
ostream &operator<<(ostream &os, CHash<T> &obj){
  using Node = typename T::Node;
  obj.print(
    [&os](Node& node, ostream &os_){
      os << '[' << node.getData() << "] = " << node.getValue() << "; " << endl;
    },
    os
  );
  return os;
}

template <typename T>//created by Edson Cáceres
istream & operator>>(istream &is, CHash<T> &obj){
  obj.read(is);
  return is;
}

#endif