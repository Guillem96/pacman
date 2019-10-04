#pragma once

#include <iostream>
#include <vector>

class Map;

class Color 
{
    public:
        int r;
        int g;
        int b;

        Color(int r, int g, int b);

        void glColor() const;

        static const Color darkGray;
        static const Color yellowPacman;
        static const Color darkGreen;
        static const Color red;
        static const Color cyan;
        static const Color pink;
};

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
    
    static const Vector2<> right;
    static const Vector2<> left;
    static const Vector2<> up;
    static const Vector2<> down;

    bool operator==(Vector2<T> other);
    bool operator!=(Vector2<T> other);
    Vector2<T> operator+(Vector2<T> other) const;
    Vector2<T> operator-(Vector2<T> other) const;
    Vector2<T> operator*(int scalar) const;
    Vector2<T> operator/(int scalar) const;
    Vector2<T> operator*(float scalar) const;
    Vector2<T> operator/(float scalar) const;

    static Vector2<T> getRandom(T maxX, T mTaxY);

    T getX() const;
    T getY() const;

    friend std::ostream& operator<<(std::ostream &out, const Vector2<T>& data) {
        out << '(' << data.getX() << ',' << data.getY() << ')';
        return out;
    }
};

template <typename T> 
const Vector2<> Vector2<T>::right = Vector2<>(0, 1);

template <typename T> 
const Vector2<> Vector2<T>::left = Vector2<>(0, -1);

template <typename T> 
const Vector2<> Vector2<T>::up = Vector2<>(-1, 0);

template <typename T> 
const Vector2<> Vector2<T>::down = Vector2<>(1, 0);

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
    return Vector2<T>(m_x + other.m_x, other.m_y + m_y);
}

template <typename T> 
Vector2<T> Vector2<T>::operator-(Vector2<T> other) const
{
    return Vector2<T>(m_x - other.m_x, m_y - other.m_y);
}

template <typename T> 
Vector2<T> Vector2<T>::operator*(int scalar) const
{
    return Vector2<T>(m_x * scalar, m_y * scalar);
}

template <typename T> 
Vector2<T> Vector2<T>::operator/(int scalar) const
{
    return Vector2<T>(m_x / scalar, m_y / scalar);
}

template <typename T> 
Vector2<T> Vector2<T>::operator*(float scalar) const
{
    return Vector2<T>(m_x * scalar, m_y * scalar);
}

template <typename T> 
Vector2<T> Vector2<T>::operator/(float scalar) const
{
    return Vector2<T>(m_x / scalar, m_y / scalar);
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

void shuffle(int*, int);                //> Shuffle an integer array inplace
int min(int, int);                      //> Return the min value between two integers
int min(float, float);
int sign(int);                          //> Returns -1 if the number is less than 0, otherwise returns 1
int* range(int, int, int);              //> range-python like function
int randomRange(int, int);              //> Generates a random value inside the [low, high] range
int even(int);                          //> Converts an integer to even
int odd(int);                           //> Converts an integer to odd
void drawCircle(float, float, float);   //> Draws a circle using opengl
void drawCube(                          //> Draws a cube using opengl
    const Vector2<>& pos, float w,
    float h);
bool shouldChangeDirection(             //> True when agent is inside an intersection
    const Vector2<>&, const Map*);
Vector2<> getRandomDirection(           //> Get a random valid direction
    const Vector2<>&, const Map*);

template <typename T>
T randomChoice(std::vector<T> v)       //> Pick a random element from a vector
{
    return v[randomRange(0, v.size() - 1)];
}

template <typename T = int> 
Vector2<T> normalizeCoords(const Vector2<T>& pos, //> Normalize coordinates with respect OpenGL
                           float h)
{
    Vector2<T> normPos = Vector2<T>(pos.getY(), 
                                    h -1 - pos.getX());
    return normPos;
}

