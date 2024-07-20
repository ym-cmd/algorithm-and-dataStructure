#include <iostream>
#include <cmath>
using std::cout;
using std::endl;
class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
    }

    ~Point()
    {
    }

    void print() const
    {
        cout << "(" << _ix 
            << ", " << _iy 
            << ")" << endl;
    }

    double getDistance() const
    {
        return hypot(_ix, _iy);
    } 

    int getX() const
    {
        return _ix;
    }

    int getY() const
    {
        return _iy;

    }

    friend std::ostream & operator<<(std::ostream &os, const Point &rhs);
private:
    int _ix;
    int _iy;
};

std::ostream & operator<<(std::ostream &os,const Point &rhs)
{
    os << "(" << rhs._ix
       << ", " <<rhs._iy
       << ")";
    return os;
}

bool operator<(const Point& lhs, const Point& rhs) {
    if (lhs.getDistance() < rhs.getDistance()) return true;
    else if (lhs.getDistance() == rhs.getDistance()) {
        if (lhs.getX() < rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() < rhs.getY())) return true;
    }
    return false;
}

bool operator>(const Point& lhs, const Point& rhs) {
    if (lhs.getDistance() > rhs.getDistance()) return true;
    else if (lhs.getDistance() == rhs.getDistance()) {
        if (lhs.getX() > rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() > rhs.getY())) return true;
    }
    return false;
}

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}
bool operator!=(const Point& lhs, const Point& rhs) {
    return !(lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY());
}