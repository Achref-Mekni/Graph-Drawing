#ifndef EDGE_H
#define EDGE_H

#include "Node.h"

class Edge
{
private:
    Node *N1;
    Node *N2;

public:
    Edge(Node *N1, Node *N2);
    std::vector<float> getCoordinates();
    void prepare(unsigned int** vao,unsigned int** vbo);
    void render(unsigned int **vao,int location);
};

#endif