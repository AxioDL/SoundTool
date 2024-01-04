#ifndef _MUDTYPES_H_
#define _MUDTYPES_H_

#include <f5/types.h>


enum MUDState {
  MUD_STATE_OK,
  MUD_STATE_UNKNOWN_NODE_TYPE,
  MUD_STATE_OUT_OF_MEMORY,
  MUD_STATE_NODE_DUPLICATED,
  MUD_STATE_NODE_STILL_REFERENCED,
  MUD_STATE_NODE_COULD_NOT_CREATE,
  MUD_STATE_NO_ID_TRANSLATION_INFO,
  MUD_STATE_PARENT_NOT_DIRECTORY,
  MUD_STATE_NODE_HAS_CHILDREN,
  MUD_STATE_NODE_READ_ONLY,
  MUD_STATE_NODE_ACCESSED_BY_ANOTHER_CALLER,
  MUD_STATE_NODE_MODIFIED_BY_ANOTHER_CALLER,
  MUD_STATE_COULD_NOT_READ_FILE,
  MUD_STATE_BAD_FILE_FORMAT,
  MUD_STATE_COULD_NOT_WRITE_FILE,
  MUD_STATE_COULD_NOT_CREATE_FILE,
  MUD_STATE_UNSUPPORTED_FILE_VERSION,
  MUD_STATE_COULD_NOT_OPEN_FILE,
  MUD_STATE_CANNOT_CHANGE_DATABASE_SUBTYPE,
  MUD_STATE_NODE_RENAME_DISALLOWED,
  MUD_STATE_DATABASE_DIFFERENT_SUBTYPE_THAN_LOADED,
  MUD_STATE_SINGULAR_NODE_TYPE_DUPLICATED,
};

enum MUDIDType {
  MUD_ID_TYPE_0,
  MUD_ID_TYPE_1,
  MUD_ID_TYPE_2,
  MUD_ID_TYPE_3,
  MUD_ID_TYPE_4,
};

enum MUDEndian {
  MUD_ENDIAN_0,
  MUD_ENDIAN_1,
};

enum MUDSampleType {
  MUD_SAMPLE_TYPE_DEFAULT,
  MUD_SAMPLE_TYPE_PCM16,
  MUD_SAMPLE_TYPE_ADPCM,
  MUD_SAMPLE_TYPE_ADPCM_PLUS,
  MUD_SAMPLE_TYPE_PCM8,
  MUD_SAMPLE_TYPE_ADPCMVS
};

enum MUDUndoSpecialType {
  MUD_UNDO_SPECIAL_NONE = 0,
  MUD_UNDO_SPECIAL_SPECIAL = 0xFFF0,
  MUD_UNDO_SPECIAL_VOID    = 0xFFF1,
  MUD_UNDO_SPECIAL_STRING  = 0xFFF2,
};

class MUDRootInterface;
class MUDModificationInfo;
typedef void (*MUDModifyNotificationCallback)(MUDRootInterface* rootInterface, const MUDModificationInfo& modificationInfo);
class MUDBaseNodeInterface;
typedef void (*MUDNodeCallbackFunc)(MUDBaseNodeInterface* baseInterface);

#endif // _MUDTYPES_H_