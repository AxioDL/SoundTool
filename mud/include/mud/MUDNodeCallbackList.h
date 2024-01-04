#ifndef _MUDNODECALLBACKLIST_H_
#define _MUDNODECALLBACKLIST_H_


#include <list>
#include <utility>

#include <mud/MUDTypeStorage.h>
#include <mud/MUDTypes.h>

class MUDBaseNodeInterface;

class MUDNodeCallback {
public:
  MUDNodeCallback(const MUDTypeStorage& _type, MUDNodeCallbackFunc _callback) {
    type = _type;
    callback = _callback;
  }

  const MUDTypeStorage& GetType() const { return type; }
  MUDNodeCallbackFunc GetCallback() const { return callback; }

private:
  MUDTypeStorage type;
  MUDNodeCallbackFunc callback;
};


class MUDNodeCallbackList : public std::list<MUDNodeCallback>{
public:
  typedef std::list<MUDNodeCallback> ListType;

  ListType::iterator find(const MUDTypeStorage& type) {
    ListType::iterator it;
    for (it = begin(); it != end(); ++it) {
      if (it->GetType() == type) {
        break;
      }
    }

    return it;
  }

  std::pair<ListType::iterator, bool> insert(const MUDNodeCallback& cb) {
    ListType::iterator it;

    it = find(cb.GetType());
    if (it != end()) {
      return std::pair<ListType::iterator, bool>(it, false);
    } 

    push_back(cb);
    return std::pair<ListType::iterator, bool>(--end(), true);
  }
  void SetModificationNotifyEnabled(bool on) { modificationNotifyEnabled = on; }
  bool IsModificationNotifyEnabled() const { return modificationNotifyEnabled; }

  
  void SetModificationNotifyCallback(MUDModifyNotificationCallback cb) { modificationNotifyCallback = cb; }
  MUDModifyNotificationCallback GetModificationNotifyCallback() const { return modificationNotifyCallback; }

private:
  MUDModifyNotificationCallback modificationNotifyCallback;
  bool modificationNotifyEnabled;
};

#endif // _MUDNODECALLBACKLIST_H_