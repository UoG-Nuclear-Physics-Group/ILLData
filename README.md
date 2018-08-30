# ILLData

This library provides the data parser and detector classes for data taken with FIPPS at the ILL.
The names of the classes typically start with TILL.

The data parser is set up to process FIPPS data written in the lst data format.

The only detector class implemented is TFipps (and TFippsHit).

Other classes included are:
 - the file and event classes for LST files,
 - the TILLMnemonic class which provides an enumeration for the detector systems and digitizers,
 - the TILLDetectorInformation which only provides functions to tell whether FIPPS data is present (which should be all the time), and
 - the TILLDetectorHit, which implemements the algorithm for how to get the time in nanoseconds from the timestamp and CFD information.
