#include <gl/glew.h>
#include <gl/freeglut.h>
#include <string>
#include "SOIL/SOIL.h"
#include <time.h>
#include <iostream>

void onDisplay();		// display 콜백함수
void onResize(int width, int height);	// reshape 콜백함수
void onMouse(int btn, int state, int x, int y);	// mousefunc 콜백함수
void mouseWheel(int wheel, int direction, int x, int y);	// mousewheelfunc 콜백함수
void mouseDrag(int x, int y);	// motionfunc 콜백함수
int LoadGLTexture(); // 이미지를 load하는 함수
void idle();

double fovy = 100.0;	// field of view (y축)
double aspect = 0.75;	// 가로-세로 비율

GLdouble vertices[][12] = { 
						  { -10, 37, 0, -27, 37, 0,  -27, 14, 0, -10, 14, 0 },
						  { -10, 11.5, 0, -27, 11.5, 0, -27, -11.5, 0, -10, -11.5, 0 },
						  { -10, -14, 0, -27, -14, 0, -27, -37, 0, -10, -37, 0 },
						  { 27, 37, 0, -7, 37, 0,-7, -37, 0, 27, -37, 0 },
						  { 30, 40, -0.1, -30, 40, -0.1, -30, -40, -0.1, 30, -40, -0.1 },

						  { 27, 37, -0.2, -27, 37, -0.2, -27, 20.75, -0.2, 27, 20.75, -0.2 },
						  { 27, 17.75, -0.2, -27, 17.75, -0.2, -27, 1.5, -0.2, 27, 1.5, -0.2 },
						  { 27, -1.5, -0.2, -27, -1.5, -0.2, -27, -17.75, -0.2, 27, -17.75, -0.2 },
						  { 27, -20.75, -0.2, -27, -20.75, -0.2, -27, -37, -0.2, 27, -37, -0.2 },
						  { 30, 40, -0.3, -30, 40, -0.3, -30, -40, -0.3, 30, -40, -0.3 },

						  { -1.5, 37, -0.4, -27, 37, -0.4,  -27, 1.5, -0.4, -1.5, 1.5, -0.4 },
						  {  27, 37, -0.4, 1.5, 37, -0.4, 1.5, 1.5, -0.4, 27, 1.5, -0.4 },
						  { -1.5, -1.5, -0.4, -27, -1.5, -0.4,  -27, -37, -0.4, -1.5, -37, -0.4 },
						  { 27, -1.5, -0.4, 1.5, -1.5, -0.4, 1.5, -37, -0.4, 27, -37, -0.4 },
						  { 30, 40, -0.5, -30, 40, -0.5, -30, -40, -0.5, 30, -40, -0.5 },

						  { -1.5, 37, -0.6, -27, 37, -0.6, -27, 19, -0.6, -1.5, 19, -0.6 },
						  { -1.5, 16, -0.6, -27, 16, -0.6, -27, -37, -0.6, -1.5, -37, -0.6 },
						  { 27, 37, -0.6, 1.5, 37, -0.6, 1.5, -16, -0.6, 27, -16, -0.6 },
						  { 27, -19, -0.6, 1.5, -19, -0.6, 1.5, -37, -0.6, 27, -37, -0.6 },
						  { 30, 40, -0.7, -30, 40, -0.7, -30, -40, -0.7, 30, -40, -0.7 }
};

GLfloat texcoord[][8] = {
	{1, 1, 0, 1, 0, 0.5, 1, 0.5}, 
	{1, 0.5, 0, 0.5, 0, 0, 1, 0},  
	{1, 1, 0, 1, 0, 0.5, 1, 0.5}, 
	{1, 1, 0.5, 1, 0.5, 0, 1, 0}, 
	{1, 1, 0, 1, 0, 0, 1, 0},  
	
	{1, 1, 0, 1, 0, 0.75, 1, 0.75}, 
	{1, 0.75, 0, 0.75, 0, 0.5, 1, 0.5}, 
	{1, 0.5, 0, 0.5, 0, 0.25, 1, 0.25}, 
	{1, 0.25, 0, 0.25, 0, 0, 1, 0}, 
	{1, 1, 0, 1, 0, 0, 1, 0}, 
	
	{0.5, 1, 0, 1, 0, 0.5, 0.5, 0.5}, 
	{1, 1, 0.5, 1, 0.5, 0.5, 1, 0.5}, 
	{0.5, 0.5, 0, 0.5, 0, 0, 0.5, 0}, 
	{1, 0.5, 0.5, 0.5, 0.5, 0, 1, 0}, 
	{1, 1, 0, 1, 0, 0, 1, 0}, 
	
	{0.5, 1, 0, 1, 0, 0.5, 0.5, 0.5}, 
	{0.5, 0.5, 0, 0.5, 0, 0, 0.5, 0}, 
	{1, 1, 0.5, 1, 0.5, 0.5, 1, 0.5}, 
	{1, 0.5, 0.5, 0.5, 0.5, 0, 1, 0}, 
	{1, 1, 0, 1, 0, 0, 1, 0}, 
};

