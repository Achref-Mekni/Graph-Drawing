#ifndef LOGIC_H
#define LOGIC_H

class Node
{
private:
    vec2 center;
    float radius;

public:
    Node(vec2 center);
    vec2 getCenter();
};

class Edge
{
private:
    Node *N1;
    Node *N2;

public:
    Edge(Node *N1, Node *N2);
    std::vector<float> getCoordinates();
    void render();
};

class Graph
{
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<float> vertices;
    std::vector<float> lines;

public:
    Graph();
    void render();

    std::vector<float> getVertices();
    std::vector<float> getLines();
};

class Camera
{
};

#endif