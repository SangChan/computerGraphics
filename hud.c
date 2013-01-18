
#include <stdlib.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <string.h>
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))

GLfloat	rot_x = 0;
GLfloat	rot_y = 0;
GLuint	obj = 1;
GLUquadric	*p_quad = NULL;
GLint	offset = 1;
GLuint	width, height;
GLint	mouse_x = 0, mouse_y = 0;
GLfloat	fovy = 45;

void render_object(void)
{
	switch(obj)
	{
		case 1:
			glutSolidCube(1);
			break;
		case 2:
			glutSolidSphere(1,24,24);
			break;
		case 3:
			glutSolidTeapot(1);
			break;
		case 4:
			gluCylinder(p_quad, 1, 1, 1, 24, 12);
			break;
	}
}

void render_axes(void)
{
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
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-5);
	glRotatef(rot_x, 1, 0, 0);
	glRotatef(rot_y, 0, 1, 0);

	glDisable(GL_LIGHTING);
	render_axes();

	glEnable(GL_LIGHTING);
	render_object();

	glPushAttrib(GL_TRANSFORM_BIT // to backup & restore "current matrix mode"
				| GL_ENABLE_BIT);	// to backup & restore "depth test" and "lighting" status
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();	// not to mess up the projection matrix...
			glLoadIdentity();
			glOrtho(0, width, 0, height, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();	// not to mess up the modelview matrix...
				glLoadIdentity();
				{
					char	buf[128];
					int		i;
					sprintf(buf, "mouse position: (%i, %i)", mouse_x, mouse_y);
					glColor3f(1,1,1);
					glRasterPos3f(10, 10, 0);
					for(i = 0 ; i < strlen(buf) ; i++)
					{
						glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buf[i]);
					}
				}
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glPopAttrib();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
			obj = key - '0';
			break;
		case 27:
		case 'q':
		case 'Q':
			exit(0);
	}
	glutPostRedisplay();
}

void set_projection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, ((GLfloat)width)/((GLfloat)height), 1, 10);
}

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	set_projection();
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
		case GLUT_KEY_PAGE_UP:
			fovy = MIN(fovy + 5, 180);
			set_projection();
			break;
		case GLUT_KEY_PAGE_DOWN:
			fovy = MAX(fovy - 5, 0);
			set_projection();
			break;
	}
	glutPostRedisplay();
}

void init(void)
{
	{
		GLfloat	position[] = {1,1,1,0};
		GLfloat	ambient[] = {.2,.2,.2,1};
		GLfloat	diffuse[] = {1,1,1,1};
		GLfloat	specular[] = {1,1,1,1};

		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	}
	{
		GLfloat	ambient[] = {0.19125, 0.0735, 0.0225, 1};
		GLfloat	diffuse[] = {0.7038, 0.27048, 0.0828, 1};
		GLfloat	specular[] = {0.256777, 0.137622, 0.086014, 1};
		GLfloat	shininess = 128.0*0.1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	}

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	p_quad = gluNewQuadric();
}

void passive_motion(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("HUD (Head-Up Display)");
	glClearColor (0.0, 0.0, 0.0, 0.0);

	init();

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(passive_motion);
	glutMainLoop();
	return 0;
}
