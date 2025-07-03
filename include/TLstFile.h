#ifndef TLSTFILE_H
#define TLSTFILE_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TLstFile
///
/// This Class is used to read and write LST files in the
/// root framework.
///
/////////////////////////////////////////////////////////////////

#include <string>

#ifdef __APPLE__
#include <_types/_uint32_t.h>
#else
#include <cstdint>
#endif

#include "TRawFile.h"

#include "TLstEvent.h"

/// Reader for ILL .lst files

class TLstFile : public TRawFile {
public:
   TLstFile() = default; ///< default constructor
   TLstFile(const char* filename, TRawFile::EOpenType open_type = TRawFile::EOpenType::kRead);
   ~TLstFile() override; ///< destructor

   bool Open(const char* filename) override; ///< Open input file

   void Close() override; ///< Close input file

   using TObject::Read;
   using TObject::Write;
#ifndef __CINT__
   int Read(std::shared_ptr<TRawEvent> lstEvent) override; ///< Read one event from the file
#endif
	void Skip(size_t nofEvents) override; ///< Skip nofEvents from the file
   std::string Status(bool long_file_description = true) override;

   int GetRunNumber() override;
   int GetSubRunNumber() override;

   int32_t Version() { return fVersion; }
   int32_t TimeBase() { return fTimeBase; }
   int32_t NbEvents() { return fNbEvents; }
   int32_t NbBoards() { return fNbBoards; }
   int32_t* BoardHeaders() { return fBoardHeaders; }
#ifndef __CINT__
   std::shared_ptr<TRawEvent> NewEvent() override { return std::make_shared<TLstEvent>(); }
#endif
private:
	void ParseHeaders();

   int32_t fVersion;
   int32_t fTimeBase;
   int32_t fNbEvents;
   int32_t fNbBoards;
   int32_t* fBoardHeaders;
   std::ifstream fInputStream;

protected:
   /// \cond CLASSIMP
   ClassDefOverride(TLstFile, 0) // Used to open and write LST Files
   /// \endcond
};
/*! @} */
#endif // TLstFile.h
