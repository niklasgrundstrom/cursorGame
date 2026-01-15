cmake_minimum_required(VERSION 3.15)
project(DefendTheCastle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find SFML in common locations
set(SFML_ROOT "" CACHE PATH "SFML installation root directory")

# Check common Windows installation paths
if(WIN32 AND NOT SFML_ROOT)
    set(COMMON_PATHS
        "C:/SFML"
        "C:/Libraries/SFML"
        "C:/Program Files/SFML/SFML-3.0.2"
        "C:/Program Files/SFML"
        "$ENV{PROGRAMFILES}/SFML/SFML-3.0.2"
        "$ENV{PROGRAMFILES}/SFML"
    )
    foreach(PATH ${COMMON_PATHS})
        # Check for SFML 3.0 structure (lib/cmake/SFML or cmake/SFML)
        if(EXISTS "${PATH}/lib/cmake/SFML" OR EXISTS "${PATH}/cmake/SFML")
            set(SFML_ROOT ${PATH})
            break()
        endif()
    endforeach()
endif()

# Try to find SFML (3.0 or 2.5+)
# SFML 3.0 requires components to be specified
find_package(SFML 3.0 COMPONENTS System Window Graphics Audio QUIET)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS system window graphics audio QUIET)
endif()

if(NOT SFML_FOUND)
    # Try manual detection for SFML 3.0
    if(SFML_ROOT AND EXISTS "${SFML_ROOT}/include/SFML")
        message(STATUS "SFML found manually at ${SFML_ROOT}")
        set(SFML_FOUND TRUE)
        
        # Set include directory
        set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
        
        # Find library directory (check common locations)
        if(EXISTS "${SFML_ROOT}/lib")
            set(SFML_LIB_DIR "${SFML_ROOT}/lib")
        elseif(EXISTS "${SFML_ROOT}/lib/x64")
            set(SFML_LIB_DIR "${SFML_ROOT}/lib/x64")
        elseif(EXISTS "${SFML_ROOT}/lib/Release")
            set(SFML_LIB_DIR "${SFML_ROOT}/lib/Release")
        else()
            set(SFML_LIB_DIR "${SFML_ROOT}/lib")
        endif()
        
        # Set libraries (try to find actual library files)
        # SFML 3.0 might have different naming, so we'll use find_library
        find_library(SFML_SYSTEM_LIB 
            NAMES sfml-system sfml-system-d
            PATHS ${SFML_LIB_DIR}
            NO_DEFAULT_PATH
        )
        find_library(SFML_WINDOW_LIB 
            NAMES sfml-window sfml-window-d
            PATHS ${SFML_LIB_DIR}
            NO_DEFAULT_PATH
        )
        find_library(SFML_GRAPHICS_LIB 
            NAMES sfml-graphics sfml-graphics-d
            PATHS ${SFML_LIB_DIR}
            NO_DEFAULT_PATH
        )
        find_library(SFML_AUDIO_LIB 
            NAMES sfml-audio sfml-audio-d
            PATHS ${SFML_LIB_DIR}
            NO_DEFAULT_PATH
        )
        
        if(SFML_SYSTEM_LIB AND SFML_WINDOW_LIB AND SFML_GRAPHICS_LIB AND SFML_AUDIO_LIB)
            set(SFML_LIBRARIES 
                ${SFML_SYSTEM_LIB}
                ${SFML_WINDOW_LIB}
                ${SFML_GRAPHICS_LIB}
                ${SFML_AUDIO_LIB}
            )
        else()
            # Fallback: try common names
            set(SFML_LIBRARIES 
                "${SFML_LIB_DIR}/sfml-system.lib"
                "${SFML_LIB_DIR}/sfml-window.lib"
                "${SFML_LIB_DIR}/sfml-graphics.lib"
                "${SFML_LIB_DIR}/sfml-audio.lib"
            )
        endif()
        
        # Copy DLLs to output directory (Windows)
        if(WIN32 AND EXISTS "${SFML_ROOT}/bin")
            file(GLOB SFML_DLLS "${SFML_ROOT}/bin/*.dll")
            foreach(DLL ${SFML_DLLS})
                configure_file(${DLL} ${CMAKE_BINARY_DIR} COPYONLY)
            endforeach()
        endif()
    else()
        message(WARNING "SFML not found!")
        message(STATUS "Please install SFML using one of these methods:")
        message(STATUS "  1. Download from https://www.sfml-dev.org/download.php")
        message(STATUS "  2. Extract to C:\\SFML or set SFML_ROOT to your installation")
        message(STATUS "  3. Or use: cmake .. -DSFML_ROOT=\"C:/Program Files/SFML/SFML-3.0.2\"")
        message(FATAL_ERROR "SFML is required. Please install it and set SFML_ROOT.")
    endif()
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Castle.cpp
    src/Enemy.cpp
    src/EnemyTypes.cpp
    src/Unit.cpp
    src/Tree.cpp
    src/Shop.cpp
    src/ResourceManager.cpp
)

