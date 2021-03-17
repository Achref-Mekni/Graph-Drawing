//=============================================================================================
// Mintaprogram: Z�ld h�romsz�g. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!!
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    :
// Neptun :
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"
#include <time.h>

class Camera
{
	vec2 wCenter;
	vec2 wSize;

public:
	Camera() : wCenter(0, 0), wSize(200, 200) {}
	mat4 V() { return TranslateMatrix(-wCenter); }
	mat4 P() { return ScaleMatrix(vec2(2 / wSize.x, 2 / wSize.y)); }
	mat4 Vinv() { return TranslateMatrix(wCenter); }
	mat4 Pinv() { return ScaleMatrix(vec2(wSize.x / 2, wSize.y / 2)); }
	void Zoom(float s) { wSize = wSize * s; }
	void Pan(vec2 t) { wCenter = wCenter + t; }
	vec2 getCenter() { return this->wCenter; }
};

class Node
{
private:
	vec2 center;
	float radius;

public:
	Node(vec2 center)
	{
		this->center = center;
		this->radius = 0.05f;
	}
	vec2 getCenter()
	{
		return this->center;
	}
};

class Edge
{
private:
	Node *N1;
	Node *N2;

public:
	Edge(Node *N1, Node *N2)
	{
		this->N1 = N1;
		this->N2 = N2;
	}
	std::vector<float> getCoordinates()
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
	void render()
	{
	}
};

class Graph
{
private:
	std::vector<Node *> nodes;
	std::vector<Edge *> edges;
	std::vector<float> vertices;
	std::vector<float> lines;

public:
	Graph()
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
			std::vector<float> coordinates = E->getCoordinates();
			lines.push_back(coordinates.at(0));
			lines.push_back(coordinates.at(1));
			lines.push_back(coordinates.at(2));
			lines.push_back(coordinates.at(3));
		}
	}
	void render()
	{
	}

	std::vector<float> getVertices()
	{
		return this->vertices;
	}
	std::vector<float> getLines()
	{
		return this->lines;
	}
};

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
bool yes = false;
double *rC = new double[50];
double *gC = new double[50];
double *bC = new double[50];
Camera camera;
Graph *graph = new Graph();
std::vector<float> vectorvertices = graph->getVertices();
std::vector<float> vectorlines = graph->getLines();
GPUProgram gpuProgram; // vertex and fragment shaders
unsigned int vao, tao; // virtual world on the GPU
float vertices[] = {
	-0.5f, -0.6f, 0.0f,
	0.5f, -0.6f, 0.0f,
	0.4f, 0.5f, 0.0f,
	0.5f, 0.6f, 0.0f,
	-0.5f, 0.6f, 0.0f,
	-0.4f, -0.5f, 0.0f

};

float colors[] = {
	1.0f, 0.0f, 0.0f, // red
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, // green
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f};

