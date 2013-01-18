/*

	sample04

	keyboard control
	r/R: decreases/increases the angle of the red arm
	g/G: decreases/increases the angle of the green arm
	b/B: decreases/increases the angle of the blue arm
	q/Q/ESC: quit

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

typedef struct
{
	GLfloat	angle;
	GLfloat	width;
	GLfloat	length;
	GLfloat	color[3];
} t_arm;

t_arm	red = {45, 1, 3, {1,0,0}};
t_arm	green = {30, 0.5, 2, {0,1,0}};
t_arm	blue = {-30, 1, 3, {0,0,1}};

void render_unit_square(void)
{
	glBegin(GL_QUADS);
		glVertex2f(-.5, -.5);
		glVertex2f(.5, -.5);
		glVertex2f(.5, .5);
		glVertex2f(-.5, .5);
	glEnd();
}

void render_arm(t_arm* p_arm)
{
	glBegin(GL_POINTS);
		glVertex2f(0,0);
	glEnd();
	glRotatef(p_arm->angle, 0, 0, 1);
	glTranslatef((p_arm->length-p_arm->width)/2.0, 0, 0);
	glPushMatrix();
		glScalef(p_arm->length, p_arm->width, 1);
		glColor3fv(p_arm->color);
		render_unit_square();
		glColor3f(1,1,1);
	glPopMatrix();
	glTranslatef((p_arm->length-p_arm->width)/2.0, 0, 0);
}

// "display" callback function.
// This function is called whenever the window needs to be updated (redrawn).
void display(void)
{
	// Clears the "color buffer". See what happens if you 
	// comment out this call.
	glClear(GL_COLOR_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	render_arm(&blue);
	render_arm(&green);
	render_arm(&red);

	glLoadIdentity();
	glColor3f(3,0,0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(1,0);
	glEnd();
	glColor3f(0,3,0);
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,1);
	glEnd();

	glutSwapBuffers ();
}

void rotate_arm(t_arm *p_arm, GLfloat angle)
{
	p_arm->angle += angle;
	if(p_arm->angle > 0 && p_arm->angle > 360)	p_arm->angle -= 360;
	else if(p_arm->angle < 0)	p_arm->angle += 360;
	
}

void stretch_arm(t_arm *p_arm, GLfloat length)
{
	p_arm->length += (length/10);
}
#define	STEP	10
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'r':
			rotate_arm(&red, -STEP);
			break;
		case 'R':
			rotate_arm(&red, STEP);
			break;
		case 'g':
			rotate_arm(&green, -STEP);
			break;
		case 'G':
			rotate_arm(&green, STEP);
			break;
		case 'b':
			stretch_arm(&blue, -STEP);
			break;
		case 'B':
			stretch_arm(&blue, STEP);
			break;
		case 'q':
		case 'Q':
		case 27:
			exit(0);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Creates the window with given title bar text.
	glutCreateWindow("sample04: transformation");

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);

	// Registers the "display" callback function.
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);

	glPointSize(4);
	glPolygonMode(GL_FRONT, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, 1, -1);

	// Go into the infinite event loop.
	glutMainLoop();

	return 0;
}



