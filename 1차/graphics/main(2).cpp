#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

void initGraphics();
void mydisplay(); // �ݹ� �Լ�
void init(); // �ʱ�ȭ �Լ�
void onResize(int width, int height);
void idle();
void mouseClick(GLint button, GLint state, GLint x, GLint y); // ���콺�� Ŭ������ ���� �̺�Ʈ ������

// ���̾� Ŭ����
class Layer 
{
public:
	GLdouble *verptr; // ���̾� ���� �����ӵ��� ��Ÿ���� ��ǥ������ ����Ű�� ������
	GLdouble *colptr;
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
Layer** ldptr = new Layer*[4];
Layer* lptr1 = new Layer(4);	// ù��° ������
Layer* lptr2 = new Layer(4);	// �ι�° ������
Layer* lptr3 = new Layer(4);	// ����° ������
Layer* lptr4 = new Layer(4);	// �׹�° ������

// ly1 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble lptr1vertices[] = { -10, 37, 0, -27, 37, 0,  -27, 14, 0, -10, 14, 0,
						  -10, 11.5, 0, -27, 11.5, 0, -27, -11.5, 0,-10, -11.5, 0,
						  -10, -14, 0, -27, -14, 0, -27, -37, 0,-10, -37, 0, 
						   27, 37, 0, -7, 37, 0,-7, -37, 0 ,27, -37, 0 };

// ly2 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble lptr2vertices[] = { 27, 37, -10, -27, 37, -10, -27, 20.75, -10,27, 20.75, -10, 
						  27, 17.75, -10, -27, 17.75, -10, -27, 1.5, -10,27, 1.5, -10,
						  27, -1.5, -10, -27, -1.5, -10, -27, -17.75, -10, 27, -17.75, -10, 
						  27, -20.75, -10, -27, -20.75, -10, -27, -37, -10, 27, -37, -10 };

// ly3 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble lptr3vertices[] = { -1.5, 37, -20, -27, 37, -20,  -27, 1.5, -20,-1.5, 1.5, -20, 
						   27, 37, -20, 1.5, 37, -20, 1.5, 1.5, -20,27, 1.5, -20,
						   -1.5, -1.5, -20, -27, -1.5, -20,  -27, -37, -20,-1.5, -37, -20, 
						   27, -1.5, -20, 1.5, -1.5, -20, 1.5, -37, -20,27, -37, -20 };

// ly4 ���� �����ӵ��� ��ǥ���� �� ����
GLdouble lptr4vertices[] = { -1.5, 37, -30, -27, 37, -30, -27, 19, -30, -1.5, 19, -30,
						  -1.5, 16, -30, -27, 16, -30, -27, -37, -30,-1.5, -37, -30,
						  27, 37, -30, 1.5, 37, -30, 1.5, -16, -30,27, -16, -30, 
						  27, -19, -30, 1.5, -19, -30, 1.5, -37, -30, 27, -37, -30 };

// ly1 ���� �����ӵ��� ������ ä�� ��
GLdouble lptr1colors[] = { 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 0.9, 0.2, 0.9, 1, 
						0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 0.4, 0.2, 0.6, 1, 
						0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 0.7, 0.9, 1.0, 1, 
						0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1, 0.0, 0.8, 0.0, 1 };

// ly2 ���� �����ӵ��� ������ ä�� ��
GLdouble lptr2colors[] = { 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 0.3, 0.4, 0.1, 1, 
						0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 0.7, 0.5, 0.9, 1, 
						0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 0.5, 0.5, 0.1, 1, 
						0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1, 0.5, 0.5, 0.2, 1 };

// ly3 ���� �����ӵ��� ������ ä�� ��
GLdouble lptr3colors[] = { 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 0.4, 0.8, 0.3, 1, 
						0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 0.4, 1.0, 0.4, 1, 
						0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 0.1, 0.2, 0.5, 1, 
						0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1, 0.8, 0.4, 0.4, 1 };

// ly4 ���� �����ӵ��� ������ ä�� ��
GLdouble lptr4colors[] = { 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 0.3, 0.7, 0.9, 1, 
						0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 0.1, 0.4, 1.0, 1, 
						0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 0.6, 0.6, 0.2, 1, 
						0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1, 0.3, 0.8, 0.5, 1 };


// glDrawElements() ��� �� �̿��ϴ� index
GLubyte indices[] = { 0, 1, 2, 3, 
					  4, 5, 6, 7,
					  8, 9, 10, 11,
					  12, 13, 14, 15 };

GLuint verticesBuffer[4];
GLuint colorsBuffer[4];
GLuint indicesBuffer[4];

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

	ldptr[0] = lptr1;
	ldptr[1] = lptr2;
	ldptr[2] = lptr3;
	ldptr[3] = lptr4;
	
	init();
	initGraphics();
	glutDisplayFunc(mydisplay);// ���÷��� �Լ� ���
	glutReshapeFunc(onResize);
	glutMouseFunc(mouseClick); // �̺�Ʈ ������ ���
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
	/* camera�� extrinsic parameter�� ����ϴµ�,
	   ī�޶��� ��ġ�� (0, 0, 40)���� �ʱ�ȭ��Ű��
	   ���콺 ���� ��ư�� ���� ������ ī�޶��� ��ġ��
	   ��ȭ��Ų��.
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0); 

	// ���̾� ���� ������ ��ǥ�� �ֱ�
	ldptr[0] -> verptr = lptr1vertices; 
	ldptr[1] -> verptr = lptr2vertices;
	ldptr[2] -> verptr = lptr3vertices;
	ldptr[3] -> verptr = lptr4vertices;

	ldptr[0] -> colptr = lptr1colors; 
	ldptr[1] -> colptr = lptr2colors;
	ldptr[2] -> colptr = lptr3colors;
	ldptr[3] -> colptr = lptr4colors;
}

void mydisplay() // ���̾�(������) �׸��� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* camera�� intrinsic parameter�� ����ϴµ�, 
	   �� ���� ������ �ʴ´�. 
	   camera�� �ٶ󺸴� �������� 35~45�� ������ ���̹Ƿ�
	   �� ���̿� ��ġ�� ���̾ ���̰� �ȴ�. */
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
		case 1: case 2: case 3: // �ι�°, ����°, �׹�° ���̾ ���� ����
			glTranslatef(0, 0, 10); // ���̾���� ������ 10��ŭ ����
			break;
		case 0:					// ù��° ���̾ ���� ����
			glTranslatef(0, 0, -30); // ���̾���� �ڷ� 30��ŭ ����(����ġ)
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