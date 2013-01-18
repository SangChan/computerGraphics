#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	MAX(x,y)	((x)>(y)?(x):(y))

typedef struct
{
	GLfloat	angle;
	GLfloat	length;
	GLfloat	thickness;
} TBox;

struct
{
	GLfloat	x;
	GLfloat	y;
	GLfloat	z;
	TBox	box;
} upper;

TBox	lower;
TBox	hand;

void render_box(GLfloat r, GLfloat g, GLfloat b, GLfloat thickness, GLfloat length)
{
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
		glVertex3f(-thickness/2.0,-thickness/2.0,0);
		glVertex3f(length-thickness/2.0,-thickness/2.0,0);
		glVertex3f(length-thickness/2.0,thickness/2.0,0);
		glVertex3f(-thickness/2.0,thickness/2.0,0);
	glEnd();
	glColor3f(1,1,1);
	glPointSize(4);
	glBegin(GL_POINTS);
		glVertex3f(0,0,0);
	glEnd();
}

void render_upper_arm(void)
{
	render_box(.3,.3,.3,upper.box.thickness, upper.box.length);
}

void render_lower_arm(void)
{
	render_box(.5,.5,.5,lower.thickness, lower.length);
}

void render_hand(void)
{
	render_box(.8, .8, .8, hand.thickness, hand.length);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// render x-axis (in red)
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
	glEnd();

	// render y-axis (in green)
	glColor3f(0,1,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
	glEnd();

	// render hierarchical arm model
	// render the upper arm
	glPushMatrix();
		glTranslatef(upper.x, upper.y, upper.z);
		glRotatef(upper.box.angle,0,0,1);
		render_upper_arm();

		// render the lower arm
		glPushMatrix();
			glTranslatef(upper.box.length-upper.box.thickness/2.0-lower.thickness/2.0, 0, 0);
			glRotatef(lower.angle,0,0,1);
			render_lower_arm();

			// render the hands
			glPushMatrix();
				glTranslatef(lower.length-lower.thickness/2.0-hand.thickness/2.0, hand.thickness/1.8, 0);
				glRotatef(hand.angle/2.0, 0, 0, 1);
				render_hand();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(lower.length-lower.thickness/2.0-hand.thickness/2.0, -hand.thickness/1.8, 0);
				glRotatef(-hand.angle/2.0, 0, 0, 1);
				render_hand();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
//	glFlush ();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	printf("windows size: %dx%d\n", w, h);
}

void init(void)
{
	upper.x = 0;
	upper.y = 0;
	upper.z = 0;

	upper.box.length = 5;
	upper.box.thickness = 2;
	upper.box.angle = 0;

	lower.angle = 0;
	lower.length = 6;
	lower.thickness = 1.6;

	hand.thickness = .8;
	hand.length = 3;
	hand.angle = 0;
}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'U':
		upper.box.angle += 5;
		if(upper.box.angle > 360)	upper.box.angle -= 360;
		break;
	case 'u':
		upper.box.angle -= 5;
		if(upper.box.angle < 0)	upper.box.angle += 360;
		break;
	case 'L':
		lower.angle += 5;
		if(lower.angle > 360)	lower.angle -= 360;
		break;
	case 'l':
		lower.angle -= 5;
		if(lower.angle < 0)	lower.angle += 360;
		break;
	case 'H':
		hand.angle = MIN(90, hand.angle + 5);
		break;
	case 'h':
		hand.angle = MAX(0, hand.angle - 5);
		break;
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		upper.y += 1;
		break;
	case GLUT_KEY_DOWN:
		upper.y -= 1;
		break;
	case GLUT_KEY_LEFT:
		upper.x -= 1;
		break;
	case GLUT_KEY_RIGHT:
		upper.x += 1;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
//	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	glClearColor (0.0, 0.0, 0.0, 0.0);

	init();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-12.0, 12.0, -12.0, 12.0, -1.0, 1.0);

	glViewport(0, 0, 250, 250);

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
