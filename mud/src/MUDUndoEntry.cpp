#include <mud/MUDUndoEntry.h>

#include <mud/MUDAccessBaseInterface.h>
#include <mud/MUDBaseNodeInterface.h>
#include <mud/MUDRoot.h>
#include <mud/MUDUndoInfo.h>
#include <mud/MUDUndoSpecial.h>

#include <mpi/MPIBaseInterface.h>
#include <mpi/MPIManagerInterface.h>

MUDUndoEntry::MUDUndoEntry(MUDRoot* _root) {
  root = _root;
  flags = 0x80000000;
}


MUDUndoEntry::~MUDUndoEntry() {
  MUDUndoInfo undoInfo;
  if (flags & 0x80000000 ) {
    return;
  }


  if (flags & 1) {
    switch(specialType) {
    case MUD_UNDO_SPECIAL_VOID:
      break;
    case MUD_UNDO_SPECIAL_SPECIAL: {
      MUDUndoSpecial* undoSpecial = this->undoSpecial;
      undoInfo = MUDUndoInfo(undoSpecial->special, undoSpecial->specialType);
      delete undoSpecial;
   }
      break;
    case MUD_UNDO_SPECIAL_STRING:
      delete stringSpecial;
      break;
    }
  } else {
    undoInfo = MUDUndoInfo(undoSpecial->special, undoSpecial->specialType);
  }

  MUDBaseNodeInterface* node = root->GetNodeFromInstanceID(instanceID, NULL);

  if (node) {
    node->GetTypeInterface()->DiscardUndoInfo(&undoInfo);
    return;
  }


  MPIBaseInterface* bi;
  void* ba = root->GetManagerInterface()->FindInterface(*type, root->GetDataBaseSubType(), bi);

  if (ba) {
    ba->CreateInterface(
  }
}