#pragma once

#include <iostream>
#include <vector>

class Vector2
{
private:
    int m_x = 0;
    int m_y = 0;

public:
    Vector2();
    Vector2(int value);
    Vector2(int x, int y);
    ~Vector2();
        
    bool operator==(Vector2 other);
    bool operator!=(Vector2 other);
    Vector2 operator+(Vector2 other) const;
    Vector2 operator-(Vector2 other) const;
    static Vector2 getRandom(int maxX, int maxY);

    int getX() const;
    int getY() const;

    friend std::ostream& operator<<(std::ostream &out, const Vector2& data) {
        out << '(' << data.getX() << ',' << data.getY() << ')';
        return out;
    }
};

void shuffle(int*, int);                    //> Shuffle an integer array inplace
int min(int, int);                          //> Return the min value between two integers
int sign(int);                              //> Returns -1 if the number is less than 0, otherwise returns 1
int* range(int start, int end, int step);   //> range-python like function
int randomRange(int low, int high);         //> Generates a random value inside the [low, high] range
int even(int val);                          //> Converts an integer to even
int odd(int val);                           //> Converts an integer to odd
