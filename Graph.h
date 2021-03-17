#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Edge.h"

class Graph
{
private:
    std::vector<Node *> nodes;
    std::vector<Edge *> edges;
    std::vector<float> vertices;
    std::vector<float> lines;
    unsigned int vbo;
    unsigned int vao;
    GPUProgram gpuProgram;

public:
    Graph();

    std::vector<float> getVertices();
    std::vector<float> getLines();

    void prepare();
    void render();
};

#endif