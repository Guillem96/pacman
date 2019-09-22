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

void shuffle(int*, int);
int min(int, int);
int sign(int);
int* range(int start, int end, int step);
int randomRange(int low, int high);
int even(int val);
int odd(int val);