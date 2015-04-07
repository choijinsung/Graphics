#include <gl/glew.h>
#include <gl/freeglut.h>
#include <time.h>
#include <iostream>

void onDisplay();		// display �ݹ��Լ�
void onResize(int width, int height);	// reshape �ݹ��Լ�
void onMouse(int btn, int state, int x, int y);	// mousefunc �ݹ��Լ�
void mouseWheel(int wheel, int direction, int x, int y);	// mousewheelfunc �ݹ��Լ�
void mouseDrag(int x, int y);	// motionfunc �ݹ��Լ�
void idle();

double fovy = 100.0;	// field of view (y��)
double aspect = 0.75;	// ����-���� ����

GLdouble vertices[][60] = { 
						  { -10, 37, 0, -27, 37, 0,  -27, 14, 0, -10, 14, 0,
						    -10, 11.5, 0, -27, 11.5, 0, -27, -11.5, 0,-10, -11.5, 0,
						    -10, -14, 0, -27, -14, 0, -27, -37, 0,-10, -37, 0,
						    27, 37, 0, -7, 37, 0,-7, -37, 0 ,27, -37, 0,
							30, 40, -0.1, -30, 40, -0.1, -30, -40, -0.1, 30, -40, -0.1 },

						  { 27, 37, -1, -27, 37, -1, -27, 20.75, -1, 27, 20.75, -1 ,
						    27, 17.75, -1, -27, 17.75, -1, -27, 1.5, -1, 27, 1.5, -1 ,
						    27, -1.5, -1, -27, -1.5, -1, -27, -17.75, -1, 27, -17.75, -1 , 
						    27, -20.75, -1, -27, -20.75, -1, -27, -37, -1, 27, -37, -1 ,
						    30, 40, -1.1, -30, 40, -1.1, -30, -40, -1.1, 30, -40, -1.1 },

						  { -1.5, 37, -2, -27, 37, -2,  -27, 1.5, -2,-1.5, 1.5, -2 , 
						    27, 37, -2, 1.5, 37, -2, 1.5, 1.5, -2, 27, 1.5, -2 ,
						    -1.5, -1.5, -2, -27, -1.5, -2,  -27, -37, -2, -1.5, -37, -2 , 
						    27, -1.5, -2, 1.5, -1.5, -2, 1.5, -37, -2, 27, -37, -2 ,
						    30, 40, -2.1, -30, 40, -2.1, -30, -40, -2.1, 30, -40, -2.1 },

                          { -1.5, 37, -3, -27, 37, -3, -27, 19, -3, -1.5, 19, -3 ,
						    -1.5, 16, -3, -27, 16, -3, -27, -37, -3, -1.5, -37, -3 ,
						    27, 37, -3, 1.5, 37, -3, 1.5, -16, -3, 27, -16, -3 , 
						    27, -19, -3, 1.5, -19, -3, 1.5, -37, -3, 27, -37, -3 ,
							30, 40, -3.1, -30, 40, -3.1, -30, -40, -3.1, 30, -40, -3.1 },
						  
						  { -10, 37, -4, -27, 37, -4,  -27, 14, -4, -10, 14, -4 },
						  { -10, 11.5, -4, -27, 11.5, -4, -27, -11.5, -4, -10, -11.5, -4 },
						  { -10, -14, -4, -27, -14, -4, -27, -37, -4, -10, -37, -4 },
						  { 27, 37, -4, -7, 37, -4,-7, -37, -4, 27, -37, -4 },
						  { 30, 40, -4.1, -30, 40, -4.1, -30, -40, -4.1, 30, -40, -4.1 },

						  { 27, 37, -5, -27, 37, -5, -27, 20.75, -5, 27, 20.75, -5 },
						  { 27, 17.75, -5, -27, 17.75, -5, -27, 1.5, -5, 27, 1.5, -5 },
						  { 27, -1.5, -5, -27, -1.5, -5, -27, -17.75, -5, 27, -17.75, -5 },
						  { 27, -20.75, -5, -27, -20.75, -5, -27, -37, -5, 27, -37, -5 },
						  { 30, 40, -5.1, -30, 40, -5.1, -30, -40, -5.1, 30, -40, -5.1 },

						  { -1.5, 37, -6, -27, 37, -6,  -27, 1.5, -6, -1.5, 1.5, -6 },
						  {  27, 37, -6, 1.5, 37, -6, 1.5, 1.5, -6, 27, 1.5, -6 },
						  { -1.5, -1.5, -6, -27, -1.5, -6,  -27, -37, -6, -1.5, -37, -6 },
						  { 27, -1.5, -6, 1.5, -1.5, -6, 1.5, -37, -6, 27, -37, -6 },
						  { 30, 40, -6.1, -30, 40, -6.1, -30, -40, -6.1, 30, -40, -6.1 },

						  { -1.5, 37, -7, -27, 37, -7, -27, 19, -7, -1.5, 19, -7 },
						  { -1.5, 16, -7, -27, 16, -7, -27, -37, -7, -1.5, -37, -7 },
						  { 27, 37, -7, 1.5, 37, -7, 1.5, -16, -7, 27, -16, -7 },
						  { 27, -19, -7, 1.5, -19, -7, 1.5, -37, -7, 27, -37, -7 },
						  { 30, 40, -7.1, -30, 40, -7.1, -30, -40, -7.1, 30, -40, -7.1 }
};

