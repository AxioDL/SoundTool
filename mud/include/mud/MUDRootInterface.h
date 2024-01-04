#ifndef _MUDROOTINTERFACE_H_
#define _MUDROOTINTERFACE_H_


#include <f5/types.h>
#include <f5/f5classes.h>
#include <mud/MUDBaseNodeInterfaceList.h>
#include <mud/MUDTypes.h>

class MUDRootInterface;
class MUDBaseNodeInterface;
class MUDTypeStorage;
class MUDModificationInfo;
typedef void (*MUDLoadNodeCallback)(MUDBaseNodeInterface* nodeInterface);

class MUDRootInterface {
public:
  MUDBaseNodeInterface* CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, const MUDTypeStorage* type, const MUDTypeStorage* forcedInstanceID);
  virtual MUDBaseNodeInterface* CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, const MUDTypeStorage* type)=0;
  bool DeleteNode(MUDBaseNodeInterface* node, bool deleteSiblings) const;
  virtual bool DeleteNode(MUDBaseNodeInterface* node) const=0;
  virtual bool DeleteHiddenNodes(MUDBaseNodeInterface * node)const =0;
  virtual const MUDBaseNodeInterface* GetRootNode() const=0;
  virtual MUDBaseNodeInterface* GetRootNode()=0;
  virtual MUDBaseNodeInterfaceList FindNode(const MUDTypeStorage* type) const=0;
  virtual const MUDBaseNodeInterface* FindNode(u16 id) const=0;
  virtual MUDBaseNodeInterface* FindNode(u16 id)=0;
  virtual const MUDBaseNodeInterface* FindNode(const f5::string& name) const=0;
  virtual MUDBaseNodeInterface* FindNode(const f5::string& name)=0;
  virtual bool RenameNode(MUDBaseNodeInterface* nodeInterface, const f5::string& _name)=0;
  virtual bool SetNodeType(MUDBaseNodeInterface* nodeInterface,const MUDTypeStorage* type)=0;
  virtual bool SetHidden(MUDBaseNodeInterface* node, bool flag)=0;
  virtual bool SetReadOnly(MUDBaseNodeInterface* node, bool flag)=0;
  virtual bool SetDirectory(MUDBaseNodeInterface* node, bool flag)=0;
  virtual bool SetSystem(MUDBaseNodeInterface* node, bool flag)=0;
  virtual bool MoveNode(MUDBaseNodeInterface * nodeInterface, MUDBaseNodeInterface* newParentInterface)=0;
  virtual MUDBaseNodeInterface* CopyNode(const MUDBaseNodeInterface* node, const f5::string& name,MUDBaseNodeInterface * parent)=0;
  virtual MUDState GetState() const=0;
  virtual bool GetLastError(f5::string& text) const=0;
  virtual void EnableIDTranslation()=0;
  virtual void DisableIDTranslation()=0;
  virtual u16 ImportIDReference(u16 oldID, MUDIDType type)=0;
  virtual bool ForceIDTranslation(u16 oldID, u16 newID, MUDIDType idTypey)=0;
  virtual MUDBaseNodeInterface* ImportNode(MUDBaseNodeInterface* parent, const f5::string& name, const MUDTypeStorage* type, u16 oldID, MUDIDType idType)=0;
  virtual bool Load(const f5::string& filename)=0;
  virtual bool Save(const f5::string& filename)=0;
  virtual const f5::string& GetFilename() const=0;
  virtual void SetFilename(const f5::string& filename)=0;
  virtual const f5::string& GetCurrentLoadLocation() const=0;
  virtual void SetExportEndian(MUDEndian endian)=0;
  virtual MUDEndian GetExportEndian() const=0;
  virtual void SetDefaultSampleType(MUDSampleType type)=0;
  virtual void OnlyDefaultSampleType(bool on)=0;
  virtual MUDSampleType GetDefaultSampleType() const=0;
  virtual bool GetOnlyDefaultSampleType() const=0;
  virtual const MUDBaseNodeInterface* GetSampleSetupNode() const=0;
  virtual MUDBaseNodeInterface* GetSampleSetupNode()=0;
  virtual const MUDBaseNodeInterface* GetMacroSetupNode() const=0;
  virtual MUDBaseNodeInterface* GetMacroSetupNode()=0;
  virtual bool SetSampleSetupNode(MUDBaseNodeInterface * sampleSetup)=0;
  virtual bool SetMacroSetupNode(MUDBaseNodeInterface* macroSetup)=0;
  virtual bool SetLoadNodeCallback(const MUDTypeStorage& type, MUDLoadNodeCallback callback)=0;
  virtual bool EnableUndo()=0;
  virtual bool DisableUndo()=0;
  virtual bool IsUndoEnabled() const=0;
  virtual bool Undo()=0;
  virtual bool GetCurrentUndoDescription(f5::string& description) const=0;
  virtual bool StartUndoGroup()=0;
  virtual bool StopUndoGroup(const f5::string& description)=0;
  virtual bool ClearUndoList()=0;
  virtual bool DisplayUndoList() const=0;
  virtual void SetModifyNotificationCallback(MUDModifyNotificationCallback callback)=0;
  virtual MUDBaseNodeInterface * GetNodeFromInstanceID(const MUDTypeStorage& instID, MUDBaseNodeInterface* node)=0;
  virtual bool IsShutDownActive()=0;
  virtual const GUID* GetDataBaseSubType() const=0;
  virtual bool SetDataBaseSubType(const GUID* )=0;
};

#endif // _MUDROOTINTERFACE_H_