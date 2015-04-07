#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

void mydisplay(); // 콜백 함수
void init(); // 초기화 함수
void mouseClick(GLint button, GLint state, GLint x, GLint y); // 마우스를 클릭했을 때의 이벤트 리스너

// 레이어 클래스
class Layer 
{
public:
	GLdouble *verptr; // 레이어 내부 프레임들을 나타내는 좌표값들을 가리키는 포인터
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
Layer* ly1 = new Layer(4);	// 첫번째 프레임
Layer* ly2 = new Layer(4);	// 두번째 프레임
Layer* ly3 = new Layer(4);	// 세번째 프레임
Layer* ly4 = new Layer(4);	// 네번째 프레임

// ly1 내부 프레임들의 좌표값이 될 숫자
GLdouble ly1vertices[] = { -10, 37, 0, -27, 37, 0, -10, 14, 0, -27, 14, 0, 
						  -10, 11.5, 0, -27, 11.5, 0, -10, -11.5, 0, -27, -11.5, 0,
						  -10, -14, 0, -27, -14, 0, -10, -37, 0, -27, -37, 0,
						   27, 37, 0, -7, 37, 0, 27, -37, 0, -7, -37, 0 };

// ly2 내부 프레임들의 좌표값이 될 숫자
GLdouble ly2vertices[] = { 27, 37, -10, -27, 37, -10, 27, 20.75, -10, -27, 20.75, -10,
						  27, 17.75, -10, -27, 17.75, -10, 27, 1.5, -10, -27, 1.5, -10,
						  27, -1.5, -10, -27, -1.5, -10, 27, -17.75, -10, -27, -17.75, -10,
						  27, -20.75, -10, -27, -20.75, -10, 27, -37, -10, -27, -37, -10 };

// ly3 내부 프레임들의 좌표값이 될 숫자
GLdouble ly3vertices[] = { -1.5, 37, -20, -27, 37, -20, -1.5, 1.5, -20, -27, 1.5, -20,
						   27, 37, -20, 1.5, 37, -20, 27, 1.5, -20, 1.5, 1.5, -20,
						   -1.5, -1.5, -20, -27, -1.5, -20, -1.5, -37, -20, -27, -37, -20,
						   27, -1.5, -20, 1.5, -1.5, -20, 27, -37, -20, 1.5, -37, -20 };

// ly4 내부 프레임들의 좌표값이 될 숫자
GLdouble ly4vertices[] = { -1.5, 37, -30, -27, 37, -30, -1.5, 19, -30, -27, 19, -30,
						  -1.5, 16, -30, -27, 16, -30, -1.5, -37, -30, -27, -37, -30,
						  27, 37, -30, 1.5, 37, -30, 27, -16, -30, 1.5, -16, -30,
						  27, -19, -30, 1.5, -19, -30, 27, -37, -30, 1.5, -37, -30 };

// ly1 내부 프레임들의 색상을 채울 값
GLdouble ly1colors[] = { 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 
						0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 
						0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 
						0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1 };

// ly2 내부 프레임들의 색상을 채울 값
GLdouble ly2colors[] = { 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 
						0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 
						0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 
						0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1 };

// ly3 내부 프레임들의 색상을 채울 값
GLdouble ly3colors[] = { 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 
						0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 
						0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 
						0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1 };

// ly4 내부 프레임들의 색상을 채울 값
GLdouble ly4colors[] = { 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 
						0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 
						0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 
						0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1 };

// glDrawElements() 사용 시 이용하는 index
GLubyte indices[] = { 0, 1, 2, 1, 3, 2, 
					  4, 5, 6, 5, 7, 6,
					  8, 9, 10, 9, 11, 10,
					  12, 13, 14, 13, 15, 14};

GLuint verticesBuffer;
GLuint indicesBuffer;
GLuint colorsBuffer;

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

	init();
	glutDisplayFunc(mydisplay);// 디스플레이 함수 등록
	glutMouseFunc(mouseClick); // 이벤트 리스너 등록
	glutMainLoop();

	return 0;
}

void init()
{
	/* camera의 intrinsic parameter를 등록하는데, 
	   이 값은 변하지 않는다. 
	   camera가 바라보는 방향으로 35~45의 범위만 보이므로
	   이 사이에 위치한 레이어만 보이게 된다. */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 0.75, 35, 45); 

	/* camera의 extrinsic parameter를 등록하는데,
	   카메라의 위치를 (0, 0, 40)으로 초기화시키고
	   마우스 왼쪽 버튼이 눌릴 때마다 카메라의 위치를
	   변화시킨다.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0); 

	// 레이어 내부 프레임 좌표값 넣기
	ly1 -> verptr = ly1vertices; 
	ly2 -> verptr = ly2vertices;
	ly3 -> verptr = ly3vertices;
	ly4 -> verptr = ly4vertices;	
}

void mydisplay() // 레이어(프레임) 그리기 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, ly1 -> verptr);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_DOUBLE, 0, ly1colors);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, ly2 -> verptr);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_DOUBLE, 0, ly2colors);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, ly3 -> verptr);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_DOUBLE, 0, ly3colors);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, ly4 -> verptr);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_DOUBLE, 0, ly4colors);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glFlush();
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