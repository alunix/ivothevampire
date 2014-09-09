cmake_minimum_required (VERSION 2.8)
project(vampire_ivo) 

### Defaults
set(APP_ARCH "")
set(APP_VER_MAJ "1")
set(APP_VER_MIN "1")
set(APP_VER_PATCH "0")

### check for required libs
find_package(SDL REQUIRED)

### Install targets (see: http://www.cmake.org/Wiki/CMake_RPATH_handling)
if (NOT INSTALL_DIR)
    set(INSTALL_DIR "./")
endif()

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# add the automatically determined parts of the RPATH
SET(CMAKE_INSTALL_RPATH "${INSTALL_DIR}/")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

### sources
include_directories(src/)
set(
    SOURCES 
    src/Animation.cpp
    src/Font.cpp 
    src/GameProc.cpp 
    src/Sprite.cpp 
    src/Vampire.cpp 
    src/vampire_ivo.cpp 
)

### specify executable

set(RES_FILES "")
if(MINGW)
    # include a Windows resource file
    set(RES_FILES "src/resource.rc")
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    
    add_executable(vampire_ivo ${SOURCES} ${RES_FILES}) 
else()
    add_executable(vampire_ivo ${SOURCES})  
endif()

### add required libarires

## libstdc runtime on Windows
if (WIN32)
    # Static linkage of the gcc libs
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

## SDL
if (UNIX AND NOT WIN32)
    # Linux
    add_definitions(`sdl-config --cflags`)
    add_definitions(`sdl-config --libs`)    
else()
    # Windows
    
    set(SDL_LIB_NAME "SDL.dll")
    
    add_definitions(-lpthread)
    include_directories($ENV{SDLDIR}\\include ${SDL_INCLUDE_DIR})
    link_directories($ENV{SDLDIR}\\lib)
    set(SDL_LIBRARY_SO $ENV{SDLDIR}\\bin\\SDL.dll)
    
    message(STATUS "Found libSDL.so/DLL at ${SDL_LIBRARY}")
    
    # copy the SDL DLL file to the install dir
    add_custom_command(TARGET vampire_ivo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${SDL_LIBRARY_SO}
        $<TARGET_FILE_DIR:vampire_ivo>)  
        
    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SDL.dll" DESTINATION ${INSTALL_DIR})
        
endif()

## SOUND libraries
if (SOUND STREQUAL "NO")
    message(STATUS "Found SOUND=NO option. Compiling without sound support!")
else()
    # SDL_mixer API
    find_package(SDL_mixer REQUIRED)
    add_definitions(-DWITH_SDLMIXER)
    set(SOUND "SDL_MIXER")

    # Copy SDL_mixer.dll and libmikmod-2.dll to installation directory
    if (WIN32)  
        INSTALL(FILES "${SDL_MIXER_LIBRARIES}" DESTINATION ${INSTALL_DIR})  
        
        # Find mikmod DLL for playing music
        find_library(SDL_MIXER_MIKMOD_LIBRARY
            NAMES libmikmod-2
            HINTS
                ENV SDLMIXERDIR
            PATH_SUFFIXES lib)
            
        if (NOT SDL_MIXER_MIKMOD_LIBRARY)
            message(STATUS "SDL_mixer libmikmod-2 dependency was NOT FOUND! Game will run WITHOUT music.")
        else()
            include_directories($ENV{SDLMIXERDIR})

            set(SDLMIXER_LIBRARY_SO $ENV{SDLMIXERDIR}\\SDL_mixer.dll)

            # copy the SDL_Mixer DLL file to the install dir
            add_custom_command(TARGET vampire_ivo POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                ${SDL_MIXER_MIKMOD_LIBRARY}
                $<TARGET_FILE_DIR:vampire_ivo>)

            add_custom_command(TARGET vampire_ivo POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                ${SDLMIXER_LIBRARY_SO}
                $<TARGET_FILE_DIR:vampire_ivo>)               
        endif()
        
        INSTALL(FILES "${SDLMIXER_LIBRARY_SO}" DESTINATION ${INSTALL_DIR}) 
        INSTALL(FILES "${SDL_MIXER_MIKMOD_LIBRARY}" DESTINATION ${INSTALL_DIR}) 
    endif()
endif()

### link required libraries

target_link_libraries(vampire_ivo ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})

### additional defines

if (UNIX AND NOT WIN32)
    add_definitions(-DLINUX_BUILD)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-g -D_DEBUG -DDEBUG) # -Wextra -Wall
else()
    add_definitions(-O3 -DNDEBUG)
endif()
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

### set architecture

if (CMAKE_SIZEOF_VOID_P MATCHES "8" AND NOT M32)
    add_definitions(-DARCH_X64)
    set(APP_ARCH "x64")
    set(DEB_ARCH "amd64")
else()
    add_definitions(-DARCH_X86)
    set(APP_ARCH "x86")
    set(DEB_ARCH "i386")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

### Configure compile time header file 
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h")
  
INSTALL(TARGETS vampire_ivo RUNTIME DESTINATION ${INSTALL_DIR})
###INSTALL(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/run.sh" DESTINATION ${INSTALL_DIR})
INSTALL(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    DESTINATION ${INSTALL_DIR})
#INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
#    DESTINATION ${INSTALL_DIR}
#    PATTERN "debug.html" EXCLUDE
#    PATTERN "pref" EXCLUDE)
    
### build a CPack driven installer package
include (InstallRequiredSystemLibraries)

if (NOT CPACK_GENERATOR AND (UNIX AND NOT WIN32))
    set(CPACK_GENERATOR "STGZ")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_NAME "Vampire Ivo")
set(CPACK_PACKAGE_VENDOR "Petar Petrov")
set(CPACK_PACKAGE_CONTACT "petar.petrov.georgiev(at-)gmail[dot)com")
set(CPACK_PACKAGE_VERSION_MAJOR "${APP_VER_MAJ}")
set(CPACK_PACKAGE_VERSION_MINOR "${APP_VER_MIN}")
set(CPACK_PACKAGE_VERSION_PATCH "${APP_VER_PATCH}")
set(PCKVER "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set(CPACK_PACKAGE_VERSION ${PCKVER})

set(CPACK_PACKAGE_FILE_NAME "vampire_ivo-${PCKVER}-${CMAKE_SYSTEM_NAME}-${APP_ARCH}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Debian DEB package stuff
if (${CPACK_GENERATOR} STREQUAL "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/opt")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH}) 
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libsdl1.2debian (>= 1.2.11)")
    # require SDL_mixer if support is enabled
    if(SOUND STREQUAL "SDL_MIXER")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libsdl-mixer1.2 (>= 1.2.12)")
    endif()
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
#set(CPACK_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/bin;/.")
#set(CPACK_IGNORE_FILES 
#"~$"
#"^.*/.git/"
#"^${PROJECT_SOURCE_DIR}/bin/pref"
#"^${PROJECT_SOURCE_DIR}/bin/debug.html"
#)
set(CPACK_PACKAGE_EXECUTABLES "vampire_ivo" "Vampire Ivo Game")

include (CPack)
