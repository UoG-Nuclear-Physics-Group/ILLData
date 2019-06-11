#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cassert>
#include <cstdlib>

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

#include "TString.h"

#include "TLstFile.h"
#include "TLstEvent.h"
#include "TRunInfo.h"
#include "TILLDetectorInformation.h"
#include "TILLMnemonic.h"
#include "ILLDataVersion.h"

/// \cond CLASSIMP
ClassImp(TLstFile)
/// \endcond

TLstFile::TLstFile()
{
   // Default Constructor
   fBytesRead = 0;
   fFileSize  = 0;
}

TLstFile::TLstFile(const char* filename, TRawFile::EOpenType open_type) : TLstFile()
{
   switch(open_type) {
	case TRawFile::EOpenType::kRead: Open(filename); break;

	case TRawFile::EOpenType::kWrite: break;
   }
}

TLstFile::~TLstFile()
{
   // Default dtor. It closes the read in lst file as well as the output lst file.
   Close();
}

std::string TLstFile::Status(bool)
{
   return Form(HIDE_CURSOR " Processed event, have processed %.2fMB/%.2f MB              " SHOW_CURSOR "\r",
               (fBytesRead / 1000000.0), (fFileSize / 1000000.0));
}

/// Open a lst .lst file with given file name.
///
/// Remote files can be accessed using these special file names:
/// - pipein://command - read data produced by given command, see examples below
/// - ssh://username\@hostname/path/file.mid - read remote file through an ssh pipe
/// - ssh://username\@hostname/path/file.mid.gz and file.mid.bz2 - same for compressed files
/// - dccp://path/file.mid (also file.mid.gz and file.mid.bz2) - read data from dcache, requires dccp in the PATH
///
/// Examples:
/// - ./event_dump.exe /ladd/data9/t2km11/data/run02696.mid.gz - read normal compressed file
/// - ./event_dump.exe ssh://ladd09//ladd/data9/t2km11/data/run02696.mid.gz - read compressed file through ssh to ladd09
/// (note double "/")
/// - ./event_dump.exe pipein://"cat /ladd/data9/t2km11/data/run02696.mid.gz | gzip -dc" - read data piped from a
/// command or script (note quotes)
/// - ./event_dump.exe pipein://"gzip -dc /ladd/data9/t2km11/data/run02696.mid.gz" - another way to read compressed
/// files
/// - ./event_dump.exe dccp:///pnfs/triumf.ca/data/t2km11/aug2008/run02837.mid.gz - read file directly from a dcache
/// pool (note triple "/")
///
/// \param[in] filename The file to open.
/// \returns "true" for succes, "false" for error, use GetLastError() to see why
bool TLstFile::Open(const char* filename)
{
   fFilename = filename;
   int32_t headerSize = 0; // Count the number of bytes in the header

   //int32_t* boardHeaders = new int32_t[nbBoards]

   try {
      std::ifstream in(GetFilename(), std::ifstream::in | std::ifstream::binary);
      in.seekg(0, std::ifstream::end);
      if(in.tellg() < 0) {
         std::cout<<R"(Failed to open ")"<<GetFilename()<<"/"<<fFilename<<R"("!)"<<std::endl;
         return false;
      }
      fFileSize = in.tellg();

      // Read Header Information
      in.seekg(0, std::ifstream::beg);
      
      in.read(reinterpret_cast<char *>(&fVersion), sizeof(int32_t));
      in.read(reinterpret_cast<char *>(&fTimeBase), sizeof(int32_t));
      in.read(reinterpret_cast<char *>(&fnbEvents), sizeof(int32_t));
      in.read(reinterpret_cast<char *>(&fnbBoards), sizeof(int32_t));
      headerSize += 4*4; // 4 chucks of 4 Bytes

      // Read Board Headers
      fBoardHeaders = new int32_t[fnbBoards];
      in.read(reinterpret_cast<char *>(fBoardHeaders), fnbBoards * sizeof(uint32_t));
      headerSize += 4*fnbBoards;

      // Read rest of bytes into event buffer
      fReadBuffer.resize(fFileSize - headerSize);
      in.seekg(headerSize, std::ifstream::beg);
      in.read(fReadBuffer.data(), fFileSize);
      in.close();

   } catch(std::exception& e) {
      std::cout<<"Caught "<<e.what()<<std::endl;
   }
   // Do we need these?
   // signal(SIGPIPE,SIG_IGN); // crash if reading from closed pipe
   // signal(SIGXFSZ,SIG_IGN); // crash if reading from file >2GB without O_LARGEFILE

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

	// setup TChannel to use our mnemonics
	TChannel::SetMnemonicClass(TILLMnemonic::Class());

   TRunInfo::SetRunInfo(GetRunNumber(), GetSubRunNumber());
   TRunInfo::ClearVersion();
   TRunInfo::SetVersion(ILLDATA_RELEASE);

   std::cout<<"Successfully read "<<fFileSize - headerSize<<" bytes into buffer!"<<std::endl;

	TILLDetectorInformation* detInfo = new TILLDetectorInformation();
	TRunInfo::SetDetectorInformation(detInfo);

   return true;
}

