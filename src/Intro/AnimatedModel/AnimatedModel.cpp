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

#include"USSEnterprise.h"

using namespace std;


GLuint vertexBuffer;
GLuint vertexArray;
GLuint program;
GLuint mvpLoc;

float xAngle = 0;
float yAngle = 0;

bool init() 
{
	glEnable(GL_DEPTH_TEST);

	string vsh_src((istreambuf_iterator<char>(fstream("AnimatedModel.vert"))), istreambuf_iterator<char>());
	string fsh_src((std::istreambuf_iterator<char>(fstream("AnimatedModel.frag"))), istreambuf_iterator<char>());

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(USSEnterpriseVerts), USSEnterpriseVerts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	int attribLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribLoc);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

vector<float> times(10);
chrono::steady_clock::time_point prevFrame;
bool firstFrame;

glm::mat4x4 proj;

void reshape(int w, int h)
{
	proj = glm::perspectiveFovRH(45.0f, float(w), float(h), 0.1f, 5.0f);
	glViewport(0, 0, w, h); 
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
		glm::translate(glm::vec3(0.0f, 0.0f, -1.5f)) *
		glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, USSEnterpriseNumVerts);
	glFlush();
	glutSwapBuffers();

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow("Animated cube");

	glewInit();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