GLdouble colors[][80] = { 
						  { 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1 ,
						    0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1 ,
						    1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1 ,
						    0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1 ,
                            0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1 },

						  { 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1 ,
						    0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1 ,
						    0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1 , 
						    0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1 ,
							0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1 },	

						  { 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1 , 
						    0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1 ,
						    0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1 , 
						    0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1 ,
						    0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1 },

                          { 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1 ,
						    1, 1, 0.8, 1, 1, 1, 0.8, 1, 1, 1, 0.8, 1, 1, 1, 0.8, 1 ,
						    0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1 , 
						    0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1 ,
							0, 0.9, 0, 1, 0, 0.9, 0, 1, 0, 0.9, 0, 1, 0, 0.9, 0, 1 },
						  
						  { 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1, 0.3, 0.3, 0.6, 1 },
						  { 0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1, 0.2, 0, 0.4, 1 },
						  { 1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1, 1, 0.1, 0.4, 1 },
						  { 0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1, 0.3, 0, 0.3, 1 },
                          { 0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1, 0.5, 0.5, 0.8, 1 },

						  { 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1, 0.6, 0.4, 0.1, 1 },
						  { 0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1, 0.3, 0, 0.5, 1 },
						  { 0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1, 0.8, 0.2, 0.5, 1 }, 
						  { 0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1, 0.9, 0.4, 0.3, 1 },
						  { 0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1, 0.5, 0.1, 1, 1 },	

						  { 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1, 0.5, 1, 0.9, 1 }, 
						  { 0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1, 0.8, 0.3, 0.1, 1 },
						  { 0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1, 0.8, 0.3, 0.4, 1 }, 
						  { 0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1, 0.2, 0.4, 0.6, 1 },
						  { 0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1, 0.9, 0.4, 0.7, 1 },

						  { 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1, 0.5, 1, 0.3, 1 },
						  { 1, 1, 0.8, 1, 1, 1, 0.8, 1, 1, 1, 0.8, 1, 1, 1, 0.8, 1 },
						  { 0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1, 0.2, 0.9, 0.6, 1 }, 
						  { 0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1, 0.8, 0.7, 1, 1 },
						  { 0, 0.9, 0, 1, 0, 0.9, 0, 1, 0, 0.9, 0, 1, 0, 0.9, 0, 1 } 
};

GLubyte indices[][20] = {{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 },
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3} };

