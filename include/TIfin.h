#ifndef TIFIN_H
#define TIFIN_H

/** \addtogroup Detectors
 *  @{
 */

#include <utility>
#include <vector>
#include <cstdio>
#include <functional>
//#include <tuple>

#include "TBits.h"
#include "TVector3.h"

#include "Globals.h"
#include "TIfinHit.h"
#include "TSuppressed.h"
#include "TTransientBits.h"

////////////////////////////////////////////////////////////
///
/// \class TIfin
///
/// The TIfin class defines the observables and algorithms used
/// when analyzing IFIN data. It includes detector positions,
/// add-back methods, etc.
///
////////////////////////////////////////////////////////////

class TIfin : public TSuppressed {
public:
   enum class EIfinBits {
      kIsAddbackSet             = 1<<0,
      kIsCrossTalkSet           = 1<<1,
      kIsSuppressedSet          = 1<<2,
      kIsSuppressedAddbackSet   = 1<<3,
      kBit4                     = 1<<4,
      kBit5                     = 1<<5,
      kBit6                     = 1<<6,
      kBit7                     = 1<<7
   };

   TIfin();
   TIfin(const TIfin&);
   ~TIfin() override;

public:
   TIfinHit* GetIfinHit(const Int_t& i);

   static TVector3 GetPosition(int DetNbr, int CryNbr = 5, double dist = 90.0); //!<!
   static const char* GetColorFromNumber(int number);
#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
   void ResetFlags() const;

   TIfin& operator=(const TIfin&); //!<!

#if !defined(__CINT__) && !defined(__CLING__)
   void SetAddbackCriterion(std::function<bool(const TDetectorHit*, const TDetectorHit*)> criterion)
   {
      fAddbackCriterion = std::move(criterion);
   }
   std::function<bool(const TDetectorHit*, const TDetectorHit*)> GetAddbackCriterion() const { return fAddbackCriterion; }

   bool AddbackCriterion(const TDetectorHit* hit1, const TDetectorHit* hit2) override { return fAddbackCriterion(hit1, hit2); }
#endif

   Int_t      GetAddbackMultiplicity();
   TIfinHit* GetAddbackHit(const int& i);
   bool       IsAddbackSet() const;
   void       ResetAddback();
   void       ResetSuppressed();
   void       ResetSuppressedAddback();
   UShort_t   GetNAddbackFrags(const size_t& idx);

   TIfinHit* GetSuppressedHit(const int& i);
   Int_t      GetSuppressedMultiplicity( const TBgo* bgo );
   bool       IsSuppressed() const;

#if !defined(__CINT__) && !defined(__CLING__)
   void SetSuppressionCriterion(std::function<bool(const TDetectorHit*, const TDetectorHit*)> criterion)
   {
      fSuppressionCriterion = std::move(criterion);
   }
   std::function<bool(const TDetectorHit*, const TDetectorHit*)> GetSuppressionCriterion() const { return fSuppressionCriterion; }

   bool SuppressionCriterion(const TDetectorHit* hit, const TDetectorHit* bgoHit) override { return fSuppressionCriterion(hit, bgoHit); }
#endif

   TIfinHit* GetSuppressedAddbackHit( const int& i );
   Int_t      GetSuppressedAddbackMultiplicity(const TBgo* bgo);
   bool       IsSuppressedAddbackSet() const;

private:
#if !defined(__CINT__) && !defined(__CLING__)
   static std::function<bool(const TDetectorHit*, const TDetectorHit*)> fAddbackCriterion;
   static std::function<bool(const TDetectorHit*, const TDetectorHit*)> fSuppressionCriterion;
#endif

   // static bool fSetBGOHits;                //!<!  Flag that determines if BGOHits are being measured

   mutable TTransientBits<UChar_t> fIfinBits; // Transient member flags

   mutable std::vector<TDetectorHit*> fAddbackHits;  //!<! Used to create addback hits on the fly
   mutable std::vector<UShort_t>  fAddbackFrags; //!<! Number of crystals involved in creating in the addback hit
   mutable std::vector<TDetectorHit*> fSuppressedHits;
   mutable std::vector<TDetectorHit*> fSuppressedAddbackHits;
   mutable std::vector<UShort_t> fSuppressedAddbackFrags;

public:
   // static bool SetBGOHits()       { return fSetBGOHits;   }  //!<!

private:
   static TVector3 gCloverPosition[17];                    //!<! Position of each HPGe Clover
   void            ClearStatus() const { fIfinBits = 0; } //!<!
   void SetBitNumber(EIfinBits bit, Bool_t set) const;
   Bool_t TestBitNumber(EIfinBits bit) const { return fIfinBits.TestBit(bit); }

   // Cross-Talk stuff
public:
   static const Double_t gStrongCT[2];           //!<!
   static const Double_t gWeakCT[2];             //!<!
   static const Double_t gCrossTalkPar[2][4][4]; //!<!
   static Double_t CTCorrectedEnergy(const TIfinHit* const hit_to_correct, const TIfinHit* const other_hit,
                                     Bool_t time_constraint = true);
   Bool_t IsCrossTalkSet() const;
   void   FixCrossTalk();

private:
   // This is where the general untouchable functions live.
   std::vector<TDetectorHit*>& GetAddbackVector();     //!<!
   std::vector<UShort_t>&   GetAddbackFragVector(); //!<!
   std::vector<TDetectorHit*>& GetSuppressedVector();
   std::vector<TDetectorHit*>& GetSuppressedAddbackVector();
   std::vector<UShort_t>& GetSuppressedAddbackFragVector();
   void SetAddback(bool flag = true) const;
   void SetCrossTalk(bool flag = true) const;
   void SetSuppressed(bool flag = true) const;
   void SetSuppressedAddback(bool flag = true) const;

public:
   void Copy(TObject&) const override;            //!<!
   void Clear(Option_t* opt = "all") override;    //!<!
   void Print(Option_t* opt = "") const override; //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TIfin, 1) // Ifin Physics structure
   /// \endcond
};
/*! @} */
#endif
