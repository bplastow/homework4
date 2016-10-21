/*******************************************************************************************
 * homework3.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: Blaine Plastow
 *              A02235496
 *              Homework 3
 *              ECE6460
 *
 * contains main function and prints the areas from different trees
 *******************************************************************************************/


#include <cstdlib>
#include <iostream>
#include <vector>

#include "slicingtr.h"

using namespace std;

int main(){
	ststack *stack;           // create stack to use to make tree

	vector<char> value, NPE;       // create a vector to store the module character value
	vector<double> width, height, left, right;  //create a vector to store the width, height
	int size;
	double area;                                //variable to store area

	readin(value, width, height);  //function to read in the values for the modules and the
	                               //area and aspect ratio and convert to width and height for
                                   //each module
	NPE.push_back(value[0]);

	for(int i = 1; i < value.size(); i++)
	{
		NPE.push_back(value[i]);
		NPE.push_back('V');
	}

	size = NPE.size()-1;
	stack = newstack();
	stnode *root = tree_from_NPE(NPE,size, stack);
	size=NPE.size();
	assignValues(value, width, height, root,size );
	cout << "NPE for tree: ";
	printNPE(root);                //Print out the polish expression for the tree
	cout << endl;
	area=areaFunct(right,left, root);  //calculate the area for the tree and print it out
	cout << "AREA: " << area;
	cout << "\n";

	annealingFunc(NPE, value, width, height);

	return 0;
}


