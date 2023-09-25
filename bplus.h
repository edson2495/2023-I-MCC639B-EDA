#ifndef __Bplus_TREE_H__  
#define __Bplus_TREE_H__ 
#include "bplus_tree_iterator.h"

using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}



template <typename K, typename V>
class NodeBPlusTree : public KeyNode<K,V>{
    public:
        using value_type   = K;
        using LinkedValueType = V;
    
    private:

        using Parent = class KeyNode<K,V>;
        using Node = NodeBPlusTree<K,V>;
  
    public:
        
        Node* m_pNext = nullptr;//publicr

    public:

        NodeBPlusTree(){}

        NodeBPlusTree(value_type key,Node*  pNext = nullptr){
            Parent::m_key = key;
            m_pNext = pNext;
        }

        NodeBPlusTree(value_type key, LinkedValueType value, Node*  pNext = nullptr) : Parent(key,value){
            m_pNext = pNext;
        }

        void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
        Node     *getpNext()               {   return getpNextRef();   }
        Node    *&getpNextRef()            {   return m_pNext;   }

};

template <typename _K,
          typename _V,
          typename _CompareFn = std::less< NodeBPlusTree<_K, _V> & >>
struct BPlusTreeTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeBPlusTree<_K,_V>;
    using  CompareFn = _CompareFn;
};

template <typename Page>
class ChildNode{

    public:

        ChildNode* m_pNext = nullptr;

    private:
        
        Page* m_child = nullptr;

    public:

        ChildNode(){}

        ChildNode(Page* child, ChildNode*  pNext = nullptr){
            m_child = child;
            m_pNext = pNext;
        }

        void      setpNext(ChildNode *pNext)  {   m_pNext = pNext;  }
        ChildNode     *getpNext()               {   return getpNextRef();   }
        ChildNode    *&getpNextRef()            {   return m_pNext;   }

        Page* getpChild(){return m_child;}

};

template <typename Traits>
class BPlusPage{

    public:

        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
        using Node      = typename Traits::Node;
        using CompareFn = typename Traits::CompareFn;
        using ChildNode_ = ChildNode<BPlusPage>;
        using myself = BPlusPage<Traits>;

    private:

        size_t m_maxDegree;
        int m_type = 2; //1 : value, 2 : no value
        BPlusPage* m_pParent = nullptr;
        Node* m_pHead = nullptr;
        Node* m_pTail = nullptr;
        size_t m_size = 0;
        ChildNode_* m_pChildHead = nullptr;
        ChildNode_* m_pChildTail = nullptr;
        size_t m_child_size = 0;
        BPlusPage* m_pPrev = nullptr;
        BPlusPage* m_pNext = nullptr;
        CompareFn Compfn;

    public:

        BPlusPage(){}

        BPlusPage(size_t maxDegree, int type, BPlusPage* pPrev = nullptr, BPlusPage* pNext = nullptr) : m_maxDegree(maxDegree), m_type(type), m_pPrev(pPrev), m_pNext(pNext){}

    private:
        
        Node **findPrev(Node *&rpPrev, const value_type &elem, size_t &position){
            if(!rpPrev || Compfn(elem, rpPrev->getData()) )//Compfn : a es mayor/menor que b retorna true
                return &rpPrev; // Retorna la direccion del puntero que me apunta
            position++;
            return findPrev((Node *&)rpPrev->getpNextRef(), elem, position);
        }

        ChildNode_** getpChildNode(ChildNode_ *&rpPrev, size_t i,size_t pos){
            if(!rpPrev || i == pos)
                return &rpPrev;
            return getpChildNode((ChildNode_ *&)rpPrev->getpNextRef(),i+1,pos);
        }

        Node* CreateNode(value_type &key, Node* pNext=nullptr){
            return new Node(key,pNext);
        }

        Node* CreateNode(const value_type &key, const LinkedValueType value, Node* pNext=nullptr){
            return new Node(key,value,pNext);
        }

