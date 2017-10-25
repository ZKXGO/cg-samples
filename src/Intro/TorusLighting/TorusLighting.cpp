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
	glm::vec3 position;
	glm::vec3 normal;
};
#pragma pack()

const int N1 = 60;
const int N2 = 30;

Vertex torus_vertices[N1 * N2];
GLuint torus_indices[N1* N2 * 4];

const double Pi = 3.14159265358979323846;

void MakeTorus(double r1, double r2) {
	int iidx = 0;
	for (int i = 0; i<N1; i++) {
		int i2 = (i<N1 - 1) ? (i + 1) : (0);
		double phi = 2 * i*Pi / N1;
		for (int j = 0; j<N2; j++) {
			int j2 = (j<N2 - 1) ? (j + 1) : (0);
			double psi = 2 * j*Pi / N2;

			double nx = cos(phi)*cos(psi);
			double ny = sin(psi);
			double nz = sin(phi)*cos(psi);

			torus_vertices[i * N2 + j].position = glm::vec3(r1*cos(phi) + r2*nx, r2*ny, r1*sin(phi) + r2*nz);
			torus_vertices[i * N2 + j].normal = glm::vec3(nx,ny,nz);

			torus_indices[iidx++] = i *N2 + j;
			torus_indices[iidx++] = i *N2 + j2;
			torus_indices[iidx++] = i2 * N2 + j2;
			torus_indices[iidx++] = i2 * N2 + j;
		}
	}
}

GLuint vertexBuffer;
GLuint vertexArray;
GLuint program;
GLuint mvpLoc;
GLuint mvLoc;
GLuint nmLoc;

float xAngle = 0;
float yAngle = 0;

bool init()
{
	MakeTorus(0.5, 0.1);

	string vsh_src((istreambuf_iterator<char>(fstream("BlinnVertex.glsl"))), istreambuf_iterator<char>());
	string fsh_src((std::istreambuf_iterator<char>(fstream("BlinnFragment.glsl"))), istreambuf_iterator<char>());

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
	mvLoc = glGetUniformLocation(program, "mv");
	nmLoc = glGetUniformLocation(program, "nm");

	glUseProgramObjectARB(program);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(torus_vertices), torus_vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	int posLoc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(posLoc);
	int normLoc = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(normLoc);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
}

vector<float> times(10);
chrono::steady_clock::time_point prevFrame;
bool firstFrame;

glm::mat4x4 proj;

void reshape(int w, int h)
{
	proj = glm::perspectiveFovRH(45.0f, float(w), float(h), 0.1f, 3.0f);
	glViewport(0, 0, w, h); // Область рисования -- все окно
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
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	

	glm::mat4x4 mv = 
		glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) *
		glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(yAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4x4 mvp = proj * mv;
	glm::mat3x3 nm = glm::transpose(glm::inverse(glm::mat3x3(mv)));

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, &mv[0][0]);
	glUniformMatrix3fv(nmLoc, 1, GL_FALSE, &nm[0][0]);

	glDrawElements(GL_QUADS, sizeof(torus_indices) / sizeof(torus_indices[0]), GL_UNSIGNED_INT, torus_indices);
	glFlush(); // Гарантируем выполнение всех операций: попробуйте закомментировать :)
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Torus with lighting");

	glewInit();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
