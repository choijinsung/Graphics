#include <gl/glew.h>
#include <gl/glut.h>

#include <time.h>

#include <iostream>

void initGraphics();
void onDisplay();
void onResize(int width, int height);
void onMouse(int btn, int state, int x, int y);
void idle();

GLfloat vertices[] = {
	-1,  1,  0,    1,  1,  0,    1, -1,  0,   -1, -1,  0,
};

GLfloat colors[] = {
	1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,

};
GLubyte indices[] = {
	0, 3, 2,   2, 1, 0,
};

GLuint mVertexBuffer;
GLuint mColorBuffer;
GLuint mIndexBuffer;


bool is_playing = false;

float angle = 0;
const float angle_start = 0;
const float angle_end = -180;

long animation_time = 1000; // milliseconds
long start_time = 0;
long current_time = 0;
long prev_time = 0;

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Simple Animation");

	if(glewInit() != GLEW_OK) {
		std::cout << "GLEW init is failed" << std::endl;
	}

	initGraphics();
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutMouseFunc(onMouse);
	glutIdleFunc(idle);

	glutMainLoop();

}

void onDisplay()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5,
			  0, 0, 0,
			  0, 1, 0);

	if(is_playing) {
		current_time = clock();
		long elapse_time = current_time - start_time;

		if(elapse_time > animation_time) {
			angle = angle_start;
			is_playing = false;
		}
		else {
			float delta = elapse_time / (float)animation_time;
			angle = ((1.0f-delta)*angle_start + (delta)*angle_end);
		}
	}
	

	// Draw a cube
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glColorPointer(4, GL_FLOAT, 0, 0);

	glRotatef(angle, 0, 1, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers();
}

void onMouse(int btn, int state, int x, int y)
{
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			start_time = clock();
			is_playing = true;
		}
}

void onResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width/(float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	glutPostRedisplay();
}

void initGraphics()
{
	GLuint* buffers = new GLuint[3];
	glGenBuffers(3, &buffers[0]);

	mVertexBuffer = buffers[0];
	mColorBuffer = buffers[1];
	mIndexBuffer = buffers[2];

	// Create a vertex buffer & transfer vertices data 
	// from client space to server space.
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 4, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a color buffer & transfer color data 
	// from client space to server space.
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors) * 4, colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a index buffer & transfer index data 
	// from client space to server space.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	start_time = clock();
	angle = 0;