#include <stdio.h>
#ifndef	WINDOWS
#	include <stdlib.h>
#endif
#include <GLUT/glut.h>

int	perspective = 0;
GLfloat	rot_x = 0;
GLfloat	rot_y = 0;
GLint	obj = 0;

struct st_window
{
	GLint	width, height;
} window;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);

	switch(obj)
	{
		case 0:
			glutSolidCube(1);
			break;
		case 1:
			glutSolidSphere(1,48,48);
			break;
		case 2:
			glutSolidTeapot(1);
			break;
	}

	glutSwapBuffers();
}

void refresh_projection(void)
{
	GLfloat	ratio = ((GLfloat)window.width)/((GLfloat)window.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(perspective)
	{
		gluPerspective(45, ratio, 1, 10);
	}
	else
	{
		glOrtho(-2.0*ratio, 2.0*ratio, -2, 2, 1, 10);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case '0':
		case '1':
		case '2':
			obj = key-'0';
			break;
		case 27:
		case 'q':
		case 'Q':
			exit(0);
			break;
		case 'p':
		case 'P':
			perspective = (perspective+1)&0x01;
			refresh_projection();
			break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// light setting (GL_LIGHT0)
	{
		GLfloat	position[] = {-1,1,1,0};	//directional light
		GLfloat	ambient[] = {.2, .2, .2, 1};
		GLfloat	diffuse[] = {1,1,1,1};
		GLfloat	specular[] = {1,1,1,1};

		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	}
	
	// material setting
	{
		GLfloat	ambient[] = {.5, .5, .5, 1};
		GLfloat	diffuse[] = {.5,.5,.5,1};
		GLfloat	specular[] = {1,1,1,1};
		GLfloat	shininess = 0.7*128.0;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			rot_y -= 5;
			break;
		case GLUT_KEY_RIGHT:
			rot_y += 5;
			break;
		case GLUT_KEY_UP:
			rot_x -= 5;
			break;
		case GLUT_KEY_DOWN:
			rot_x += 5;
			break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	window.width = w;
	window.height = h;
	glViewport(0, 0, w, h);
	refresh_projection();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(250, 250); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("projection demo");

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
