#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

void initGraphics();
void mydisplay(); // 콜백 함수
void init(); // 초기화 함수
void onResize(int width, int height);
void idle();
void mouseClick(GLint button, GLint state, GLint x, GLint y); // 마우스를 클릭했을 때의 이벤트 리스너

// 레이어 클래스
class Layer 
{
public:
	GLdouble *verptr; // 레이어 내부 프레임들을 나타내는 좌표값들을 가리키는 포인터
	GLdouble *colptr;
	int verNum;	// 레이어 당 프레임 개수 
	Layer(int n) 
	{
		verNum = n;
	}
	~Layer()
	{
		delete verptr;
	}
};

// 레이어(포인터)들을 전역변수로 선언
Layer** ldptr = new Layer*[4];
Layer* lptr1 = new Layer(4);	// 첫번째 프레임
Layer* lptr2 = new Layer(4);	// 두번째 프레임
Layer* lptr3 = new Layer(4);	// 세번째 프레임
Layer* lptr4 = new Layer(4);	// 네번째 프레임

// ly1 내부 프레임들의 좌표값이 될 숫자
GLdouble lptr1vertices[] = { -10, 37, 0, -27, 37, 0,  -27, 14, 0, -10, 14, 0,
						  -10, 11.5, 0, -27, 11.5, 0, -27, -11.5, 0,-10, -11.5, 0,
						  -10, -14, 0, -27, -14, 0, -27, -37, 0,-10, -37, 0, 
						   27, 37, 0, -7, 37, 0,-7, -37, 0 ,27, -37, 0 };

// ly2 내부 프레임들의 좌표값이 될 숫자
GLdouble lptr2vertices[] = { 27, 37, -10, -27, 37, -10, -27, 20.75, -10,27, 20.75, -10, 
						  27, 17.75, -10, -27, 17.75, -10, -27, 1.5, -10,27, 1.5, -10,
						  27, -1.5, -10, -27, -1.5, -10, -27, -17.75, -10, 27, -17.75, -10, 
						  27, -20.75, -10, -27, -20.75, -10, -27, -37, -10, 27, -37, -10 };

// ly3 내부 프레임들의 좌표값이 될 숫자
GLdouble lptr3vertices[] = { -1.5, 37, -20, -27, 37, -20,  -27, 1.5, -20,-1.5, 1.5, -20, 
						   27, 37, -20, 1.5, 37, -20, 1.5, 1.5, -20,27, 1.5, -20,
						   -1.5, -1.5, -20, -27, -1.5, -20,  -27, -37, -20,-1.5, -37, -20, 
						   27, -1.5, -20, 1.5, -1.5, -20, 1.5, -37, -20,27, -37, -20 };

// ly4 내부 프레임들의 좌표값이 될 숫자
GLdouble lptr4vertices[] = { -1.5, 37, -30, -27, 37, -30, -27, 19, -30, -1.5, 19, -30,
						  -1.5, 16, -30, -27, 16, -30, -27, -37, -30,-1.5, -37, -30,
						  27, 37, -30, 1.5, 37, -30, 1.5, -16, -30,27, -16, -30, 
						  27, -19, -30, 1.5, -19, -30, 1.5, -37, -30, 27, -37, -30 };

// ly1 내부 프레임들의 색상을 채울 값
GLdouble lptr1colors[] = { 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 
						0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 
						0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 
						0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1 };

// ly2 내부 프레임들의 색상을 채울 값
GLdouble lptr2colors[] = { 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 
						0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 
						0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 
						0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1 };

// ly3 내부 프레임들의 색상을 채울 값
GLdouble lptr3colors[] = { 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 
						0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 
						0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 
						0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1 };

// ly4 내부 프레임들의 색상을 채울 값
GLdouble lptr4colors[] = { 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 
						0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 
						0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 
						0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1 };


