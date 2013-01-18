/*

   sample02.c : OpenGL program with keyboard interaction
   9 Sep 2010 by Minho Kim (minhokim@uos.ac.kr)

 */
#include <iostream>
#include <GLUT/glut.h>

GLint	iCurrentColor = 0;
const GLint	numColor = 3;
GLfloat	color[numColor][3] =
{
	{1,0,0},
	{0,1,0},
	{0,0,1},
};

// "display" callback function.
// This function is called whenever the window needs to be updated (redrawn).
void display(void)
{
	// Clears the "(color) buffer". 
	// See what happens if you comment out this call.
	glClear(GL_COLOR_BUFFER_BIT);	

	// Specifies the "current color" state.
	// All the primitives will be drawn in this color from now on.
	// Try setting other colors yourself.
	glColor3fv(color[iCurrentColor]);

	// Draws a quad (quadrilateral) by specifying
	// the potisions of four corner vertices.
	glBegin(GL_QUADS);
		glVertex2f(-0.5, -0.5);
		glVertex2f( 0.5, -0.5);
		glVertex2f( 0.5,  0.5);
		glVertex2f(-0.5,  0.5);
	glEnd();

	// Empties the "commands queue" and "swaps" the buffers.
	// See what happens if you call "glFlush" instead.
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{

	switch(key)
	{
		case ' ':
			// Circulates the colors.
			iCurrentColor = (iCurrentColor+1)%numColor;

			// Forces the window redrawn.
			// See what happens without this call.
			glutPostRedisplay();
			break;
		case 'q':
		case 'Q':
		case 27:	// ESC key
			exit(0);	// Terminates the program.
	}

}

// Reshape callback function.
// Maps the area (-1,1)x(-1,1) to the center 
// of the screen such that the aspect ratio is kept.
// (How?)
void reshape(int width, int height)
{
	if(width>height)
		glViewport((width-height)/2, 0, height, height);
	else
		glViewport(0, (height-width)/2, width, width);
}

int main(int argc, char** argv)
{
	// Initializes GLUT library. 
	// Should be called at the beginning before any GLUT function is called.
	glutInit(&argc, argv);	

	// Specifies the type of the color buffer. 
	// A double buffer (for animation) with RGB type.
	// See the reference for more details.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Creates the window with given title bar text.
	glutCreateWindow("sample02: a square changing its color");

	// Sets the color used to clear the background.
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// Registers the "display" callback function.
	// Needs to be called after "glutCreateWindow"
	glutDisplayFunc(display); 

	// Registers the keyboard callback function.
	glutKeyboardFunc(keyboard);

	// Registers the reshape callback function.
	// See what happens when resizing the window without this call.
	glutReshapeFunc(reshape);

	// Go into the infinite event loop.
	glutMainLoop();

	return 0;
}
/*
	Try yourself:
 
 	- How is the coordinate system mapped to the window?
	- How can you change the default window size?
	- How can you change the initial position of the window?
	- How can you change the color used to clear the buffer?
	- ...and more
 */



