#ifndef NODE_H
#define NODE_H

class Node
{
private:
    vec2 center;
    float radius;
    std::vector<float> colors;
    unsigned int vao[3];
    unsigned int vbo[3];

public:
    Node(vec2 center);
    vec2 getCenter();
    void prepare();
    void prepareOne(int p);
    void render(int location);
    void renderOne(int location, int p);
};

#endif