#----------------------------------------------------------------------------
# Set up colors
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(ILLData)

#----------------------------------------------------------------------------
# find the git package (needed to determine the version of the code)
find_package(Git REQUIRED)

#----------------------------------------------------------------------------
# find the root package (and add COMPONENTS)
#find_package(ROOT REQUIRED COMPONENTS Gui GuiHtml Minuit Spectrum)
find_package(ROOT OPTIONAL_COMPONENTS XMLParser)

if(${ROOT_xml_FOUND})
	add_compile_options(-DHAS_XML)
	set(CLING_OPTIONS "-DHAS_XML")
else()
	message("${Red}XML feature for ROOT not found (ROOT_xml_FOUND = ${ROOT_xml_FOUND})!${ColourReset}")
endif()

#----------------------------------------------------------------------------
# set c++ standard to the one used for root
if(DEFINED ROOT_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
else()
	set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

#----------------------------------------------------------------------------
# configure the include directories and set other variables
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_BINDIR ${CMAKE_BINARY_DIR}/bin)

#----------------------------------------------------------------------------
# copy includes
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR})

#----------------------------------------------------------------------------
# define all libraries and root dictionaries
# for some we also create dependencies on other libraries to remove linking errors later on

add_library(TLst SHARED
	${PROJECT_SOURCE_DIR}/libraries/TLst/TLstEvent.cxx
	${PROJECT_SOURCE_DIR}/libraries/TLst/TLstFile.cxx
	)
root_generate_dictionary(G__TLst TLstEvent.h TLstFile.h MODULE TLst LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TLst/LinkDef.h OPTIONS ${CLING_OPTIONS})
target_link_libraries(TLst TILLFormat ${ROOT_LIBRARIES} ${GRSI_LIBRARIES})
add_dependencies(TLst ILLDataVersion)

add_library(TILLFormat SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLFormat/TILLDetectorInformation.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLFormat/TILLMnemonic.cxx
	)
root_generate_dictionary(G__TILLFormat TILLDetectorInformation.h TILLMnemonic.h MODULE TILLFormat LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TILLFormat/LinkDef.h)
target_link_libraries(TILLFormat TFipps TFippsLaBr TFippsTAC TFippsPulser ${GRSI_LIBRARIES})

add_library(TILLDataParser SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLDataParser/TILLDataParser.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLDataParser/TILLDataParserException.cxx
	)
target_link_libraries(TILLDataParser TLst ${GRSI_LIBRARIES})

add_library(TFipps SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFipps/TFipps.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFipps/TFippsHit.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFipps/TFippsBgo.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFipps/TFippsBgoHit.cxx
	)
root_generate_dictionary(G__TFipps TFipps.h TFippsHit.h TFippsBgo.h TFippsBgoHit.h MODULE TFipps LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFipps/LinkDef.h)
target_link_libraries(TFipps ${GRSI_LIBRARIES})

add_library(TFippsLaBr SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsLaBr/TFippsLaBr.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsLaBr/TFippsLaBrHit.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsLaBr/TFippsLaBrBgo.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsLaBr/TFippsLaBrBgoHit.cxx
	)
root_generate_dictionary(G__TFippsLaBr TFippsLaBr.h TFippsLaBrHit.h TFippsLaBrBgo.h TFippsLaBrBgoHit.h MODULE TFippsLaBr LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsLaBr/LinkDef.h)
target_link_libraries(TFippsLaBr ${GRSI_LIBRARIES})

add_library(TFippsTAC SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsTAC/TFippsTAC.cxx
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsTAC/TFippsTACHit.cxx
	)
root_generate_dictionary(G__TFippsTAC TFippsTAC.h TFippsTACHit.h MODULE TFippsTAC LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsTAC/LinkDef.h)
target_link_libraries(TFippsTAC ${GRSI_LIBRARIES})

add_library(TFippsPulser SHARED
	${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsPulser/TFippsPulser.cxx
	)
root_generate_dictionary(G__TFippsPulser TFippsPulser.h MODULE TFippsPulser LINKDEF ${PROJECT_SOURCE_DIR}/libraries/TILLAnalysis/TFippsPulser/LinkDef.h)
target_link_libraries(TFippsPulser ${GRSI_LIBRARIES})

##----------------------------------------------------------------------------
# add the global library with the functions to create files and parsers
set(ILLDATA_LIBRARIES TLst TILLFormat TILLDataParser TFipps TFippsLaBr TFippsTAC TFippsPulser)

add_library(ILLData SHARED
	${PROJECT_SOURCE_DIR}/src/ILLDataLibrary.cxx
	)
target_link_libraries(ILLData ${ILLDATA_LIBRARIES} ${GRSI_LIBRARIES})
add_dependencies(ILLData ILLDataVersion)

##----------------------------------------------------------------------------
# custom target to generate the version header
# CMAKE_SOURCE_DIR is the GRSISort directory
# PROJECT_SOURCE_DIR is the _deps/grsidata-src directory
add_custom_target(ILLDataVersion
	${CMAKE_COMMAND} -D SRC=${PROJECT_SOURCE_DIR}/include/ILLDataVersion.h.in
	                 -D DST=${PROJECT_SOURCE_DIR}/include/ILLDataVersion.h
                    -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                    -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )
