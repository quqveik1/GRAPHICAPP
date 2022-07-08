#pragma once


#include "TXLib.cpp"
#include "Double comparision.h"

struct Vector 
{
    double x;
    double y;

    Vector &operator = (const Vector &a1);
    explicit operator double ();



    Vector getNullVector() { return Vector { 0,0 }; };
    void print (const char *str = NULL);
};

void equal (Vector &a, const Vector &b);
inline Vector operator +  (const Vector &a, const Vector &b);
inline Vector& operator += (Vector &a, const Vector &b);
inline Vector& operator -= (Vector& a, const Vector& b);
inline Vector operator -  (const Vector &a, const Vector &b);
inline void lining ();
inline Vector operator *  (const Vector &a, const double b);
inline Vector operator *  (const Vector &a, const Vector &b);
inline Vector& operator *= (Vector &a, const Vector &b);
inline Vector operator /  (const Vector &a, double m);
inline Vector operator /  (const Vector &a, const Vector &b);
inline Vector operator / (const double a, const Vector &b);
       Vector operator ^ (const Vector &vector, int degree);
       bool   operator > (const Vector &a, const Vector &b);
       bool   operator > (const Vector &a, const int &b);
       bool   operator < (const Vector &a, const Vector &b);
       bool operator == (const Vector &a, const Vector &b);
       bool operator != (const Vector &a, const Vector &b); 
       bool operator == (const Vector &a, const double &b);
       bool operator != (const Vector &a, const double &b);


void Vector::print (const char *str /* = NULL*/)
{
    printf ("%s: {%lf, %lf}\n", str, x, y);
}

bool operator == (const Vector &a, const Vector &b)
{
    if (isEqual (a.x, b.x) && isEqual(a.y, b.y)) return true;
    return false;
}  

bool operator != (const Vector &a, const Vector &b)
{
    if ( (! isEqual(a.x, b.x) ) || (!isEqual(a.y, b.y))) return true;
    return false;
}

bool operator != (const Vector& a, const double& b)
{
    if ( (!isEqual(a.x, b)) || (!isEqual(a.y, b)) ) return true;
    return false;
} 

bool operator == (const Vector& a, const double& b)
{
    if (isEqual(a.x, b) && isEqual(a.x, b)) return true;
    return false;
}

bool operator > (const Vector &a, const int &b)
{                                   
    if (isBigger(a.x, (double)b))
    {
        if (isBigger(a.y, (double)b))
        {
            return true;
        }
    }
    return false;
}

inline Vector operator * (const Vector &a, const double b)
{
    Vector result = {};
    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}

inline Vector operator / (const double a, const Vector &b)
{
    Vector result = {};
    result.x = a / b.x;
    result.y = a / b.y;

    return result;
}

inline Vector operator + (const Vector &a, const Vector &b)
{
    Vector result = {};
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline Vector operator - (const Vector &a, const Vector &b)
{
    Vector result = {};
    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

inline Vector &operator += (Vector &a, const Vector &b)
{
   a.x += b.x;
   a.y += b.y;
   
   return a;
}

inline Vector& operator -= (Vector& a, const Vector& b)
{
    a.x -= b.x;
    a.y -= b.y;

    return a;
}

inline Vector operator * (const Vector &a, const Vector &b)
{
    Vector copyA  = a;
    return copyA *= b;
}

inline Vector &operator *= (Vector &a, const Vector &b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

inline Vector operator / (const Vector &a, double m)
{
    return {.x = a.x / m,
            .y = a.y / m
    };
} 

inline Vector operator / (const Vector &a, const Vector &b)
{
    return {.x = a.x / b.x,
            .y = a.y / b.y
    };
}

Vector operator ^ (const Vector &vector, int degree)    
{
    Vector result = {};
    for (int i = 1; i <= degree; i++)
    {
        //assert (0 <= i && i < BallMax);
        result.x = vector.x * vector.x;
        result.y = vector.y * vector.y;
    }

    return result;
};

void equal (Vector &a, const Vector &b)
{
    a.x = b.x;
    a.y = b.y;
}

Vector& Vector::operator = (const Vector &a1)
{
    this->x = a1.x;
    this->y = a1.y;

    return *this;
};


inline void lining ()
{
    printf ("////////////////////////////////////\n");
}

bool operator > (const Vector &a, const Vector &b)
{
    if (isBigger (a.x, b.x))
    {
        if (isBigger(a.y, b.y))
        {
            return true;
        }

        else return false;
    }
    else return false;
}

bool operator < (const Vector &a, const Vector &b)
{
    if (isSmaller (a.x, b.x))
    {
        if (isSmaller(a.y, b.y))
        {
            return true;
        }

        else return false;
    }
    else return false;
}

Vector::operator double ()
{
    return sqrt (x * x + y * y);
}
