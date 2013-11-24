/*
 *	Author: John Hany
 *	Website: http://johnhany.net/
 *	Source code updates: https://github.com/johnhany/SegmentsIntersection
 *	If you have any advice, you could contact me at: johnhany@163.com
 *
 */

#include "stdafx.h"
#include "windows.h"
#include "scanTree.h"

//Add a new node "now" into the scan tree "top".
//The position "now" to be placed is determined by the X axis
//on the sweep line(whose Y axis is y_scan).
void addIntoScan(struct tree *top,struct tree *now,float y_scan)
{
	struct tree *temp;
	temp=(struct tree *)malloc(LEN_S);
	temp=top;
	float x_now,x_temp;

	//Calculate the proper X axises.
	if(now->id->type==ORD_UPPER && (now->id->k!=-1 || now->id->b>0)){
		x_now=now->id->k * y_scan + now->id->b;
	}else{
		x_now=now->id->x;
	}
	if(temp->id->type==ORD_UPPER && (temp->id->k!=-1 || temp->id->b>0)){
		x_temp=temp->id->k * y_scan + temp->id->b;
	}else{
		x_temp=temp->id->x;
	}

	while((x_now < x_temp && temp->left!=NULL) || (x_now >= x_temp && temp->right!=NULL))
	{
		if(x_now < x_temp)
		{
			temp=temp->left;
		}else{
			temp=temp->right;
		}

		if(temp->id->type==ORD_UPPER && (temp->id->k!=-1 || temp->id->b>0)){
			x_temp=temp->id->k * y_scan + temp->id->b;
		}else{
			x_temp=temp->id->x;
		}
	}
	if(x_now < x_temp)
	{
		temp->left=now;
		now->father=temp;
	}else{
		temp->right=now;
		now->father=temp;
	}
	now->left=NULL;
	now->right=NULL;
}

//Search for node according to line numbers and type of the point.
//srchType==SCAN_L: search for upper point that it->l==line
//srchType==SCAN_L2: search for upper point that it->l==line2
//srchType==SCAN_L_L2: search for inte-point that it->l==line and it->l2==line2
tree *searchInScan(tree *temp,int line,int line2,int srchType)
{
	tree *out;
	out=(tree *)malloc(LEN_S);

	if(temp==NULL)
		return NULL;
	if(srchType==SCAN_L && temp->id->type==ORD_UPPER && temp->id->l==line)
		return temp;
	else if(srchType==SCAN_L2 && temp->id->type==ORD_UPPER && temp->id->l==line2)
		return temp;
	else if(srchType==SCAN_L_L2 && (temp->id->l==line && temp->id->l2==line2 || temp->id->l==line2 && temp->id->l2==line))
		return temp;
	else{
		//If the node doesn't meet any requirement,
		out=NULL;
		if(temp->left!=NULL)
		{
			//search further in its left sub-tree,
			out=searchInScan(temp->left,line,line2,srchType);
			if(out!=NULL)
				return out;
		}
		if(temp->right!=NULL)
		{
			//and its right sub-tree.
			out=searchInScan(temp->right,line,line2,srchType);
			if(out!=NULL)
				return out;
		}
		//If can't find any in its sub-trees.
		return NULL;
	}
}

//Delete a node "now" from the scan tree "top".
//If the root node changes because of the delete,
//the function returns the new root node.
//WARNING: NO FREE() FUNCTION
struct tree *dltFromScan(tree *top,tree *now)
{
	tree *temp;
	tree *new_top;
	temp=(tree *)malloc(LEN_S);

