#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>

#define	MENU_0	0
#define	MENU_1	1
#define	MENU_2	2
#define	MENU_3	3
#define	MENU_4	4
#define	MENU_1_1	5
#define	MENU_1_2	6
#define	MENU_1_1_1	7
#define	MENU_1_1_2	8
#define	MENU_3_1	9
#define	MENU_3_2	10

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
void menu(int id)
{
	char	menu_name[32];
	switch(id)
	{
	case MENU_0:
		strcpy(menu_name, "0");
		break;
	case MENU_1:
		strcpy(menu_name, "1");
		break;
	case MENU_2:
		strcpy(menu_name, "2");
		break;
	case MENU_3:
		strcpy(menu_name, "3");
		break;
	case MENU_4:
		strcpy(menu_name, "4");
		break;
	case MENU_1_1:
		strcpy(menu_name, "1.1");
		break;
	case MENU_1_2:
		strcpy(menu_name, "1.2");
		break;
	case MENU_1_1_1:
		strcpy(menu_name, "1.1.1");
		break;
	case MENU_1_1_2:
		strcpy(menu_name, "1.1.2");
		break;
	case MENU_3_1:
		strcpy(menu_name, "3.1");
		break;
	case MENU_3_2:
		strcpy(menu_name, "3.2");
		break;
	}
	printf("Menu #%s is selected!\n", menu_name);
}

void init_menu(void)
{
	int	menu_top;
	int	menu_1;
	int	menu_1_1;
	int	menu_3;

	menu_1_1 = glutCreateMenu(menu);
	glutAddMenuEntry("menu #1.1.1", MENU_1_1_1);
	glutAddMenuEntry("menu #1.1.2", MENU_1_1_2);


	menu_1 = glutCreateMenu(menu);
	glutAddSubMenu("menu #1.1", menu_1_1);
	glutAddMenuEntry("menu #1.2", MENU_1_2);

	menu_3 = glutCreateMenu(menu);
	glutAddMenuEntry("menu #3.1", MENU_3_1);
	glutAddMenuEntry("menu #3.2", MENU_3_2);

	menu_top = glutCreateMenu(menu);
	glutAddMenuEntry("menu #0", MENU_0);
	glutAddSubMenu("menu #1", menu_1);
	glutAddMenuEntry("menu #2", MENU_2);
	glutAddSubMenu("menu #3", menu_3);
	glutAddMenuEntry("menu #4", MENU_4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	init_menu();
	glutMainLoop();
	return 0;
}
