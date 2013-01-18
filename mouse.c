#include <GL/glut.h>
#include <stdio.h>

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f (0.25, 0.25, 0.0);
		glVertex3f (0.75, 0.25, 0.0);
		glVertex3f (0.75, 0.75, 0.0);
		glVertex3f (0.25, 0.75, 0.0);
	glEnd();

	glFlush ();
}

void mouse(int button, int state, int x, int y)
{
	char	btn[32];
	char	stt[32];
	int		modifiers;
	char	mod[128];
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		strcpy(btn, "left");
		break;
	case GLUT_MIDDLE_BUTTON:
		strcpy(btn, "middle");
		break;
	case GLUT_RIGHT_BUTTON:
		strcpy(btn, "right");
		break;
	}
	switch(state)
	{
	case GLUT_DOWN:
		strcpy(stt, "pressed");
		break;
	case GLUT_UP:
		strcpy(stt, "released");
		break;
	}
	modifiers = glutGetModifiers();
	if(modifiers)
	{
		strcpy(mod, " while [");
		if(modifiers & GLUT_ACTIVE_SHIFT)	sprintf(mod, "%s Shift(or Capslock)", mod);
		if(modifiers & GLUT_ACTIVE_CTRL)	sprintf(mod, "%s Ctrl", mod);
		if(modifiers & GLUT_ACTIVE_ALT)		sprintf(mod, "%s Alt", mod);
		sprintf(mod, "%s] is pressed", mod);
	}
	else
	{
		strcpy(mod, "");
	}
	printf("The %s mouse button is %s at (%d,%d)%s.\n", btn, stt, x, y, mod);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (250, 250); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("hello");
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
