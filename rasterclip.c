//
// rasterpos2.c
//
// how-to:
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLUT/glut.h>

#ifndef	PI
#	define	PI	3.14159
#endif
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	DEG2RAD(d)	((d)*PI/180.0)

GLfloat	raster_x = 0;
GLfloat	raster_y = 0;


void display(void)
{
	int	i;
	char	buf[128];

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1,1,1);
	glRasterPos3f(raster_x,raster_y,0);
	strcpy(buf, "This is a demo to show how glRasterPos*() function works...");
	for(i = 0 ; i < strlen(buf) ; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buf[i]);
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
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		raster_x -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		raster_x += 0.1;
		break;
	case GLUT_KEY_UP:
		raster_y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		raster_y -= 0.1;
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
	glOrtho(-1, 1, -1, 1, -1, 1);

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
	glutCreateWindow("raster position clipped");

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
