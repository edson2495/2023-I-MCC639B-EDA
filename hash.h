#ifndef __HASH_H__  
#define __HASH_H__
#include "avl.h"
#include "hash_iterators.h"

template <typename Traits>
class CHash{

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

    public:
        
        CHash(){}

        size_t getSize(){return m_size;}

        value_type getKey(size_t index){return m_keys[index];}

        iterator begin() { sortKeys(); iterator iter(this, 0); return iter;   }
        iterator end()   { iterator iter(this, m_size-1); return iter;   }
        riterator rbegin() { sortKeys(); riterator iter(this, m_size-1); return iter;   }
        riterator rend()   { riterator iter(this, 0); return iter;   }
    
        LinkedValueType& operator[](value_type key){//a
            Node* pNode = m_avl.search(key);//in order to avoid duplicated insertion
            if(!pNode){
                LinkedValueType any;
                pNode = m_avl.insert(key,any);
                m_keys.push_back(key);
                m_size++;
            }
            //cout<<m_avl<<endl;
            return pNode->getValueRef();   
        }

        Node* search(value_type key){
            return m_avl.search(key);
        }



};

#endif