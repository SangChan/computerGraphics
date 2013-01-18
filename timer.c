#include <GL/glut.h>
#include <stdio.h>
GLfloat	angle = 0;

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (1.0, 0.5, 0.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle, 0, 0, 1);
	glBegin(GL_POLYGON);
		glVertex3f (-1, -1, 0.0);
		glVertex3f (1, -1, 0.0);
		glVertex3f (1, 1, 0.0);
		glVertex3f (-1, 1, 0.0);
	glEnd();
	glutPostRedisplay();
	glutSwapBuffers();
}

void timer(void)
{
	angle += 1;
	if(angle > 360)	angle -= 360;
	glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (250, 250); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -1.0, 1.0);

	glutDisplayFunc(display); 
	glutTimerFunc(10, timer, 0);
	glutMainLoop();
	return 0;
}
