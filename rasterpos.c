//
// rasterpos.c
//
// how-to:
//
// arrows (left/right/up/down): change the spotlight direction
// q/Q/ESC: quit
//

#include <stdio.h>
#ifndef	WINDOWS
#	include <stdlib.h>
#endif
#include <GL/glut.h>

#ifndef	PI
#	define	PI	3.14159
#endif
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	DEG2RAD(d)	((d)*PI/180.0)

GLfloat	rot_x = 0;
GLfloat	rot_y = 0;


void display(void)
{
	int	i;
	char	buf[128];

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.5);
	glRotatef(-rot_x, 1, 0, 0);
	glRotatef(-rot_y, 0, 1, 0);

	glColor3f(1,1,1);
	glRasterPos3f(1,0,0);
	glColor3f(1,1,0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'x');
	glRasterPos3f(0,1,0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'y');
	glRasterPos3f(0,0,1);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'z');
	glRasterPos3f(0,0,0);
	strcpy(buf, "origin");
	for(i = 0 ; i < strlen(buf) ; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buf[i]);
	}

	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,1,0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1);
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
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		rot_y = MIN(rot_y+5, 90);
		break;
	case GLUT_KEY_RIGHT:
		rot_y = MAX(rot_y-5, -90);
		break;
	case GLUT_KEY_UP:
		rot_x = MIN(rot_x+5, 90);
		break;
	case GLUT_KEY_DOWN:
		rot_x = MAX(rot_x-5, -90);
		break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// Since the projection never changes, 
	// it needs to be set only once at the beginning.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	// Enables the depth test. (NOTE: It's not glEnable(GL_DEPTH)!!!)
	// See what happens if we don't enable it.
	// (especially when the sphere and the plane overlap.)
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(250, 250); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("raster position");

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
