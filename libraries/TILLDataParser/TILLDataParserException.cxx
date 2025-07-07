#include "TILLDataParserException.h"
#include "TILLDataParser.h"

TILLDataParserException::TILLDataParserException(TILLDataParser::EDataParserState state, int failedWord, bool multipleErrors)
   : fParserState(state), fFailedWord(failedWord), fMultipleErrors(multipleErrors)
{
   /// default constructor for TILLDataParserException, stores the data parser state and the word the parser failed on
   /// and creates a message based on them that can be accessed via TILLDataParserException::what()
   std::ostringstream stream;
   stream << "TILLDataParser failed ";
   if(fMultipleErrors) {
      stream << "on multiple words, first was ";
   } else {
      stream << "only on ";
   }
   stream << fFailedWord << ". word: ";
   switch(fParserState) {
   case TILLDataParser::EDataParserState::kGood: stream << "state is good, no idea what went wrong!" << std::endl; break;
   case TILLDataParser::EDataParserState::kBadHeader:
      stream << "bad header (either not high nibble 0x8 or an undefined bank)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kMissingWords: stream << "missing scaler words" << std::endl; break;
   case TILLDataParser::EDataParserState::kBadScalerLowTS:
      stream << "bad scaler word with low time stamp bits (high nibble not 0xa)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadScalerValue:
      stream << "bad scaler value (should never happen?)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadScalerHighTS:
      stream << "bad scaler word with high time stamp bits (either high nibble not 0xe or the 8 LSB don't match the 8 "
                "LSB of time stamp)"
             << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadScalerType: stream << "undefined scaler type" << std::endl; break;
   case TILLDataParser::EDataParserState::kBadTriggerId:
      stream << "bad word with channel trigger ID (high nibble not 0x9)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadLowTS:
      stream << "bad word with low time stamp bits (high nibble not 0xa)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadHighTS:
      stream << "bad word with deadtime/high time stamp bits (high nibble not 0xb)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kSecondHeader:
      stream << "found a second header (w/o finding a footer first)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kWrongNofWords: stream << "wrong number of words" << std::endl; break;
   case TILLDataParser::EDataParserState::kNotSingleCfd:
      stream << "expected a single cfd word, got either none or multiple ones" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kSizeMismatch:
      stream << "number of charge, cfd, and integration length words doesn't match" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadFooter:
      stream << "bad footer (mismatch between lowest 14 bits of channel trigger ID)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kFault:
      stream << "found a fault word (high nibble 0xf) from the DAQ" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kMissingPsd: stream << "missing psd words" << std::endl; break;
   case TILLDataParser::EDataParserState::kMissingCfd:
      stream << "missing the cfd word (second word w/o MSB set)" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kMissingCharge:
      stream << "missing charge words (should be at least two words w/o MSB set" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kBadBank: stream << "undefined bank" << std::endl; break;
   case TILLDataParser::EDataParserState::kBadModuleType: stream << "undefined module type" << std::endl; break;
   case TILLDataParser::EDataParserState::kEndOfData:
      stream << "reached end of bank data but not end of fragment" << std::endl;
      break;
   case TILLDataParser::EDataParserState::kUndefined:
      stream << "undefined state, should not be possible?" << std::endl;
      break;
   default: break;
   };

   fMessage = stream.str();
}

TILLDataParserException::~TILLDataParserException()
{
   /// default destructor
}

const char* TILLDataParserException::what() const noexcept
{
   /// return message string built in default constructor
   return fMessage.c_str();
}
