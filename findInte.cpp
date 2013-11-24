/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#include "stdafx.h"
#include "windows.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"
#include "findInte.h"

//Calculate the coordinate of an intersection point by the coordinates of upper and lower points of two lines.
int intePt(float xa1,float ya1,float xa0,float ya0,float xb1,float yb1,float xb0,float yb0,int n1,int n2,float x_scan,float y_scan)
{
	float s1,s2,s3,s4;
	float a,b;
	//See if points a1 and a0 are at two sides of line [b1,b0].
	s1=(xb0-xa0)*(yb1-ya0)-(xb1-xa0)*(yb0-ya0);
	s2=(xb0-xa1)*(yb1-ya1)-(xb1-xa1)*(yb0-ya1);
	if(s1*s2<=0)
	{
		//One of a1 and a0 on line [b1,b0] is acceptable.
		//See if points b1 and b0 are at two sides of line [a1,a0].
		s3=(xa1-xb0)*(ya0-yb0)-(xa0-xb0)*(ya1-yb0);
		s4=s2-s1+s3;
		if(s3*s4<=0)
		{
			//One of b1 and b2 on line [a1,a0] is acceptable.
			//Calculate the coordinate of the intersection point.

			if(xa0==xa1 || xb0==xb1)
			{
				if(xa0==xa1)
				{
					b=(yb0-yb1)/(xb0-xb1);
					inte[_inteCount][0]=xa0;
					inte[_inteCount][1]=b*(inte[_inteCount][0]-xb1)+yb1;
				}else{
					a=(ya0-ya1)/(xa0-xa1);
					inte[_inteCount][0]=xb0;
					inte[_inteCount][1]=a*(inte[_inteCount][0]-xa1)+ya1;
				}
			}else{
				a=(ya0-ya1)/(xa0-xa1);
				b=(yb0-yb1)/(xb0-xb1);
				inte[_inteCount][0]=(a*xa1-b*xb1-ya1+yb1)/(a-b);
				inte[_inteCount][1]=a*(inte[_inteCount][0]-xa1)+ya1;
			}

			found[_inteCount][0]=n1;
			found[_inteCount][1]=n2;
			found[_inteCount][2]=FND_2;

			_inteCount++;
			return 1;
		}else
			return 0;
	}else
		return 0;
}

//Check whether the two "left" and "right" lines intersect or not. 
//Always input upper points of two lines.
//The function searches for their lower points and pass 4 points to intePt()
//to check intersection.
//If two lines intersect, insert the inte-point node into ordered list,
//save the inte-pt coordinates in inte[], save the numbers of two lines in found[],
//and add a new node pointing to the POrder node in the scan tree.
void checkInte(POrder *head,POrder *left,POrder *right,tree *top,float x_scan,float y_scan)
{
	//a for left line, b for right line.
	//1 for upper point, 0 for lower point.
	float xa1,ya1,xb1,yb1,xa0,ya0,xb0,yb0;
	float x_left,x_right;

	//nlLine for left line(it's on the left just above the intersection point and it will
	//be on the right below the intersection point)
	//nrLine for right line

	int nlLine,nrLine;
	nlLine=left->l;
	nrLine=right->l;

	//In case the intersection point has already been found.
	if(nlLine==nrLine)
		return ;
	int i;
	for(i=0;i<_inteCount;i++)
	{
		if(found[i][0]==nlLine && found[i][1]==nrLine || found[i][0]==nrLine && found[i][1]==nlLine)
			return ;
	}

	xb1=right->x;
	yb1=right->y;

	xa1=left->x;
	ya1=left->y;

	POrder *temp;
	temp=head;
	while(temp!=NULL)
	{
		if(temp->l==nrLine && temp->type==ORD_LOWER)
		{
			xb0=temp->x;
			yb0=temp->y;
			break;
		}
		temp=temp->next;
	}
	temp=head;
	while(temp!=NULL)
	{
		if(temp->l==nlLine && temp->type==ORD_LOWER)
		{
			xa0=temp->x;
			ya0=temp->y;
			break;
		}
		temp=temp->next;
	}

	if(intePt(xa1,ya1,xa0,ya0,xb1,yb1,xb0,yb0,nlLine,nrLine,x_scan,y_scan))
	{
		if(left->k!=-1 || left->b>0){
			x_left=left->k * y_scan + left->b;
		}else{
			x_left=left->x;
		}
		if(right->k!=-1 || right->b>0){
			x_right=right->k * y_scan + right->b;
		}else{
			x_right=right->x;
		}
		POrder *ordInte;
		if(x_left < x_right)
			ordInte=addIntoOrdered(head,inte[_inteCount-1][0],inte[_inteCount-1][1],NO_LINE,NO_LINE,nlLine,nrLine,ORD_INTE);
		else
			ordInte=addIntoOrdered(head,inte[_inteCount-1][0],inte[_inteCount-1][1],NO_LINE,NO_LINE,nrLine,nlLine,ORD_INTE);

		tree *scan_inte;
		scan_inte=(tree *)malloc(LEN_S);
		scan_inte->id=ordInte;

		addIntoScan(top,scan_inte,y_scan);
	}
}

