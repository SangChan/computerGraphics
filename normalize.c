//
// normalize.c
//
// To demonstrate the effect of `scaling' normals.
// Compare how the `shading' of the box look different in mode 2 & 3.
// Mode 2 looks OK but mode 3 does not, due to the scaling transformation.
// (Normal vectors, generated inside glutSolidCube function, are incorrectly scaled.)
//
#include <stdio.h>
#ifndef	WINDOWS
#	include	<stdlib.h>
#endif
#include <GL/glut.h>

GLfloat	rot_x = 0;
GLfloat	rot_y = 0;
GLint	mode = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);


	glDisable(GL_LIGHTING);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);	glVertex3f(2,0,0);
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);	glVertex3f(0,2,0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);	glVertex3f(0,0,2);
	glEnd();

#define	LENX	.2
#define	LENY	.5
#define	LENZ	.8

	switch(mode)
	{
		case 0:
			glDisable(GL_LIGHTING);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1,1);
			glColor3f(1,1,1);
			glPolygonMode(GL_FRONT, GL_LINE);
			glutSolidCube(1);
			glColor3f(.5,.5,.5);
			glPolygonMode(GL_FRONT, GL_FILL);
			glutSolidCube(1);
			break;
		case 1:
			glEnable(GL_LIGHTING);
			glutSolidCube(1);
			break;
		case 2:
			glEnable(GL_LIGHTING);
			glBegin(GL_QUADS);
				glNormal3f(1,0,0);
				glVertex3f(LENX, -LENY, -LENZ);
				glVertex3f(LENX, LENY, -LENZ);
				glVertex3f(LENX, LENY, LENZ);
				glVertex3f(LENX, -LENY, LENZ);
				glNormal3f(-1,0,0);
				glVertex3f(-LENX, -LENY, -LENZ);
				glVertex3f(-LENX, -LENY, LENZ);
				glVertex3f(-LENX, LENY, LENZ);
				glVertex3f(-LENX, LENY, -LENZ);
				glNormal3f(0,1,0);
				glVertex3f(-LENX, LENY, -LENZ);
				glVertex3f(-LENX, LENY, LENZ);
				glVertex3f(LENX, LENY, LENZ);
				glVertex3f(LENX, LENY, -LENZ);
				glNormal3f(0,-1,0);
				glVertex3f(-LENX, -LENY, -LENZ);
				glVertex3f(LENX, -LENY, -LENZ);
				glVertex3f(LENX, -LENY, LENZ);
				glVertex3f(-LENX, -LENY, LENZ);
				glNormal3f(0,0,1);
				glVertex3f(-LENX, -LENY, LENZ);
				glVertex3f(LENX, -LENY, LENZ);
				glVertex3f(LENX, LENY, LENZ);
				glVertex3f(-LENX, LENY, LENZ);
				glNormal3f(0,0,-1);
				glVertex3f(-LENX, -LENY, -LENZ);
				glVertex3f(-LENX, LENY, -LENZ);
				glVertex3f(LENX, LENY, -LENZ);
				glVertex3f(LENX, -LENY, -LENZ);
			glEnd();
			break;
		case 3:
			glEnable(GL_LIGHTING);
			glScalef(2.0*LENX, 2.0*LENY, 2.0*LENZ);
			glutSolidCube(1);
			break;
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
		case 'q':
		case 'Q':
			exit(0);
		case '0':
		case '1':
		case '2':
		case '3':
			mode = key-'0';
			break;
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
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	{
		GLfloat	position[] = {1,1,1,0};	// directional light (from (1,1,1))
		GLfloat	ambient[] = {.2,.2,.2,1};
		GLfloat	diffuse[] = {1,1,1,1};
		GLfloat	specular[] = {1,1,1,1};

		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	}
	glEnable(GL_LIGHT0);

	{
		GLfloat	ambient[] = {.5,.5,.5,1};
		GLfloat	diffuse[] = {.5,.5,.5,1};
		GLfloat	specular[] = {.5,.5,.5,1};
		GLfloat	shininess = 0.5*128.0;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_NORMALIZE);	// Compare mode 2 & 3 with/without commenting this line out.
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (250, 250); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("normalize");

	init();

	glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