bool start_direct[20] = { 1, 1, 1, 1, 0,
						  1, 1, 1, 1, 0,
						  1, 1, 1, 1, 0,
						  1, 1, 1, 1, 0 }; // 각 이미지들이 플로팅하는 첫번째 방향

bool second_direct[20] = {0}; // 두번째 방향
bool third_direct[20] = {0};  // 세번째 방향
bool fourth_direct[20] = {0}; // 네번째 방향

GLubyte indices[][4] = {
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, 
{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3} };

GLfloat tx[20] = {0};	// x축 방향으로 translate하는 거리
GLfloat ty[20] = {0};	// y축 방향으로 translate하는 거리
GLfloat tz[20] = {0};	// z축 방향으로 translate하는 거리
GLfloat ang[20] = {0};	// y축을 기준으로 rotate하는 각도

GLuint mVertexBuffer[20];	
GLuint mTexcoodBuffer[20]; 
GLuint mIndexBuffer[20];

GLuint texture_id[20]; 

GLint count = 0;	// 어떤 프레임이 보이는가

bool is_rplaying = false;	// 오른쪽을 눌렀을 때
bool is_lplaying = false;	// 왼쪽을 눌렀을 때
	
float angle = 0;			// rotate할 때 얼만큼의 각도로 회전하는가
const float angle_start = 0;
const float angle_end = -180;

long animation_time = 1000; // milliseconds
long start_time = 0;	// 회전 시작 시간
long current_time = 0;	// 현재 시간

GLint old_x = 0;		// 드래그할 때 이전 x 좌표
GLint old_y = 0;		// 드래그할 때 이전 y 좌표

