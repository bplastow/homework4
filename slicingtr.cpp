/********************************************************************************************
 * slicingtr.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: Blaine Plastow
 *              A02235496
 *              ECE6460
 *
 *    functions to create the slicing tree from the polish expression, and find the area.
 ********************************************************************************************/

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>
#include <stack>
#include "slicingtr.h"

using namespace std;

//read in values from file for module, area, and aspect ratio
void readin(vector<char> &value, vector<double> &width, vector<double> &height){
	char testValue;
	double areaNode;
	double ratio;
	ifstream myfile;
	myfile.open("input.txt"); //file in in parenthesis
	string mystring;
	while (getline(myfile,mystring))
	{
		if(mystring.size()<5)
			continue;
		istringstream buffer(mystring);
		buffer>>testValue>>areaNode>>ratio;   //read in the values from the file and push them
		value.push_back(testValue);           //into the width and height vectors
		width.push_back(areaNode);
		height.push_back(ratio);

	}
	myfile.close();                           //close the file
    for(unsigned int w=0; w<height.size(); w++)
    {
        areaNode=width[w];
        width[w]=sqrt(areaNode*height[w]);      //convert values from area and aspect ratio
        height[w]=areaNode/width[w];            //to the width and height
    }
    return;
}

/********************************************************************************************
 * function that takes the slicing binary tree as an input and assigns the width and height
 * values to each node
 ********************************************************************************************/
void assignValues (vector<char> &value, vector<double> &width, vector<double> &height, stnode *pointer, int size)
{

	if(pointer->val == 'H' || pointer->val == 'V')//checks to make sure it isn't a leaf
	{
		assignValues(value, width, height, pointer->right, size); // goes right to find the leaf
		assignValues(value,width,height, pointer->left, size); //goes left to find the leaf
	}
	else
	{
		int i=0;
		while(pointer->val !=value[i] && i<size) // looks for the key value to find which values to store
		{
			i++;
		}
		if(pointer->val != value[i])
		{
			cout<<"Value not found.\n";
		}
		else
		{
			pointer->height=height[i]; //stores widths and heights
			pointer->width=width[i];

		}

	}
	return;
}

void checkTosee(stnode *pointer)
{
	if(pointer->val == 'H' || pointer->val == 'V')//goes through the tree and prints the values to verify working correctly
	{
		checkTosee(pointer->right);
		checkTosee(pointer->left);
	}
	else
	{
		cout<<pointer->val<<" "<<pointer->width<<" "<<pointer->height<<endl;
	}
	return;
}

/********************************************************************************************
 * function to delete the tree when done with it
 ********************************************************************************************/
void deleteTree(stnode *pointer)
{
	if(pointer->left!=NULL)
	{
		deleteTree(pointer->left);
	}
	if( pointer->right!=NULL )
    {
		deleteTree(pointer->right);
    }
	delete pointer;
	return;
}

/*********************************************************************************************
 *Function that prints the tree in post-order traversal which is the normalized polish
 *expression for the tree
 *********************************************************************************************/
void printNPE(stnode *root)
{
	if(root != NULL)
	{
		printNPE(root->left);
		printNPE(root->right);
		cout << root->val;
	}
}

/*********************************************************************************************
 * function that calculates the area for the tree which is the cost
 *********************************************************************************************/
double areaFunct(vector<double> &right, vector<double> &left, stnode *pointer)
{
	double tempArea;// holding place for area

	//base case for when tree is complete
	if(pointer->val != 'H' && pointer->val != 'V')
	{
		right.clear();
		right.push_back(pointer->width); // here you store the weights to take back up and be used in the  compute area
		right.push_back(pointer->height);
		right.push_back(right[1]);
		right.push_back(right[0]);
		return 0; //goes back to h or v so it can compare there and do calculation
	}
	//stores the widths and heights of the right side
	vector<double> tempVector;
	//always start by putting the heights and width in the right
	tempArea= areaFunct(right, left, pointer->right);
	//stores the right values temporarily to protect change later down
	for(unsigned int q=0; q<right.size(); q++)
	{
		tempVector.push_back(right[q]);
	}
	tempArea=areaFunct(left, right, pointer->left); // goes left to get the height and widths, notice left and right are switched so that it really stores them in left
	right.clear();

	for(unsigned int w=0;w<tempVector.size(); w++)
	{
		right.push_back(tempVector[w]);
	}
	tempVector.clear();

	for(unsigned int i=0; i < right.size(); i=i+2) // for loop to go through the right values
	{
		for(unsigned int j=0; j< left.size();j=j+2) //for loop to go through the left values
		{
			if(pointer->val == 'H')//checks to see if you need to keep width or height
			{
				if(right[i]>left[j]) //stores the greater width(since heights are added )
				{
					tempVector.push_back(right[i]);
				}
				else
				{
					tempVector.push_back(left[j]);
				}
			    tempVector.push_back(right[i+1]+left[j+1]);//gets the height by adding them together (left and right
			}
			else
			{
	          	tempVector.push_back(right[i]+left[j]);//adds widths
			    if(right[i+1]>left[j+1])
	            {
					tempVector.push_back(right[i+1]); //gets the bigger height stored
	            }
	            else
	            {
	                tempVector.push_back(left[j+1]);
	            }
			}
		}
	}
	unsigned int r=2;
	right.clear();

	for(unsigned int l=0; l<tempVector.size(); l=l+2)
	{
		bool arrayTest=true;
		for(unsigned int p=0; p<tempVector.size(); p=p+2)
		{
			if(tempVector[p]<=tempVector[l]&& tempVector[p+1]<=tempVector[l+1]&& l!=p)
			{
				arrayTest=false;
			}//checks to see if it is one of the smallest
		}
		if(arrayTest)
		{
			right.push_back(tempVector[l]);
			right.push_back(tempVector[l+1]); //stores the smallest values in the arrays
		}
	}
	tempArea=right[0]*right[1];//calculates 1st area
	while(r<right.size())
	{
		double output=right[r]*right[r+1]; //calculates the other areas and checks to see whic is the
		if(tempArea>output)
		{
			tempArea=output;}
			r=r+2;
		}
		return tempArea;
}

