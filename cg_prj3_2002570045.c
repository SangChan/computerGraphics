#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLUT/glut.h>

#include "bricks.c"
#include "glm.h"
//#include <iostream>

//

#define	PI	3.14159
#define	MAX(x,y)	((x)>(y)?(x):(y))
#define	MIN(x,y)	((x)<(y)?(x):(y))
#define	DEG2RAD(d)	((d)*PI/180.0)

#define WINDOW_WIDTH	768
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
#define SHAPE_SOCCER 3

#define VISIBLE   0
#define UNVISIBLE 1

#define IN_CONTROL   0
#define IN_ANIMATION 1

#define HIT  0
#define FAIL 1

#define CLAW_OPEN  0
#define CLAW_CLOSE 1

/*	Create checkerboard texture	*/
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif


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
		SHAPE_SOCCER,
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
	GLfloat y;
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

struct struct_light
{
	GLfloat	x, y, z;
	GLfloat	cutoff;
	GLfloat	rot_x, rot_y;
} spotlight, drlight;

GLMmodel* pmodel = 0;

//GLuint resol = 50;
void init_object(void) 
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//glLightfv(GL_LIGHT, GL_POSITION, light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	//glEnable(GL_DEPTH_TEST);
	
   	{
        pmodel = glmReadOBJ("soccerball.obj");
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
	}
	
}

void makeCheckImage(void)
{
	int i, j, c;
    
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}

void init_texture(void)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	#ifdef GL_VERSION_1_1
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	#endif
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	#ifdef GL_VERSION_1_1
	//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
	//              0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image.width, gimp_image.height, 
				 0, GL_RGB, GL_UNSIGNED_BYTE, gimp_image.pixel_data);
	#else
	glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 
				 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	#endif
}


void init_light(void)
{
	spotlight.x = armData.x;
	spotlight.y = (LARGE_CUBE_SIZE /2 );
	spotlight.z = armData.z;
	spotlight.cutoff = 60;	
	
	GLfloat	ambient[] = {0.2,0.2,0.2,1};
	GLfloat	diffuse[] = {0.8,0.8,0.8,1};
	GLfloat	specular[] = {0.8,0.8,0.8,1};
	
	GLfloat	position[] = {armData.x,(LARGE_CUBE_SIZE /2 ),armData.z,1};
	GLfloat	direction[] = {0,-1,0};
	
	GLfloat ambient1[] = {0.6,0.6,0.6,1};
	GLfloat diffuse1[] = {0.8,0.8,0.8,1};
	GLfloat specular1[] = {0.8,0.8,0.8,1};
	
	GLfloat	position1[] = {0.0,9.0,0.0,0};
	GLfloat	direction1[] = {0,-1,0};
			
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	
	glEnable(GL_LIGHT1);
	
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 360);
	
	
	glEnable(GL_LIGHTING);

}

void initDefault(void)
{
	armData.x = 0.0;
	armData.y = (LARGE_CUBE_SIZE /2 )-1;
	armData.z = 0.0;
	armData.visible = UNVISIBLE;
	armData.inputDirection = INPUT_NO_DIRECTION;
	armData.inputEnable = INPUT_ENABLE;
	armData.inAnimation = IN_CONTROL;
	armData.counter = 0;
	
	init_light();
	init_texture();
	init_object();
	
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
	armData.y -= height;
}