GLfloat tx[24] = {0};
GLfloat ty[24] = {0};
GLfloat tz[24] = {0};
GLfloat ang[24] = {0};

GLuint mVertexBuffer[24];	
GLuint mColorBuffer[24];
GLuint mIndexBuffer[24];

GLint count = 0;	// � �������� ���̴°�

bool is_rplaying = false;	// �������� ������ ��
bool is_lplaying = false;	// ������ ������ ��
	
float angle = 0;			// rotate�� �� ��ŭ�� ������ ȸ���ϴ°�
const float angle_start = 0;
const float angle_end = -180;

long animation_time = 1000; // milliseconds
long start_time = 0;	// ȸ�� ���� �ð�
long current_time = 0;	// ���� �ð�

GLint old_x = 0;		// �巡���� �� ���� x ��ǥ
GLint old_y = 0;		// �巡���� �� ���� y ��ǥ

bool valid = false;		// ���콺�� �����ִ°�

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutCreateWindow("Simple Animation");

	if(glewInit() != GLEW_OK) {
		std::cout << "GLEW init is failed" << std::endl;
	}

	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutMouseFunc(onMouse);
	glutMouseWheelFunc(mouseWheel);
	glutMotionFunc(mouseDrag);
	glutIdleFunc(idle);

	glutMainLoop();

}

void onDisplay()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if(is_rplaying) {	// ȭ���� �������� ������ ��

		current_time = clock();
		long elapse_time = current_time - start_time; // ��� �ð�

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(30, 0, 40,
					30, 0, 0,
					 0, 1, 0);

		if(elapse_time > animation_time) { // ��� �ð��� ������ �ð����� �����ٸ�
			is_rplaying = false;
			for(int i=0; i<24; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
			ang[0] = ang[1] = ang[2] = ang[3] = 0;
			
			switch(count) 
			{
			case 1:
				tz[0] = -3; tz[1] = 1; tz[2] = 1; tz[3] = 1;
				break;
			case 2:
				tz[0] = -2; tz[1] = -2; tz[2] = 2; tz[3] = 2;
				break;
			case 3:
				tz[0] = -1; tz[1] = -1; tz[2] = -1; tz[3] = 3;
				break;
			case 4:
				count = 0;
				tz[0] = 0; tz[1] = 0; tz[2] = 0; tz[3] = 0;
				break;
			}
		}
		else {
			float delta = elapse_time / (float)animation_time / 3;
			angle = (delta*angle_end);	// �帥 �ð���ŭ ������ ����
			
			for(int i=0; i<24; i++)
				tx[i] = 30;
			
			switch(count) {
			case 1:
				ang[0] = angle;
				break;
			case 2:
				ang[1] = angle;
				break;
			case 3:
				ang[2] = angle;
				break;
			case 4:
				ang[3] = angle;
				break;
			}
		}
	}
	else if(is_lplaying) {	// ȭ���� ������ ������ ��
	
		current_time = clock();
		long elapse_time = current_time - start_time;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-30, 0, 40,
			  -30, 0, 0,
			  0, 1, 0);

		if(elapse_time > animation_time) {
			for(int i=0; i<24; i++)
				ang[i] = angle_start;
			is_lplaying = false;
			for(int i=0; i<24; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}

			switch(count) 
			{
			case 2:
				tz[0] = -2; tz[1] = 0; tz[2] = 2; tz[3] = 0;
				break;
			case 1:
				tz[0] = -1; tz[1] = 1; tz[2] = -1; tz[3] = 1;
				break;
			case 0:
				tz[0] = 0; tz[1] = -2; tz[2] = 0; tz[3] = 2;
				break;
			case -1:
				count = 3;
				tz[0] = -3; tz[1] = -1; tz[2] = 1; tz[3] = 3;
				break;
			}
		}
		else {
			float delta = elapse_time / (float)animation_time / 3;
			angle = -(delta*angle_end);		// �帥 �ð���ŭ ���� ��ȭ(�������� ������ ���� �ݴ�)
			
			for(int i=0; i<24; i++)
				tx[i] = -30;
				
			switch(count) {
			case 2:
				ang[3] = angle;
				break;
			case 1:
				ang[2] = angle;
				break;
			case 0:
				ang[1] = angle;
				break;
			case -1:
				ang[0] = angle;
				break;
			}
		}
	}
	else {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 40,
			  0, 0, 0,
			  0, 1, 0);
	}
	