void TLstFile::Close()
{
}

/// \param [in] lstEvent Pointer to an empty TLstEvent
/// \returns "true" for success, "false" for failure, see GetLastError() to see why
///
///  EDITED FROM THE ORIGINAL TO RETURN TOTAL SUCESSFULLY BYTES READ INSTEAD OF TRUE/FALSE,  PCB
///
int TLstFile::Read(std::shared_ptr<TRawEvent> lstEvent)
{
   if(fBytesRead < fFileSize) {
      try {
         std::static_pointer_cast<TLstEvent>(lstEvent)->SetData(fReadBuffer);
      } catch(std::exception& e) {
         std::cout<<e.what()<<std::endl;
      }
      fBytesRead = fFileSize;
      return fFileSize;
   }
   return 0;
}

void TLstFile::Skip(size_t)
{
	std::cerr<<"Sorry, but we can't skip events in an LST file, the whole file is treated as a single event!"<<std::endl;
	return;
}

int TLstFile::GetRunNumber()
{
   // Parse the run number from the current TMidasFile. This assumes a format of
   // run#####_###.lst or run#####.lst.
   if(fFilename.length() == 0) {
      return 0;
   }
   std::size_t foundslash = fFilename.rfind('/');
   std::size_t found      = fFilename.rfind(".lst");
   if(found == std::string::npos) {
      return 0;
   }
   std::size_t found2 = fFilename.rfind('-');
   if((found2 < foundslash && foundslash != std::string::npos) || found2 == std::string::npos) {
      found2 = fFilename.rfind('_');
   }
   //   printf("found 2 = %i\n",found2);
   if(found2 < foundslash && foundslash != std::string::npos) {
      found2 = std::string::npos;
   }
   std::string temp;
   if(found2 == std::string::npos || fFilename.compare(found2 + 4, 4, ".lst") != 0) {
      temp = fFilename.substr(found - 5, 5);
   } else {
      temp = fFilename.substr(found - 9, 5);
   }
   // printf(" %s \t %i \n",temp.c_str(),atoi(temp.c_str()));
   return atoi(temp.c_str());
}

int TLstFile::GetSubRunNumber()
{
   // Parse the sub run number from the current TMidasFile. This assumes a format of
   // run#####_###.lst or run#####.lst.
   if(fFilename.length() == 0) {
      return -1;
   }
   std::size_t foundslash = fFilename.rfind('/');
   std::size_t found      = fFilename.rfind('-');
   if((found < foundslash && foundslash != std::string::npos) || found == std::string::npos) {
      found = fFilename.rfind('_');
   }
   if(found < foundslash && foundslash != std::string::npos) {
      found = std::string::npos;
   }
   if(found != std::string::npos) {
      std::string temp = fFilename.substr(found + 1, 3);
      // printf("%i \n",atoi(temp.c_str()));
      return atoi(temp.c_str());
   }
   return -1;
}

// end
