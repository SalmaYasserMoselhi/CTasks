#include <iostream>
#include "Circle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Triangle.h"


using namespace std;

int main(){
    Circle c(5);
    cout << "Circle = " << c.calcArea() << endl;

    Rectangle r(2, 4);
    cout << "Rect = " << r.calcArea() << endl;

    Square s(5);    
    cout << "Square = " << s.calcArea() << endl;

    Triangle t(12, 4);
    cout << "Triangle = " << t.calcArea() << endl;


    return 0;
}