        Node* insert_forward(const value_type &key, const LinkedValueType value, size_t &position){
            Node **pParent = findPrev(key,position);
            if(m_type == 1){ //nd
                Node *pNew = CreateNode(key,value);
                ::CreateBridge(*pParent, pNew, &Node::m_pNext);
                if( !pNew->getpNext() )
                    m_pTail = pNew;
                m_size++;
            }
            //return pParent;
            return *pParent;
        }

    public:

        size_t getChildSize(){return m_child_size;}
        void setChildSize(size_t n){m_child_size = n;}
        size_t getSize(){return m_size;}
        void setSize(size_t n){m_size = n;}
        BPlusPage* getpPrev(){return m_pPrev;}
        void setpPrev(BPlusPage* pPrev){m_pPrev = pPrev;}
        BPlusPage* getpNext(){return m_pNext;}
        void setpNext(BPlusPage* pNext){m_pNext = pNext;}

        Node* getpHead(){return m_pHead;}
        void setpHead(Node* pHead){m_pHead = pHead;}
        Node* getpTail(){return m_pTail;}
        void setpTail(Node* pTail){m_pTail = pTail;}
        void sizePlus(size_t n){m_size+=n;}

        ChildNode_* getpChildHead(){return m_pChildHead;}
        void setpChildHead(ChildNode_* pChildHead){m_pChildHead = pChildHead;}
        ChildNode_* getpChildTail(){return m_pChildTail;}
        void setpChildTail(ChildNode_* pChildTail){m_pChildTail = pChildTail;}        

        void setpChildNode(size_t pos, ChildNode_* pChildNodeNew){
            ChildNode_** pChildNode = getpChildNode(pos);
            ChildNode_* pTemp = *pChildNode;
            if(pTemp == nullptr){m_child_size++;}
            *pChildNode = pChildNodeNew;
        }

        ChildNode_** getpChildNode(size_t pos){
            return getpChildNode(m_pChildHead,0,pos);
        }

        void pushChildNode(size_t n, ChildNode_* pNew){
            ChildNode_** pPrev = getpChildNode(n);
            ::CreateBridge(*pPrev, pNew, &ChildNode_::m_pNext);
            if( !pNew->getpNext() )
                m_pChildTail = pNew;
            m_child_size++;
        }

        BPlusPage* getParent(){return m_pParent;}
        void setParent(BPlusPage*& pParent){m_pParent = pParent;}

        Node **findPrev(const value_type &elem, size_t &position) {   return findPrev(m_pHead, elem,position);   }

        Node* getpNode(size_t pos){
            assert(m_size > 0);
            Node* pHeadTemp = m_pHead;
            size_t i = 0;
            while(pHeadTemp && i < pos){
                pHeadTemp = pHeadTemp->getpNext();
                i++;
            }
            return pHeadTemp;
        }

        size_t getMaxDegree(){return m_maxDegree;}
        size_t getType(){return m_type;}

        string writeKeys(){
            string answer = "";
            Node* pHeadTemp = m_pHead;
            while(pHeadTemp){
                answer.append(" ").append(to_string(pHeadTemp->getData()));
                pHeadTemp = pHeadTemp->getpNext();
            }
            return answer;
        }

        void writeKeysAndValues(ostream &os){
            Node* pHeadTemp = m_pHead;
            while(pHeadTemp){
                os<<" -> (k:"<<pHeadTemp->getData()<<",v:"<<pHeadTemp->getValue()<<")";
                pHeadTemp = pHeadTemp->getpNext();
            }
        }

        void printRoot(){
            Node* pHeadTemp = m_pHead;
            while(pHeadTemp){
                cout<<pHeadTemp->getData()<<" ";
                pHeadTemp = pHeadTemp->getpNext();
            }
            ChildNode_* pChildHeadTemp = m_pChildHead;
            while(pChildHeadTemp){
                pChildHeadTemp->getpChild()->printRoot();
                pChildHeadTemp = pChildHeadTemp->getpNext();
            }  
        }

