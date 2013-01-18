//
// back.c
//
#include <stdio.h>
#include	<stdlib.h>
#include <GL/glut.h>

GLfloat	rot_x = 0;
GLfloat	rot_y = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);

	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glVertex3f(-1,-1,0);
		glVertex3f(1,-1,0);
		glVertex3f(1,1,0);
		glVertex3f(-1,1,0);
	glEnd();

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
	glClearColor (0.0, 0.0, 0.5, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	{
		GLfloat	position[] = {0,1,0,0};	// directional light (from (1,1,1))
		GLfloat	ambient[] = {.2,.2,.2,1};
		GLfloat	diffuse[] = {1,1,1,1};
		GLfloat	specular[] = {1,1,1,1};

		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	}
	{
		GLfloat	ambient[] = {.2,.2,.2,1};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	}
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	{
		GLfloat	ambient[] = {0.19125, 0.0735, 0.0225, 1};
		GLfloat	diffuse[] = {0.7038, 0.27048, 0.0828, 1};
		GLfloat	specular[] = {0.256777, 0.137622, 0.086014, 1};
		GLfloat	shininess = 0.1*128.0;
		GLint	face = GL_FRONT;
//		GLint	face = GL_FRONT_AND_BACK;


		glMaterialfv(face, GL_AMBIENT, ambient);
		glMaterialfv(face, GL_DIFFUSE, diffuse);
		glMaterialfv(face, GL_SPECULAR, specular);
		glMaterialf(face, GL_SHININESS, shininess);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
//	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (250, 250); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("back face shading");

	init();

	glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
