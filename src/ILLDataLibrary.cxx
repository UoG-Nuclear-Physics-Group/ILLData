#include <string>

#include "TLstFile.h"
#include "TILLDataParser.h"
#include "ILLDataVersion.h"

extern "C" TLstFile* CreateFile(std::string& fileName) { return new TLstFile(fileName.c_str()); }
extern "C" void DestroyFile(TLstFile* obj) { delete obj; }

extern "C" TILLDataParser* CreateParser() { return new TILLDataParser; }
extern "C" void DestroyParser(TILLDataParser* obj) { delete obj; }

extern "C" std::string LibraryVersion() { return std::string(ILLDATA_RELEASE); }