        void printData(){
            Node* pHeadTemp = m_pHead;
            while(pHeadTemp){
                cout<<pHeadTemp->getData()<<" ";
                pHeadTemp = pHeadTemp->getpNext();
            }
            cout<<" -> ";
            if(m_type == 1){
                if(m_pNext != nullptr){m_pNext->printData();}                
            }          
        }

        bool isBalanced(){return m_size < m_maxDegree;}

        myself* insert(const value_type &key, const LinkedValueType value){
            size_t position = 0;
            insert_forward(key,value,position);
            if(m_type == 2){
                return (*getpChildNode(position))->getpChild()->insert(key,value);
            }
            return this;
        }

};

template <typename Traits> //created by Edson Cáceres
class CBPlus{

    public:

        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
        using Node      = typename Traits::Node;
        using BPlusPage_      = BPlusPage<Traits>;
        using ChildNode_ = ChildNode<BPlusPage_>;
        using myself = CBPlus<Traits>;
        using iterator = bplus_tree_iterator<myself,BPlusPage_>;

    private:

        size_t m_size = 0;
        size_t m_maxDegree;
        BPlusPage_* m_pRoot = nullptr;
        BPlusPage_* m_pData = nullptr;
        mutex mtx;

    private:
                
        BPlusPage_* balance(BPlusPage_*& pBplusPage){ //created by Edson Cáceres
            // cout<<"BALANCEANDO : "<<pBplusPage->print()<<endl;
            // if(pBplusPage->getParent()){cout<<"padre : "<<pBplusPage->getParent()->print()<<endl;}
            
            size_t maxDegree = pBplusPage->getMaxDegree();
            size_t type = pBplusPage->getType();
            int divNode = maxDegree / 2;
            
            Node* pNodeDivision = pBplusPage->getpNode(divNode);
            value_type key = pNodeDivision->getData();
            Node* pPrevNodeDivision = pBplusPage->getpNode(divNode - 1);
            BPlusPage_* BplusPage2 =  new BPlusPage_(maxDegree, type);
            pBplusPage->setSize(divNode);
            BplusPage2->setSize( (maxDegree % 2 == 0?divNode:divNode+1) );


            Node* pTailFirstPage = pPrevNodeDivision;//in case type = 2
            if(type == 2){pPrevNodeDivision = pPrevNodeDivision->getpNext();BplusPage2->setSize(BplusPage2->getSize()-1);}
            BplusPage2->setpHead(pPrevNodeDivision->getpNext());
            BplusPage2->setpTail(pBplusPage->getpTail());
            pTailFirstPage->setpNext(nullptr);
            pBplusPage->setpTail(pTailFirstPage);

            if(type == 2){

                int divChildNode = (maxDegree+1) / 2;
                //ChildNode_* pPrevChildNodeDivision = *(pBplusPage->getpChildNode(divChildNode - 1));//it's correct but I just modify in order to coincide with the example page
                ChildNode_* pPrevChildNodeDivision = *(pBplusPage->getpChildNode( (maxDegree % 2 == 0?divChildNode:divChildNode - 1) ));
                pBplusPage->setChildSize(divChildNode);
                BplusPage2->setChildSize( (maxDegree % 2 == 0?divChildNode+1:divChildNode) );
                BplusPage2->setpChildHead(pPrevChildNodeDivision->getpNext());
                BplusPage2->setpChildTail(pBplusPage->getpChildTail());
                pPrevChildNodeDivision->setpNext(nullptr);
                pBplusPage->setpChildTail(pPrevChildNodeDivision);

                ChildNode_* pTemp = BplusPage2->getpChildHead();
                int k = 0;
                while(pTemp){
                    pTemp->getpChild()->setParent(BplusPage2);
                    pTemp = pTemp->getpNext();
                }
            }
            

            
            size_t pos = 0;
            BPlusPage_* pParent = pBplusPage->getParent();
            pParent = (pParent?pParent:new BPlusPage_(maxDegree, 2));


            if(type == 1){
                BplusPage2->setpPrev(pBplusPage);//mb nɑːt necsy prev ˈpɔɪn.t̬ɚ
                BplusPage2->setpNext(pBplusPage->getpNext());
                pBplusPage->setpNext(BplusPage2);
            }

            pBplusPage->setParent(pParent);
            BplusPage2->setParent(pParent);
                       
            Node** pPrevParent = pParent->findPrev(key,pos);
            Node* pNew = new Node(key);
            ::CreateBridge(*pPrevParent, pNew, &Node::m_pNext);
            if( !pNew->getpNext() )
                pParent->setpTail(pNew);
            pParent->sizePlus(1);

            ChildNode_* pchild2 = new ChildNode_(BplusPage2);
            ChildNode_* pchild1 = new ChildNode_(pBplusPage,(*(pParent->getpChildNode(pos+1))));
            
            pParent->setpChildNode(pos,pchild1);
            pParent->pushChildNode(pos+1,pchild2);

            return pParent;
        }
 
