#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include "demo.h"
#include "array.h"
using namespace std;

void DemoSmartPointers(){
    CArray<int> v2("Lucero"), *pX; //, *pV3 = new CArray("Luis");
    shared_ptr< CArray<float> > pV3(new CArray<float>("Luis")), pV4;
    pV4 = pV3;
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(i);
        pV3->insert(sqrt(i));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
}

void DemoIfstream(){

    cout<<endl<<"------------------DEMO INPUT FILESTREAM----------------"<<endl<<endl;

    ifstream test("test.txt");
    CArray<float>  *ptrArray = new CArray<float>("Edson Cáceres");

    cout<<"----------------------------Reading from test.txt------------------------------"<<endl;
    test>>*ptrArray;
    cout<<"----------------------------Printing Array-------------------------------------"<<endl;
    cout<<*ptrArray;
    ptrArray->~CArray();//it doesn't destroy itself
    test.close();
}

void DemoArray(){   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    CArray<int> v1("Antonio"); 
    for(auto i = 0 ; i < 15 ; i++)
        v1.insert(i);   //  insert(&v1);

    cout << "Vector #2()" <<endl;
    CArray<float> v2("Cristian Vera"), *pV3 = new CArray<float>("Guiomar ABC");
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(sqrt(i));
        pV3->insert(i);
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << v1; // v1.print(cout);
    ostream &tmp = cout << v2 << "More text" << endl;
    tmp << "Hola !!!" << endl;
    cout << &tmp << "..." << &cout <<endl;
    // cout << x << f << y << endl;
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
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

