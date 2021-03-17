#include "framework.h"
#include "logic.h"
#include "Graph.h"
#include <time.h>

Graph::Graph()
{
    srand(time(0));
    nodes = {};
    vertices = {};
    edges = {};
    lines = {};
    for (int i = 0; i < 50; i++)
    {
        vec2 center{randab(-1, 1), randab(-1, 1)};
        nodes.push_back(new Node(center));
        vertices.push_back(center.x);
        vertices.push_back(center.y);
    }
    for (int i = 0; i < 50; i++)
    {
        for (int j = i + 1; j < 50; j++)
        {
            if (rand() % 100 < 5)
            {
                edges.push_back(new Edge(nodes.at(i), nodes.at(j)));
                std::vector<float> coordinates = edges.at(i)->getCoordinates();
                lines.push_back(coordinates.at(0));
                lines.push_back(coordinates.at(1));
                lines.push_back(coordinates.at(2));
                lines.push_back(coordinates.at(3));
            }
        }
    }
}

void Graph::prepare()
{
    // vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
    const char *const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
	layout(location = 0) in vec2 vp;	// Varying input: vp = vertex position is expected in attrib array 0

	void main() {
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
	}
)";

    // fragment shader in GLSL
    const char *const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers
	
	uniform vec3 color;		// uniform variable, the color of the primitive
	out vec4 outColor;		// computed color of the current pixel

	void main() {
		outColor = vec4(color, 1);	// computed color is the color of the primitive
	}
)";
    glEnable(GL_PROGRAM_POINT_SIZE_EXT);
    glPointSize(10);
    glViewport(0, 0, windowWidth, windowHeight);

    for (int i = 0; i < 50; i++)
    {
        nodes.at(i)->prepare();
    }
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);
    glGenBuffers(1, &(this->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, this->vao);
    float vertices[lines.size()];
    for (int i = 0; i < lines.size(); i++)
        vertices[i] = lines.at(i);
    glBufferData(GL_ARRAY_BUFFER,  // Copy to GPU target
                 sizeof(vertices), // # bytes
                 vertices,         // address
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                     // vbo -> AttribArray 0
                          2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
                          0, NULL);              // stride, offset: tightly packed
    gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

void Graph::render()
{
    glClearColor(0, 0, 0, 0);            // background color
    glClear(GL_COLOR_BUFFER_BIT);        // clear frame buffer
    float MVPtransf[4][4] = {1, 0, 0, 0, // MVP matrix,
                             0, 1, 0, 0, // row-major!
                             0, 0, 1, 0,
                             0, 0, 0, 1};

    int location = glGetUniformLocation(gpuProgram.getId(), "MVP");
    glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);

    location = glGetUniformLocation(gpuProgram.getId(), "color");
    glUniform3f(location, 1, 1, 1);
    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINES, 0 /*startIdx*/, lines.size() /*# Elements*/);
    for (int i = 0; i < 50; i++)
    {
        location = glGetUniformLocation(gpuProgram.getId(), "color");
        nodes.at(i)->render(location);
    }
    glutSwapBuffers();
}

std::vector<float> Graph::getVertices()
{
    return this->vertices;
};

std::vector<float> Graph::getLines()
{
    return this->lines;
};