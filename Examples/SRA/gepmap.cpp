#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/BasicBlock.h"
#include <set>
#include "llvm/Support/InstIterator.h"
#include "gepmap.h"
#include <iostream>

using namespace llvm;

class GEPCompare {
public:
  bool operator() (const Value* x, const Value* y) const {
    const User *ux = static_cast<const User*>(x);
    const User *uy = static_cast<const User*>(y);
    if (ux->getNumOperands() < uy->getNumOperands())
      return true;

    if (ux->getNumOperands() > uy->getNumOperands())
      return false;
    
    for(unsigned i=1; i<uy->getNumOperands(); i++) {
      if (ux->getOperand(i)!=uy->getOperand(i)) {
	const ConstantInt *cix = cast<ConstantInt>(ux->getOperand(i));
	const ConstantInt *ciy = cast<ConstantInt>(uy->getOperand(i));
	if (cix->getSExtValue() < ciy->getSExtValue()) {
	  return true;
	} else if (cix->getSExtValue() > ciy->getSExtValue()){
	  return false;
	}
      }
    }
    return false;
  }
};

typedef std::map<Value*,Value*,GEPCompare> GEPMap_internal;
/* Create an empty worklist */

GEPMap GEPMapCreate()
{
  GEPMap_internal *map = new GEPMap_internal();
  return (GEPMap) map;
}

void GEPMapDestroy(GEPMap map)
{
  GEPMap_internal *m = (GEPMap_internal*)map;
  delete m;
}

int GEPMapInsert(GEPMap map, LLVMValueRef key, LLVMValueRef data)
{
  Value *k = unwrap(key);
  Value *d = unwrap(data);

  GEPMap_internal *m = (GEPMap_internal*) map;
  
  if (m->find(k)==m->end()) {
    m->insert( std::pair<Value*,Value*>(k,d) );
    return 1;
  } else {
    return 0;
  }
}

LLVMValueRef GEPMapFind(GEPMap map, LLVMValueRef key) 
{
  Value *k = unwrap(key);
  GEPMap_internal *m = (GEPMap_internal*) map;

  if (m->find(k) == m->end()) {
    return NULL;
  }
  else {
    return wrap(m->find(k)->second);
  }
}
