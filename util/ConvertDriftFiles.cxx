#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "TChannel.h"

int main(int argc, char** argv)
{
   if(argc < 3) {
      std::cerr << "Usage: " << argv[0] << " <cal-file> <drift-files>" << std::endl;
      return 1;
   }

   for(int i = 2; i < argc; ++i) {
      // re-open the TChannel for each file to reset all parameters
      TChannel::DeleteAllChannels();
      TChannel::ReadCalFile(argv[1]);

      // open the input file
      std::ifstream driftFile(argv[i]);
      // get the run number from the file name
      std::string       name(argv[i]);
      std::stringstream str(name.substr(name.size() - 10, 6));
      int               runNumber;
      str >> runNumber;

      // create name for the output cal-file from the run number
      str.str("");
      str.clear();
      str << "run" << runNumber << ".cal";
      name = str.str();

      std::string line;
      while(std::getline(driftFile, line)) {
         // skip empty lines and comments
         if(line.empty() || std::all_of(line.begin(), line.end(), [](char c) { return std::isspace(c); }) || line[0] == '#') continue;

         int    globId;
         double coefficent;

         str.str(line);
         str.clear();
         str >> globId;
         //std::cout<<"from line \""<<line<<"\" got globId "<<globId<<std::endl;

         TChannel* channel = TChannel::GetChannelByNumber(globId + 1);

         if(channel == nullptr) {
            std::cerr << "Failed to find a channel for ID " << globId << std::endl;
            continue;
         }
         //std::cout<<"Found channel "<<channel->GetName()<<" for ID "<<globId<<std::endl;

         while(!(str >> coefficent).fail()) {
            channel->AddENGDriftCoefficent(coefficent);
         }
      }

      // write the cal-file
      TChannel::WriteCalFile(name);
   }

   return 0;
}
