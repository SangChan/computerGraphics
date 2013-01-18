//
// gouraud2.c
//
// To demonstrate the Gouraud shading
//
// page up/down: increase/decrease the resolution of the quads
// left/right: rotate the quad about the z-axis
// up/down: change the normal direction at the corner of the quad`
// 
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#ifndef	WINDOWS
#	include <stdlib.h>
#endif
#ifndef	PI
#	define	PI	3.14159
#endif
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	DEG2RAD(d)	((d)*PI/180.0)
GLint	resol = 10;
#define	FLAG_NORMAL	(1<<0)
#define	SQRLEN3(n)	(n[0]*n[0]+n[1]*n[1]+n[2]*n[2])
GLint	flag = FLAG_NORMAL;
GLfloat	cutoff = 10;
GLfloat	norm = 0;
GLfloat rot = 0;

GLfloat	nedge_angle = 0;	// angle of the `nedge' between the z-axis
GLfloat	nedge[] = {0,0,1};	// normal vector at the corner (calculated by nedge_angle)

void interpolate_normal(GLfloat x, GLfloat y, GLfloat n[3])
{
	n[0] = x*nedge[0];
	n[1] = y*nedge[1];
	n[2] = (1.0-x)*(1.0-y) + (x+y-x*y)*nedge[2];
}

void render_quarter(int n)
{
	int	i, j;
	GLfloat	step = 1.0/((GLfloat)n-1);
	GLfloat	x[2];
	GLfloat	y[2];
	GLfloat	normal[2][2][3];
	for(i = 0 ; i < n-1 ; i++)
	{
		for(j = 0 ; j < n-1 ; j++)
		{
			x[0] = step*((GLfloat)i);
			x[1] = step*((GLfloat)(i+1));
			y[0] = step*((GLfloat)j);
			y[1] = step*((GLfloat)(j+1));

			interpolate_normal(x[0],y[0],normal[0][0]);
			interpolate_normal(x[0],y[1],normal[0][1]);
			interpolate_normal(x[1],y[0],normal[1][0]);
			interpolate_normal(x[1],y[1],normal[1][1]);

#define	SET_VERT(ix,iy)				\
	glNormal3fv(normal[ix][iy]);	\
	glVertex3f(x[ix],y[iy],0);
#define	SCALE	.2
			if(flag & FLAG_NORMAL)
			{
				glDisable(GL_LIGHTING);
				glColor3f(1,1,1);
				glBegin(GL_LINES);
					glVertex3f(x[0],y[0],0);	glVertex3f(x[0]+SCALE*normal[0][0][0], y[0]+SCALE*normal[0][0][1], SCALE*normal[0][0][2]);
					glVertex3f(x[0],y[1],0);	glVertex3f(x[0]+SCALE*normal[0][1][0], y[1]+SCALE*normal[0][1][1], SCALE*normal[0][1][2]);
					glVertex3f(x[1],y[0],0);	glVertex3f(x[1]+SCALE*normal[1][0][0], y[0]+SCALE*normal[1][0][1], SCALE*normal[1][0][2]);
					glVertex3f(x[1],y[1],0);	glVertex3f(x[1]+SCALE*normal[1][1][0], y[1]+SCALE*normal[1][1][1], SCALE*normal[1][1][2]);
				glEnd();
				glEnable(GL_LIGHTING);
			}
			glBegin(GL_QUADS);
				SET_VERT(0,0);
				SET_VERT(1,0);
				SET_VERT(1,1);
				SET_VERT(0,1);
			glEnd();
		}
	}
}

void render_quads(void)
{
	render_quarter(resol);
	glRotatef(90,0,0,1);
	render_quarter(resol);
	glRotatef(90,0,0,1);
	render_quarter(resol);
	glRotatef(90,0,0,1);
	render_quarter(resol);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,-1,-5);
	glRotatef(-70, 1, 0, 0);
	glRotatef(rot, 0, 0, 1);

	{
		GLfloat	position[] = {0,0,1,0};	//directional light
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}

	render_quads();

	glutSwapBuffers();
}
void calc_nedge(void)
{
	nedge[2] = cos(DEG2RAD(nedge_angle));
	nedge[0] = nedge[1] = sin(DEG2RAD(nedge_angle))/sqrt(2.0);
//	printf("angle=%g nedge = (%g, %g, %g) length=%g \n", nedge_angle, nedge[0], nedge[1], nedge[2], SQRLEN3(nedge));
}

void init (void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 10);

	glShadeModel(GL_SMOOTH);
	{
		GLfloat	ambient[] = {.2,.2,.2,1};
		GLfloat	diffuse[] = {1,1,1,1};
		GLfloat	specular[] = {1,1,1,1};

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	{
		GLfloat	ambient[] = {0,0,0,1};
		GLfloat	diffuse[] = {.5,.5,.5,1};
		GLfloat	specular[] = {1,1,1,1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, .8*128.0);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	calc_nedge();
}


void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_PAGE_UP:
		resol++;
		break;
	case GLUT_KEY_PAGE_DOWN:
		resol = MAX(1, resol-1);
		break;
	case GLUT_KEY_UP:
		nedge_angle = MIN(90, nedge_angle+5);
		calc_nedge();
		break;
	case GLUT_KEY_DOWN:
		nedge_angle = MAX(0, nedge_angle-5);
		calc_nedge();
		break;
	case GLUT_KEY_RIGHT:
		rot += 10;
		break;
	case GLUT_KEY_LEFT:
		rot -= 10;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'n':
	case 'N':
		if(flag & FLAG_NORMAL)	flag -= FLAG_NORMAL;
		else					flag += FLAG_NORMAL;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Gouraud shading");
	init();
	glutDisplayFunc(display); 
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
