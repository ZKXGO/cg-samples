#include<GL/glew.h>
#include<GL/freeglut.h>

#include<iostream>
#include<fstream>
#include<string>

using namespace std;

#pragma pack(1)
struct Vertex
{
	float x, y, z;
};
#pragma pack()

Vertex cube_vertices[] = {
	{ -0.5f, -0.5f, -0.5f },
	{ -0.5f, 0.5f, -0.5f },
	{ 0.5f, 0.5f, -0.5f },
	{ 0.5f, -0.5f, -0.5f },
	{ -0.5f, -0.5f, 0.5f },
	{ -0.5f, 0.5f, 0.5f },
	{ 0.5f, 0.5f, 0.5f },
	{ 0.5f, -0.5f, 0.5f }
};

GLuint cube_indices[] = {
	0,1,2,3,
	4,5,6,7,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7
};

GLuint vertexBuffer;
GLuint vertexArray;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); // Область рисования -- все окно
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, cube_indices);
	glFlush(); // Гарантируем выполнение всех операций: попробуйте закомментировать :)
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutCreateWindow("OpenGL cube");

	glewInit();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
