cmake_minimum_required(VERSION 3.13)

set(PROJ_VERSION 1.0.0)

if (WIN32)
    project(EasyHamLog LANGUAGES CXX VERSION ${PROJ_VERSION})
elseif(UNIX)
    project(EasyHamLog)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo;Debug" CACHE STRING "" FORCE)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
message(STATUS "$ENV{QTDIR}\\lib\\cmake\\Qt5")
set(Qt5_DIR "$ENV{QTDIR}\\lib\\cmake\\Qt5")
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS QuickWidgets REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.cpp"
    "${FORMS_DIR}/*.h"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE FORMS "${FORMS_DIR}/*.ui" "${FORMS_DIR}/*.qrc")
source_group("ui" FILES ${FORMS})

#=================== SETUP EXECTUABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(EasyHamLog WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(EasyHamLog ${SOURCES})
endif()

# Add the target includes for MY_PROJECT 
target_include_directories(EasyHamLog PRIVATE ${FORMS_DIR})
target_include_directories(EasyHamLog PRIVATE ${INCLUDE_DIR})
target_include_directories(EasyHamLog PRIVATE ${SOURCE_DIR})
target_compile_definitions(EasyHamLog PRIVATE EHL_APP_VERSION="${PROJ_VERSION}")


#===================== LINKING LIBRARIES =======================#
target_link_libraries(EasyHamLog Qt5::Widgets Qt5::Core Qt5::Xml Qt5::QuickWidgets)