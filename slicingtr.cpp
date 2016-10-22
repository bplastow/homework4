/********************************************************************************************
 * slicingtr.cpp
 *
 *  Created on: Oct 21, 2016
 *      Author: Blaine Plastow
 *              A02235496
 *              ECE6460
 *
 *    functions to create the slicing tree from the polish expression, and find the area.
 *    Has the annealing function to optimize the tree and find the best area
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

/*******************************************************************************
* Function to read in values from file for module, area, and aspect ratio
********************************************************************************/
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
stnode *tree_from_NPE(vector<char> &expr,int size, ststack *stack)
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

/***************************************************************************************************
 * Function to perform the annealing on the topology to find the best area for floorplan
 ***************************************************************************************************/
void annealingFunc(vector<char>& Enot, vector<char>& value, vector<double>& width, vector<double>& height)
{
	//setup initial values
	double BestCost, costOfbest, ratio=0.85, lamdatf=.005, P=.99, epsilon=.001, t0=-1, t, oldCost=0, costNew=0, changeOfcost=0;
	int nmoves=10, iseed = 3, n=6, mt=12, uphill, reject, j=0, i=0, N;
	ststack *stack;
	int size;
	stnode *root3;

	bool test=true;
	vector<char> newE ,E, Best;
	for(int r=0; r<Enot.size(); r++)
	{
		//setup initial NPE
		E.push_back(Enot[r]); Best.push_back(Enot[r]); newE.push_back(Enot[r]);
	}
	vector <double> right, left;
	stack = newstack();
	size = newE.size()-1;
	stnode *root = tree_from_NPE(newE, size, stack);//creates a tree from the NPE
	size=newE.size();
	assignValues(value, width, height, root, size);
	oldCost= areaFunct(right, left, root);          //calculate the cost of the initial NPE
	BestCost=oldCost;                               //set initial best cost
	double avgCost[1000];
	int z=0;
	while(z<1000)
	{
		//	i=newE.size()-1;
			i=rand()%newE.size(); // gets the random number to choose which value to randomly move
			switch (rand()%3+1) // chooses a random number from 1 to 3 to choose a case
		//	switch (3)
			{
					case 1:{

						while(newE[i] == 'V' || newE[i] == 'H')// checks to make sure that the random value is an operand not an operator
                                                {
							i++;
							if(newE.size()== i)
								{
									i=0;
								}
						}//increments it if it is an operator
						if(i+1 == newE.size())
							{
								j=i-1;
								test=false;
							}//checks to make sure it isn't at end of the array, if so starts j decreasing
						else
							{
								j=i+1;
							} //else it starts j increasing to find the next p
						while(newE[j] == 'V' || newE[j] == 'H')// helps to fin the next opreand
                                                {
							if(test)
							{
								if(j+1 == newE.size())
								{
									j=i-1;
									test=false;
								}
								else
								{
									j++; if(i== j){j++;}
								}
							}
							else
							{
								j--; if(i==j){j--;}
							}}//finds it wheter it is is before or after the value
						char tempvalue = newE[i]; //temp value
						newE[i]=newE[j]; //stores it
						newE[j]=tempvalue; // stores the other value
						//swap values
						break;//leaves case statement
						}
					case 2:{
							while(newE[i] != 'V' && newE[i] != 'H')//checks to make sure that i find an operator
							{i++; if(i == newE.size()){i=0;}} //increases if it is an operand
							j=i-1;
							while(newE[j] == 'V' || newE[j] == 'H') // keeps flipping for the line of v and h's
                                                        {
                                                                if(newE[j] == 'V') // checks if its a v and changes it to h
                                                                {
                                                                        newE[j]='H';
                                                                }
                                                                else  // else changes it to v since it was an H
                                                                {
                                                                        newE[j]='V';
                                                                }
                                                                j--;//moves along the array
                                                        }
							while(newE[i] == 'V' || newE[i] == 'H') // keeps flipping for the line of v and h's
							{
								if(newE[i] == 'V') // checks if its a v and changes it to h
								{
									newE[i]='H';
								}
								else  // else changes it to v since it was an H
								{
									newE[i]='V';
								}
								i++;//moves along the array
							}
							break; //exits the casement
						}
					case 3:
						{
							if(newE.size()-1==i) // checks to see if it the last elment, if so it starts at the first of the array
							{i=0;}
							//looks for an operand and operator right next to each other to perform the m3 swap
							while(((newE[i]=='V' || newE[i] == 'H')&&(newE[i+1]=='V' || newE[i+1] =='H')) || ((newE[i]!='V' && newE[i]!='H')&&(newE[i+1]!='V' && newE[i+1] != 'H')))
                                                	 {
								i++;// moves to next element to check
								if(i == newE.size()-2 || i>newE.size()-2 ) //checks to make sure we aren't on the last two values, if so starts it over at 0
								{i=0;}
							 }
							char tempValue=newE[i];// stores the value temporarily
							newE[i]=newE[i+1];// stores the other value to swap
							newE[i+1]=tempValue; //finishes the swap
							int operators=0, operands=0;
							test=true;
							for(int q=0; q<newE.size(); q++) //balloting property
							{
								if(newE[q] == 'V' || newE[q] == 'H')
                                                        	{operands+=1;}
								else
								{operators+=1;}
								if(operators<=operands) //checks to see that there are more operands than operators to ensure its valid
								{test=false;} //marks if its not valid
							}
							if(i>0)
							{if (newE[i-1] == newE[i])
								{test=false;}
							}
							if(i<newE.size()-2)
							{ if(newE[i+1]==newE[i+2])
								{test=false;}
							}
							if(!test)
							{reject+=1;
							char tempValue=newE[i];// stores the value temporarily
                                                        newE[i]=newE[i+1];// stores the other value to swap
                                                        newE[i+1]=tempValue; //finishes the swap
							} // rejects it since its not proven
							break; //leaves the case statemts
						}

				}

		stack = newstack();
		size = newE.size()-1;
		stnode *root = tree_from_NPE(newE, size, stack);//creates a node for the base of tree
		size=newE.size();
		assignValues(value, width, height, root, size);
		costNew= areaFunct(right, left, root); //finds the cost of the area
		deleteTree(root);
		avgCost[z]=costNew - oldCost;// checks the different in cost
		z++;
		}

	int averageNumbers;
	for(int m=0; m<1000; m++)
	{

		if(avgCost[m]>0)
		{
			t0 = t0 + avgCost[m];      //find the average number of moves to use for the initial temperature
			averageNumbers++;
		}
	}
	t0=t0/averageNumbers;
	t0=-t0/log(P);
	n=Enot.size();
	N = n*nmoves;
	t = t0;
	test=true;
	newE.clear(); Best.clear(); E.clear();
	for(int r=0; r<Enot.size(); r++)
	{
		E.push_back(Enot[r]);
		Best.push_back(Enot[r]);
		newE.push_back(Enot[r]);
	}
	size = newE.size()-1;
	stnode *root2 = tree_from_NPE(newE, size, stack);//creates a node for the base of tree
	size = newE.size();
	assignValues(value, width, height, root2, size);
	oldCost= areaFunct(right, left, root2);
	BestCost=oldCost;
	deleteTree(root);
	srand(iseed);
	int count=0;
	do // checks to see if the temp is greater than epsilon, or if the reject criteria is greater than 95% to know to stop
	{
		mt=uphill=reject=0; // intializes everything to 0
		do //checks to see if uphill and temp are good
		{
			i=rand()%newE.size(); // gets the random number to choose which value to randomly move
			switch (rand()%3+1) // chooses a random number from 1 to 3 to choose a case
			{
				case 1:{

						while(newE[i] == 'V' || newE[i] == 'H')// checks to make sure that the random value is an operand not an operator
                        {
							i++;
							if(newE.size()== i)
							{
								i=0;
							}
						}//increments it if it is an operator
						if(i+1 == newE.size())
						{
							j=i-1;
							test=false;
						}//checks to make sure it isn't at end of the array, if so starts j decreasing
						else
						{
							j=i+1;
						} //else it starts j increasing to find the next p
						while(newE[j] == 'V' || newE[j] == 'H')// helps to fin the next opreand
                                                {
							if(test)
							{
								if(j+1 == newE.size())
								{
									j=i-1;
									test=false;
								}
								else
								{
									j++; if(i== j){j++;}
								}
							}
							else
							{
								j--; if(i==j){j--;}
							}
						}//finds it weather it is is before or after the value
						char tempvalue = newE[i]; //temp value
						newE[i]=newE[j]; //stores it
						newE[j]=tempvalue; // stores the other value
						//swap values
						break;//leaves case statement
					}
					case 2:
					{
							while(newE[i] != 'V' && newE[i] != 'H')//checks to make sure that i find an operator
							{
								i++; if(i == newE.size()){i=0;}} //increases if it is an operand
							    j=i-1;
							    while(newE[j] == 'V' || newE[j] == 'H') // keeps flipping for the line of v and h's
							    {
							    	if(newE[j] == 'V') // checks if its a v and changes it to h
                                    {
							    		newE[j]='H';
                                    }
                                    else  // else changes it to v since it was an H
                                    {
                                        newE[j]='V';
                                    }
                                    j--;//moves along the array
                                }
							    while(newE[i] == 'V' || newE[i] == 'H') // keeps flipping for the line of v and h's
							    {
							    	if(newE[i] == 'V') // checks if its a v and changes it to h
							    	{
							    		newE[i]='H';
							    	}
							    	else  // else changes it to v since it was an H
							    	{
							    		newE[i]='V';
							    	}
							    	i++;//moves along the array
							    }
							    break; //exits the casement
					}
					case 3:
					{
							if(newE.size()-1==i) // checks to see if it the last elment, if so it starts at the first of the array
							{i=0;}
							//looks for an operand and operator right next to each other to perform the m3 swap
							while(((newE[i]=='V' || newE[i] == 'H')&&(newE[i+1]=='V' || newE[i+1] =='H')) || ((newE[i]!='V' && newE[i]!='H')&&(newE[i+1]!='V' && newE[i+1] != 'H')))
                                                	 {
								i++;// moves to next element to check
								if(i == newE.size()-2 || i>newE.size()-2 ) //checks to make sure we aren't on the last two values, if so starts it over at 0
								{i=0;}
							 }
							char tempValue=newE[i];// stores the value temporarily
							newE[i]=newE[i+1];// stores the other value to swap
							newE[i+1]=tempValue; //finishes the swap
							int operators=0, operands=0;
							test=true;
							for(int q=0; q<newE.size(); q++) //balloting property
							{
								if(newE[q] == 'V' || newE[q] == 'H')
                                                        	{operands+=1;}
								else
								{operators+=1;}
								if(operators<=operands) //checks to see that there are more operands than operators to ensure its valid
								{test=false;} //marks if its not valid
							}
							if(i>0)
							{if (newE[i-1] == newE[i])
								{test=false;}
							}
							if(i<newE.size()-2)
							{ if(newE[i+1]==newE[i+2])
								{test=false;}
							}
							if(!test)
							{reject+=1;
							char tempValue=newE[i];// stores the value temporarily
                                                        newE[i]=newE[i+1];// stores the other value to swap
                                                        newE[i+1]=tempValue; //finishes the swap
							} // rejects it since its not proven
							break; //leaves the case statemts
					}
			} //count++;
		mt=mt+1; //increases move count
		vector <double> right, left;
		stack = newstack();
		size = newE.size()-1;
		stnode *root = tree_from_NPE(newE, size, stack);//creates a node for the base of tree
		size=newE.size();
		assignValues(value, width, height, root, size);
		costNew= areaFunct(right, left, root); //finds the cost of the area
		deleteTree(root);
		changeOfcost=costNew - oldCost;// checks the different in cost
		double testVal= exp(-changeOfcost/t);
		double random = rand()%100+1;
		random=random/100;
			if((changeOfcost <= 0) || (random < testVal )) // checks to see if the change is better or if it changes randomly
			{
				for(int r=0; r<E.size(); r++)
				{
					E[r]=newE[r];
				}
				if(changeOfcost > 0) // stores E as the new e since it was accepted
				{
					uphill+=1;
				}

				if(costNew < BestCost)
				{
					for(int r=0; r<E.size(); r++)
        			{
						Best[r] = E[r];
        			}

					stack = newstack();
					size = Best.size()-1;
					root3 = tree_from_NPE(Best, size, stack);

					BestCost=costNew;
				}
			//	uphill+=1;//increases since it was an uphill movement
			}
			else
			{
					reject+=1; //rejects it because it isn't a good move
			}
		//oldCost=costNew;
		}while((uphill < N) && (mt < 2*N));
		t=lamdatf*t;

	}while(((reject/mt) <= .95) && (t>epsilon));

	cout << "Final optimized topology: ";
	printNPE(root3);
	cout << endl;
	cout<<"Final cost: "<<BestCost;

}
