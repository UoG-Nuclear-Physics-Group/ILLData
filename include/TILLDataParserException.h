#ifndef TILLDATAPARSEREXCEPTION_H
#define TILLDATAPARSEREXCEPTION_H
/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

#include "TILLDataParser.h"

class TILLDataParserException : public std::exception {
public:
   TILLDataParserException(TILLDataParser::EDataParserState state, int failedWord, bool multipleErrors);
   ~TILLDataParserException() override;

   const char* what() const noexcept override;

   int                              GetFailedWord() { return fFailedWord; }
   TILLDataParser::EDataParserState GetParserState() { return fParserState; }
   bool                             GetMultipleErrors() { return fMultipleErrors; }

private:
   TILLDataParser::EDataParserState fParserState;
   int                              fFailedWord;
   bool                             fMultipleErrors;
   std::string                      fMessage;
};
/*! @} */
#endif