// Initialization, create an OpenGL context
void onInitialization()
{
	glEnable(GL_PROGRAM_POINT_SIZE_EXT);
	glPointSize(5);
	//Graph *graph = new Graph();
	//std::vector<float> vectorvertices = graph->getVertices();
	glViewport(0, 0, windowWidth, windowHeight);

	glGenVertexArrays(1, &vao); // get 1 vao id
	glBindVertexArray(vao);		// make it active

	unsigned int vbo;	   // vertex buffer object
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Geometry with 24 bytes (6 floats or 3 x 2 coordinates)
	float *vertices = new float[vectorvertices.size()];
	for (int i = 0; i < vectorvertices.size(); i++)
	{
		vertices[i] = vectorvertices.at(i);
		//printf("%f",vertices[i]);
	}

	glBufferData(GL_ARRAY_BUFFER,							 // Copy to GPU target
				 (vectorvertices.size()) * sizeof(vertices), // # bytes
				 vertices,									 // address
				 GL_STATIC_DRAW);							 // we do not change later

	glEnableVertexAttribArray(0);				 // AttribArray 0
	glVertexAttribPointer(0,					 // vbo -> AttribArray 0
						  2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
						  0, NULL);				 // stride, offset: tightly packed

	// create program for the GPU
	gpuProgram.create(vertexSource, fragmentSource, "outColor");

	glGenVertexArrays(1, &tao); // get 1 vao id
	glBindVertexArray(tao);		// make it active
	glGenBuffers(1, &vbo);		// Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Geometry with 24 bytes (6 floats or 3 x 2 coordinates)

	float *lines = new float[vectorlines.size()];

	for (int i = 0; i < vectorlines.size(); i++)
	{
		lines[i] = vectorlines.at(i);
	}

	glBufferData(GL_ARRAY_BUFFER,					   // Copy to GPU target
				 (vectorlines.size()) * sizeof(lines), // # bytes
				 lines,								   // address
				 GL_STATIC_DRAW);					   // we do not change later

	glEnableVertexAttribArray(0);				 // AttribArray 0
	glVertexAttribPointer(0,					 // vbo -> AttribArray 0
						  2, GL_FLOAT, GL_FALSE, // two floats/attrib, not fixed-point
						  0, NULL);				 // stride, offset: tightly packed

	// create program for the GPU
	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

// Window has become invalid: Redraw
void onDisplay()
{

	glClearColor(1, 2, 5, 6);	  // background color
	glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer
	int location = glGetUniformLocation(gpuProgram.getId(), "color");
	int xC = camera.getCenter().x;
	int yC = camera.getCenter().y;
	float MVPtransf[4][4] = {10, 0, 0, 0, // MVP matrix,
							 0, 10, 0, 0, // row-major!
							 0, 0, 10, 0,
							 xC, yC, 0, 10};

	int location3 = glGetUniformLocation(gpuProgram.getId(), "MVP"); // Get the GPU location of uniform variable MVP

	glUniformMatrix4fv(location3, 1, GL_TRUE, &MVPtransf[0][0]); // Load a 4x4 row-major float matrix to the specified location

	// Set color to (0, 1, 0) = green
	glBindVertexArray(vao); // Draw call
	if (yes == false)
	{
		for (int i = 0; i < 50; i++)
		{
			double r = ((double)rand() / (RAND_MAX));
			double g = ((double)rand() / (RAND_MAX));
			double b = ((double)rand() / (RAND_MAX));

			rC[i] = r;
			gC[i] = g;
			bC[i] = b;

			glUniform3f(location, r, g, b); // 3 floats

			glDrawArrays(GL_POINTS, i /*startIdx*/, 1 /*# Elements*/);
		}
		yes = true;
	}
	else
	{
		for (int i = 0; i < 50; i++)
		{

			glUniform3f(location, rC[i], gC[i], bC[i]); // 3 floats

			glDrawArrays(GL_POINTS, i /*startIdx*/, 1 /*# Elements*/);
		}
	}

	glUniform3f(location, 1.0f, 0.0f, 0.0f); // 3 floats

	glBindVertexArray(tao); // Draw call
	glDrawArrays(GL_LINES, 0 /*startIdx*/, 50 /*# Elements*/);
	glutSwapBuffers(); // exchange buffers for double buffering
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY)
{
	switch (key)
	{

	case 'z':
		camera.Pan(vec2(-1, 0));
		break;
	case 's':
		camera.Pan(vec2(+1, 0));
		break;
	case 'q':
		camera.Pan(vec2(0, 1));
		break;
	case 'd':
		camera.Pan(vec2(0, -1));
		break;
	}
	glutPostRedisplay(); // if d, invalidate display, i.e. redraw
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY)
{
}

//float oldx = 0;
//float oldy = 0;
bool clicked = false;
// Move mouse with key pressed
void onMouseMotion(int pX, int pY)
{ // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space

	float cX = 0.08 * (2.0f * pX / windowWidth - 1); // flip y axis
	float cY = 0.08 * (1.0f - 2.0f * pY / windowHeight);

	camera.Pan(vec2(-cX, -cY));
	printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
	//oldx = cX;
	//oldy = cY;
	if (clicked == true)
	{
		glTranslatef(cX, cY, 0);
		glutPostRedisplay();
	}

	// if d, invalidate display, i.e. redraw
	//glutPostRedisplay(); // if d, invalidate display, i.e. redraw
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY)
{ // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	//float cX = 0.05*(2.0f * pX / windowWidth - 1); // flip y axis
	//float cY = 0.05*(1.0f - 2.0f * pY / windowHeight);
	float cX = 0.08 * (2.0f * pX / windowWidth - 1); // flip y axis
	float cY = 0.08 * (1.0f - 2.0f * pY / windowHeight);
	char *buttonStat;
	switch (state)
	{
	case GLUT_DOWN:
		buttonStat = "pressed";
		//camera.Pan(vec2(-cX, -cY));
		//glutPassiveMotionFunc(onMouseMotion);
		clicked = true;

		break;
	case GLUT_UP:
		buttonStat = "released";
		clicked = false;
		break;
	}

	switch (button)
	{
	case GLUT_LEFT_BUTTON:

		//printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	case GLUT_MIDDLE_BUTTON:
		//printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	case GLUT_RIGHT_BUTTON:
		//printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	}
}

// Idle event indicating that some time elapsed: do animation here
void onIdle()
{
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
}
