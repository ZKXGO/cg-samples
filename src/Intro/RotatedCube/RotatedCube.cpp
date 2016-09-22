#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

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

	string vsh_src((istreambuf_iterator<char>(fstream("RotatedCube.vert"))), istreambuf_iterator<char>());
	string fsh_src((std::istreambuf_iterator<char>(fstream("RotatedCube.frag"))), istreambuf_iterator<char>());

	// Create Shader And Program Objects
	GLenum program = glCreateProgram();
	GLenum vertex_shader = glCreateShader(GL_VERTEX_SHADER_ARB);
	GLenum fragment_shader = glCreateShader(GL_FRAGMENT_SHADER_ARB);

	// Load Shader Sources
	const char *src = vsh_src.c_str();
	glShaderSource(vertex_shader, 1, &src, NULL);
	src = fsh_src.c_str();
	glShaderSource(fragment_shader, 1, &src, NULL);

	// Compile The Shaders
	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	// Attach The Shader Objects To The Program Object
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// Link The Program Object
	glLinkProgram(program);

	char log[10000];
	int log_len;
	glGetProgramInfoLog(program, sizeof(log) / sizeof(log[0]) - 1, &log_len, log);
	log[log_len] = 0;
	cout << "Shader compile result: " << log << endl;

	glm::mat4x4 mvp = glm::perspectiveFovRH(45.0f, 100.0f, 100.0f, 1.0f, 3.0f) * 
		              glm::translate(glm::vec3(0.0f, 0.0f, -2.0f)) *
		              glm::rotate(60.0f, glm::vec3(1.0f, 0.5f, 0.3f));

	glUseProgramObjectARB(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, &mvp[0][0]);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	int attribLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribLoc);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
