#include "framework.h"
#include "logic.h"
#include "Node.h"
#include <time.h>

Node::Node(vec2 center)
{
    this->center = center;
    this->colors = {randab(0, 1), randab(0, 1), randab(0, 1), // MVP matrix,
                    randab(0, 1), randab(0, 1), randab(0, 1), // row-major!
                    randab(0, 1), randab(0, 1), randab(0, 1)};
    this->radius = 0.05f;
}

vec2 Node::getCenter() { return this->center; }

void Node::prepare()
{
    this->prepareOne(0);
    this->prepareOne(1);
    this->prepareOne(2);
}

void Node::prepareOne(int p)
{
    glGenVertexArrays(1, &(this->vao[p]));
    glBindVertexArray(this->vao[p]);
    glGenBuffers(1, &(this->vbo[p]));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[p]);
    float point[8] = {center.x - 0.045f, center.y - 0.045f,
                      center.x - 0.045f, center.y + 0.045f,
                      center.x + 0.045f - 0.03f * p, center.y - 0.045f,
                      center.x + 0.045f - 0.03f * p, center.y + 0.045f

    };
    glBufferData(GL_ARRAY_BUFFER, // Copy to GPU target
                 sizeof(point),   // # bytes
                 point,           // address
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                     // vbo[0] -> AttribArray 0
                          2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                          0, NULL);              // stride, offset: tightly packed
}

void Node::renderOne(int location, int p)
{
    glUniform3f(location, colors.at(3*p), colors.at(3*p+1), colors.at(3*p+2));
    glBindVertexArray(this->vao[p]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0 /*startIdx*/, 4 /*# Elements*/);
}

void Node::render(int location)
{
    this->renderOne(location, 0);
    this->renderOne(location, 1);
    this->renderOne(location, 2);
}