bool valid = false;		// 마우스가 눌려있는가

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutCreateWindow("Simple Animation");

	if(glewInit() != GLEW_OK) {
		std::cout << "GLEW init is failed" << std::endl;
	}

	LoadGLTexture();	// display하기 전 이미지를 load한다.
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
	glEnable(GL_CULL_FACE); 
	glEnable(GL_TEXTURE_2D); 

	/* texcoord의 값을 변화시켜서 frame 안에서 이미지가 떠다니게 한다. */

	// 첫번째 layer의 첫번째 frame
	if(start_direct[0]) {
		if(texcoord[0][5]<=0 || texcoord[0][7]<=0) {
			start_direct[0] = 0;
			second_direct[0] = 1;
		} else {
			texcoord[0][1] -= 0.0003;
			texcoord[0][3] -= 0.0003;
			texcoord[0][5] -= 0.0003;
			texcoord[0][7] -= 0.0003;
		}
	}
	else if(second_direct[0]) {
		if(texcoord[0][1]>=1 || texcoord[0][3]>=1) {
			start_direct[0] = 1;
			second_direct[0] = 0;
		} else {
			texcoord[0][1] += 0.0003;
			texcoord[0][3] += 0.0003;
			texcoord[0][5] += 0.0003;
			texcoord[0][7] += 0.0003;
		}
	}
	// 첫번째 layer의 두번째 frame
	if(start_direct[1]) {
		if(texcoord[1][1]>=1 || texcoord[1][3]>=1) {
			start_direct[1] = 0;
			second_direct[1] = 1;
		} else {
			texcoord[1][1] += 0.0003;
			texcoord[1][3] += 0.0003;
			texcoord[1][5] += 0.0003;
			texcoord[1][7] += 0.0003;
		}
	}
	else if(second_direct[1]) {
		if(texcoord[1][5]<=0 || texcoord[1][7]<=0) {
			start_direct[1] = 1;
			second_direct[1] = 0;
		} else {
			texcoord[1][1] -= 0.0003;
			texcoord[1][3] -= 0.0003;
			texcoord[1][5] -= 0.0003;
			texcoord[1][7] -= 0.0003;
		}
	}
	// 첫번째 layer의 세번째 frame
	if(start_direct[2]) {
		if(texcoord[2][5]<=0 || texcoord[2][7]<=0) {
			start_direct[2] = 0;
			second_direct[2] = 1;
		} else {
			texcoord[2][1] -= 0.0003;
			texcoord[2][3] -= 0.0003;
			texcoord[2][5] -= 0.0003;
			texcoord[2][7] -= 0.0003;
		}
	}
	else if(second_direct[2]) {
		if(texcoord[2][1]>=1 || texcoord[2][3]>=1) {
			start_direct[2] = 1;
			second_direct[2] = 0;
		} else {
			texcoord[2][1] += 0.0003;
			texcoord[2][3] += 0.0003;
			texcoord[2][5] += 0.0003;
			texcoord[2][7] += 0.0003;
		}
	}
	// 첫번째 layer의 네번째 frame
	if(start_direct[3]) {
		if(texcoord[3][2]<=0 || texcoord[3][4]<=0) {
			start_direct[3] = 0;
			second_direct[3] = 1;
		} else {
			texcoord[3][0] -= 0.0003;
			texcoord[3][2] -= 0.0003;
			texcoord[3][4] -= 0.0003;
			texcoord[3][6] -= 0.0003;
		}
	}
	else if(second_direct[3]) {
		if(texcoord[3][0]>=1 || texcoord[3][6]>=1) {
			start_direct[3] = 1;
			second_direct[3] = 0;
		} else {
			texcoord[3][0] += 0.0003;
			texcoord[3][2] += 0.0003;
			texcoord[3][4] += 0.0003;
			texcoord[3][6] += 0.0003;
		}
	}

	// 두번째 layer의 첫번째, 두번째 frame (이미지가 움직이는 방식이 동일)
	for(int i=5; i<=6; i++) {
		if(start_direct[i]) {
			if(texcoord[i][5]<=0 || texcoord[i][7]<=0) {
				start_direct[i] = 0;
				second_direct[i] = 1;
			} else {
				texcoord[i][1] -= 0.0003;
				texcoord[i][3] -= 0.0003;
				texcoord[i][5] -= 0.0003;
				texcoord[i][7] -= 0.0003;
			}
		}
		else if(second_direct[i]) {
			if(texcoord[i][1]>=1 || texcoord[i][3]>=1) {
				start_direct[i] = 1;
				second_direct[i] = 0;
			} else {
				texcoord[i][1] += 0.0003;
				texcoord[i][3] += 0.0003;
				texcoord[i][5] += 0.0003;
				texcoord[i][7] += 0.0003;
			}
		}
	}
	// 두번째 layer의 세번째, 네번째 frame (이미지가 움직이는 방식이 동일)
	for(int i=7; i<=8; i++) {
		if(start_direct[i]) {
			if(texcoord[i][1]>=1 || texcoord[i][3]>=1) {
				start_direct[i] = 0;
				second_direct[i] = 1;
			} else {
				texcoord[i][1] += 0.0003;
				texcoord[i][3] += 0.0003;
				texcoord[i][5] += 0.0003;
				texcoord[i][7] += 0.0003;
			}
		}
		else if(second_direct[i]) {
			if(texcoord[i][5]<=0 || texcoord[i][7]<=0) {
				start_direct[i] = 1;
				second_direct[i] = 0;
			} else {
				texcoord[i][1] -= 0.0003;
				texcoord[i][3] -= 0.0003;
				texcoord[i][5] -= 0.0003;
				texcoord[i][7] -= 0.0003;
			}
		}
	}
	
	// 세번째 layer의 모든 frame (이미지가 움직이는 방식이 동일)
	for(int i=10; i<14; i++) {
		if(start_direct[i]) {
			if(texcoord[i][0]>=1 || texcoord[i][6]>=1) {
				start_direct[i] = 0;
				second_direct[i] = 1;
			} else {
				texcoord[i][0] += 0.0003;
				texcoord[i][2] += 0.0003;
				texcoord[i][4] += 0.0003;
				texcoord[i][6] += 0.0003;
			}
		}
		else if(second_direct[i]) {
			if(texcoord[i][5]<=0 || texcoord[i][7]<=0) {
				second_direct[i] = 0;
				third_direct[i] = 1;
			} else {
				texcoord[i][1] -= 0.0003;
				texcoord[i][3] -= 0.0003;
				texcoord[i][5] -= 0.0003;
				texcoord[i][7] -= 0.0003;
			}
		}
		else if(third_direct[i]) {
			if(texcoord[i][2]<=0 || texcoord[i][4]<=0) {
				third_direct[i] = 0;
				fourth_direct[i] = 1;
			} else {
				texcoord[i][0] -= 0.0003;
				texcoord[i][2] -= 0.0003;
				texcoord[i][4] -= 0.0003;
				texcoord[i][6] -= 0.0003;
			}
		}
		else if(fourth_direct[i]) {
			if(texcoord[i][1]>=1 || texcoord[i][3]>=1) {
				fourth_direct[i] = 0;
				start_direct[i] = 1;
			} else {
				texcoord[i][1] += 0.0003;
				texcoord[i][3] += 0.0003;
				texcoord[i][5] += 0.0003;
				texcoord[i][7] += 0.0003;
			}
		}
	}

	// 네번째 layer의 모든 frame (이미지가 움직이는 방식이 동일)
	for(int i=15; i<19; i++) {
		if(start_direct[i]) {
			if(texcoord[i][6]>=1 || texcoord[i][7]<=0) {
				start_direct[i] = 0;
				second_direct[i] = 1;
			} else {
				texcoord[i][0] += 0.0003; texcoord[i][1] -= 0.0003;
				texcoord[i][2] += 0.0003; texcoord[i][3] -= 0.0003;
				texcoord[i][4] += 0.0003; texcoord[i][5] -= 0.0003;
				texcoord[i][6] += 0.0003; texcoord[i][7] -= 0.0003;
			}
		}
		else if(second_direct[i]) {
			if(texcoord[i][1]>=1 || texcoord[i][3]>=1) {
				second_direct[i] = 0;
				third_direct[i] = 1;
			} else {
				texcoord[i][1] += 0.0003;
				texcoord[i][3] += 0.0003;
				texcoord[i][5] += 0.0003;
				texcoord[i][7] += 0.0003;
			}
		}
		else if(third_direct[i]) {
			if(texcoord[i][4]<=0 || texcoord[i][5]<=0) {
				third_direct[i] = 0;
				fourth_direct[i] = 1;
			} else {
				texcoord[i][0] -= 0.0003; texcoord[i][1] -= 0.0003;
				texcoord[i][2] -= 0.0003; texcoord[i][3] -= 0.0003;
				texcoord[i][4] -= 0.0003; texcoord[i][5] -= 0.0003;
				texcoord[i][6] -= 0.0003; texcoord[i][7] -= 0.0003;
			}
		}
		else if(fourth_direct[i]) {
				if(texcoord[i][1]>=1 || texcoord[i][3]>=1) {
					fourth_direct[i] = 0;
				start_direct[i] = 1;
			} else {
				texcoord[i][1] += 0.0003;
				texcoord[i][3] += 0.0003;
				texcoord[i][5] += 0.0003;
				texcoord[i][7] += 0.0003;
			}
		}
	}
	
	if(is_rplaying) {	// 화면의 오른쪽이 눌렸을 때

		current_time = clock();
		long elapse_time = current_time - start_time; // 경과 시간

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(30, 0, 40,	
					30, 0, 0,
					 0, 1, 0);	// 카메라의 위치와 바라보는 방향을 x축 방향으로 30만큼 옮김

		if(elapse_time > animation_time) { // 경과 시간이 정해진 시간보다 지났다면
			is_rplaying = false;
			for(int i=0; i<20; i++) {
				tx[i] = 0;
				ty[i] = 0;
				ang[i] = 0;
			}
			
			if(count == 4)
				count = 0;
		}
		else {
			float delta = elapse_time / (float)animation_time / 3;
			angle = (delta*angle_end);	// 흐른 시간만큼 각도가 변함
			
			for(int i=0; i<20; i++)
				tx[i] = 30;
			
			switch(count) {
			case 1:
				for(int i=0; i<5; i++)
					ang[i] = angle;
				break;
			case 2:
				for(int i=5; i<10; i++)
					ang[i] = angle;
				break;
			case 3:
				for(int i=10; i<15; i++)
					ang[i] = angle;
				break;
			case 4:
				for(int i=15; i<20; i++)
					ang[i] = angle;
				break;
			}
		}
	}
	else if(is_lplaying) {	// 화면의 왼쪽이 눌렸을 때
	
		current_time = clock();
		long elapse_time = current_time - start_time;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-30, 0, 40,
			  -30, 0, 0,
			  0, 1, 0);	// 카메라의 위치와 바라보는 방향을 x축 방향으로 -30만큼 옮김

		if(elapse_time > animation_time) {
			for(int i=0; i<20; i++)
				ang[i] = angle_start;
			is_lplaying = false;
			for(int i=0; i<20; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
			
			if(count == -1)
				count = 3;
		}
		else {
			float delta = elapse_time / (float)animation_time / 3;
			angle = -(delta*angle_end);		// 흐른 시간만큼 각도 변화(오른쪽이 눌렸을 때와 반대)
			
			for(int i=0; i<20; i++)
				tx[i] = -30;
				
			switch(count) {
			case 2:
				for(int i=15; i<20; i++)
					ang[i] = angle;
				break;
			case 1:
				for(int i=10; i<15; i++)
					ang[i] = angle;
				break;
			case 0:
				for(int i=5; i<10; i++)
					ang[i] = angle;
				break;
			case -1:
				for(int i=0; i<5; i++)
					ang[i] = angle;
				break;
			}
		}
	}
	else {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 40,
			  0, 0, 0,
			  0, 1, 0);	// 카메라의 위치와 바라보는 방향을 x=0 으로 복귀시킴
	}
	
	for(int i=0; i<20; i++) {
		GLuint* buffers = new GLuint[3];
		glGenBuffers(3, &buffers[0]);

		mVertexBuffer[i] = buffers[0];
		mTexcoodBuffer[i] = buffers[1];
		mIndexBuffer[i] = buffers[2];

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, mTexcoodBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord[i]), texcoord[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[i]), indices[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, texture_id[i]); 

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[i]);
		glVertexPointer(3, GL_DOUBLE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, mTexcoodBuffer[i]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0); 

		glPushMatrix();
		glRotatef(ang[i], 0, 1, 0);	// y축으로 기준으로 각도 angle만큼 회전(translate 후)
		glTranslatef(tx[i], ty[i], tz[i]);	// rotate하기 전에 이동변환 일어남
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[i]);
		glDrawElements(GL_QUADS, sizeof(indices[i]), GL_UNSIGNED_BYTE, 0);
		glPopMatrix();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glutSwapBuffers();
}

