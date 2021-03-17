#include "framework.h"
#include "logic.h"
#include "Edge.h"
#include <time.h>

Edge::Edge(Node *N1, Node *N2)
{
    this->N1 = N1;
    this->N2 = N2;
}

std::vector<float> Edge::getCoordinates()
{
    std::vector<float> temp = {};
    vec2 v1 = N1->getCenter();
    vec2 v2 = N2->getCenter();
    temp.push_back(v1.x);
    temp.push_back(v1.y);
    temp.push_back(v2.x);
    temp.push_back(v2.y);
    return temp;
}

void Edge::prepare(unsigned int **vao, unsigned int **vbo)
{
    glGenVertexArrays(1, (*vao));
    glBindVertexArray(**vao);
    glGenBuffers(1, (*vbo));
    glBindBuffer(GL_ARRAY_BUFFER, (**vbo));
    float line[4] = {this->N1->getCenter().x,
                     this->N1->getCenter().y,
                     this->N2->getCenter().x,
                     this->N2->getCenter().y};
    glBufferData(GL_ARRAY_BUFFER, // Copy to GPU target
                 4,               // # bytes
                 line,            // address
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                     // vbo -> AttribArray 0
                          2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                          0, NULL);              // stride, offset: tightly packed
}