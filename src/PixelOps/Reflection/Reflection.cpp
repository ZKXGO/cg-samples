#include<GL/freeglut.h>
#include<FreeImage.h>

double x_angle = 30; // ???? ???????? ????????
double y_angle = 0;

int mouse_x = 0, mouse_y = 0, mouse_button = -1, mouse_state = GLUT_UP;

const int ObjectSize = 3;

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	if (h > 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, w / (GLdouble)h, 1, 3000);
	}
}

void RenderObjects()
{

	GLfloat c_emissive[4] = { 0.0f, 0.4f, 0.0f, 0.0f };
	GLfloat c_diffuse[4] = { 0.2f, 0.8f, 0.6f, 0.0f };
	GLfloat c_specular[4] = { 0.2f, 0.6f, 0.2f, 0.0f };
	GLfloat c_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);

	glPushMatrix();
	glTranslated(-1.0, 0.5, -1.0);
	glutSolidCube(1.0);
	glPopMatrix();


	GLfloat s_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat s_diffuse[4] = { 0.8f, 0.2f, 0.2f, 0.0f };
	GLfloat s_specular[4] = { 0.6f, 0.2f, 0.2f, 0.0f };
	GLfloat s_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, s_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, s_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, s_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);

	glPushMatrix();
	glTranslated(1.0, 0.5, -1.0);
	glutSolidSphere(0.5, 15, 10);
	glPopMatrix();


	GLfloat t_emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat t_diffuse[4] = { 0.2f, 0.2f, 0.8f, 0.0f };
	GLfloat t_specular[4] = { 0.2f, 0.2f, 0.6f, 0.0f };
	GLfloat t_ambient[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, t_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, t_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, t_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, t_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);

	glPushMatrix();
	glTranslated(0.0, 0.5, 1.0);
	glutSolidTeapot(0.8);
	glPopMatrix();
}

void RenderSurface()
{
	glBegin(GL_QUADS);
	glColor4d(0, 0, 0.9, 0.5);
	glVertex3d(ObjectSize, 0, ObjectSize);
	glVertex3d(-ObjectSize, 0, ObjectSize);
	glVertex3d(-ObjectSize, 0, -ObjectSize);
	glVertex3d(ObjectSize, 0, -ObjectSize);
	glEnd();
}

GLfloat l_position[4] = { 4.0f, 4.0f, 4.0f, 1.0f };

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(0, 0, -3 * ObjectSize);
	glRotated(x_angle, 1, 0, 0);
	glRotated(y_angle, 0, 1, 0);

	glEnable(GL_LIGHTING);


	glLightfv(GL_LIGHT0, GL_POSITION, l_position);

	RenderObjects();


	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	RenderSurface();
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, 1, 1);
	glPushMatrix();
	glScaled(1, -1, 1);
						
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	RenderObjects();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderSurface();
	glDisable(GL_BLEND);

	glFlush();
	glutSwapBuffers();
}

void idle() {
	// ... Nothing to do here
}

void keypress(unsigned char key, int x, int y)
{
	if (key == 27) {
		glutDestroyWindow(glutGetWindow());
		exit(1);
	}
}

void mouseClick(int button, int state, int x, int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}

void mouseMotion(int x, int y) 
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;
	if (mouse_state == GLUT_DOWN) {
		if (mouse_button == GLUT_LEFT_BUTTON) {
			y_angle += dx / 3;
			if (y_angle < 0)
				y_angle += 360;
			if (y_angle >= 360)
				y_angle -= 360;
			x_angle += dy / 3;
			if (x_angle < 5)
				x_angle = 10;
			if (x_angle > 90)
				x_angle = 90;
			display();
		}
	}
	mouse_x = x;
	mouse_y = y;
}

bool Init()
{

	GLfloat l_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_specular[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat l_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	
	GLfloat l_position[4] = { 4.0f, 4.0f, 4.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	return true;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("Reflect Demo");
	if (!Init())
		return 1;
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keypress);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
