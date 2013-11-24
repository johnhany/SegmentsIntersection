/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#ifndef __PORDER_H
#define __PORDER_H

//type: 1 for upper point, 0 for lower point, -1 for intersection point
//k!=-1: x=k*y+b
//k==-1 && b<0: y=-screenWidth*b
struct POrder
{
	float x;
	float y;
	float k;
	float b;
	int type;
	int l;
	int l2;
	struct POrder *next;
	struct POrder *prior;
};

#define ORD_UPPER 0
#define ORD_LOWER 1
#define ORD_INTE 2

#define NUM_PT 30

#define NO_LINE -1

#define LEN_P sizeof(struct POrder)

//number of points in ordered list at present
extern int _maxPtOrd;

POrder *addIntoOrdered(POrder *head,float x,float y,float k,float b,int nl,int nr,int type);
void initPt(POrder *ordHead,float pt[][2]);

#endif
