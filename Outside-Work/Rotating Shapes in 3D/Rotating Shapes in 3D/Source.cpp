//Made following tutorial at http://www.videotutorialsrock.com/opengl_tutorial/basic_shapes/text.php

#include <cstdlib> //needed for "exit" function
#include <iostream>
#include "GL/glut.h"

using namespace std;

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //escape key
		exit(0); //exit the program
	}
}

//initialises 3D rendering
void initRendering()
{
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
}

//Called when window is resized
void handleResize(int w, int h)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to the setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,       //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,                   //The near z clipping coordinate
		200.0);            //The far z clipping coordinate
}

float _angle = 30.0f;
float _cameraAngle = 10.0f;

//Draws the 3D scene
void drawScene()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	glRotatef(-_cameraAngle, 0.0f, 1.0f, 0.0f); //Rotate the camera
	glTranslatef(0.0f, 0.0f, -5.0f); //Move forward 5 units

	glPushMatrix(); //Save the transformations performed thus far
	glTranslatef(0.0f, -1.0f, 0.0f); //Move to the center of the trapezoid
	glRotatef(_angle, 0.0f, 0.0f, 1.0f); //Rotate about the z-axis

	glColor3f(0.5f, 0.0f, 0.8f);
	glBegin(GL_QUADS); //Begin quadrilateral coordinates

	//Trapezoid
	glColor3f(0.5f, 0.0f, 0.8f);
	glVertex3f(-0.7f, -1.5f, 0.0f);
	glColor3f(0.0f, 0.9f, 0.0f);
	glVertex3f(0.7f, -1.5f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.5f, 0.0f);
	glColor3f(0.0f, 0.65f, 0.65f);
	glVertex3f(-0.4f, -0.5f, 0.0f);

	glEnd(); //End quadrilateral coordinates

	glPopMatrix(); //Undo the move to the center of the trapezoid
	glPushMatrix(); //Save the current state of transformations
	glTranslatef(1.0f, 1.0f, 0.0f); //Move to the center of the pentagon
	glRotatef(_angle, 0.0f, 1.0f, 0.0f); //Rotate about the y-axis
	glScalef(0.7f, 0.7f, 0.7f); //Scale by 0.7 in the x, y, and z directions

	glBegin(GL_TRIANGLES); //Begin triangle coordinates
	glColor3f(0.0f, 0.75f, 0.0f);

	//Pentagon
	glVertex3f(0.5, 0.5f, 0.0f);
	glVertex3f(1.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 1.0f, 0.0f);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 0.5f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);

	glVertex3f(0.5f, 1.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.5f, 0.0f);

	glEnd(); //End triangle coordinates

	glPopMatrix(); //Undo the move to the center of the pentagon
	glPushMatrix(); //Save the current state of transformations
	glTranslatef(-1.0f, 1.0f, 0.0f); //Move to the center of the triangle
	glRotatef(_angle, 1.0f, 2.0f, 3.0f); //Rotate about the vector (1, 2, 3)

	glColor3f(0.0f, 0.65f, 0.65f);
	glBegin(GL_TRIANGLES);

	//Triangle
	glColor3f(1.0f, 0.7f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.5f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, 0.5f, 0.0f);

	glEnd();

	glPopMatrix(); //Undo the move to the center of the triangle

	glutSwapBuffers(); //Send the 3D scene to the screen
}

void update(int value)
{
	_angle += 2.0f;
	if (_angle > 360)
	{
		_angle -= 360;
	}

	glutPostRedisplay(); //Tell GLUT that the scene has changed

	//Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv)
{
	//Initialise GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialise rendering
	glutTimerFunc(25, update, 0); //Add a timer

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop(); //Start the main loop. glutMainLoop doesn't return.
	return 0; //This line is never reached
}