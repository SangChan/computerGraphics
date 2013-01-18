#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
//#include <iostream>

#define WINDOW_WIDTH	512
#define WINDOW_HEIGHT	512

#define LARGE_CUBE_SIZE 12  
#define INPUT_NO_DIRECTION 0
#define INPUT_UP           1
#define INPUT_DOWN         2
#define INPUT_LEFT         3
#define INPUT_RIGHT        4
#define INPUT_ENABLE       5
#define INPUT_DISABLE      6

#define SHAPE_CUBE   0
#define SHAPE_TEAPOT 1
#define SHAPE_SPHERE 2

#define VISIBLE   0
#define UNVISIBLE 1

#define IN_CONTROL   0
#define IN_ANIMATION 1

#define HIT  0
#define FAIL 1

#define CLAW_OPEN  0
#define CLAW_CLOSE 1

struct ShapeObject {
	GLfloat	point[3]; //x,y,z
	GLfloat size[3]; //w,h,d
	GLfloat	color[3];
	int shape;
	int visible;
}stObj[5] = {
	{
		{
			0, -(LARGE_CUBE_SIZE /2 ) + 1, 0
		},
		{
			1,1,1
		},
		{
			1,1,0
		},
		SHAPE_TEAPOT,
		VISIBLE	
	},
	{
		{
			3, -(LARGE_CUBE_SIZE /2 ) + 1, 3
		},
		{
			1.5,1.5,1.5
		},
		{
			0,0,1
		},
		SHAPE_CUBE,
		VISIBLE
	},
	{
		{
			-3, -(LARGE_CUBE_SIZE /2 ) + 1, -3
		},
		{
			1.5,1.5,1.5
		},
		{
			1,0,1
		},
		SHAPE_CUBE,
		VISIBLE
	},
	{
		{
			-3, -(LARGE_CUBE_SIZE /2 ) + 1, 3
		},
		{
			1,1,1
		},
		{
			0,1,0
		},
		SHAPE_SPHERE,
		VISIBLE
	},
	{
		{
			3, -(LARGE_CUBE_SIZE /2 ) + 1, -3
		},
		{
			1,1,1
		},
		{
			0,1,1
		},
		SHAPE_SPHERE,
		VISIBLE
	}
	
};


struct Rectangle {
	GLfloat top;
	GLfloat bottom;
	GLfloat left;
	GLfloat right;
}rcSrc,rcDest;


struct ArmObjectData {
	GLfloat x;
	GLfloat z;
	int visible;
	int inputDirection;
	int inputEnable;
	int inAnimation;
	int counter;
}armData;


typedef struct ArmPart {
	GLfloat	angle[3];
	GLfloat	width;
	GLfloat	height;
	GLfloat deep;
	GLfloat translate[3];
	GLfloat	color[3];
	int shape;
}t_arm;


t_arm band = {
	{
		0,0,0
	},1.2,0.8,0.7,
	{
		0,0,0
	},
	{
		1,0,0
	},SHAPE_CUBE
};

