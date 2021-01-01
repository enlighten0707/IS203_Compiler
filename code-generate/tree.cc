//
// See copyright.h for copyright notice and limitation of liability
// and disclaimer of warranty provisions.
//
#include "copyright.h"


///////////////////////////////////////////////////////////////////////////
//
// file: tree.cc
//
// This file defines the basic class of tree node
//
///////////////////////////////////////////////////////////////////////////

#include "tree.h"

/* line number to assign to the current node being constructed */
int node_lineno = 1;

///////////////////////////////////////////////////////////////////////////
//
// tree_node::tree_node
//
// constructor of tree node
//
///////////////////////////////////////////////////////////////////////////
tree_node::tree_node()
{
    line_number = node_lineno;
}

SymbolTable<Symbol ,AddAndType>* tree_node::map = new SymbolTable<Symbol ,AddAndType>();
SymbolTable<int,char>* tree_node::strMap = new SymbolTable<int,char>();
int tree_node::StringNum = 0;
int tree_node::rspAdd = 0;
int tree_node::jumpNum = 0;
int tree_node::varNum = 0;
bool tree_node::flag = 0;
int tree_node::stmtIndex = 0;
SymbolTable<int,Help>* tree_node::breakPoint = new SymbolTable<int,Help>();
SymbolTable<int,Help>* tree_node::continuePoint = new SymbolTable<int,Help>();

///////////////////////////////////////////////////////////////////////////
//
// tree_node::get_line_number
//
///////////////////////////////////////////////////////////////////////////
int tree_node::get_line_number()
{	
	return line_number;
}

//
// Set up common area from existing node
//
tree_node *tree_node::set(tree_node *t) {
   line_number = t->line_number;
   return this;
}
