#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>

template <typename T>
class NodeMatrix
{
public:
  using value_type   = T;
private:
  using myself      = NodeMatrix<T> ;
public:
    value_type       m_key;

public:
    NodeMatrix(value_type key) 
        : m_key(key) {}

    NodeMatrix(const myself& other) : 
        NodeMatrix(other.m_key) {}

    NodeMatrix(myself&& other) // Move constructor
        : m_key  (std::move(other.m_key)) {}
    NodeMatrix() {}

    NodeMatrix& operator=(const NodeMatrix& other) {//c
        if (this != &other) {
            m_key = other.m_key;
        }
        return *this;
    }

    value_type operator+=(const value_type value) {//c
        m_key += value;
        return m_key;
    }

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }

    constexpr operator value_type() const noexcept { // since C++14
        return m_key;
    }

    bool operator<(const myself& other) const { 
        return m_key < other.m_key;
    }
    // Error was here. Next line was missing
    bool operator>(const myself& other) const { 
        return m_key > other.m_key;
    }
};

template <typename _K>
struct MatrixTrait
{
    using  value_type   = _K;
    using  Node      = NodeMatrix<_K>;
    //using  CompareFn = _CompareFn;
};

using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using Node            = typename Traits::Node;
    using myself          = CMatrix<Traits>;
    //using iterator        = matrix_iterator<myself>;

    private:
        Node **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
public:
    CMatrix(size_t rows, size_t cols)
    {   create(rows, cols);
    }
    CMatrix(size_t rows, size_t cols, Node ** ppMatrix) : m_rows(rows), m_cols(cols), m_ppMatrix(ppMatrix) {
    }
    CMatrix(const myself &other) : CMatrix(other.getM_rowss,other.m_cols,other.m_ppMatrix) {
        
    }

    CMatrix(myself &&other) : m_rows(std::move(other.m_rows)), m_cols(std::move(other.m_cols)), m_ppMatrix(std::move(other.m_ppMatrix)){        
        
    }

    CMatrix& operator=(const CMatrix& other) {
        if (this != &other) {
            m_rows = other.m_rows;
            m_cols = other.m_cols;
            m_ppMatrix = other.m_ppMatrix;
        }
        return *this;
    }

    ~CMatrix(){
        destroy();
    }

    void create(size_t rows, size_t cols){
        destroy();
        m_rows = rows;
        m_cols = cols;
        m_ppMatrix = new Node *[m_rows]; //like in array.h
        for(auto i = 0 ; i < m_rows ; i++)
            m_ppMatrix[i] = new Node[m_cols];
            // *(res+i) = new TX[m_cols];
            // *(i+res) = new TX[m_cols];
            // i[res]   = new TX[m_cols];
        
    }
    
    void fill(value_type val){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = Node(val);
                //m_ppMatrix[y][x] = val;
                // *(m_ppMatrix+y)[x] = val;
                // *(*(m_ppMatrix+y)+x) = val;
                // *(y[m_ppMatrix]+x) = val;
                // x[y[m_ppMatrix]] = val;
    }

    void print(ostream &os){
        os << m_rows << " " << m_cols << endl;
        for(auto y = 0 ; y < m_rows ; y++){
            for(auto x = 0 ; x < m_cols ; x++)
                os << m_ppMatrix[y][x] << " ";
            os << endl;
        }
    }
    void destroy(){
        for(auto y = 0 ; y < m_rows ; y++)
            delete [] m_ppMatrix[y];
        delete [] m_ppMatrix;
        m_ppMatrix = nullptr;
        m_rows = m_cols = 0;
    }

    Node* operator[](size_t row)
    {
        if(row < m_rows){
            return m_ppMatrix[row]; //a
        }
        throw std::invalid_argument( "row can't be greater then m_rows." );
    }

    value_type &operator()(size_t row, size_t col)
    {
        if(row < m_rows && col < m_cols){
            return m_ppMatrix[row][col].getDataRef(); //b
        }
        throw std::invalid_argument( "row or col is too big." );
    }

    size_t getM_rowss()
    {
        return m_rows;
    }

    size_t getM_cols()
    {
        return m_cols;
    }

    myself operator*(myself &other){
        if(m_cols == other.getM_rowss()){
            myself answer(m_rows,other.getM_cols());
            myself &me = *this;
            for(auto row = 0; row < m_rows; row++){
                for(auto col = 0; col < other.getM_cols(); col++){
                    answer[row][col] = 0 ; //elimino valores proximos al 0
                    for(auto i = 0 ; i < m_cols ; i++){
                        //answer[row][col] =  answer[row][col].getData() + me[row][i].getData() * other[i][col].getData();
                        answer[row][col] +=  me[row][i] * other[i][col];
                    }
                }
            }
            cout << answer;
            return answer;
        }
        throw std::invalid_argument( "Cols from the first matrix and Rows from the second matrix have to be equals." );
    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif