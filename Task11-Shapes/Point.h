#ifndef POINT_H
#define POINT_H

class Point
{
    int x, y;

public:
    Point();
    Point(int x, int y);

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
};

#endif // POINT_H