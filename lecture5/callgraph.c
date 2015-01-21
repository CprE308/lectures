/**
 * @file      callgraph.c
 * @author    Jeramie Vens
 * @date      12/23/14 - Created
 * @brief     Create a callgraph of the fibonacci sequence
 * @detail    This will create a callgraph of the fibonacci sequence
 *            generator to find the nth term of the sequence.  The
 *            callgraph is generated using the graphviz tool.
 * @copyright MIT License (c) 2014-2015
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/// This object represents a function which can have one parrent
/// and numerous children and siblings
struct FUNCTION_CONTEXT
{
	struct FUNCTION_CONTEXT *parrent;		// the parrent function
	struct FUNCTION_CONTEXT *children;		// first child function
	struct FUNCTION_CONTEXT *next_sibling;	// next sibling in the list
	int num_children;						// number of children this function has
	char* name;								// name of this function
	int param;								// param this function was called with
	int uid;								// Unique ID for this function
};

/// The current function running on the stack
struct FUNCTION_CONTEXT* current_function = NULL;

/**
 * Call each time a function is entered
 * @param     name
 *				The name of the function
 * @param     n
 *              Parameter the function was called with
 */ 
void enter_function(char* name, int n)
{
	// create new function object
	struct FUNCTION_CONTEXT* new_function = (struct FUNCTION_CONTEXT*)malloc(sizeof(struct FUNCTION_CONTEXT));
	struct FUNCTION_CONTEXT* i;
	// create a new unique ID
	new_function->uid = rand();
	// set the parrent of this function to the current running function
	new_function->parrent = current_function;
	// check if current function exists (may not if this is the first call) and that it has children alread
	if(current_function && current_function->children)
	{
		// walk the list looking for the end
		for(i=current_function->children; i->next_sibling != NULL; i=i->next_sibling);
		// add this new function to the end of the list
		i->next_sibling = new_function;
		// increment the number of children the parrent has
		current_function->num_children++;
	}
	// if the current function exists, but this is the first child
	else if(current_function)
	{
		// add this function to the head of the children list
		current_function->children = new_function;
		// increment the number of children the parrent has
		current_function->num_children++;
	}
	// Set this to the last element in the sibling list
	new_function->next_sibling = NULL;
	// Set this to have no children
	new_function->children = NULL;
	new_function->num_children = 0;
	// Set this function name and param
	new_function->name = name;
	new_function->param = n;
	// Set this as the current function
	current_function = new_function;
}

/**
 * Call this before leaving a function
 */
void exit_function()
{
	// Set the parrent to be the current function again
	current_function = current_function->parrent;
}

/**
 * Add a level to the output file
 * @param     fp
 * 				Pointer to the FILE
 * @param     root
 * 				The level to print to fp
 */
void add_level(FILE* fp, struct FUNCTION_CONTEXT* root)
{
	struct FUNCTION_CONTEXT* child;
	// create the root object
	fprintf(fp, "%d [label=\"%s(%d)\"];\n", root->uid, root->name, root->param);
	// for each child
	for(child=root->children; child != NULL; child = child->next_sibling)
	{
		// print the child level
		add_level(fp, child);
		// link this root to its child
		fprintf(fp, "%d -> %d;\n", root->uid, child->uid);
	}
}

/**
 * Create a new function call graph
 * @param     filename
 *              Name to call the output file without extension
 */
void create_graph(char* filename)
{
	FILE* fp;
	// create .dot filename
	char* in_name = malloc(strlen(filename) + 5);
	strcpy(in_name, filename);
	strcat(in_name, ".dot");
	// open file
	fp = fopen(in_name, "w");
	// print header info
	fprintf(fp, "digraph function_call {\n");
	fprintf(fp, "rankdir=UD;\n");
	// print the call graph
	add_level(fp, current_function);
	// end the file and close it
	fprintf(fp, "}\n");
	fclose(fp);

	// fork the process
	if(fork() == 0)
	{
		// crate output file name
		char* out_name = malloc(strlen(filename) + 5);
		strcpy(out_name, filename);
		strcat(out_name, ".png");
		printf("calling dot...\n");
		// call the dot compiler program
		execlp("dot", "dot", "-Tpng", "-o", out_name, in_name, (char*)NULL);
		printf("this isnt good...\n");
	}
}

char* fib_name = "fib";
/**
 * Calculate the nth element of the Fibonacci sequence
 * @param     n
 *              Element of the Fibonacci sequence to search for.
 * @return    The result
 */
int fib(int n)
{
	int a, b;
	// log this function entry
	enter_function(fib_name, n);
	if (n == 0 || n == 1)
	{
		// log this function exit
		exit_function();
		return n;
	}
	else
	{
		// calculate the two fibonacci number needed to calculate this one
		a = fib(n-1);
		b = fib(n-2);
		// log this function exit
		exit_function();
		// return result
		return a + b;
	}
}


char* main_name = "main";
int main(int argc, char const *argv[])
{
	int num;

	// mark the entry of the main function
	enter_function(main_name, argc);

	// if a number is given on the command line
	if(argc > 1)
	{
		// use that as the fib level to calculate
		num = atoi(argv[1]);
	}
	else
	{
		// ask user for a number
		printf("Enter a number: ");
		scanf("%d", &num);
	}

	// call fib and print the result
	printf("The %d term of the fibonacci sequence is %d\n", num, fib(num));
	
	// create the call graph
	create_graph("./fib_out");
	
	return 0;
}
