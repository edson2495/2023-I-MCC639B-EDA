#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include "demo.h"
//#include "array.h"
#include "array_iterator_without_inheritance.h"
#include "recorrer.h"
using namespace std;

void DemoSmartPointers(){
    CArray< TraitArrayIntInt > v2("Lucero"), *pX; //, *pV3 = new CArray("Luis");
    
    shared_ptr< CArray< TraitArrayFloatString > > pV3(new CArray< TraitArrayFloatString >("Luis")), pV4;
    pV4 = pV3;
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(i, i*i);
        pV3->insert(sqrt(i), string("**")+to_string(sqrt(i)+5)+string("**"));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << "Printing pV3 float -> string (greater)" << endl;
    cout << *pV3 ;
}

void DemoArray(){   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    
    CArray< TraitArrayIntInt > v1("Antonio"); 
    for(auto i = 0 ; i < 15 ; i++)
        v1.insert(i, i+5);   //  insert(&v1);

    cout << "Vector #2()" <<endl;
    CArray< TraitFloatLong > v2("Cristian Vera"), 
           *pV3 = new CArray< TraitFloatLong >("Guiomar ABC");
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(sqrt(i), i);
        pV3->insert(i, sqrt(i));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << "Printing V1 (TraitArrayIntInt)" << endl;
    cout << v1; // v1.print(cout);

    cout << "Printing V2 (TraitFloatLong)" << endl;
    ostream &tmp = cout << v2 << "More text" << endl;
    tmp << "Hola !!!" << endl;
    cout << &tmp << "..." << &cout <<endl;
    // cout << x << f << y << endl;

    cout << "Printing pv3 (TraitFloatLong)" << endl;
    pV3->print(cout);
    // (*pV3).print();     *pV3 is already an object
    // rA.print();          rA is also an object
    // pV3[0].print();      pV3 is also an array with just 1 element [0]
    // (*(pV3+0)).print();
    // (*(0+pV3)).print();
    // 0[pV3].print();
    // delete pV3;

    // Using an array with []
    for(auto i = 0 ; i < v2.size() ; i++)
        cout << "v2[" << i << "] = " << v2[i] << endl;
    ofstream of("test.txt", ios::out);
    of << v2 << endl; 
    cout << "DemoArray finished !" << endl;

    using TraitStringString = ArrayTrait<string, string  , std::less<NodeArray<string, string> &>>;
    CArray< TraitStringString > vx("Ernesto Cuadros");
    vx.insert("Ernesto", "Cuadros");
    vx.insert("Luis"   , "Tejada");
    vx.insert("Jorge"  , "Lozano");
    vx.insert("Edson"  , "Caceres");
    vx.insert("Franz"  , "Maguiña");
    vx.print(cout);
}

template <typename T, int N>
void increment(T &x)
{  x+= N; }

template <typename T>
void print(T &x)
{  cout << x << "  "; }

template <typename T>
class ClassX
{          int m_inc = 0;
    public:  ClassX(int n) : m_inc(n){}
    void operator()(T &n){  n += m_inc;     }
};

void DemoIterators(){
    CArray< TraitArrayIntInt > v1("Jorge");
    
    v1.insert(30, 40);
    v1.insert(18, 45);
    v1.insert(20, 35);
    v1.insert(7 , 64);
    v1.insert(12, 25);
    v1.insert(8 , 17);

    cout << v1 << endl;
    recorrer(v1.begin(), v1.end(), ::increment<TX, 7>);
    cout << v1 << endl;
    recorrer(v1, ::increment<TX, 4>);
    cout << v1 << endl;

    recorrer(v1, ::print<TX>);
    cout << endl;
    // Lambda function
    recorrer(v1, [](TX &n){ n *= 2; });
    recorrer(v1, ::print<TX>); cout << endl;
    ClassX<TX> ope(5);
    recorrer(v1, ope);
    recorrer(v1, ::print<TX>); cout << endl;
    recorrer(v1, ClassX<TX>(8) );
    recorrer(v1, ::print<TX>); cout << endl;
}

void DemoReverseIterators(){
    cout << "DEMO REVERSE ITERATORS FOR ARRAY AS A CONTAINER : " << endl;
    CArray< TraitArrayIntInt > v1("Edson Cáceres");
    v1.insert(30, 40);
    v1.insert(18, 45);
    v1.insert(20, 35);
    v1.insert(7 , 64);
    v1.insert(12, 25);
    v1.insert(8 , 17);

    cout << "Printing asc : " << endl;
    cout << v1 << endl;
    cout << "Printing desc : " << endl;
    recorrer(v1.rbegin(), v1.rend(), ::print<TX>);
}

void DemoIteratorsWithoutInheritance(){
    cout <<endl<< "DEMO ITERATORS WITHOUT INHERITANCE FOR ARRAY AS A CONTAINER : " <<endl<< endl;
    CArray< TraitArrayIntInt > v1("Edson Cáceres");
    v1.insert(30, 40);
    v1.insert(18, 45);
    v1.insert(20, 35);
    v1.insert(7 , 64);
    v1.insert(12, 25);
    v1.insert(8 , 17);

    cout << "Printing operator with << : " << endl;
    cout << v1 << endl;

    cout << "Printing with iterator : " << endl;
    recorrer(v1.begin(), v1.end(), ::print<TX>);
    cout<< endl;

    cout << "Printing with reverse iterator : " << endl;
    recorrer(v1.rbegin(), v1.rend(), ::print<TX>);
    cout<< endl;
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

