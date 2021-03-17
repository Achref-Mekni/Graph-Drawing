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
#include "Graph.h"
//#include "Camera.h"
#include "Node.h"
#include "Edge.h"

Graph *graph = new Graph();
Camera *camera = new Camera();
std::vector<float> vectorvertices = graph->getVertices();
std::vector<float> vectorlines = graph->getLines();

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

GPUProgram gpuProgram; // vertex and fragment shaders
unsigned int vao, tao; // virtual world on the GPU

// Initialization, create an OpenGL context
void onInitialization()
{
	graph->prepare();
}

// Window has become invalid: Redraw
void onDisplay()
{
	graph->render(camera->getCenter());
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY)
{
	// if (key == 'd')
	// 	glutPostRedisplay(); // if d, invalidate display, i.e. redraw
	switch (key)
	{

	case 'w':
		camera->slide(vec2(0, 1));
		break;
	case 's':
		camera->slide(vec2(0, -1));
		break;
	case 'a':
		camera->slide(vec2(-1, 0));
		break;
	case 'd':
		camera->slide(vec2(1, 0));
		break;
	case 'r':
		vec2 current = vec2(0, 0) - camera->getCenter();
		camera->slide(current);
		break;
	}
	glutPostRedisplay();
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY)
{
}

// Move mouse with key pressed
void onMouseMotion(int pX, int pY)
{ // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1; // flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;
	camera->slide(vec2(cX, cY));
	glutPostRedisplay();
	printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY)
{ // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1; // flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	char *buttonStat;
	switch (state)
	{
	case GLUT_DOWN:
		buttonStat = "pressed";
		break;
	case GLUT_UP:
		buttonStat = "released";
		break;
	}

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	case GLUT_MIDDLE_BUTTON:
		printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	case GLUT_RIGHT_BUTTON:
		printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);
		break;
	}
}

// Idle event indicating that some time elapsed: do animation here
void onIdle()
{
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
}