void drawObject(struct ShapeObject p_obj)
{
	GLfloat	ambient[] = {0.24725,0.1995,0.0745,1};
	GLfloat	diffuse[] = {0.75164,0.60648,0.22648,1};
	GLfloat	specular[] = {0.508273,0.508273,0.508273,1};
	GLfloat	shininess = 0.4;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	
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
		else if (p_obj.shape == SHAPE_SPHERE) {
			glutWireSphere(p_obj.size[0],15,15);
		}
		else if (p_obj.shape == SHAPE_SOCCER) {
			glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
		}
		glColor4f(p_obj.color[0],p_obj.color[1],p_obj.color[2],1);
		if (p_obj.shape == SHAPE_CUBE) {
			glutSolidCube(p_obj.size[0]);
		}
		else if (p_obj.shape == SHAPE_TEAPOT) {
			glutSolidTeapot(p_obj.size[0]);
		}
		else if (p_obj.shape == SHAPE_SPHERE) {
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

			if (armData.counter < 30) {
				grabClaw(CLAW_OPEN);
			}

			
			stretch_arm(&hand,0.05);
		}
		else if (armData.counter > 170 && armData.counter <= 200) {
						
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
				grabClaw(CLAW_CLOSE);
				
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
	
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	#ifdef GL_VERSION_1_1
	glBindTexture(GL_TEXTURE_2D, texName);
	#endif	
	glColor3f(0.1,0.5,0.5);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f (-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ));
	glTexCoord2f(0.0, 1.0);
	glVertex3f ( (LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ));
	glTexCoord2f(1.0, 1.0);
	glVertex3f ( (LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ), (LARGE_CUBE_SIZE /2 ));
	glTexCoord2f(1.0, 0.0);
	glVertex3f (-(LARGE_CUBE_SIZE /2 ),-(LARGE_CUBE_SIZE /2 ), (LARGE_CUBE_SIZE /2 ));	
	glEnd(); 
	glDisable(GL_TEXTURE_2D);
		
}

void drawArmUnit(GLfloat color[], int shape)
{
	GLfloat	ambient[] = {0.24725,0.1995,0.0745,1};
	GLfloat	diffuse[] = {0.75164,0.60648,0.22648,1};
	GLfloat	specular[] = {0.508273,0.508273,0.508273,1};
	GLfloat	shininess = 0.4;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	
	glColor3f(1,1,1);
	if (shape == SHAPE_CUBE) {
		glutWireCube(1);
	}
	else if (shape == SHAPE_TEAPOT) {
		glutWireTeapot(1);
	}
	else if (shape == SHAPE_SPHERE) {
		glutWireSphere(1,15,15);
	}
	else if (shape == SHAPE_SOCCER) {
		glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
	}
	glColor4f(color[0],color[1],color[2],1);
	if (shape == SHAPE_CUBE) {
		glutSolidCube(1);
	}
	else if (shape == SHAPE_TEAPOT) {
		glutSolidTeapot(1);
	}
	else if (shape == SHAPE_SPHERE) {
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
	glViewport(0,0,(GLsizei)WINDOW_WIDTH,(GLsizei)WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1, 100);
	gluLookAt(0,15, 20, 0, 0, 0, 0, 1, 0);

	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	
	init_light();
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
		
	glPushAttrib(GL_TRANSFORM_BIT // to backup & restore "current matrix mode"
				 | GL_ENABLE_BIT);	// to backup & restore "depth test" and "lighting" status
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	// not to mess up the projection matrix...
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	// not to mess up the modelview matrix...
	glLoadIdentity();
	{
		char	buf[128];
		int		i;
		sprintf(buf, "claw position: (%f, %f, %f)", armData.x, armData.y ,armData.z);
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
	
	
	glViewport(638,0,150,150);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,150/150,1, 100);
	//glOrtho(, 2.0, -2, 2, 1, 10);
	gluLookAt(armData.x,armData.y,armData.z, armData.x,-6,armData.z, 0, 0, -1);
	
	glMatrixMode(GL_MODELVIEW);
	
	init_light();
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
	
	glutCreateWindow ("cg_prj3_2002570045");
	
	glClearColor (0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);
	
	/*glViewport(0,0,(GLsizei)WINDOW_WIDTH,(GLsizei)WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 1, 100);
	gluLookAt(0,15, 20, 0, 0, 0, 0, 1, 0);*/
	
	initDefault();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	
	glutMainLoop();
	return 0;
	
}