for(int i=0; i<24; i++) {
		GLuint* buffers = new GLuint[3];
		glGenBuffers(3, &buffers[0]);

		mVertexBuffer[i] = buffers[0];
		mColorBuffer[i] = buffers[1];
		mIndexBuffer[i] = buffers[2];

	
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors[i]), colors[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[i]), indices[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
		// Draw a cube
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[i]);
		glVertexPointer(3, GL_DOUBLE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer[i]);
		glColorPointer(4, GL_DOUBLE, 0, 0);

		glPushMatrix();
		glRotatef(ang[i], 0, 1, 0);	// y������ �������� ���� angle��ŭ ȸ��(translate ��)
		glTranslatef(tx[i], ty[i], tz[i]);	// rotate�ϱ� ���� �̵���ȯ �Ͼ
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[i]);
		glDrawElements(GL_QUADS, sizeof(indices[i]), GL_UNSIGNED_BYTE, 0);
		glPopMatrix();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glutSwapBuffers();
}

void onMouse(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP && x > 550) {
			++count;
			for(int i=0; i<4; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
			
			switch(count) {
			case 1:
				tz[0] = 0; tz[1] = 0; tz[2] = 0; tz[3] = 0;
				break;
			case 2:
				tz[0] = -3; tz[1] = 1; tz[2] = 1; tz[3] = 1;
				break;
			case 3:
				tz[0] = -2; tz[1] = -2; tz[2] = 2; tz[3] = 2;
				break;
			case 4:
				tz[0] = -1; tz[1] = -1; tz[2] = -1; tz[3] = 3;
				break;
			}
			start_time = clock();
			is_rplaying = true;
		}
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP && x < 50) {
			--count;
			
			for(int i=0; i<4; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
				
			switch(count) {
			case 2:
				tz[0] = -3; tz[1] = -1; tz[2] = 1; tz[3] = 3;
				break;
			case 1:
				tz[0] = -2; tz[1] = 0; tz[2] = 2; tz[3] = 0;
				break;
			case 0:
				tz[0] = -1; tz[1] = 1; tz[2] = -1; tz[3] = 1;
				break;
			case -1:
				tz[0] = 0; tz[1] = -2; tz[2] = 0; tz[3] = 2;
				break;
			}
			start_time = clock();
			is_lplaying = true;
		}
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {// �巡��
			old_x = x; 
			old_y = y;
			valid = true;
		}
}

void onResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, 35, 45);
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction < 0) // wheel ���� ��
		fovy += 5;	// ���
	else if (direction > 0)	// wheel �ø� ��
		fovy -= 5;	// Ȯ��
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, 35, 45);
}

void mouseDrag(int x, int y) {
  if (valid) {
	  int dx = (x - old_x)*0.2;	// ���� x��ǥ�� ���� x��ǥ�� ��ȭ��
	  int dy = (y - old_y)*0.25;// ���� y��ǥ�� ���� y��ǥ�� ��ȭ�� 
		  
	  old_x = x;	// ���� x��ǥ�� old_x�� ����
	  old_y = y;	// ���� y��ǥ�� old_y�� ����

	  switch(count) {
		case 0:
			tx[0] += dx;
			ty[0] -= dy;
			break;
		case 1:
			tx[1] += dx;
			ty[1] -= dy;
		break;
		case 2:
			tx[2] += dx;
			ty[2] -= dy;
			break;
		case 3:
			tx[3] += dx;
			ty[3] -= dy;
			break;
		}
  }
}