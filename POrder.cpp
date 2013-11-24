/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#include "stdafx.h"
#include "windows.h"
#include "POrder.h"

int _maxPtOrd;

//Add a new point into ordered list
POrder *addIntoOrdered(POrder *head,float x,float y,float k,float b,int nl,int nr,int type)
{
	_maxPtOrd++;

	int i=0;
	POrder *temp;
	temp=(POrder *)malloc(LEN_P);
	temp=head;

	POrder *now;
	now=(POrder *)malloc(LEN_P);
	now->l=nl;
	now->l2=nr;
	now->x=x;
	now->y=y;
	now->k=k;
	now->b=b;
	now->type=type;

	while(temp->next!=NULL && (y < temp->next->y || (y==temp->next->y && x >= temp->next->x)))
		temp=temp->next;
	if(temp->next==NULL)
	{
		temp->next=now;
		now->prior=temp;
		now->next=NULL;
	}else{
		now->next=temp->next;
		temp->next->prior=now;
		temp->next=now;
		now->prior=temp;
	}
	return now;
}


//Initialize point list "pt" with random values
//and insert them into ordered list
void initPt(POrder *ordHead,float pt[][2])
{
	int i;
	float kk,bb;

	for(i=0;i<NUM_PT;i+=2)
	{

		pt[i][0]=(float)(rand()%600+100);
		pt[i][1]=(float)(rand()%400+100);
		pt[i+1][0]=(float)(rand()%600+100);
		pt[i+1][1]=(float)(rand()%400+100);

		if(pt[i][1]!=pt[i+1][1])
		{
			kk=(pt[i][0]-pt[i+1][0])/(pt[i][1]-pt[i+1][1]);
			bb=(pt[i+1][0]*pt[i][1]-pt[i][0]*pt[i+1][1])/(pt[i][1]-pt[i+1][1]);
		}else{
			kk=-1;
			bb=-pt[i][1];
		}

		//The X axis of upper point should be larger than that of lower point
		if(pt[i][1]>pt[i+1][1] || pt[i][1]==pt[i+1][1] && pt[i][0]<pt[i+1][0])
		{
			addIntoOrdered(ordHead,pt[i][0],pt[i][1],kk,bb,i/2,i/2,ORD_UPPER);
			addIntoOrdered(ordHead,pt[i+1][0],pt[i+1][1],NO_LINE,NO_LINE,i/2,i/2,ORD_LOWER);
		}else{
			addIntoOrdered(ordHead,pt[i][0],pt[i][1],NO_LINE,NO_LINE,i/2,i/2,ORD_LOWER);
			addIntoOrdered(ordHead,pt[i+1][0],pt[i+1][1],kk,bb,i/2,i/2,ORD_UPPER);
		}
	}
}
