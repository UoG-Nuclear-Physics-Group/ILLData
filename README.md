# ILLData

This library provides the data parser and detector classes for data taken with FIPPS at the ILL.
The names of the classes typically start with TILL.

The data parser is set up to process FIPPS data written in the lst data format.

Several detector classes are implimented: Fipps HPGe (TFipps), LaBr (TFippsLaBr), TAC (TFippsTAC), etc.

Other classes included are:
 - File and event classes for LST files,
 - TILLMnemonic, which provides an enumeration for the detector systems and digitizers,
 - TILLDetectorInformation, which only provides functions to tell whether FIPPS data is present (which should be all the time), and
 - TILLDetectorHit, which implemements the algorithm for how to get the time in nanoseconds from the timestamp and CFD information.

## Cal Information

The most of the detector nomenclature of ILLData follows the [GRSI wiki](https://grsi.wiki.triumf.ca/index.php/Detector_Nomenclature). The exceptions are:
 - **FI** denotes the TFipps system
 - **PU** denotes a pulser, and is based on the Generic Detector system

## Transfering Fipps LUTs to Cal files

Fipps detectors are labeled from 0 to 15, while the detector numbering in the cal file are labeled from 1 to 16. Fipps detector numbers from the experiment LUTs should be incremented by one and then inputed into the cal file.

In order to match the GRIFFIN clover colors to the FIPPS clover numbers the following table should be used:

| FIPPS Clover Label | Cal File Color |
|:------------------:|:--------------:|
| 0                  | Red            |
| 1                  | White          |
| 2                  | Blue           |
| 3                  | Green          |

If the stated rules are followed, there should be a 1-to-1 relationship between the GRIFFIN crystals and FIPPS crystals from the targets frame of reference. 
