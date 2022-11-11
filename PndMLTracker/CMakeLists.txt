# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.


############### System Include Directories
Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${GEANT3_INCLUDE_DIR}
    ${CLHEP_INCLUDE_DIR}
    ${BASE_INCLUDE_DIRECTORIES}
    ${VMC_INCLUDE_DIRS}
)


############### Include Directories
Set(INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/tracking/PndMLTracker
    ${CMAKE_SOURCE_DIR}/tools
    ${CMAKE_SOURCE_DIR}/detectors/mvd
    ${CMAKE_SOURCE_DIR}/detectors/gem 
    ${CMAKE_SOURCE_DIR}/detectors/stt
    ${CMAKE_SOURCE_DIR}/pnddata
    ${CMAKE_SOURCE_DIR}/pnddata/SdsData
    ${CMAKE_SOURCE_DIR}/pnddata/GemData
    ${CMAKE_SOURCE_DIR}/pnddata/SttData
    ${CMAKE_SOURCE_DIR}/pnddata/TrackData
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


############### Link Directories
set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${FAIRROOT_LIBRARY_DIR}
    ${SIMPATH}/lib
)

link_directories(${LINK_DIRECTORIES})


############### Adeel: libMLTracker (start) #############
set(SRCS
#Particle.cxx
PndMLTracking.cxx
)

set(LINKDEF  PndMLTrackingLinkDef.h)
set(LIBRARY_NAME MLTracker)
############### Adeel: libMLTracker (start) #############

set(DEPENDENCIES Base GeoBase ParBase PndData Geane Gem Stt)
PANDA_GENERATE_LIBRARY()
 