// glDrawElements() 사용 시 이용하는 index
GLubyte indices[] = { 0, 1, 2, 3, 
					  4, 5, 6, 7,
					  8, 9, 10, 11,
					  12, 13, 14, 15 };

GLuint verticesBuffer[4];
GLuint colorsBuffer[4];
GLuint indicesBuffer[4];

GLint count = 0; // 마우스 왼쪽 버튼 클릭 횟수

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 800);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("simple");

	if( glewInit() != GLEW_OK ) {
		std::cout << "GLEW failed to init. :-(" << std::endl;
	}

	ldptr[0] = lptr1;
	ldptr[1] = lptr2;
	ldptr[2] = lptr3;
	ldptr[3] = lptr4;
	
	init();
	initGraphics();
	glutDisplayFunc(mydisplay);// 디스플레이 함수 등록
	glutReshapeFunc(onResize);
	glutMouseFunc(mouseClick); // 이벤트 리스너 등록
	glutIdleFunc(idle);

	glutMainLoop();

	for(int i=0; i<4; i++)
		delete ldptr[i];
	
	delete []ldptr;

	return 0;
}

void initGraphics() 
{
	GLuint* buffers = new GLuint[12];
	glGenBuffers(12, &buffers[0]);

	verticesBuffer[0] = buffers[0];
	colorsBuffer[0] = buffers[1];
	indicesBuffer[0] = buffers[2];
	verticesBuffer[1] = buffers[3];
	colorsBuffer[1] = buffers[4];
	indicesBuffer[1] = buffers[5];
	verticesBuffer[2] = buffers[6];
	colorsBuffer[2] = buffers[7];
	indicesBuffer[2] = buffers[8];
	verticesBuffer[3] = buffers[9];
	colorsBuffer[3] = buffers[10];
	indicesBuffer[3] = buffers[11];

	for(int i=0; i<4; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ldptr[i]->verptr), ldptr[i]->verptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ldptr[i]->colptr), ldptr[i]->colptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void init()
{
	/* camera의 extrinsic parameter를 등록하는데,
	   카메라의 위치를 (0, 0, 40)으로 초기화시키고
	   마우스 왼쪽 버튼이 눌릴 때마다 카메라의 위치를
	   변화시킨다.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0); 

	// 레이어 내부 프레임 좌표값 넣기
	ldptr[0] -> verptr = lptr1vertices; 
	ldptr[1] -> verptr = lptr2vertices;
	ldptr[2] -> verptr = lptr3vertices;
	ldptr[3] -> verptr = lptr4vertices;

	ldptr[0] -> colptr = lptr1colors; 
	ldptr[1] -> colptr = lptr2colors;
	ldptr[2] -> colptr = lptr3colors;
	ldptr[3] -> colptr = lptr4colors;
}

void mydisplay() // 레이어(프레임) 그리기 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* camera의 intrinsic parameter를 등록하는데, 
	   이 값은 변하지 않는다. 
	   camera가 바라보는 방향으로 35~45의 범위만 보이므로
	   이 사이에 위치한 레이어만 보이게 된다. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 0.75, 35, 45); 

	for(int i=0; i<1; i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer[i]);
		glColorPointer(4, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer[i]);
		glDrawElements(GL_QUADS, sizeof(indices), GL_UNSIGNED_BYTE, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	
	glutSwapBuffers();
}

void mouseClick(GLint button, GLint state, GLint x, GLint y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		++count;
		count %= 4;

		switch(count)
		{
		case 1: case 2: case 3: // 두번째, 세번째, 네번째 레이어가 보일 차례
			glTranslatef(0, 0, 10); // 레이어들이 앞으로 10만큼 전진
			break;
		case 0:					// 첫번째 레이어가 보일 차례
			glTranslatef(0, 0, -30); // 레이어들이 뒤로 30만큼 전진(원위치)
		}
	}
	glutPostRedisplay();
}

void onResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, width/(float)height, 35, 45);
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	glutPostRedisplay();
}