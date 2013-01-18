#include <math.h>
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
GLint	resol = 1;
#define	FLAG_GRID	(1<<0)
#define	FLAG_CIRCLE	(1<<1)
GLint	flag = FLAG_GRID | FLAG_CIRCLE;
GLfloat	cutoff = 10;

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
void render_circle(GLfloat r)
{
	int	i;
#define	STEPS	48
	GLfloat	angle = 2.0*PI/((GLfloat)STEPS);
	glBegin(GL_LINE_LOOP);
		for(i = 0 ; i < STEPS ; i++)
		{
			glVertex3f(r*cos(angle*((GLfloat)i)), r*sin(angle*((GLfloat)i)), 0);
		}
	glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3f (1.0, 0, 0);
//	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_FILL);
	render_quads();
	if((flag & FLAG_GRID) || (flag & FLAG_CIRCLE))
	{
		glDisable(GL_LIGHTING);
		glColor3f(1,1,1);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(1,1);
		glPolygonMode(GL_FRONT, GL_LINE);
		if(flag & FLAG_GRID)	render_quads();
		if(flag & FLAG_CIRCLE)	render_circle(tan(DEG2RAD(cutoff)));
	}

/* don't wait!  
 * start processing buffered OpenGL routines 
 */
   glFlush ();
}

void refresh_light(void)
{
	GLfloat	p[] = {0,0,1,1};
	GLfloat	dir[] = {0,0,-1};
//		GLfloat	ambient[] = {.01,.01,.01,1};
	GLfloat	ambient[] = {0,0,0,1};
	GLfloat	diffuse[] = {1,1,1,1};
	GLfloat	specular[] = {1,1,1,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, p);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1);
}

void init (void) 
{
/* select clearing color 	*/
   glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.5, 1.5, -1.5, 1.5, -1.0, 1.0);

   refresh_light();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	{
		GLfloat	ambient[] = {0,0,0,1};
		GLfloat	diffuse[] = {1,0,0,1};
		GLfloat	specular[] = {0,0,0,1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 50);
	}
}

void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		resol++;
		break;
	case GLUT_KEY_DOWN:
		resol = MAX(1, resol-1);
		break;
	case GLUT_KEY_RIGHT:
		cutoff = MIN(90, cutoff+2);
		break;
	case GLUT_KEY_LEFT:
		cutoff = MAX(0, cutoff-2);
		break;
	}
	refresh_light();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'l':
	case 'L':
		if(flag & FLAG_GRID)	flag -= FLAG_GRID;
		else					flag += FLAG_GRID;
		break;
	case 'c':
	case 'C':
		if(flag & FLAG_CIRCLE)	flag -= FLAG_CIRCLE;
		else					flag += FLAG_CIRCLE;
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
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (250, 250); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Gouraud shading");
   init ();
   glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
