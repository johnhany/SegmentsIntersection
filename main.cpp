/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 *	------------Sweep Algorithm For Segments Intersection-------------
 *
 *	The program updates two structures during the whole algorithm:
 *	-Link list "POrder{}" saves all the points and intersection points (ordered list)
 *	-Binary tree "tree{}" saves the event points being scanned at present (scan tree)
 *
 *	The ordered list is arranged by Y axis decreasingly.
 *	The program scans from start of the list "POrder" to the end.
 *	When detecting a new intersection point, it will be inserted into the ordered list.
 *	For the lines that are being scanned, their upper points, lower points
 *	parameters(k and b) and intersection points are saved in the "POrder" node.
 *
 *	The scan tree is arranged by X axis increasingly.
 *	For each node in scan tree, (scan->left->id->x) < (scan->id->x) < (scan->right->id->x).
 *	The "id" value in "tree" node is a pointer towards certain node in ordered list.
 *
 *	-------------------------------------------------------------------
 *
 *	When scanning the "POrder" list, the program does such three things:
 *
 *	(1).For an upper point, add a new "scan" node in the scan tree, pointing to the point.
 *	Get its left and right neighbors in the scan tree. 
 *	Then check intersection between it and its two neighbors.
 *	If there is any intersection point, add it into the ordered list,
 *	and they will be processed in the future.
 *
 *	(2).For a lower point, search for its upper point in the scan tree and
 *	get left and right neighbors of the upper point.
 *	Delete the upper point from the tree, then check intersection between the two neighbors.
 *	If there is any intersection point, add it into the ordered list to be processed in the future.
 *
 *	(3).For an intersection point, get the left neighbor of the left line and
 *	right neighbor of the right line.
 *	Switch the order of two lines of the same intersection point in the scan tree.
 *	Check intersection between its left line(the new right line) and its right neighbor
 *	Check intersection between its right line(the new left line) and its left neighbor.
 *	
 */

#include "stdafx.h"
#include "windows.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"
#include <math.h>
#include "findInte.h"

const GLint screenWidth=800,screenHeight=600;

int found[NUM_INTE][3];
float points[NUM_PT][2];
float inte[NUM_INTE][2];
int _inteCount;

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_FLAT);
	glLineWidth(1.0);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);

	findIntersection(points);

	glFlush();
}

void reshape(int width,int height)
{
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)width,0.0,(GLdouble)height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(screenWidth,screenHeight);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Look!");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	myInit();
	glutMainLoop();
}
