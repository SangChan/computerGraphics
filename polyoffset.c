#ifndef	WINDOWS
#	include <stdlib.h>
#endif
#include <stdio.h>
#include <GL/glut.h>

GLfloat	rot_x = 0;
GLfloat	rot_y = 0;
GLuint	obj = 1;
GLUquadric	*p_quad = NULL;
GLint	offset = 1;

void render_object(void)
{
	switch(obj)
	{
		case 1:
			glutSolidCube(1);
			break;
		case 2:
			glutSolidSphere(1,24,24);
			break;
		case 3:
			glutSolidTeapot(1);
			break;
		case 4:
			gluCylinder(p_quad, 1, 1, 1, 24, 12);
			break;
	}
}

void render_axes(void)
{
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(2,0,0);
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,2,0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,0,2);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-5);
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);

	render_axes();

	glColor3f(1,1,1);
	glPolygonMode(GL_FRONT, GL_LINE);
	render_object();
	glColor3f(.5,.5,.5);
	glPolygonMode(GL_FRONT, GL_FILL);
	render_object();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
			obj = key - '0';
			break;
		case ' ':
			offset = (offset + 1)&0x01;
			if(offset)	glEnable(GL_POLYGON_OFFSET_FILL);
			else		glDisable(GL_POLYGON_OFFSET_FILL);
			break;
		case 27:
		case 'q':
		case 'Q':
			exit(0);
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			rot_y -= 10;
			break;
		case GLUT_KEY_RIGHT:
			rot_y += 10;
			break;
		case GLUT_KEY_UP:
			rot_x -= 10;
			break;
		case GLUT_KEY_DOWN:
			rot_x += 10;
			break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	glEnable(GL_DEPTH_TEST);

	glPolygonOffset(1,1);

	p_quad = gluNewQuadric();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	glClearColor (0.0, 0.0, 0.0, 0.0);

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
