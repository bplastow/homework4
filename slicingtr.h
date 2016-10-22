/**********************************************************************************************
 * slicingtr.h
 *
 *  Created on: Oct 21, 2016
 *      Author: Blaine Plastow
 *              A02235496
 *              ECE6460
 *
 * functions to create the slicing tree from the polish expression, and find the area.
 * Also contains the struct for the stack and the slicing tree.
 **********************************************************************************************/

#ifndef SLICINGTR_H_
#define SLICINGTR_H_

#include<cstdlib>
#include<vector>

using namespace std;

#define MAX_STACK 128

//structure for the nodes of the slicing tree that holds the value, and the height and width
struct stnode
{
	char val;
	double height;
	double width;
	stnode *left;
	stnode *right;
};

//structure for the stack to use to make the slicing tree
struct ststack
{
	stnode *array[MAX_STACK];
	int top;
};

stnode *tree_from_NPE(vector<char> &expr, int size, ststack *stack);
ststack *newstack(void);
void stpush(ststack *stack, stnode *node);
stnode *stpop(ststack *stack);
int isfull(ststack *stack);
int isempty(ststack *stack);
void freestack(ststack *stack);
void stackclear(ststack *stack);

void printNPE(stnode *root);
void assignValues (vector<char> &value, vector<double> &width, vector<double> &height, stnode *pointer, int size);
void checkTosee(stnode *pointer);
void createTree(stnode *pointer, int &size, char *tree);
double areaFunct(vector<double> &right, vector<double> &left, stnode *pointer);
void readin(vector<char> &value, vector<double> &width, vector<double> &height);
void deleteTree(stnode *pointer);

void annealingFunc(vector<char>& Enot, vector<char>& value, vector<double>& width, vector<double>& height);


#endif /* SLICINGTR_H_ */