        void writeAsTree(BPlusPage_* pPage, ostream &os, size_t level){ //created by Edson Cáceres
            if(pPage){
                BPlusPage_* pParent = pPage->getParent();
                int child_size = pPage->getChildSize();
                bool middle = child_size == 0;
                child_size--;
                while(child_size >= 0){
                    writeAsTree((*(pPage->getpChildNode(child_size)))->getpChild(),os,level+1);
                    if(pPage->getChildSize()/2 == child_size){os << string("  |  ") * level <<pPage->writeKeys()<<"(p:"<<(pParent?pParent->writeKeys():"Root")<<",s:"<<pPage->getSize()<<",cs:"<<pPage->getChildSize()<<")"<<endl;}
                    child_size--;
                }
                if(middle){os << string("  |  ") * level <<pPage->writeKeys()<<"(p:"<<(pParent?pParent->writeKeys():"Root")<<",s:"<<pPage->getSize()<<",cs:"<<pPage->getChildSize()<<")"<<endl;}
            }
        }

    public:

        CBPlus(){}

        CBPlus(
            size_t maxDegree,
            BPlusPage_* pRoot = nullptr,
            BPlusPage_* pData = nullptr) :
            m_maxDegree(maxDegree),
            m_pRoot(pRoot),
            m_pData(pData){
            if(pRoot == nullptr){
                m_pRoot = new BPlusPage_(m_maxDegree, 1);
                m_pData = m_pRoot;
            }
        }

        iterator begin() { iterator iter(this, m_pData);    return iter;    }
        iterator end()   { iterator iter(this, nullptr);    return iter;    }

        template <typename F, typename... Args> //created by Edson Cáceres
        void forech(F func, Args&&... args){
            foreach(begin(),end(), func,args...);
        }

        template <typename F, typename C, typename... Args> //created by Edson Cáceres
        void execFuncInConcurrencyControl(F&& func, C& object, Args&&... args){
            thread th(func,&object,args...);
            th.join();
        }

        void insert(const value_type &key, const LinkedValueType value){ //created by Edson Cáceres
            
            mtx.lock();
            BPlusPage_* pBPlusPage = m_pRoot->insert(key,value);
            m_size++;

            while(!pBPlusPage->isBalanced()){
                pBPlusPage = balance(pBPlusPage);
                //writeAsTree(cout);//
            }
            
            if(pBPlusPage->getParent() == nullptr){
                m_pRoot = pBPlusPage;
            }
            mtx.unlock();
            
        }

        void writeAsTree  (ostream &os)    {   writeAsTree  (m_pRoot, os, 0);  }

        void print(){
            cout<<"IMPRIMIENDO DATA"<<endl;
            m_pData->printData();
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

//TODO add operator<<
template <typename T> //created by Edson Cáceres
ostream &operator<<(ostream &os, CBPlus<T> &obj){
    using Node = typename T::Node;
    obj.writeAsTree(os);
    return os;
}

// TODO add operator>>
template <typename T>
istream & operator>>(istream &is, CBPlus<T> &obj){
    obj.read(is);
    return is;
}

#endif