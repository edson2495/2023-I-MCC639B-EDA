#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <cassert>
#include <bits/stdc++.h>

#include "types.h"

using namespace std;

template <typename K>
class Edge{//created by Edson Cáceres
    public:

        using value_type   = K;

    private:

        value_type m_key;
        weight_type m_weight;

    public:

        Edge(){}

        Edge(value_type key, weight_type weight){
            m_key = key;
            m_weight = weight;
        }

        value_type getKey(){return m_key;}
        weight_type getWeight(){return m_weight;}

};

template <typename K, typename V>
class Vertex : public KeyNode<K,V>{//created by Edson Cáceres
    public:

        using value_type   = K;
        using LinkedValueType = V;
        using mysefl = Vertex<K,V>;
        using Parent = class KeyNode<K,V>;
        using iterator = typename map<value_type, Edge<K>>::iterator;

    private:

        using Node = Vertex<K,V>;

        size_t m_size = 0;
        value_type m_key;
        LinkedValueType value;
        map<value_type, Edge<K>>  m_edges;

    protected:

        bool isEdge(const value_type &key){//created by Edson Cáceres
            iterator it;
            it = m_edges.find(key);
            return (it != m_edges.end()?true:false);
        }

    public:

        Vertex(){}

        Vertex(const value_type& key, LinkedValueType value) : Parent(key,value){
        }

        map<value_type, Edge<K>> getEdges(){return m_edges;}//created by Edson Cáceres

        mysefl& addEdge(const value_type& key, const weight_type& weight){//created by Edson Cáceres
            if(!isEdge(key))
                m_size++;
            m_edges[key] = Edge(key,weight);
            return *this;
        }

};

template <typename _K, typename _V>
struct VertexTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Vertex_      = Vertex<_K,_V>;
};

template <typename Traits> //created by Edson Cáceres
class CGraph{

    public:

        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
        using Vertex_         = typename Traits::Vertex_;
        using iterator        = typename map<value_type, Vertex_>::iterator;

    private:
        
        string    m_name = "Edson Greig";
        map<value_type, Vertex_> m_vertexes;
        size_t m_size = 0;

    protected:

        bool isVertex(const value_type &key){
            iterator it;
            it = m_vertexes.find(key);
            return (it != m_vertexes.end()?true:false);
        }

    public:

        CGraph(){}
        CGraph(string name)  : m_name(name){}
        
        void addVertex(const value_type &key, LinkedValueType value){//created by Edson Cáceres
             if(!isVertex(key))
                m_size++;
            m_vertexes[key] = Vertex(key, value);
        }

        void addEdge(const value_type &keyOrigin, const value_type &keyDest, weight_type weight){//created by Edson Cáceres
            assert( isVertex(keyOrigin) && isVertex(keyDest) );
            m_vertexes[keyOrigin] = m_vertexes[keyOrigin].addEdge(keyDest,weight);
        }

        void print(ostream &os){//created by Edson Cáceres
            os<<"Vertex -> Edges"<<endl;
            os<<"key(value) -> key(weight), key(weight), ..."<<endl<<endl;

            for(auto vertex : m_vertexes){
                os<<vertex.first<<"("<<vertex.second.getValue()<<") -> ";
                for(auto edges : vertex.second.getEdges()){
                    os<<edges.second.getKey()<<"("<<edges.second.getWeight()<<"), ";
                }
                os<<endl;
            }
                

        }
        void read(istream &is){//created by Edson Cáceres
            
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
                addVertex(key,value);
            }

        }

        size_t size()
        {  return m_size;    }
};

template <typename T> //created by Edson Cáceres
ostream &operator<<(ostream &os, CGraph<T> &obj){
  obj.print(cout);
  return os;
}

template <typename T> //created by Edson Cáceres
istream & operator>>(istream &is, CGraph<T> &obj){
  obj.read(is);
  return is;
}

#endif
