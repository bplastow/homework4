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
	char treeA[]="12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV";  //create the 3 polish expressions
	char treeB[] = "12H3H4H5H6H7H8H9HaHbHcHdHeHfHgHiHjHkHlH";  //to create tree for and find
	char treeC[] = "213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH";  //area for

	ststack *stack;           // create stack to use to make tree

	vector<char> value;       // create a vector to store the module character value
	vector<double> width, height, left, right;  //create a vector to store the width, height

	double area;                                //variable to store area
	char userInput='l';                         // get user input for which tree to make

	//keep asking for input until the user puts a A, B, or C
	while(userInput!='A' && userInput != 'B' && userInput != 'C')
	{
		cout<<"What tree would you like to calculate the area of: A, B, or C?";
		cin>>userInput;
	}

	readin(value, width, height);  //function to read in the values for the modules and the
	                               //area and aspect ratio and convert to width and height for
                                   //each module
	switch (userInput)
	{
		case 'A':                            //create tree a which is all vertical cuts
		{
			int size = sizeof(treeA)-2;
			stack = newstack();
			stnode *root = tree_from_NPE(treeA,size, stack);
			size=sizeof(treeA);
			assignValues(value, width, height, root,size );
			cout << "NPE for tree A: ";
			printNPE(root);                //Print out the polish expression for the tree
			cout << endl;
			area=areaFunct(right,left, root);  //calculate the area for the tree and print it out
			cout<<"AREA: "<<area;
			break;
		}
		case 'B':                           //create tree B which is all horizontal cuts
		{
			int size = sizeof(treeB)-2;
			stack = newstack();
			stnode *root = tree_from_NPE(treeB,size, stack);
			size=sizeof(treeB);
			assignValues(value, width, height, root,size );
			cout << "NPE for tree B: ";
			printNPE(root);                 //Print out the polish expression for the tree
			cout << endl;
			area=areaFunct(right,left, root);    //calculate the area for the tree and print it out
			cout<<"AREA: "<<area;
			break;
		}
		case 'C':                          //create tree C which is a combination of vertical and horizontal cuts
		{
			int size = sizeof(treeC)-2;
			stack = newstack();
			stnode *root = tree_from_NPE(treeC,size, stack);
			size=sizeof(treeC);
			assignValues(value, width, height, root,size );
			cout << "NPE for tree C: ";
			printNPE(root);                 //Print out the polish expression for the tree
			cout << endl;
			area=areaFunct(right,left, root);   //calculate the area for the tree and print it out
			cout<<"AREA: "<<area;
			break;
		}
	}

	return 0;
}


