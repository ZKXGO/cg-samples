#include<gl/freeglut.h>

void idle()
{
	// Nothing to do here
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); // ������� ��������� -- ��� ����
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush(); // ����������� ���������� ���� ��������: ���������� ���������������� :)

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("����� ������� OpenGL ���������");

	glClearColor(0.0, 1.0, 0.0, 0.0);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
