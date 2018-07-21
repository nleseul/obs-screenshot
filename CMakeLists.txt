cmake_minimum_required(VERSION 2.8.12)

project(obs-screenshot)

if(APPLE)
	find_library(COCOA Cocoa)
	include_directories(${COCOA})
endif()

if(UNIX AND NOT APPLE)
	find_package(X11 REQUIRED)
	link_libraries(${X11_LIBRARIES})
	include_directories(${X11_INCLUDE_DIR})
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

if (OBSSourcePath OR DEFINED ENV{OBSSourcePath})
    # Set already
else()
    set(OBSSourcePath "" CACHE PATH "Path to OBS source code (e.g., C:/Dev/obs-studio/libobs/)")
    message("OBSSourcePath is missing. Please set this variable to the location of the OBS source (e.g., C:/Dev/obs-studio/libobs/).")
endif()

if (OBSFrontendApiSourcePath OR DEFINED ENV{OBSFrontendApiSourcePath})
	# Set already
else()
	set(OBSFrontendApiSourcePath "" CACHE PATH "Path to obs-frontend-api source code (e.g., C:/Dev/obs-studio/UI/obs-frontend-api/)")
    message("OBSFrontendApiSourcePath is missing. Please set this variable to the location of the obs-frontend-api source (e.g., C:/Dev/obs-studio/UI/obs-frontend-api/).")
endif()

if(DEFINED QTDIR${_lib_suffix})
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR${_lib_suffix}}")
elseif(DEFINED QTDIR)
	list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
elseif(DEFINED ENV{QTDIR${_lib_suffix}})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR${_lib_suffix}}")
elseif(DEFINED ENV{QTDIR})
	list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()


find_path(OBS_LIB_DIR
    NAMES obs.dll obs.lib
    HINTS
        ${OBSSourcePath}/../build${_lib_suffix}/libobs/Release/
        ${OBSSourcePath}/../build/libobs/Release/
        ${OBSSourcePath}/../build${_lib_suffix}/libobs/Debug/
        ${OBSSourcePath}/../build/libobs/Debug/
    PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib)

find_path(OBSFRONTENDAPI_LIB_DIR
    NAMES obs-frontend-api.dll obs-frontend-api.lib
    HINTS
        ${OBSFrontendApiSourcePath}/../../build${_lib_suffix}/UI/obs-frontend-api/Release/
        ${OBSFrontendApiSourcePath}/../../build/UI/obs-frontend-api/Release/
        ${OBSFrontendApiSourcePath}/../../build${_lib_suffix}/UI/obs-frontend-api/Debug/
        ${OBSFrontendApiSourcePath}/../../build/UI/obs-frontend-api/Debug/
    PATHS
		/usr/lib /usr/local/lib /opt/local/lib /sw/lib)

find_package(Qt5Widgets REQUIRED)

set(obs-sccreenshot_HEADERS
	${obs-screenshot_HEADERS}
	)
set(obs-screenshot_SOURCES
	${obs-screenshot_SOURCES}
    src/obs-screenshot.cpp
	)
set(obs-screenshot_UI
	${obs-screenshot_UI}
	)

qt5_wrap_ui(obs-screenshot_UI_HEADERS
	${obs-screenshot_UI}
   )

# libobs
include_directories(${OBSSourcePath})
add_library (libobs SHARED IMPORTED)
set_property (TARGET libobs PROPERTY IMPORTED_LOCATION ${OBS_LIB_DIR}/obs.dll)
set_property (TARGET libobs PROPERTY IMPORTED_IMPLIB ${OBS_LIB_DIR}/obs.lib)

# obs-frontend-api
include_directories(${OBSFrontendApiSourcePath})
add_library (obs-frontend-api SHARED IMPORTED)
set_property (TARGET obs-frontend-api PROPERTY IMPORTED_LOCATION ${OBSFRONTENDAPI_LIB_DIR}/obs-frontend-api.dll)
set_property (TARGET obs-frontend-api PROPERTY IMPORTED_IMPLIB ${OBSFRONTENDAPI_LIB_DIR}/obs-frontend-api.lib)

add_library(obs-screenshot SHARED
	${obs-screenshot_HEADERS}
	${obs-screenshot_SOURCES}
	${obs-screenshot_UI_HEADERS}
	)
target_link_libraries(obs-screenshot
	Qt5::Widgets
	obs-frontend-api
	libobs)

target_link_libraries (${PROJECT_NAME} libobs)

install(TARGETS obs-screenshot RUNTIME DESTINATION obs-plugins/${_lib_suffix}bit)
install(FILES ${CMAKE_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb DESTINATION obs-plugins/${_lib_suffix}bit CONFIGURATIONS Debug)
install(DIRECTORY data/ DESTINATION data/obs-plugins/obs-screenshot/)