void onMouse(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP && x > 550) {
			++count;
			for(int i=0; i<20; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
			
			switch(count) {
			case 4:
				for(int i=0; i<5; i++)
					tz[i] = 0;
				for(int j=5; j<10; j++)
					tz[j] = 0;
				for(int k=10; k<15; k++)
					tz[k] = 0;
				for(int l=15; l<20; l++)
					tz[l] = 0;
				break;
			case 1:
				for(int i=0; i<5; i++)
					tz[i] = -0.6;
				for(int j=5; j<10; j++)
					tz[j] = 0.2;
				for(int k=10; k<15; k++)
					tz[k] = 0.2;
				for(int l=15; l<20; l++)
					tz[l] = 0.2;
				break;
			case 2:
				for(int i=0; i<5; i++)
					tz[i] = -0.4;
				for(int j=5; j<10; j++)
					tz[j] = -0.4;
				for(int k=10; k<15; k++)
					tz[k] = 0.4;
				for(int l=15; l<20; l++)
					tz[l] = 0.4;
				break;
			case 3:
				for(int i=0; i<5; i++)
					tz[i] = -0.2;
				for(int j=5; j<10; j++)
					tz[j] = -0.2;
				for(int k=10; k<15; k++)
					tz[k] = -0.2;
				for(int l=15; l<20; l++)
					tz[l] = 0.6;
				break;
			}

			start_time = clock();
			is_rplaying = true;
		}
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP && x < 50) {
			--count;
			
			for(int i=0; i<20; i++) {
				tx[i] = 0;
				ty[i] = 0;
			}
				
			switch(count) {
			case -1:
				for(int i=0; i<5; i++)
					tz[i] = -0.6;
				for(int j=5; j<10; j++)
					tz[j] = -0.2;
				for(int k=10; k<15; k++)
					tz[k] = 0.2;
				for(int l=15; l<20; l++)
					tz[l] = 0.6;
				break;
			case 2:
				for(int i=0; i<5; i++)
					tz[i] = -0.4;
				for(int j=5; j<10; j++)
					tz[j] = 0;
				for(int k=10; k<15; k++)
					tz[k] = 0.4;
				for(int l=15; l<20; l++)
					tz[l] = 0;
				break;
			case 1:
				for(int i=0; i<5; i++)
					tz[i] = -0.2;
				for(int j=5; j<10; j++)
					tz[j] = 0.2;
				for(int k=10; k<15; k++)
					tz[k] = -0.2;
				for(int l=15; l<20; l++)
					tz[l] = 0.2;
				break;
			case 0:
				for(int i=0; i<5; i++)
					tz[i] = 0;
				for(int j=5; j<10; j++)
					tz[j] = -0.4;
				for(int k=10; k<15; k++)
					tz[k] = 0;
				for(int l=15; l<20; l++)
					tz[l] = 0.4;
				break;
			}
			start_time = clock();
			is_lplaying = true;
		}
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {// 드래그
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
	if (direction < 0) // wheel 내릴 때
		fovy += 5;	// 축소
	else if (direction > 0)	// wheel 올릴 때
		fovy -= 5;	// 확대
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, 35, 45);
}

