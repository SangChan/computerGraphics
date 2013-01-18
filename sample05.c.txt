/*

	sample05

	keyboard control
	left/right: rotates around the y-axis of the cube
	up/down: rotates around the x-axis of the cube
	q/Q/ESC: quit

 */
#include <GL/glut.h>
#include <stdio.h>

// view frustum
struct
{
	GLfloat	left;
	GLfloat	right;
	GLfloat	bottom;
	GLfloat	top;
	GLfloat	near;
	GLfloat	far;
} frustum =
{
	-1,1,-1,1,1,5
};

GLfloat	rotx = 0;
GLfloat	roty = 0;

// "display" callback function.
// This function is called whenever the window needs to be updated (redrawn).
void display(void)
{
	// Clears the "color buffer". See what happens if you 
	// comment out this call.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// translates objects such that they are inside of the frustum
	glTranslatef(0, 0, -(frustum.near + frustum.far)/2.0);
	glRotatef(roty, 0, 1, 0);
	glRotatef(rotx, 1, 0, 0);

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

	glColor3f(1,1,1);
	glutWireCube(1);

	glColor4f(.5,.5,.5,0.9);
	glutSolidCube(1);

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
	glutPostRedisplay();
}

void rotate(GLfloat *angle, GLfloat theta)
{
	*angle += theta;
	if(theta > 0 && *angle > 360)	*angle -= 360;
	else if(*angle < 0)	*angle += 360;
}

#define	STEP	10
void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			rotate(&roty, -STEP);
			break;
		case GLUT_KEY_RIGHT:
			rotate(&roty, STEP);
			break;
		case GLUT_KEY_UP:
			rotate(&rotx, -STEP);
			break;
		case GLUT_KEY_DOWN:
			rotate(&rotx, STEP);
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// Initializes GLUT library. Should be called at the beginning
	// before any GLUT function is called.
	glutInit(&argc, argv);	

	// Specifies the type of the color buffer. 
	// A single buffer with RGB type.
	// See the reference for more details.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Creates the window with given title bar text.
	glutCreateWindow("sample05: perspective projection");


	// Registers the "display" callback function.
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glClearColor (0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	// for transparent color
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// to remove "stitching" artifacts
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frustum.left,
				frustum.right,
				frustum.bottom,
				frustum.top,
				frustum.near,
				frustum.far);

	// Go into the infinite event loop.
	glutMainLoop();

	return 0;
}



