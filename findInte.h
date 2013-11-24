/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#ifndef __FIND_INTE_H
#define __FIND_INTE_H

#include "scanTree.h"

#define NUM_INTE 80

//found[i][0] and found[i][1] are intersecting lines found,
//found[i][2] is for deleting intersection point node from scan tree
//found[i][2]==FND_2: both two intersection lines are not deleted from scan tree
//found[i][2]==FND_1: one of the two lines is still in the scan tree, keep the inte-node in scan tree
//found[i][2]==FND_0: both two lines are just deleted from scan tree, inte-node should be deleted
//found[i][2]==FND_D: all of two lines and inte-node are already deleted, nothing to be done
extern int found[NUM_INTE][3];
#define FND_2 6
#define FND_1 7
#define FND_0 8
#define FND_D 9

//point pt[i] and pt[i+1] belong to the same line
extern float points[NUM_PT][2];
//save the intersection points
extern float inte[NUM_INTE][2];
//number of intersection points found at present
extern int _inteCount;

int intePt(float xa1,float ya1,float xa0,float ya0,float xb1,float yb1,float xb0,float yb0,int n1,int n2,float x_scan,float y_scan);
void checkInte(POrder *head,POrder *left,POrder *right,tree *top,float x_scan,float y_scan);
void findIntersection(float pt[][2]);

#endif
