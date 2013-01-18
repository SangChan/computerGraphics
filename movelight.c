//
// movelight.c
//
// how-to:
//
// x/X: decrease/increase the x value
// y/Y: decrease/increase the y value
// z/Z: decrease/increase the z value
// c/C: decrease/increase the spotlight cut-off angle
// arrows (left/right/up/down): change the spotlight direction
// q/Q/ESC: quit
//

#include <stdio.h>
#ifndef	WINDOWS
#	include <stdlib.h>
#endif
#include <GLUT/glut.h>

#ifndef	PI
#	define	PI	3.14159
#endif
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	DEG2RAD(d)	((d)*PI/180.0)

struct struct_splight
{
	GLfloat	x, y, z;
	GLfloat	cutoff;
	GLfloat	rot_x, rot_y;
} spotlight;

GLuint resol = 50;


// render a quad split into small ones (to show the spotlight effect)
void render_quads(void)
{
	int	i, j;
	GLfloat	step = 2.0/((GLfloat)resol);
	glBegin(GL_QUADS);
		for(i = 0 ; i < resol ; i++)
		{
			for(j = 0 ; j < resol ; j++)
			{
				glNormal3f(0,0,1);
				glVertex3f(-1.0+((GLfloat)i)*step, -1.0+((GLfloat)j)*step, 0);
				glNormal3f(0,0,1);
				glVertex3f(-1.0+((GLfloat)(i+1))*step, -1.0+((GLfloat)j)*step, 0);
				glNormal3f(0,0,1);
				glVertex3f(-1.0+((GLfloat)(i+1))*step, -1.0+((GLfloat)(j+1))*step, 0);
				glNormal3f(0,0,1);
				glVertex3f(-1.0+((GLfloat)i)*step, -1.0+((GLfloat)(j+1))*step, 0);
			}
		}
	glEnd();
}
void render_axes(void)
{
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
}
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -1, -3.5);
	glRotatef(-90, 1, 0, 0);

	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
		glDisable(GL_LIGHTING);
		glLineWidth(3);
		render_axes();	
	glPopAttrib();

	glPushMatrix();

		// transformation for the light.
		// applied both to the (1) spotlight and (2) the white sphere
		glTranslatef(spotlight.x, spotlight.y, spotlight.z);
		glRotatef(spotlight.rot_y, 0, 1, 0);	// rotation about the y-axis
		glRotatef(spotlight.rot_x, 1, 0, 0);	// rotation about the x-axis

		// Since the position, direction and cut-off angle are
		// dynamically change, we should keep defining those values in every frame.
		{
			GLfloat	position[] = {0,0,0,1};
			GLfloat	direction[] = {0,0,-1};
			glLightfv(GL_LIGHT0, GL_POSITION, position);
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotlight.cutoff);
		}

		// renders a white spere to show where the light is...
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			render_axes();	// renders the (local) three axes of the sphere
			glColor3f(1,1,1);
			glutSolidSphere(0.1, 24, 24);

			// renders the white light indicating the spotlight direction
			glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,-1);
			glEnd();
		glPopAttrib();
	glPopMatrix();

	// sets the material
	// In fact, since it never changes, it doesn't have to be set every frame...
	{
		GLfloat	ambient[] = {.5,.5,.5,1};
		GLfloat	diffuse[] = {.5,.5,.5,1};
		GLfloat	specular[] = {1,1,1,1};
		GLfloat	shininess = 0.1*128.0;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	}

	
	// renders the plane
	render_quads();

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'X':
		spotlight.x += 0.1;
		break;
	case 'x':
		spotlight.x -= 0.1;
		break;
	case 'Y':
		spotlight.y += 0.1;
		break;
	case 'y':
		spotlight.y -= 0.1;
		break;
	case 'Z':
		spotlight.z += 0.1;
		break;
	case 'z':
		spotlight.z -= 0.1;
		break;
	case 'C':
		spotlight.cutoff = MIN(spotlight.cutoff+5, 90);
		break;
	case 'c':
		spotlight.cutoff = MAX(spotlight.cutoff-5, 0);
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
		spotlight.rot_y = MIN(spotlight.rot_y+5, 90);
		break;
	case GLUT_KEY_RIGHT:
		spotlight.rot_y = MAX(spotlight.rot_y-5, -90);
		break;
	case GLUT_KEY_UP:
		spotlight.rot_x = MIN(spotlight.rot_x+5, 90);
		break;
	case GLUT_KEY_DOWN:
		spotlight.rot_x = MAX(spotlight.rot_x-5, -90);
		break;
	}
	glutPostRedisplay();
}

void init_light(void)
{
	GLfloat	ambient[] = {.2,.2,.2,1};
	GLfloat	diffuse[] = {1,1,1,1};
	GLfloat	specular[] = {1,1,1,1};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	spotlight.x = 0;
	spotlight.y = 0;
	spotlight.z = 1;
	spotlight.cutoff = 10;
}
void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// Since the projection never changes, 
	// it needs to be set only once at the beginning.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	init_light();

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
	glutCreateWindow("hello");

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}