	if(now->father==NULL)
	{
		//To delete the root node.
		if(now->right!=NULL)
		{
			//If its right sub-tree exists.
			temp=now->right;
			new_top=temp;
			if(now->left!=NULL)
			{
				//If both of its left and right sub-tree exist.
				//Its left sub-tree becomes the left-most sub-tree
				//of its right sub-tree.
				while(temp->left!=NULL)
				{
					temp=temp->left;
				}
				temp->left=now->left;
				now->left->father=temp;
			}
			//Its right-tree knot becomes the new root node.
			now->right->father=NULL;
		}else if(now->left!=NULL)
		{
			//If its right sub-tree does not exist,
			//Its left sub-tree becomes the new root node.
			now->left->father=NULL;
			new_top=now->left;
		}else{
			//Both of its left and right sub-tree do not exist,
			//which means the scan tree is only root node itself.
			new_top=NULL;
		}
	}else{
		if(now->left==NULL)
		{
			if(now->right==NULL)
			{
				//If delete a leaf, only delete itself.
				//But we don't know which side sub-tree it is of its father.
				if(now->father->left!=NULL && now->father->left==now)
					now->father->left=NULL;
				else if(now->father->right!=NULL && now->father->right==now)
					now->father->right=NULL;
			}else{
				//If it does not have a left sub-tree, its right
				//sub-tree becomes the sub-tree of its father.
				now->right->father=now->father;
				if(now->father->left!=NULL && now->father->left==now)
					now->father->left=now->right;
				else if(now->father->right!=NULL && now->father->right==now)
					now->father->right=now->right;
			}
		}else{
			//If the node has both left and right sub-trees.
			//Its left sub-tree takes place of itself, and its right
			//sub-tree becomes the right-most sub-tree of its left sub-tree.
			now->left->father=now->father;
			if(now->father->left!=NULL && now->father->left==now)
				now->father->left=now->left;
			else if(now->father->right!=NULL && now->father->right==now)
				now->father->right=now->left;
			temp=now->left;
			if(now->right!=NULL)
			{
				while(temp->right!=NULL)
				{
					temp=temp->right;
				}
				now->right->father=temp;
				temp->right=now->right;
			}
		}
		new_top=top;
	}

	return new_top;
}

//Get left neighbor of node "now" in the scan tree and return it.
struct tree *get_left(tree *now)
{
	//Don't use malloc() or free()
	//"temp" and "result" are both pointers directly pointing to the original tree
	tree *temp;
	tree *result;

	if(now->left==NULL){
		//If node "now" doesn't have left sub-tree.
		if(now->father!=NULL)
		{
			if(now->father->right!=NULL && now->father->right==now){
				//If it's its father's right sub-tree,
				//its father should be its left neighbor.
				result=now->father;
				return result;
			}else if(now->father->left!=NULL && now->father->left==now){
				//If it's its father's left sub-tree,
				//look for its ancestor(in which every node is always its father's left sub-tree),
				//and if the ancestor is its father's right sub-tree, the ancestor's father
				//should be "now"'s left neighbor.("now" is the left-most leaf in the
				//ancestor's father's right sub-tree)
				temp=now->father;
				while(temp->father!=NULL && temp->father->left==temp)
					temp=temp->father;
				if(temp->father!=NULL)
				{
					result=temp->father;
					return result;
				}else
					return NULL;
			}
		}else
			return NULL;
	}else{
		//If it has left sub-tree,
		//the right-most leaf in its left sub-tree should be its left neighbor.
		temp=now->left;
		result=temp;
		while(temp->right!=NULL)
		{
			result=temp->right;
			temp=temp->right;
		}
		return result;
	}
}

//Get right neighbor of node "now" in the scan tree and return it.
struct tree *get_right(tree *now)
{
	tree *temp;
	tree *result;

	if(now->right==NULL){
		//If node "now" doesn't have right sub-tree
		if(now->father!=NULL)
		{
			if(now->father->left!=NULL && now->father->left==now){
				//If it's its father's left sub-tree,
				//its father should be its right neighbor.
				result=now->father;
				return result;
			}else if(now->father->right!=NULL && now->father->right==now){
				//If it's its father's right sub-tree,
				//look for its ancestor(in which every node is always its father's right sub-tree),
				//and if the ancestor is its father's left sub-tree, the ancestor's father
				//should be "now"'s right neighbor.("now" is the right-most leaf in the
				//ancestor's father's left sub-tree)
				temp=now;
				while(temp->father!=NULL && temp->father->right==temp)
					temp=temp->father;
				if(temp->father!=NULL)
				{
					result=temp->father;
					return result;
				}else
					return NULL;
			}
		}else
			return NULL;
	}else{
		//If it has right sub-tree,
		//the left-most leaf in its right sub-tree should be its left neighbor.
		temp=now->right;
		result=temp;
		while(temp->left!=NULL)
		{
			result=temp->left;
			temp=temp->left;
		}
		return result;
	}
}

//Switch the orders of nodes "left_s" and "right_s" in the scan tree.
void switchScan(tree* left_s,tree* right_s)
{
	POrder *temp;

	//Because the "left", "right" and "father" are only pointers in the tree
	//and have no connections with the ordered list.
	//Just switch the "id" pointer will do the job.
	temp=left_s->id;
	left_s->id=right_s->id;
	right_s->id=temp;
}

