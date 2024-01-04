#include <mud/MUDDataBaseBuilder.h>
#include <mud/MUDRoot.h>

MUDRootInterface* MUDDataBaseBuilder::CreateDataBase(MPIManagerInterface * mpi, const f5::string& name, const GUID* subType) {
  MUDRoot* db;
  if (!MUDDataBaseBuilder::FindDataBase(name)) {
    if ((db = new MUDRoot(mpi, subType))) {
      MUDRoot::GetDataBaseList().push_back(MUDRoot::MUDRootEntry(db, name));
      return db;
    }
  }
  return NULL;
}

bool MUDDataBaseBuilder::DestroyDataBase(const f5::string& name) {
  std::list<MUDRoot::MUDRootEntry>::const_iterator it;
  for (it = MUDRoot::GetDataBaseList().begin(); it != MUDRoot::GetDataBaseList().end(); ++it) {
    if ((*it).GetName().str() == name.str()) {
      break;
    }
  }

  if (it != MUDRoot::GetDataBaseList().end()) {
    ((MUDRoot*)(*it).GetNode())->ShutDownActive();
    (*it).GetNode()->ClearUndoList();
    (*it).GetNode()->DisableUndo();
    delete (*it).GetNode();
    return true;
  }
  return false;
}

MUDRootInterface * MUDDataBaseBuilder::FindDataBase(const f5::string& name) {
  std::list<MUDRoot::MUDRootEntry>::const_iterator it;
  for (it = MUDRoot::GetDataBaseList().begin(); it != MUDRoot::GetDataBaseList().end(); ++it) {
    if ((*it).GetName().str() == name.str()) {
      return (*it).GetNode();
    }
  }
  return NULL;
}

std::list<f5::string> MUDDataBaseBuilder::GetAllDataBaseNames(void) {
  std::list<f5::string> list;
  std::list<MUDRoot::MUDRootEntry>::const_iterator it;
  for (it = MUDRoot::GetDataBaseList().begin(); it != MUDRoot::GetDataBaseList().end(); ++it) {
    list.push_back((*it).GetName());
  }

  return list;
}