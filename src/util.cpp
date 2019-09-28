#include <GL/glut.h>
#include "util.h"

#include <stdlib.h>
#include <math.h>


void shuffle(int *array, int n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int sign(int x)
{
    if (x < 0)
        return -1;
    return 1;
}

int *range(int start, int end, int step)
{
    int size = end - start;
    size /= step;
    size += 1;

    int *res = new int[size];
    int it = 0;
    for (int i = start; i < end; i += step)
    {
        res[it] = i;
        it++;
    }
    return res;
}

int randomRange(int low, int high)
{
    return (rand() % (high - low + 1)) + low;
}

int even(int val)
{
    if (val % 2 == 1)
        val--;
    return val;
}

int odd(int val)
{
    if (val % 2 == 0)
        val--;
    return val;
}

void drawCircle(float x, float y, float r)
{
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.1415f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (r * cos(i * twicePi / triangleAmount)),
            y + (r * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}
