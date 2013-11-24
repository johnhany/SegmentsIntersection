/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#ifndef __SCAN_TREE_H
#define __SCAN_TREE_H

#include "POrder.h"

struct tree
{
	tree *left;
	tree *right;
	tree *father;
	POrder *id;
};

#define SCAN_UP 3
#define SCAN_L 4
#define SCAN_L2 5
#define SCAN_L_L2 11

#define LEN_S sizeof(struct tree)

void addIntoScan(struct tree *top,struct tree *now,float y_scan);
tree *searchInScan(tree *temp,int line,int line2,int srchType);
struct tree *dltFromScan(tree *top,tree *now);
struct tree *get_left(tree *now);
struct tree *get_right(tree *now);
void switchScan(tree* left_s,tree* right_s);

#endif