t_arm hand = {
	{
		0,0,0
	},0.5,1.0,0.5,
	{
		0,0,0
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};

t_arm claw1 = {
	{
		0,0,-60
	},0.2,1.2,0.5,
	{
		-0.5,0,0
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw2 = {
	{
		0,0,60
	},0.2,1.2,0.5,
	{
		0.5,0,0
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw3 = {
	{
		60,0,0
	},0.5,1.2,0.2,
	{
		0,0,-0.5
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw4 = {
	{
		-60,0,0
	},0.5,1.2,0.2,
	{
		0,0,0.5
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw5 = {
	{
		0,0,60
	},0.2,1.2,0.5,
	{
		-0.5,0,0
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw6 = {
	{
		0,0,-60
	},0.2,1.2,0.5,
	{
		0.5,0,0
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw7 = {
	{
		-60,0,0
	},0.5,1.2,0.2,
	{
		0,0,-0.5
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};
t_arm claw8 = {
	{
		60,0,0
	},0.5,1.2,0.2,
	{
		0,0,0.5
	},
	{
		0.5,0.5,0.5
	},SHAPE_CUBE
};

t_arm hanggedObject = {
	{
		0,0,0
	},1,1,1,
	{
		0,0,0
	},
	{
		0,0,0
	},SHAPE_CUBE
};

void initDefault(void)
{
	armData.x = 0.0;
	armData.z = 0.0;
	armData.visible = UNVISIBLE;
	armData.inputDirection = INPUT_NO_DIRECTION;
	armData.inputEnable = INPUT_ENABLE;
	armData.inAnimation = IN_CONTROL;
	armData.counter = 0;
	
}

int isCollinsion(struct Rectangle source, struct Rectangle dest)
{
	if (source.left <= dest.right && source.right >= dest.left) {
		if (source.top <= dest.bottom && source.bottom >= dest.top) {
			return HIT;
		}
	}
	return FAIL;
}


void rotate_arm(t_arm *p_arm, GLfloat angle[])
{
	int i;
	for (i = 0; i < 3; i++) {
		p_arm->angle[i] += angle[i];
		if(p_arm->angle[i] > 0 && p_arm->angle[i] > 360)	p_arm->angle[i] -= 360;
		else if(p_arm->angle[i] < 0)	p_arm->angle[i] += 360;
	}
	
	
}

void stretch_arm(t_arm *p_arm, GLfloat height)
{
	p_arm->height += height;
}

void drawObject(struct ShapeObject p_obj)
{
	if (p_obj.visible == VISIBLE) {
		glLoadIdentity();
		glTranslatef(p_obj.point[0], p_obj.point[1], p_obj.point[2]);
		glColor3f(1,1,1);
		if (p_obj.shape == SHAPE_CUBE) {
			glutWireCube(p_obj.size[0]);
		}
		else if (p_obj.shape == SHAPE_TEAPOT) {
			glutWireTeapot(p_obj.size[0]);
		}
		else {
			glutWireSphere(p_obj.size[0],15,15);
		}
		glColor4f(p_obj.color[0],p_obj.color[1],p_obj.color[2],1);
		if (p_obj.shape == SHAPE_CUBE) {
			glutSolidCube(p_obj.size[0]);
		}
		else if (p_obj.shape == SHAPE_TEAPOT) {
			glutSolidTeapot(p_obj.size[0]);
		}
		else {
			glutSolidSphere(p_obj.size[0],15,15);
		}
	}
}

void moveArm(void)
{
	if (armData.inputDirection == INPUT_LEFT) {
		armData.x -= 0.05;
		if (armData.x <= -(LARGE_CUBE_SIZE /2 )) {
			armData.x = -(LARGE_CUBE_SIZE /2 );
		}
	}
	else if (armData.inputDirection == INPUT_RIGHT) {
		armData.x += 0.05;
		if (armData.x >= (LARGE_CUBE_SIZE /2 )) {
			armData.x = (LARGE_CUBE_SIZE /2 );
		}
	}
	else if (armData.inputDirection == INPUT_UP) {
		armData.z -= 0.05;
		if (armData.z <= -(LARGE_CUBE_SIZE /2 )) {
			armData.z = -(LARGE_CUBE_SIZE /2 );
		}
	}
	else if (armData.inputDirection == INPUT_DOWN) {
		armData.z += 0.05;
		if (armData.z >= (LARGE_CUBE_SIZE /2 )) {
			armData.z = (LARGE_CUBE_SIZE /2 );
		}
	}
	glutPostRedisplay();
}

void grabClaw(int mode) {
	GLfloat angle[3] = {
		0,0,0
	} ;
	if (mode == CLAW_OPEN) {
		angle[2] = -1;
		rotate_arm(&claw1, angle);
		angle[2] = 1;
		rotate_arm(&claw2, angle);
		angle[2] = 0;
		angle[0] = -1;
		rotate_arm(&claw4, angle);
		angle[0] = 1;
		rotate_arm(&claw3, angle);
		angle[0] = 0;
	}
	else {
		angle[2] = 1;
		rotate_arm(&claw1, angle);
		angle[2] = -1;
		rotate_arm(&claw2, angle);
		angle[2] = 0;
		angle[0] = 1;
		rotate_arm(&claw4, angle);
		angle[0] = -1;
		rotate_arm(&claw3, angle);
	}

}

void downArm(void)
{
	int i;
	
	if (armData.inputDirection == INPUT_NO_DIRECTION && armData.inputEnable == INPUT_DISABLE && armData.inAnimation == IN_ANIMATION) 
	{
		if (armData.counter <= 170) {
			/*if (armData.visible == VISIBLE){
				if (armData.counter > 141 && armData.counter <= 170) {
					angle[2] = -1;
					rotate_arm(&claw1, angle);
					angle[2] = 1;
					rotate_arm(&claw2, angle);
					angle[2] = 0;
					angle[0] = -1;
					rotate_arm(&claw4, angle);
					angle[0] = 1;
					rotate_arm(&claw3, angle);
					angle[0] = 0;
				}
			}*/
			if (armData.visible == UNVISIBLE) {
				if (armData.counter < 30) {
					grabClaw(CLAW_OPEN);
				}
			}

			
			stretch_arm(&hand,0.05);
		}
		else if (armData.counter > 170 && armData.counter <= 200) {
			if (armData.visible == UNVISIBLE) {
				grabClaw(CLAW_CLOSE);
			}
			else {
				grabClaw(CLAW_OPEN);
			}
			if (armData.counter == 200) {
				rcSrc.top = armData.z - 0.3;
				rcSrc.left = armData.x - 0.3;
				rcSrc.right = armData.x + 0.3;
				rcSrc.bottom = armData.z + 0.3;
				if (armData.visible == VISIBLE) {
					for (i = 0; i < 5; i++) {
						if (stObj[i].visible == UNVISIBLE) {
							stObj[i].visible = VISIBLE;
							armData.visible = UNVISIBLE;
							stObj[i].point[0] = armData.x;
							stObj[i].point[2] = armData.z;
						}
					}
				}
				else {
				
				
					for (i = 0; i < 5; i++) {
						rcDest.top    = stObj[i].point[2] - 0.5;
						rcDest.left   = stObj[i].point[0] - 0.5;
						rcDest.right  = stObj[i].point[0] + 0.5;
						rcDest.bottom = stObj[i].point[2] + 0.5;
						if (isCollinsion(rcSrc,rcDest) == HIT && stObj[i].visible == VISIBLE && armData.visible == UNVISIBLE) {
							stObj[i].visible = UNVISIBLE;
							armData.visible = VISIBLE;
							hanggedObject.shape = stObj[i].shape;
							hanggedObject.width = stObj[i].size[0];
							hanggedObject.height = stObj[i].size[1];
							hanggedObject.deep = stObj[i].size[2];
							hanggedObject.color[0] = stObj[i].color[0];
							hanggedObject.color[1] = stObj[i].color[1];
							hanggedObject.color[2] = stObj[i].color[2];
						}
					}
				}
			}
		}
		else if (armData.counter > 200 && armData.counter <= 371) {
			if (armData.counter > 200 && armData.counter <= 230) {
				if (armData.visible == UNVISIBLE) {
					grabClaw(CLAW_CLOSE);
				}
			}
			stretch_arm(&hand,-0.05);
		}

		else {
			armData.inputEnable = INPUT_ENABLE;
			armData.inAnimation = IN_CONTROL;
		}
		
				
		armData.counter++;
	}
}

void drawCage(void)
{
	glLoadIdentity();
	glScalef(1,1,1);
	glTranslatef(0, 0, 0);
		
	glColor3f(1,1,1);
	glutWireCube(LARGE_CUBE_SIZE);
	
	glColor3f(0.1,0.5,0.5);
	glBegin(GL_POLYGON);
	glVertex3f (-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ));
	glVertex3f ( (LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ));
	glVertex3f ( (LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ), (LARGE_CUBE_SIZE /2 ));
	glVertex3f (-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ), (LARGE_CUBE_SIZE /2 ));	
	glEnd(); 
}

void drawArmUnit(GLfloat color[], int shape)
{
	glColor3f(1,1,1);
	if (shape == SHAPE_CUBE) {
		glutWireCube(1);
	}
	else if (shape == SHAPE_TEAPOT) {
		glutWireTeapot(1);
	}
	else {
		glutWireSphere(1,15,15);
	}
	glColor4f(color[0],color[1],color[2],1);
	if (shape == SHAPE_CUBE) {
		glutSolidCube(1);
	}
	else if (shape == SHAPE_TEAPOT) {
		glutSolidTeapot(1);
	}
	else {
		glutSolidSphere(1,15,15);
	}
	
}

void rotateArmUnit(GLfloat angle[])
{
	glRotatef(angle[0],1,0,0);
	glRotatef(angle[1],0,1,0);
	glRotatef(angle[2],0,0,1);
}

void renderArm(t_arm* p_arm)
{
	rotateArmUnit(p_arm->angle);
	glTranslatef(0,-(p_arm->height/2),0);
	glPushMatrix();
		glScalef(p_arm->width,p_arm->height,p_arm->deep);
		glTranslatef(p_arm->translate[0],p_arm->translate[1],p_arm->translate[2]);
		drawArmUnit(p_arm->color,p_arm->shape);
	glPopMatrix();
	glTranslatef(0,-(p_arm->height/2),0);
}

void drawArmBand(void)
{
	glLoadIdentity();
	glLineWidth(5.0);
	glColor3f(0.2,0.3,0.8);
	glBegin(GL_LINE_LOOP);
		glVertex3f (-(LARGE_CUBE_SIZE /2 ),(LARGE_CUBE_SIZE /2 ),armData.z);
		glVertex3f ( (LARGE_CUBE_SIZE /2 ),(LARGE_CUBE_SIZE /2),armData.z);
	glEnd(); 
	
	glColor3f(0.2,0.3,0.8);
	glBegin(GL_LINE_LOOP);
		glVertex3f (armData.x, (LARGE_CUBE_SIZE /2),-(LARGE_CUBE_SIZE /2 ));
		glVertex3f (armData.x, (LARGE_CUBE_SIZE /2), (LARGE_CUBE_SIZE /2 ));
	glEnd(); 
	
	glLineWidth(1.0);
}

void display(void)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	
	drawCage();
	
	for (i = 0; i < 5; i++) {
		drawObject(stObj[i]);
	}
	
	drawArmBand();
	glLoadIdentity();
	glTranslatef(armData.x, (LARGE_CUBE_SIZE /2)+0.5, armData.z);
	renderArm(&band);
	glRotatef(45,0,1,0);
	renderArm(&hand);
	
	glPushMatrix();
		renderArm(&claw1);
		renderArm(&claw5);
	glPopMatrix();
	
	glPushMatrix();
		renderArm(&claw2);
		renderArm(&claw6);
	glPopMatrix();
	
	glPushMatrix();
		renderArm(&claw3);
		renderArm(&claw7);
	glPopMatrix();
	
	glPushMatrix();
		renderArm(&claw4);
		renderArm(&claw8);
	glPopMatrix();
	
	
	if (armData.visible == VISIBLE) {
		glTranslatef(0,-0.5,0);
		glRotatef(-45,0,1,0);
		renderArm(&hanggedObject);
	}
	//glutPostRedisplay();
	glutSwapBuffers();
	
}

void keyboard(unsigned char key, int x, int y)
{
	if (armData.inputEnable == INPUT_ENABLE) 
	{
		switch(key)
		{
			case 32:
				armData.inputDirection = INPUT_NO_DIRECTION;
				armData.inputEnable = INPUT_DISABLE;
				armData.inAnimation = IN_ANIMATION;
				armData.counter = 0;
				break;
			case 'q':
			case 'Q':
			case 27:
				exit(0);
		}
		glutPostRedisplay();
	}
}


void special(int key, int x, int y)
{
	if (armData.inputEnable == INPUT_ENABLE) 
	{
		switch(key)
		{	
			case GLUT_KEY_LEFT:
				if (armData.inputDirection == INPUT_LEFT) {
					armData.inputDirection = INPUT_NO_DIRECTION;
				}
				else {
					armData.inputDirection = INPUT_LEFT;
				}
				break;
			case GLUT_KEY_RIGHT:
				if (armData.inputDirection == INPUT_RIGHT) {
					armData.inputDirection = INPUT_NO_DIRECTION;
				}
				else {
					armData.inputDirection = INPUT_RIGHT;
				}
				break;
			case GLUT_KEY_UP:
				if (armData.inputDirection == INPUT_UP) {
					armData.inputDirection = INPUT_NO_DIRECTION;
				}
				else {
					armData.inputDirection = INPUT_UP;
				}
				break;
			case GLUT_KEY_DOWN:
				if (armData.inputDirection == INPUT_DOWN) {
					armData.inputDirection = INPUT_NO_DIRECTION;
				}
				else {
					armData.inputDirection = INPUT_DOWN;
				}
				break;
		}
	}
}

void reshape(int width, int height)
{
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (float)width / (float)height , 1, 100);
	gluLookAt(0,15, 20, 0, 0, 0, 0, 1, 0);
	
}

void timer(int input)
{
	moveArm();
	downArm();
	glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv)
{	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
	glutInitWindowPosition (100, 100);
	
	glutCreateWindow ("cg_prj2_2002570045");
	
	glClearColor (0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);
	
	glViewport(0,0,(GLsizei)WINDOW_WIDTH,(GLsizei)WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1, 100);
	gluLookAt(0,15, 20, 0, 0, 0, 0, 1, 0);
	
	initDefault();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	
	glutMainLoop();
	return 0;
	
}
