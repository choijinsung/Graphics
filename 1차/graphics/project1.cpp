#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

void mydisplay(); // �ݹ� �Լ�
void init(); // �ʱ�ȭ �Լ�
void mouseClick(GLint button, GLint state, GLint x, GLint y); // ���콺�� Ŭ������ ���� �̺�Ʈ ������

// ���̾� Ŭ����
class Layer 
{
public:
	GLdouble *verptr; // ���̾� ���� �����ӵ��� ��Ÿ���� ��ǥ������ ����Ű�� ������
	int verNum;	// ���̾� �� ������ ���� 
	Layer(int n) 
	{
		verNum = n;
	}
	~Layer()
	{
		delete verptr;
	}
};

// ���̾�(������)���� ���������� ����
Layer* ly1 = new Layer(4);	// ù��° ������
Layer* ly2 = new Layer(4);	// �ι�° ������
Layer* ly3 = new Layer(4);	// ����° ������
Layer* ly4 = new Layer(4);	// �׹�° ������

// ly1 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble ly1vertices[] = { -10, 37, 0, -27, 37, 0, -10, 14, 0, -27, 14, 0, 
						  -10, 11.5, 0, -27, 11.5, 0, -10, -11.5, 0, -27, -11.5, 0,
						  -10, -14, 0, -27, -14, 0, -10, -37, 0, -27, -37, 0,
						   27, 37, 0, -7, 37, 0, 27, -37, 0, -7, -37, 0 };

// ly2 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble ly2vertices[] = { 27, 37, -10, -27, 37, -10, 27, 20.75, -10, -27, 20.75, -10,
						  27, 17.75, -10, -27, 17.75, -10, 27, 1.5, -10, -27, 1.5, -10,
						  27, -1.5, -10, -27, -1.5, -10, 27, -17.75, -10, -27, -17.75, -10,
						  27, -20.75, -10, -27, -20.75, -10, 27, -37, -10, -27, -37, -10 };

// ly3 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble ly3vertices[] = { -1.5, 37, -20, -27, 37, -20, -1.5, 1.5, -20, -27, 1.5, -20,
						   27, 37, -20, 1.5, 37, -20, 27, 1.5, -20, 1.5, 1.5, -20,
						   -1.5, -1.5, -20, -27, -1.5, -20, -1.5, -37, -20, -27, -37, -20,
						   27, -1.5, -20, 1.5, -1.5, -20, 27, -37, -20, 1.5, -37, -20 };

// ly4 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble ly4vertices[] = { -1.5, 37, -30, -27, 37, -30, -1.5, 19, -30, -27, 19, -30,
						  -1.5, 16, -30, -27, 16, -30, -1.5, -37, -30, -27, -37, -30,
						  27, 37, -30, 1.5, 37, -30, 27, -16, -30, 1.5, -16, -30,
						  27, -19, -30, 1.5, -19, -30, 27, -37, -30, 1.5, -37, -30 };

// ly1 ���� �����ӵ��� ������ ä�� ��
GLdouble ly1colors[] = { 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 
						0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 
						0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 
						0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1 };

// ly2 ���� �����ӵ��� ������ ä�� ��
GLdouble ly2colors[] = { 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 
						0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 
						0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 
						0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1 };

// ly3 ���� �����ӵ��� ������ ä�� ��
GLdouble ly3colors[] = { 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 
						0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 
						0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 
						0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1 };

// ly4 ���� �����ӵ��� ������ ä�� ��
GLdouble ly4colors[] = { 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 
						0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 
						0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 
						0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1 };

// glDrawElements() ��� �� �̿��ϴ� index
GLubyte indices[] = { 0, 1, 2, 1, 3, 2, 
					  4, 5, 6, 5, 7, 6,
					  8, 9, 10, 9, 11, 10,
					  12, 13, 14, 13, 15, 14};

GLuint verticesBuffer;
GLuint indicesBuffer;
GLuint colorsBuffer;

GLint count = 0; // ���콺 ���� ��ư Ŭ�� Ƚ��

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
	glutDisplayFunc(mydisplay);// ���÷��� �Լ� ���
	glutMouseFunc(mouseClick); // �̺�Ʈ ������ ���
	glutMainLoop();

	return 0;
}

void init()
{
	/* camera�� intrinsic parameter�� ����ϴµ�, 
	   �� ���� ������ �ʴ´�. 
	   camera�� �ٶ󺸴� �������� 35~45�� ������ ���̹Ƿ�
	   �� ���̿� ��ġ�� ���̾ ���̰� �ȴ�. */
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 0.75, 35, 45); 

	/* camera�� extrinsic parameter�� ����ϴµ�,
	   ī�޶��� ��ġ�� (0, 0, 40)���� �ʱ�ȭ��Ű��
	   ���콺 ���� ��ư�� ���� ������ ī�޶��� ��ġ��
	   ��ȭ��Ų��.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0); 

	// ���̾� ���� ������ ��ǥ�� �ֱ�
	ly1 -> verptr = ly1vertices; 
	ly2 -> verptr = ly2vertices;
	ly3 -> verptr = ly3vertices;
	ly4 -> verptr = ly4vertices;	
}

void mydisplay() // ���̾�(������) �׸��� 
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
		case 1: case 2: case 3: // �ι�°, ����°, �׹�° ���̾ ���� ����
			glTranslatef(0, 0, 10); // ���̾���� ������ 10��ŭ ����
			break;
		case 0:					// ù��° ���̾ ���� ����
			glTranslatef(0, 0, -30); // ���̾���� �ڷ� 30��ŭ ����(����ġ)
		}
	}
	glutPostRedisplay();
}