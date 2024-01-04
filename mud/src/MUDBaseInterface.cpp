#include <mud/MUDBaseInterface.h>

#include <mud/MUDServiceDataBase.h>
#include <mud/MUDServiceInOutInterface.h>
#include <mud/MUDServiceScript.h>

#include <mpi/MPIVersionInfo.h>
#include <mpi/MPIInterfaceDescriptor.h>

#include <scripthost.h>
#include <rpc.h>

static const MPIInterfaceInfo Interfaces[] = {
  { 
    {"Database Service"}, 
    {0xFB8D35DEL, 0xA9E8, 0x11D3, {0x87, 0x67, 0x00, 0x50, 0x04, 0x69, 0xA2, 0x67}}, 
    {0}, 
    0
  },
  { 
    {"Script Service"}, 
    {0xF050E2BAL, 0xAA92, 0x11D3, {0x87, 0x67, 0x00, 0x50, 0x04, 0x69, 0xA2, 0x67}}, 
    {0}
  },
  { 
    {"InOut Service"}, 
    {0x1251B836L, 0xAD35, 0x11D3, {0x87, 0x69, 0x00, 0x50, 0x04, 0x69, 0xA2, 0x67}}, 
    {0}, 
    0
  },
};

static s32 GetInterfaceIndex(GUID* id);

static const MPIVersionInfo versionInfo = {
  {"MUD Services"},
  {"(C)1999 Factor5,LLC"},
  {0},
  {"Thomas Engel"},
  0x100
};

MPIMUDServiceBaseInterface::MPIMUDServiceBaseInterface() {
  printf("BUILT!!!\n");
  managerInterface = NULL;
  for (u32 i = 0; i < sizeof(Interfaces) / sizeof(Interfaces[0]); ++i) {
    interfaceInfos.push_back(Interfaces[i]);
  }
  databaseInterfaceCount = 0;
  scriptInterfaceCount = 0;
  inOutInterfaceCount = 0;
}


void MPIMUDServiceBaseInterface::SetManagerInterface(MPIManagerInterface* _managerInterface) {
  assert(managerInterface!=NULL);
  managerInterface = _managerInterface;
}

const MPIVersionInfo* MPIMUDServiceBaseInterface::GetVersionInfo() const {
  return &versionInfo;
}

const MPIInterfaceInfo* MPIMUDServiceBaseInterface::GetFirstInterface() const {
  if (!interfaceInfos.empty()) {
    currentInterfaceInfo = interfaceInfos.begin();
    return &*(currentInterfaceInfo++);
  }

  return NULL;
}

const MPIInterfaceInfo* MPIMUDServiceBaseInterface::GetNextInterface() const {
  if (currentInterfaceInfo != interfaceInfos.end()) {
    return &*(currentInterfaceInfo++);
  }

  return NULL;
}

static const char* GuidToString(const GUID* guid, char* buf) {
  sprintf(buf, "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}", guid->Data1, guid->Data2, guid->Data3,
          guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3], guid->Data4[4], guid->Data4[5],
          guid->Data4[6], guid->Data4[7]);
  return buf;
}

void* MPIMUDServiceBaseInterface::CreateInterface(const MPIInterfaceDescriptor* interfaceDesc) {
  char guidStr[40];
  GuidToString(&interfaceDesc->id, guidStr);
  printf("Attempting to create interface %s\n", guidStr);
  assert(interfaceDesc!=NULL);
  void* inf;

  s32 interfaceIdx = GetInterfaceIndex((GUID*)&interfaceDesc->id);

  switch(interfaceIdx) {
  case 0:
    printf("BUILDING DataBase!\n");
    if ((inf = (void*)(new MUDServiceDataBase()))) {
      ++databaseInterfaceCount;
    }

    return inf;
  case 1:
    printf("BUILDING Script!\n");
    if ((inf = (void*)(new MUDServiceScript()))) {
      ++scriptInterfaceCount;
    }

    return inf;
  case 2:
    printf("BUILDING InOut!\n");
    MUDServiceInOutInterface* io = MUDServiceInOutInterface::Initialize();

    if (io) {
      if (scrhostAddFunction("Message", MUDServiceInOutInterface::scrMessage, SRCHOST_PT_0, SCRHOSTparam(2, 17, 18))
        && scrhostAddFunction("Verbose", MUDServiceInOutInterface::scrVerbose, SRCHOST_PT_0, SCRHOSTparam(2, 17, 18))
        && scrhostAddFunction("Error", MUDServiceInOutInterface::scrError, SRCHOST_PT_0, SCRHOSTparam(2, 17, 18))) {
        ++inOutInterfaceCount;
        return io;
      }
      MUDServiceInOutInterface::Shutdown();
    }
  }
  return NULL;
}

void* MPIMUDServiceBaseInterface::CreateInterface(const GUID* guid,const GUID* subType) {
  MPIInterfaceDescriptor interDesc;
  interDesc.id = *guid;
  interDesc.hasSubType = subType != NULL;
  if (subType) {
    interDesc.subType = *subType;
  }

  return CreateInterface(&interDesc);
}


void MPIMUDServiceBaseInterface::ReleaseInterface(const MPIInterfaceDescriptor* interfaceDesc, void* interfacePtr) {
  assert(interfacePtr && interfaceDesc);

  switch(GetInterfaceIndex((GUID*)&interfaceDesc->id)) {
  case 0:
    --databaseInterfaceCount;
    delete reinterpret_cast<MUDServiceDataBase*>(interfacePtr);
    break;
  case 1:
    --scriptInterfaceCount;
    delete reinterpret_cast<MUDServiceScript*>(interfacePtr);
    break;
  case 2:
    --inOutInterfaceCount;
    delete reinterpret_cast<MUDServiceInOutInterface*>(interfacePtr);
    break;
  }
}

void MPIMUDServiceBaseInterface::ReleaseInterface(const GUID* guid, void* interfacePtr) {
  assert(interfacePtr && guid);

  MPIInterfaceDescriptor interDesc;
  interDesc.id = *guid;
  ReleaseInterface(&interDesc, interfacePtr);
}

static s32 GetInterfaceIndex(GUID* id) {
  RPC_STATUS stat;
  for (u32 i = 0; i < sizeof(Interfaces) / sizeof(Interfaces[0]); ++i) {
    if (!UuidCompare((GUID*)&Interfaces[i].id, id, &stat)) {
      return i;
    }
  }

  return -1;
}

bool MPIMUDServiceBaseInterface::RequestRemoval() { return !databaseInterfaceCount && !scriptInterfaceCount && !inOutInterfaceCount; }
