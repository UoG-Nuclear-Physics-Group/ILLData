#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "TChannel.h"
#include "TFipps.h"

int main(int argc, char** argv) {
	if(argc < 4) {
		std::cerr<<"Usage: "<<argv[0]<<" <LUT file> <cal file(s)> <cross talk file>"<<std::endl;
		return 1;
	}

	std::stringstream str;
	std::string line;

	TChannel* channel = nullptr;

	int adc;
	int detType;
	int detNumber;
	int cryNumber;
	int globId;
	int timeOffset;

	//////////////////////////////////////// look up table ////////////////////////////////////////
	std::ifstream lutFile(argv[1]);

	while(std::getline(lutFile, line)) {
		if(line.empty() || std::all_of(line.begin(), line.end(), [](char c){ return std::isspace(c); }) || line[0] == '#') continue;

		str.clear();
		str.str(line);

		str>>adc>>detType>>detNumber>>cryNumber>>globId>>timeOffset;// ignoring rangemin, rangemax at the end of the line
		//std::cout<<"line \""<<line<<"\" adc "<<adc<<", detType "<<detType<<", cryNumber "<<cryNumber<<", globId "<<globId<<", timeOffset "<<timeOffset<<std::endl;

		// Detector Types:
		//  -- 0 -> NotUsed
		//  -- 1 -> FIPPS Ge
		//  -- 2 -> IFIN Ge
		//  -- 3 -> FIPPS AC
		//  -- 4 -> IFIN AC
		//  -- 5 -> Fission Tag
		//  -- 6 -> LaBr3
		//  -- 7 -> TAC
		//
		// In case of AC, CloverId -> Associated clover
		//
		// DetId:  0->3 for Ge
		//         0    for IFIN AC
		//         0->2 for FIPPS AC (Back, Front, Side)
		//         id   for TAG, LaBr3, TAC

		str.clear();
		str.str(std::string());
		//std::cout<<"cleared - "<<str.str()<<std::endl;
		switch(detType) {
			case 1: // FIPPS Ge
				str<<"FIG";
				break;
			case 2: // IFIN Ge
				str<<"IFG";
				break;
			case 3: // FIPPS BGO
				str<<"FIS";
				break;
			case 4: // IFIN BGO
				str<<"IFS";
				break;
			default:
				std::cerr<<"unknown detector type "<<detType<<std::endl;
				continue;
		}
		//std::cout<<"system - "<<str.str()<<std::endl;
		str<<std::setfill('0')<<std::setw(2)<<detNumber+1; // LUT starts counting at 0, cal file starts at 1
		//std::cout<<"detector # - "<<str.str()<<std::endl;
		if(detType == 1 || detType == 2) { // Ge detectors - set crystals as colors
			str<<TFipps::GetColorFromNumber(cryNumber);
		} else {
			str<<std::setw(1)<<cryNumber;
		}
		//std::cout<<"crystal - "<<str.str()<<std::endl;
		str<<"N00X";
		//std::cout<<"final - "<<str.str()<<std::endl;
		channel = new TChannel(str.str().c_str());
		channel->SetAddress(adc);
		channel->SetNumber(TPriorityValue<int>(globId+1, EPriority::kForce));
		channel->SetTimeOffset(TPriorityValue<Long64_t>(-timeOffset, EPriority::kForce));
		TChannel::AddChannel(channel);
		//channel->Print();
	}

	//////////////////////////////////////// energy calibration ////////////////////////////////////////
	double a0;
	double a1;
	double a2;
	double a3;
	int minRange;
	int maxRange;
	for(int i = 2; i < argc-1; ++i) {
		std::ifstream calFile(argv[i]);
		while(std::getline(calFile, line)) {
			if(line.empty() || std::all_of(line.begin(), line.end(), [](char c){ return std::isspace(c); }) || line[0] == '#') continue;

			str.clear();
			str.str(line);
			str>>globId>>a0>>a1>>a2>>a3>>minRange>>maxRange;
			channel = TChannel::GetChannelByNumber(globId+1);
			if(channel != nullptr) {
				channel->DestroyENGCal();
				channel->AddENGCoefficient(a0);
				channel->AddENGCoefficient(a1);
				channel->AddENGCoefficient(a2);
				channel->AddENGCoefficient(a3);
				//std::cout<<"Set energy coefficients for channel "<<globId+1<<" - "<<channel->GetMnemonic()->SystemString()<<channel->GetMnemonic()->SubSystemString()<<channel->GetMnemonic()->ArrayPosition()<<std::endl;
			} else {
				std::cerr<<"Failed to find detector ID "<<globId+1<<" in TChannel"<<std::endl;
			}
		}
	}

	//////////////////////////////////////// cross talk ////////////////////////////////////////
	std::ifstream xTalkFile(argv[argc-1]);
	detNumber = 1;
	int col;
	int row;
	double val;
	while(std::getline(xTalkFile, line)) {
		if(line.empty() || std::all_of(line.begin(), line.end(), [](char c){ return std::isspace(c); }) || line[0] == '#') continue;

		str.clear();
		str.str(line);
		str>>col>>row>>val;
		//std::cout<<"from line \""<<line<<"\" got col "<<col<<", row "<<row<<", val "<<val<<std::endl;

		// col = affected crystal, row = affecting crystal
		if(row == 0) {
			channel = TChannel::FindChannelByName(Form("FIG%02d%sN00X", detNumber, TFipps::GetColorFromNumber(col)));
			if(channel == nullptr) {
				channel = TChannel::FindChannelByName(Form("IFG%02d%sN00X", detNumber, TFipps::GetColorFromNumber(col)));
			}
			if(channel == nullptr) {
				std::cerr<<"Failed to find channel \""<<Form("FI/IFG%02d%sN00X", detNumber, TFipps::GetColorFromNumber(col))<<"\""<<std::endl;
				std::cerr<<"Got color "<<TFipps::GetColorFromNumber(col)<<" from number "<<col<<std::endl;
				break;
			}
			channel->DestroyCTCal();
			
			//std::cout<<"destroyed cross talk coefficients for channel "<<channel->GetMnemonic()->SystemString()<<channel->GetMnemonic()->SubSystemString()<<std::setw(2)<<channel->GetMnemonic()->ArrayPosition()<<TFipps::GetColorFromNumber(channel->GetMnemonic()->Segment())<<std::endl;
		}
		channel->AddCTCoefficient(val);
		if(col == 3 && row == 3) ++detNumber;
	}

	//std::cout<<std::endl<<"done"<<std::endl<<std::endl;

	TChannel::WriteCalFile();
}
