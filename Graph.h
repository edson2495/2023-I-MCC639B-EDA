#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <cassert>
#include "types.h"

using namespace std;

template <typename _K, typename _V>
struct VertexTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Vertex      = Vertex<_K,_V>;
    // using  CompareFn = _CompareFn;
};

template <typename K, typename V>
class Edge{
    public:

        using value_type   = K;
        using LinkedValueType = V;
        using Parent = class KeyNode<K,V>;

    private:

        value_type m_key;
        weight_type m_weight;

    public:

        Edge(){}

        Edge(value_type key, weight_type weight){
            m_key = key;
            m_weight = weight;
        }

};

template <typename K, typename V>
class Vertex : public KeyNode<K,V>{
    public:

        using value_type   = K;
        using LinkedValueType = V;
        using Parent = class KeyNode<K,V>;

    private:

        using Node = Vertex<K,V>;

        size_t m_size = 0;
        value_type m_key;
        LinkedValueType value;
        map<size_t, Edge>  m_edges;//seria un map de size_t y Edge, ya que, este ultimo tiene un peso
        
        //map<keyType, Edge> ? what happen when there is no the key?-----------------this could be an idea


  public:

    // Vertex(value_type key,){ // θɪŋk ðɪs ɪz juːst baɪ kəmˈper
    //     Parent::m_key = key;
    // }

    Vertex(){}

    Vertex(value_type key, LinkedValueType value) : Parent(key,value){
    }

    void insertEdge(const value_type& key, const weight_type& weight){
        m_edges[m_size++] = Edge(key,weight);
    }

};

template <typename Traits> //created by Edson Cáceres
class CGraph{
public:
    using value_type      = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Vertex          = typename Traits::Vertex;
    // using CompareFn       = typename Traits::CompareFn;
    // using myself          = CGraph<Traits>;
private:
    
    string    m_name = "Edson Greig";
    map<value_type, Vertex> m_vertexes;


    // TODO definir colección de nodes
    // map<string, NodeType>

    // map<KeyType, NodeType>
    // map<string, NodeType>
    /*
    donde se guardan los vertices seria un map con KeyType como llave y Vertex como dato
    vertex seria un objeto que contiene Edges (map int - Edge)
    */

public:
    CGraph(string name)  : m_name(name){ destroy();  }
    CGraph()                           { destroy();  }
    virtual ~CGraph(){
        cerr << "Destroying " << m_name << "..." << endl;
        reset();
    }
    void destroy(){
        
    }

    // CGraph operator=(CGraph &other){
    //     destroy();
        
    // }
    
    // TODO: Replantear
    void insert(const value_type &key, LinkedValueType value){
    }

    void print        (ostream &os){
        
    }
    void read(istream &is){
        
    }

    size_t size()
    {  return 0;    }

    // TODO : agregar el operator value_type &
    // value_type &operator[](size_t pos)
    // {   return m_heap[pos].getDataRef();    }
};

// bool operator<(const string & s1, const string & s2) {
//     return true;
// }

#endif
