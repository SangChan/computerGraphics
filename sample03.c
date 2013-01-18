/*

	sample03

	keyboard control
	q/Q/ESC: quit
	left/right/up/down: moves the camera to the corresponding direction
	page up/down: zooms in/out

 */
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>

#ifndef	MAX
#	define	MAX(x,y)	((x)>(y)?(x):(y))
#endif
#ifndef	MIN
#	define	MIN(x,y)	((x)<(y)?(x):(y))
#endif

struct
{
	GLfloat	center[2];
	GLfloat	width;
	GLfloat	height;
} camera =
{
	{0,0},
	2,
	2
};

void set_camera(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
			camera.center[0]-camera.width/2.0,
			camera.center[0]+camera.width/2.0,
			camera.center[1]-camera.height/2.0,
			camera.center[1]+camera.height/2.0,
			1,-1);
}

void print_projection_matrix(void)
{
	GLfloat	m[4][4];
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)m);
	printf("[%g\t%g\t%g\t%g]\n", m[0][0], m[1][0], m[2][0], m[3][0]);
	printf("[%g\t%g\t%g\t%g]\n", m[0][1], m[1][1], m[2][1], m[3][1]);
	printf("[%g\t%g\t%g\t%g]\n", m[0][2], m[1][2], m[2][2], m[3][2]);
	printf("[%g\t%g\t%g\t%g]\n", m[0][3], m[1][3], m[2][3], m[3][3]);

}

// "display" callback function.
// This function is called whenever the window needs to be updated (redrawn).
void display(void)
{
	// Clears the "color buffer". See what happens if you 
	// comment out this call.
	glClear(GL_COLOR_BUFFER_BIT);	

	glColor3f(1,0,0);
	glBegin(GL_QUADS);
		glVertex2f(-2,-2);
		glVertex2f( 2,-2);
		glVertex2f( 2, 2);
		glVertex2f(-2, 2);
	glEnd();

	glColor3f(0,0,1);
	glutSolidTeapot(.5);

	glutSwapBuffers ();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		case 'Q':
		case 27:
			exit(0);
	}
}

void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camera.center[0] = MAX(-1, camera.center[0]-0.1);
			break;
		case GLUT_KEY_RIGHT:
			camera.center[0] = MIN(1, camera.center[0]+0.1);
			break;
		case GLUT_KEY_UP:
			camera.center[1] = MIN(1, camera.center[1]+0.1);
			break;
		case GLUT_KEY_DOWN:
			camera.center[1] = MAX(-1, camera.center[1]-0.1);
			break;
		case GLUT_KEY_PAGE_DOWN:
			camera.width = MIN(8, camera.width*1.2);
			camera.height = MIN(8, camera.height*1.2);
			break;
		case GLUT_KEY_PAGE_UP:
			camera.width = MAX(0.2, camera.width*0.8);
			camera.height = MAX(0.2, camera.height*0.8);
			break;
	}
	printf("camera setting---\n");
	printf("center=(%g,%g), width=%g, height=%g\n", 
		camera.center[0], camera.center[1], camera.width, camera.height);
	printf("projection matrix:\n");
	set_camera();	// Refreshes the camera setting.
	print_projection_matrix();

	glutPostRedisplay();	// Forces the screen refreshed.
}

void reshape(int width, int height)
{
	if(width > height)
		glViewport((width-height)/2, 0, height, height);
	else
		glViewport(0, (height-width)/2, width, width);
}

int main(int argc, char** argv)
{
	// Initializes GLUT library. Should be called at the beginning
	// before any GLUT function is called.
	glutInit(&argc, argv);	

	// Specifies the type of the color buffer. 
	// A single buffer with RGB type.
	// See the reference for more details.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Creates the window with given title bar text.
	glutCreateWindow("sample03");

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);

	// Registers the "display" callback function.
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);

	printf("initial projection matrix:\n");
	print_projection_matrix();

	set_camera();

	// Go into the infinite event loop.
	glutMainLoop();

	return 0;
}
/*
	Try yourself:
	-How can we remove the distortion of the circles?

 */



