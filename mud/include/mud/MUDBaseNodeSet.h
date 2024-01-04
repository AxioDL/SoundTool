#ifndef _MUDBASENODESET_H_
#define _MUDBASENODESET_H_

#pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
#include <set>
class MUDBaseNode;

class MUDBaseNodeAddrCmp {
public:
  bool operator()(MUDBaseNode* a, MUDBaseNode* b) const {
    return a < b;
  }
};

class MUDBaseNodeSet : public std::set<MUDBaseNode*, MUDBaseNodeAddrCmp> {
public:
  MUDBaseNodeSet() {}

  void Add(MUDBaseNode* node) { insert(node); }
  bool Remove(MUDBaseNode* node) {
    bool result;

    MUDBaseNodeSet::iterator it = find(node);
    if (it == end()) {
      result = false;
    } else {
      erase(it);
      result = true;
    }

    return result;
  }
  
};

#endif // _MUDBASENODESET_H_