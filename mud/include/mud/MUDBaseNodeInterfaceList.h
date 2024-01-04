#ifndef _MUDBASENODEINTERFACE_LIST_H_
#define _MUDBASENODEINTERFACE_LIST_H_

#include <list>
#pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information

class MUDBaseNodeInterface;

class MUDBaseNodeInterfaceList : public std::list<MUDBaseNodeInterface*> {
};

#endif // _MUDBASENODEINTERFACE_LIST_H_