//The main function of the sweep algorithm.
void findIntersection(float pt[][2])
{
	int i,j;

	//"ordHead" doesn't save any data
	//ordHead->next is the first point in ordered list
	POrder *ordHead;
	POrder *ordTemp;
	ordHead = (POrder *)malloc(LEN_P);
	ordHead->next=NULL;

	//Initialize the original points and ordered list.
	initPt(ordHead,pt);

	//Draw the original points and lines.
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glColor3f(0.0,0.0,0.0);
	for(i=0;i<NUM_PT;i+=2){
		glVertex2f(pt[i][0],pt[i][1]);
		glVertex2f(pt[i+1][0],pt[i+1][1]);
	}
	glEnd();
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_LINES);
	for(i=0;i<NUM_PT;i++)
		glVertex2f(pt[i][0],pt[i][1]);
	glEnd();

	//Scan the ordered list and check intersection,
	//"top" is always the root node of the scan tree
	tree *top;
	top=(struct tree *)malloc(LEN_S);
	top->id=ordHead->next;
	top->left=NULL;
	top->right=NULL;
	top->father=NULL;

	_inteCount=0;
	_maxPtOrd=NUM_PT;
	tree *left_s;
	tree *right_s;
	tree *left_l;
	tree *right_l;

	tree *scan_temp;

	ordTemp = ordHead->next->next;
	i=1;
	float x_scan;
	float y_scan;

	while(i<_maxPtOrd)
	{
		if(ordTemp==NULL)
			break;
		tree *scan;
		scan=(struct tree *)malloc(LEN_S);
		scan->id=ordTemp;

		x_scan=scan->id->x;
		y_scan=scan->id->y;

		if(scan->id->type==ORD_UPPER)
		{
			//For an upper point:

			//Add it into scan tree.
			if(top==NULL)
			{
				top=scan;
				scan->id=ordTemp;
				scan->father=NULL;
				scan->left=NULL;
				scan->right=NULL;
			}else
				addIntoScan(top,scan,scan->id->y);

			//Get left and right neighbors in the scan tree.
			left_s=get_left(scan);
			while(left_s!=NULL && left_s->id->type!=ORD_UPPER)
				left_s=get_left(left_s);
			right_s=get_right(scan);
			while(right_s!=NULL && right_s->id->type!=ORD_UPPER)
				right_s=get_right(right_s);

			//Check intersection points.
			if(left_s!=NULL)
				checkInte(ordHead->next,left_s->id,scan->id,top,x_scan,y_scan);
			if(right_s!=NULL)
				checkInte(ordHead->next,scan->id,right_s->id,top,x_scan,y_scan);

		}else if(scan->id->type==ORD_LOWER){
			//For a lower point:

			//Find its upper point in scan tree.
			scan_temp=searchInScan(top,scan->id->l,NO_LINE,SCAN_L);

			//Get left and right neighbors of the upper point in scan tree.
			left_s=get_left(scan_temp);
			while(left_s!=NULL && (left_s->id->type!=ORD_UPPER || left_s==scan))
				left_s=get_left(left_s);
			right_s=get_right(scan_temp);
			while(right_s!=NULL && (right_s->id->type!=ORD_UPPER || right_s==scan))
				right_s=get_right(right_s);

			//Check intersection between left and right neighbors of the deleted line.
			if(left_s!=NULL && right_s!=NULL)
				checkInte(ordHead->next,left_s->id,right_s->id,top,x_scan,y_scan);

			//Delete the upper point.
			top=dltFromScan(top,scan_temp);

			//Find inte-points of the line, and mark them.
			//If both of two inte-lines have been deleted,
			//their inte-points have to be deleted in the scan tree.
			for(j=0;j<_inteCount;j++)
			{
				if(found[j][0]==scan->id->l || found[j][1]==scan->id->l)
				{
					if(found[j][2]==FND_2)
						found[j][2]=FND_1;
					else if(found[j][2]==FND_1){
						scan_temp=searchInScan(top,found[j][0],found[j][1],SCAN_L_L2);
						if(scan_temp!=NULL)
							top=dltFromScan(top,scan_temp);
						found[j][2]=FND_D;
					}
				}
			}
		}else if(scan->id->type==ORD_INTE){
			//For an intersection point:

			//Find its node in the scan tree
			//Because the current "scan" node is not in the scan tree,
			//only its "id" pointer is evaluated.
			scan=searchInScan(top,scan->id->l,scan->id->l2,SCAN_L_L2);

			//Check for inte-points between its left line and its right neighbor,
			//and between its right line and its left neighbor.

			left_l=searchInScan(top,scan->id->l,NO_LINE,SCAN_L);
			right_l=searchInScan(top,NO_LINE,scan->id->l2,SCAN_L2);

			left_s=get_left(left_l);
			while(left_s!=NULL && (left_s->id->type!=ORD_UPPER || left_s==left_l || left_s==right_l))
				left_s=get_left(left_s);
			right_s=get_right(right_l);
			while(right_s!=NULL && (right_s->id->type!=ORD_UPPER || right_s==left_l || right_s==right_l))
				right_s=get_right(right_s);

			switchScan(left_l,right_l);

			if(left_s!=NULL)
				checkInte(ordHead->next,left_s->id,left_l->id,top,x_scan,y_scan);
			if(right_s!=NULL)
				checkInte(ordHead->next,right_l->id,right_s->id,top,x_scan,y_scan);

			//Delete the inte-point node in the scan tree.
			top=dltFromScan(top,scan);
		}
		//Get next point in the ordered list
		ordTemp=ordTemp->next;
		i++;
	}

	//Draw all the intersection points.
	glPointSize(4.0);
	glBegin(GL_POINTS);
	glColor3f(0.0,0.0,1.0);
	for(i=0;i<_inteCount;i++)
		glVertex2f(inte[i][0],inte[i][1]);
	glEnd();
}
