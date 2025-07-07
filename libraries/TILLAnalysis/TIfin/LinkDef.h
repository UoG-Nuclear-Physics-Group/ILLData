//TIfin.h TIfinHit.h TIfinBgo.h TIfinBgoHit.h
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off     nestedclasses;

//#pragma link C++ class std::vector<Short_t>+;

#pragma link C++ class TIfinHit + ;
#pragma link C++ class std::vector < TIfinHit> + ;
#pragma link C++ class std::vector < TIfinHit*> + ;
#pragma link C++ class TIfin + ;
#pragma link C++ class TIfinBgoHit + ;
#pragma link C++ class TIfinBgo + ;

#endif
