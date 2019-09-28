#pragma once

#include <iostream>
#include <vector>

template <typename T = int> 
class Vector2
{
private:
    T m_x = 0;
    T m_y = 0;

public:
    Vector2();
    Vector2(T value);
    Vector2(T x, T y);
    ~Vector2();
        
    bool operator==(Vector2<T> other);
    bool operator!=(Vector2<T> other);
    Vector2<T> operator+(Vector2<T> other) const;
    Vector2<T> operator-(Vector2<T> other) const;
    static Vector2<T> getRandom(T maxX, T mTaxY);

    T getX() const;
    T getY() const;

    friend std::ostream& operator<<(std::ostream &out, const Vector2<T>& data) {
        out << '(' << data.getX() << ',' << data.getY() << ')';
        return out;
    }
};


template <typename T> 
Vector2<T>::Vector2() : m_x(0), m_y(0)
{
}

template <typename T> 
Vector2<T>::Vector2(T value) : m_x(value), m_y(value)
{
}

template <typename T> 
Vector2<T>::Vector2(T x, T y) : m_x(x), m_y(y)
{
}

template <typename T> 
Vector2<T> Vector2<T>::getRandom(T maxX, T maxY)
{
    return Vector2(rand() % maxX, rand() % maxY);
}

template <typename T> 
Vector2<T>::~Vector2()
{
}

template <typename T> 
bool Vector2<T>::operator==(Vector2<T> other)
{
    return m_x == other.m_x && other.m_y == m_y;
}

template <typename T> 
bool Vector2<T>::operator!=(Vector2<T> other)
{
    return !((*this) == other);
}

template <typename T> 
Vector2<T> Vector2<T>::operator+(Vector2<T> other) const
{
    return Vector2(m_x + other.m_x, other.m_y + m_y);
}

template <typename T> 
Vector2<T> Vector2<T>::operator-(Vector2<T> other) const
{
    return Vector2(m_x - other.m_x, m_y - other.m_y);
}

template <typename T> 
T Vector2<T>::getX() const
{
    return m_x;
}

template <typename T>
T Vector2<T>::getY() const
{
    return m_y;
}

void shuffle(int*, int);                    //> Shuffle an integer array inplace
int min(int, int);                          //> Return the min value between two integers
int sign(int);                              //> Returns -1 if the number is less than 0, otherwise returns 1
int* range(int start, int end, int step);   //> range-python like function
int randomRange(int low, int high);         //> Generates a random value inside the [low, high] range
int even(int val);                          //> Converts an integer to even
int odd(int val);                           //> Converts an integer to odd
void drawCircle(float x, float y, float r); //> Draws a circle using opengl

template <typename T = int> 
Vector2<T> normalizeCoords(const Vector2<T>& pos, //> Normalize coordinates with respect OpenGL
                           float h)
{
    Vector2<T> normPos = Vector2<T>(pos.getY(), 
                                    h -1 - pos.getX());
    return normPos;
}