/**********************************************************************************************
 * function to create the stack to make the tree with
 **********************************************************************************************/
ststack *newstack(void)
{
	ststack *stack;
	stack = new ststack();
	stack->top = 0;       //initialize the top of the stack to zero
	return stack;
}

/*********************************************************************************************
 * function to be able to push a tree node to the stack
 *********************************************************************************************/
void stpush(ststack *stack, stnode *node)
{
	if(!isfull(stack))
	{
		stack->array[stack->top] = node;
		stack->top++;
	}
	else
		cout << "attempting to push into a full stack";
}

/*********************************************************************************************
 * function to pop a value off the stack which is a tree node
 *********************************************************************************************/
stnode *stpop(ststack *stack)
{
	if(!isempty(stack))                     //check if stack is empty before trying to pop
	{                                       //value off of it
		stack->top--;
		return stack->array[stack->top];
	}
	else
	{
		cout << "stack is empty";           //if the stack is empty then print out a message
		return NULL;
	}
}

/*********************************************************************************************
 * function to check if the stack is full
 *********************************************************************************************/
int isfull(ststack *stack)
{
	if(stack->top >= MAX_STACK)   //if the stack is at its max then return true
		return 1;
	return 0;
}

/*********************************************************************************************
 * function to check if stack is empty
 *********************************************************************************************/
int isempty(ststack *stack)
{
	if(stack->top <= 0)        //if the top of the stack is 0 or less than the stack is empty
		return 1;              //so return true otherwise return false
	return 0;
}

/*********************************************************************************************
 * function to free up the stack after you are done using it
 *********************************************************************************************/
void freestack(ststack *stack)
{
	free(stack);
}

/*********************************************************************************************
 * function to clear the stack
 *********************************************************************************************/
void stackclear(ststack *stack)
{
	stack->top = 0;
}

/**********************************************************************************************
 * function to create a tree from a given polish expression using pushes and pops from the stack
 **********************************************************************************************/
stnode *tree_from_NPE(char *expr,int size, ststack *stack)
{
	int i;
	stnode *node=NULL, *left, *right;   //temporary node values to help

	for(i = 0; i <= size; i++)
	{
		node = new stnode();                   //create a new node
		if(expr[i] != 'V' && expr[i] != 'H')   //if the value is an operand then assign the value
		{                                      //to the node and push it onto the stack
			node->left = node->right = NULL;
			node->val = expr[i];
			stpush(stack, node);
		}
		else                                   //if the value is an operator then pop twice
		{                                      //and create a tree with the two values and
			right = stpop(stack);              //then push the new subtree back onto the stack
			left = stpop(stack);               //keep doing this until the entire tree is
			node->left = left;                 //made
			node->right = right;
			node->val = expr[i];
			stpush(stack,node);
		}
	}
	stackclear(stack);                         //clear the stack when done
	return node;                               //return the completed tree
}


void annealing()
{
	double P = 0.99, T0 = 20/log(P), M = 0, epsilon, r = 0.85, delta_cost, newcost, oldcost;
	int MT = 0, uphill = 0, reject = 0, N;
	double T = T0;
	bool done;
	do{
		MT = uphill = reject = 0;
		do{
			switch(M)
			{
				case 1:
					NE = swap(E,ei,ej);
					break;
				case 2:
					NE = compliment(E,C);
					break;
				case 3:
					done = false;
					while(!done)
					{

					}
					NE = swap(E,ei,ei+1);
					break;
			}
			MT = MT + 1;
			delta_cost = cost(NE) - cost(E);
			if(delta_cost <= 0)
			{
				if(delta_cost > 0)
				{
					uphill = uphill + 1;
				}
				E = NE;
				if(cost(E) < cost(Best))
				{
					Best = E;
				}
				else
				{
					reject = reject + 1;
				}
			}
		}while((uphill <= N) && (MT <= 2*N));
	T = r*T;
	}while((reject/MT <= 0.95) &&(T >= epsilon));

	stnode *root2 = new stnode();
	root2->left = root2->right = NULL;

}
