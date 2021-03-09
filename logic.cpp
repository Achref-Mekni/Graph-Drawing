#include "framework.h"
#include "logic.h"
#include <time.h>

Node::Node(vec2 center)
{
    this->center = center;
    this->radius = 0.05f;
}

vec2 Node::getCenter() { return this->center; }

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

Graph::Graph()
{
    srand(time(0));
    nodes = {};
    vertices = {};
    edges = {};
    lines = {};

    for (int i = 0; i < 50; i++)
    {
        vec2 center{(float)(rand() % 200 - 100) / 100, (float)(rand() % 200 - 100) / 100};
        nodes.push_back(new Node(center));
    }
    for (Node *N : nodes)
    {
        vec2 center = N->getCenter();
        vertices.push_back(center.x);
        vertices.push_back(center.y);
    }
    for (int i = 0; i < 50; i++)
    {
        for (int j = i + 1; j < 50; j++)
        {
            if (rand() % 100 <= 5)
                edges.push_back(new Edge(nodes.at(i), nodes.at(j)));
        }
    }
    for (Edge *E : edges)
    {
        std::vector<float> coordinates= E->getCoordinates();
        lines.push_back(coordinates.at(0));
        lines.push_back(coordinates.at(1));
        lines.push_back(coordinates.at(2));
        lines.push_back(coordinates.at(3));
    }
}

void Graph::render()
{
}

std::vector<float> Graph::getVertices()
{
    return this->vertices;
};

std::vector<float> Graph::getLines()
{
    return this->lines;
};