#include <iostream>
#include <bitset>
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

#define READ_EVENT_SIZE 10000

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
   if( fBoardHeaders != nullptr )
       delete[] fBoardHeaders;
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
      fInputStream.open(GetFilename(), std::ifstream::in | std::ifstream::binary);
      fInputStream.seekg(0, std::ifstream::end);
      if(fInputStream.tellg() < 0) {
         std::cout<<R"(Failed to open ")"<<GetFilename()<<"/"<<fFilename<<R"("!)"<<std::endl;
         return false;
      }
      fFileSize = fInputStream.tellg();

      // Read Header Information
      fInputStream.seekg(0, std::ifstream::beg);
      
      fInputStream.read(reinterpret_cast<char *>(&fVersion), sizeof(int32_t));
      fInputStream.read(reinterpret_cast<char *>(&fTimeBase), sizeof(int32_t));
      fInputStream.read(reinterpret_cast<char *>(&fnbEvents), sizeof(int32_t));
      fInputStream.read(reinterpret_cast<char *>(&fnbBoards), sizeof(int32_t));
      headerSize += 4*4; // 4 chucks of 4 Bytes

      // Read Board Headers
      fBoardHeaders = new int32_t[fnbBoards];
      fInputStream.read(reinterpret_cast<char *>(fBoardHeaders), fnbBoards * sizeof(uint32_t));
      headerSize += 4*fnbBoards;

      fReadBuffer.reserve(READ_EVENT_SIZE*4*sizeof(int32_t));
      fReadBuffer.resize(READ_EVENT_SIZE*4*sizeof(int32_t));
      fInputStream.seekg(headerSize, std::ifstream::beg);

   } catch(std::exception& e) {
      std::cout<<"Caught "<<e.what() << " at " << __FILE__ << " : "  << __LINE__ <<std::endl;
   }

	// setup TChannel to use our mnemonics
	TChannel::SetMnemonicClass(TILLMnemonic::Class());

   TRunInfo::SetRunInfo(GetRunNumber(), GetSubRunNumber());
   TRunInfo::SetRunLength(300); 
   TRunInfo::ClearVersion();
   TRunInfo::SetVersion(ILLDATA_RELEASE);

   std::cout<<"Successfully read "<<fFileSize - headerSize<<" bytes into buffer!"<<std::endl;

	TILLDetectorInformation* detInfo = new TILLDetectorInformation();
	TRunInfo::SetDetectorInformation(detInfo);

   return true;
}

void TLstFile::Close()
{
   fInputStream.close();
}

/// \param [in] Event Pointer to an empty TLstEvent
/// \returns "true" for success, "false" for failure, see GetLastError() to see why
///
///  EDITED FROM THE ORIGINAL TO RETURN TOTAL SUCESSFULLY BYTES READ INSTEAD OF TRUE/FALSE,  PCB
///
int TLstFile::Read(std::shared_ptr<TRawEvent> Event)
{
   if( Event == nullptr )
      return -1;

   size_t LastReadSize = 0;
   std::shared_ptr<TLstEvent> LstEvent = std::static_pointer_cast<TLstEvent>(Event);
   LstEvent->Clear();

   LstEvent->SetLstVersion(fVersion);


   if(fBytesRead < fFileSize) {
      // Fill the buffer
      char tempBuff[READ_EVENT_SIZE*4*sizeof(int32_t)] ; 
      try {
         fInputStream.read( tempBuff, READ_EVENT_SIZE*4*sizeof(int32_t));
         LastReadSize = static_cast<size_t>(fInputStream.gcount());
         fBytesRead += LastReadSize;

         fReadBuffer.clear();
         for(size_t i = 0; i < LastReadSize; i++) {
            fReadBuffer.push_back(tempBuff[i]);
         }

      } catch(std::exception& e) {
         std::cout<<"Caught "<<e.what() << " at " << __FILE__ << " : "  << __LINE__ <<std::endl;
      }

      // Write data to event
      LstEvent->SetData(fReadBuffer);

      return LastReadSize;
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
   // Parse the run number from the current TLstFile. This assumes a format of
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
   if(found2 < foundslash && foundslash != std::string::npos) {
      found2 = std::string::npos;
   }
   std::string temp;
   if(found2 == std::string::npos || fFilename.compare(found2 + 4, 4, ".lst") != 0) {
      temp = fFilename.substr(found - 5, 5);
   } else {
      temp = fFilename.substr(found - 9, 5);
   }
   return atoi(temp.c_str());
}

int TLstFile::GetSubRunNumber()
{
   // There are no subruns in .lst files
   return -1;
}

// end