int LoadGLTexture() // texture_id 배열에 이미지와 레이어 이미지(패턴 이미지)를 load
{
	texture_id[0] = SOIL_load_OGL_texture(
		"1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[1] = SOIL_load_OGL_texture(
		"2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[2] = SOIL_load_OGL_texture(
		"3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);
	
	texture_id[3] = SOIL_load_OGL_texture(
		"4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);
	
	texture_id[4] = SOIL_load_OGL_texture(
		"17.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[5] = SOIL_load_OGL_texture(
		"5.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[6] = SOIL_load_OGL_texture(
		"6.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[7] = SOIL_load_OGL_texture(
		"7.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[8] = SOIL_load_OGL_texture(
		"8.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[9] = SOIL_load_OGL_texture(
		"17.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[10] = SOIL_load_OGL_texture(
		"9.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[11] = SOIL_load_OGL_texture(
		"10.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[12] = SOIL_load_OGL_texture(
		"11.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[13] = SOIL_load_OGL_texture(
		"12.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[14] = SOIL_load_OGL_texture(
		"17.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[15] = SOIL_load_OGL_texture(
		"13.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[16] = SOIL_load_OGL_texture(
		"14.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[17] = SOIL_load_OGL_texture(
		"15.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[18] = SOIL_load_OGL_texture(
		"16.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	texture_id[19] = SOIL_load_OGL_texture(
		"17.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	for(int i=0; i<20; i++) {
		glBindTexture(GL_TEXTURE_2D, texture_id[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	return true;
}

void mouseDrag(int x, int y) {
  if (valid) {
	  float dx = (x - old_x)*0.2;	// 현재 x좌표와 이전 x좌표의 변화율
	  float dy = (y - old_y)*0.25;// 현재 y좌표와 이전 y좌표의 변화율 
		  
	  old_x = x;	// 현재 x좌표를 old_x에 저장
	  old_y = y;	// 현재 y좌표를 old_y에 저장

	  switch(count) {
		case 0:
			for(int i=0; i<5; i++) {
				tx[i] += dx;
				ty[i] -= dy;
			}
			break;
		case 1:
			for(int i=5; i<10; i++) {
				tx[i] += dx;
				ty[i] -= dy;
			}
			break;
		case 2:
			for(int i=10; i<15; i++) {
				tx[i] += dx;
				ty[i] -= dy;
			}
			break;
		case 3:
			for(int i=15; i<20; i++) {
				tx[i] += dx;
				ty[i] -= dy;
			}
			break;
		}
  }
}