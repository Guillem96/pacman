#pragma once

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
    Vector2 operator+(Vector2 other);
    Vector2 operator-(Vector2 other);
    static Vector2 getRandom(int maxX, int maxY);

    int getX() const;
    int getY() const;
};

