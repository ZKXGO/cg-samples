#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<chrono>
#include<numeric>

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
GLuint program;
GLuint mvpLoc;

float xAngle = 0;
float yAngle = 0;

bool init() 
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	string vsh_src((istreambuf_iterator<char>(fstream("AnimatedCube.vert"))), istreambuf_iterator<char>());
	string fsh_src((std::istreambuf_iterator<char>(fstream("AnimatedCube.frag"))), istreambuf_iterator<char>());

	// Create Shader And Program Objects
	program = glCreateProgram();
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

	mvpLoc = glGetUniformLocation(program, "mvp");

	glUseProgramObjectARB(program);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	int attribLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribLoc);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

vector<float> times(10);
chrono::steady_clock::time_point prevFrame;
bool firstFrame;

glm::mat4x4 proj;

void reshape(int w, int h)
{
	proj = glm::perspectiveFovRH(45.0f, float(w), float(h), 1.0f, 5.0f);
	glViewport(0, 0, w, h); // ������� ��������� -- ��� ����
	times.clear();
	firstFrame = true;
}

void idle(void) {
	if (firstFrame) {
		prevFrame = chrono::steady_clock::now();
		firstFrame = false;
	}
	else {
		chrono::steady_clock::time_point currFrame = chrono::steady_clock::now();
		double delta = double(chrono::duration_cast<std::chrono::microseconds>(currFrame - prevFrame).count());
		prevFrame = currFrame;
		if (times.size() >= 10)
			times.pop_back();
		times.push_back(delta);

		double avg = accumulate(times.begin(), times.end(), 0.0) / times.size();
		xAngle += 0.000005 * avg;
		yAngle -= 0.000003 * avg;

	}
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glm::mat4x4 mvp = proj *
		glm::translate(glm::vec3(0.0f, 0.0f, -3.0f)) *
		glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, cube_indices);
	glFlush(); // ����������� ���������� ���� ��������: ���������� ���������������� :)

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Animated cube");

	glewInit();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