# Header files
set(HEADERS
    include/Game.h
    include/Castle.h
    include/Enemy.h
    include/EnemyTypes.h
    include/Unit.h
    include/Tree.h
    include/Shop.h
    include/ResourceManager.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link SFML
if(SFML_FOUND)
    # SFML 3.0 uses imported targets (SFML::System, SFML::Window, etc.)
    if(TARGET SFML::System)
        target_link_libraries(${PROJECT_NAME} 
            SFML::System
            SFML::Window
            SFML::Graphics
            SFML::Audio
        )
        
        # Copy DLLs for SFML 3.0 imported targets
        if(WIN32)
            # Try to get SFML_ROOT from cache or find it
            if(NOT SFML_ROOT)
                get_target_property(SFML_GRAPHICS_LOCATION SFML::Graphics LOCATION)
                if(SFML_GRAPHICS_LOCATION)
                    get_filename_component(SFML_LIB_DIR "${SFML_GRAPHICS_LOCATION}" DIRECTORY)
                    get_filename_component(SFML_ROOT "${SFML_LIB_DIR}/.." ABSOLUTE)
                endif()
            endif()
            
            if(SFML_ROOT)
                set(SFML_BIN_DIR "${SFML_ROOT}/bin")
                if(EXISTS "${SFML_BIN_DIR}/sfml-graphics-3.dll")
                    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${SFML_BIN_DIR}/sfml-system-3.dll"
                        "${SFML_BIN_DIR}/sfml-window-3.dll"
                        "${SFML_BIN_DIR}/sfml-graphics-3.dll"
                        "${SFML_BIN_DIR}/sfml-audio-3.dll"
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>
                        COMMENT "Copying SFML DLLs to output directory"
                    )
                endif()
            endif()
        endif()
    # SFML 2.x or manual linking
    elseif(TARGET sfml-system)
        target_link_libraries(${PROJECT_NAME} 
            sfml-system 
            sfml-window 
            sfml-graphics 
            sfml-audio
        )
    else()
        # Manual linking fallback
        target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
        
        # Add library directory if using manual linking
        if(SFML_LIB_DIR)
            target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_LIB_DIR})
        endif()
    endif()
    
    # Copy DLLs to output directory (Windows) - fallback if imported targets don't handle it
    if(WIN32 AND NOT TARGET SFML::System)
        # Find SFML bin directory
        if(SFML_ROOT)
            set(SFML_BIN_DIR "${SFML_ROOT}/bin")
            if(EXISTS "${SFML_BIN_DIR}")
                # Copy DLLs after build
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${SFML_BIN_DIR}/sfml-system-3.dll"
                    "${SFML_BIN_DIR}/sfml-window-3.dll"
                    "${SFML_BIN_DIR}/sfml-graphics-3.dll"
                    "${SFML_BIN_DIR}/sfml-audio-3.dll"
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
                    COMMENT "Copying SFML DLLs to output directory"
                )
            endif()
        endif()
    endif()
endif()

# Windows-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
