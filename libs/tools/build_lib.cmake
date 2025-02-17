# __component_add_sources, __component_check_target, __component_add_include_dirs
#
# Utility macros for component registration. Adds source files and checks target requirements,
# and adds include directories respectively.

macro(__parse_sources sources)
    # Get the source file that participated in the compilation
    set(sources "")
    if(__SRCS)
        if(__SRC_DIRS)
            # When __SRC_DIRS and __SRCS are both specified，
            # sources is the base directory + filename of each source file in __SRCS
            message(WARNING "SRCS and SRC_DIRS are both specified; ignoring SRC_DIRS.")
        endif()
        foreach(src ${__SRCS})
            get_filename_component(src "${src}" ABSOLUTE BASE_DIR ${COMPONENT_DIR})
            list(APPEND sources ${src})
        endforeach()
    else()
        if(__SRC_DIRS)
            # When __SRCS is not specified __SRC_DIRS is specified
            # sources is the base directory + filename of all the.c/.cpp/.S files in __SRC_DIRS
            foreach(dir ${__SRC_DIRS})
                get_filename_component(abs_dir ${dir} ABSOLUTE BASE_DIR ${COMPONENT_DIR})

                if(NOT IS_DIRECTORY ${abs_dir})
                    message(FATAL_ERROR "SRC_DIRS entry '${dir}' does not exist.")
                endif()

                file(GLOB dir_sources "${abs_dir}/*.c" "${abs_dir}/*.cpp" "${abs_dir}/*.S")
                list(SORT dir_sources)

                if(dir_sources)
                    foreach(src ${dir_sources})
                        get_filename_component(src "${src}" ABSOLUTE BASE_DIR ${COMPONENT_DIR})
                        list(APPEND sources "${src}")
                    endforeach()
                else()
                    message(WARNING "No source files found for SRC_DIRS entry '${dir}'.")
                endif()
            endforeach()
        endif()

        if(__EXCLUDE_SRCS)
            # Delete every file path that appears in __EXCLUDE_SRCS in sources
            foreach(src ${__EXCLUDE_SRCS})
                get_filename_component(src "${src}" ABSOLUTE)
                list(REMOVE_ITEM sources "${src}")
            endforeach()
        endif()
    endif()

    list(REMOVE_DUPLICATES sources)
endmacro()

macro(__add_include_dirs lib dirs type)
    # Use target_include_directories(lib type dir) 
    # to specify the header file path that the project lib needs to include
    foreach(dir ${dirs})
        get_filename_component(_dir ${dir} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
        if(NOT IS_DIRECTORY ${_dir})
            message(WARNING "Include directory '${_dir}' is not a directory.")
        else()
            message(STATUS "INCLUDE_path: ${_dir}")
            target_include_directories(${lib} ${type} ${_dir})
        endif()
    endforeach()
endmacro()

macro(__add_dependencys_path dirs depds)
    # Get the path of each dynamic link library in depds and specify the path of the third-party library 
    # with the link_directories command
    set(extra_lib_path)
    set(system_lib_path)
    set(_lib_system_libs)
    foreach(depd ${depds})
        if(UNIX)
            if(NOT EXISTS "${dirs}/lib${depd}.so")
                find_library(system_lib_path lib${depd}.so)
                list(APPEND _lib_system_libs "${system_lib_path}")
            else()
                list(APPEND _lib_internal_libs "${dirs}/lib${depd}.so")
            endif()
        elseif(WIN32)
            if(NOT EXISTS "${dirs}/${depd}.dll")
                find_library(system_lib_path ${depd}.dll)
                list(APPEND _lib_system_libs "${system_lib_path}")
            else()
                list(APPEND _lib_internal_libs "${dirs}/${depd}.dll")
            endif()
        endif()
    endforeach()
    link_directories(${dirs} ${_lib_system_libs})
    message(STATUS "extra_lib_path: ${dirs}")
endmacro()

macro(__add_dependencys lib dirs depds type)
    #For each dependency, the library that specifies the target link through the target_link_libraries() command
    #PUBLIC: The library after public is linked to target, and the symbols inside are exported for use by third parties
    #PRIVATE: Libraries after private are only linked to target. Third parties cannot know what libraries are being called
    #INTERFACE: Libraries introduced after interface are not linked into target and only export symbols
    foreach(depd ${depds})
        target_link_libraries(${lib} ${type} ${depd})
    endforeach()
endmacro()

macro(share_lib_add module_name libs_path)
    #Get system platform _platform_name and compiler _compiler_name
    set(_platform_name "windows")
    set(_compiler_name "linux")
    if(UNIX)
        message(STATUS "current platform: Linux ")
        set(_platform_name "linux")
    elseif(WIN32)
        message(STATUS "current platform: Windows ")
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set(_compiler_name "mingw")
        elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
            set(_compiler_name "msvc")
        else()
            message(FATAL_ERROR "current compiler: unknown")
        endif()
        message(STATUS "current compiler: ${_compiler_name}")
    else()
        message(FATAL_ERROR "current platform: unknown ")
    endif()    
    message(STATUS "libs in: ${libs_path}")
    _share_lib_add(${module_name} ${libs_path} ${ARGV})
endmacro()

macro(executable_add module_name libs_path)
    #Get system platform _platform_name and compiler _compiler_name
    set(_platform_name "windows")
    set(_compiler_name "linux")
    if(UNIX)
        message(STATUS "current platform: Linux ")
        set(_platform_name "linux")
    elseif(WIN32)
        message(STATUS "current platform: Windows ")
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set(_compiler_name "mingw")
        elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
            set(_compiler_name "msvc")
        else()
            message(FATAL_ERROR "current compiler: unknown")
        endif()
        message(STATUS "current compiler: ${_compiler_name}")
    else()
        message(FATAL_ERROR "current platform: unknown ")
    endif()    
    message(STATUS "libs in: ${libs_path}")
    _executable_add(${module_name} ${libs_path} ${ARGV})
endmacro()

macro(project_share_lib libs_path)
    #Get module name _module_name
    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set(_module_name "${PARENT_DIR}")
    message(STATUS "name: ${_module_name}")
    share_lib_add(${_module_name} ${libs_path} ${ARGV})
endmacro()

macro(project_executable libs_path)
    #Get module name _module_name
    set(_module_name "${PROJECT_NAME}")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
    message(STATUS "name: ${_module_name}")
    executable_add(${_module_name} ${libs_path} ${ARGV})
endmacro()

function(_get_version version file_path)
    if(EXISTS ${file_path})
        file(READ ${file_path} ver)
        string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
        set(ver_major ${CMAKE_MATCH_1})
        string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
        set(ver_minor ${CMAKE_MATCH_1})
        string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
        set(ver_patch ${CMAKE_MATCH_1})
        message(STATUS "${ver_major}.${ver_minor}.${ver_patch}")
        set(${version} "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
    else()
        message(WARNING " ${file_path} not exist")
        get_filename_component(parent_dir ${file_path} DIRECTORY)
        file(MAKE_DIRECTORY ${parent_dir})
        set(version "0.0.0" PARENT_SCOPE)
        file(WRITE ${file_path} "VERSION_MAJOR 0\nVERSION_MINOR 0\nVERSION_PATCH 0\n")
    endif()
endfunction()

function(_executable_add lib_name libs_path)
    set(options)
    set(single_value VERSION)
    set(multi_value SRCS SRC_DIRS EXCLUDE_SRCS
                    INCLUDE_DIRS PRIV_INCLUDE_DIRS
                    REQUIRES PRIV_REQUIRES EXPORT_EXT_LIBS)
    cmake_parse_arguments(_ "${options}" "${single_value}" "${multi_value}" ${ARGN})
    list(APPEND __SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
    message(STATUS "SRC_DIRS:" ${__SRC_DIRS})

    __parse_sources(sources)
    message(STATUS "SRCS:" ${sources})
    set(_lib_path ${libs_path}/bin/${_compiler_name})
    set(_lib_version_file "version.txt")
    set(_current_path_version_file ${CMAKE_CURRENT_SOURCE_DIR}/${_lib_version_file})
    set(_module_version "0.0.0")
    set(_lib_external_libs ${__EXPORT_EXT_LIBS})
    set(_lib_internal_libs)
    _get_version(_module_version ${_current_path_version_file})
    message(STATUS "version: ${_module_version}")

    if(sources)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        __add_dependencys_path(${_lib_path} "${__REQUIRES}")
        __add_dependencys_path(${_lib_path} "${__PRIV_REQUIRES}")
        add_executable(${lib_name} ${sources})
        __add_include_dirs(${lib_name} "${__INCLUDE_DIRS}" PUBLIC)
        __add_include_dirs(${lib_name} "${__PRIV_INCLUDE_DIRS}" PRIVATE)
        __add_include_dirs(${lib_name} ${libs_path}/include PUBLIC)
        __add_dependencys(${lib_name} ${_lib_path} "${__REQUIRES}" PUBLIC)
        __add_dependencys(${lib_name} ${_lib_path} "${__PRIV_REQUIRES}" PRIVATE)
        install(TARGETS ${lib_name} DESTINATION bin)
        install(FILES ${_lib_external_libs} DESTINATION bin)
        install(FILES ${_lib_internal_libs} DESTINATION bin)
    else()
        message(FATAL_ERROR "no source files found!")
    endif()
endfunction()

function(_share_lib_add lib_name libs_path)
    set(options)
    set(single_value VERSION)
    set(multi_value SRCS SRC_DIRS EXCLUDE_SRCS
                    INCLUDE_DIRS PRIV_INCLUDE_DIRS
                    REQUIRES PRIV_REQUIRES EXPORT_HEADERS EXPORT_DIRS EXPORT_EXT_LIBS)
    cmake_parse_arguments(_ "${options}" "${single_value}" "${multi_value}" ${ARGN})
    list(APPEND __SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
    list(APPEND __INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    message(STATUS "SRC_DIRS:" ${__SRC_DIRS})
    __parse_sources(sources)
    message(STATUS "SRCS:" ${sources})
    set(_lib_path ${libs_path}/bin/${_compiler_name})
    set(_lib_inc_public "include/${lib_name}.h" "include/export_global.h" ${__EXPORT_HEADERS})
    set(_lib_dirs_public ${__EXPORT_DIRS})
    set(_lib_external_libs ${__EXPORT_EXT_LIBS})
    set(_lib_internal_libs)
    set(_lib_current_version_file "version.txt")
    set(_lib_installed_version_file "${lib_name}_${_compiler_name}_version.txt")
    set(_current_path_version_file ${CMAKE_CURRENT_SOURCE_DIR}/${_lib_current_version_file})
    set(_lib_path_version_file ${_lib_path}/${_lib_installed_version_file})
    set(_module_version "0.0.0")
    set(_module_installed_version "0.0.0")
    _get_version(_module_version ${_current_path_version_file})
    _get_version(_module_installed_version ${_lib_path_version_file})
    message(STATUS "new version: ${_module_version}")
    message(STATUS "old version: ${_module_installed_version}")
    if(${_module_version} VERSION_GREATER ${_module_installed_version})
        message(STATUS "new version found!")
    else()
        message(WARNING "new version not found!")
        set(_module_version "0.0.0")
    endif()

    if(sources)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        __add_dependencys_path(${_lib_path} "${__REQUIRES}")
        __add_dependencys_path(${_lib_path} "${__PRIV_REQUIRES}")
        add_library(${lib_name} SHARED ${sources})
        __add_include_dirs(${lib_name} "${__INCLUDE_DIRS}" PUBLIC)
        __add_include_dirs(${lib_name} "${__PRIV_INCLUDE_DIRS}" PRIVATE)
        target_compile_definitions(${lib_name} PRIVATE EXPORT_LIBRARY)
        __add_dependencys(${lib_name} ${_lib_path} "${__REQUIRES}" PUBLIC)
        __add_dependencys(${lib_name} ${_lib_path} "${__PRIV_REQUIRES}" PRIVATE)
        __add_include_dirs(${lib_name} "${libs_path}/include" PUBLIC)
        if(${_module_version} VERSION_GREATER "0.0.0")
            install(TARGETS ${lib_name} DESTINATION "${_lib_path}")
            install(FILES ${_lib_external_libs} DESTINATION "${_lib_path}")
            install(FILES ${_current_path_version_file} RENAME "${_lib_installed_version_file}" DESTINATION "${_lib_path}")    
            install(FILES ${_lib_inc_public} DESTINATION "${libs_path}/include/${lib_name}")
            install(DIRECTORY ${_lib_dirs_public} DESTINATION "${libs_path}/include/${lib_name}")
        else()
            install(FILES)
            message(WARNING "no new version found! will not install")
        endif()
    else()
        message(FATAL_ERROR "no source files found!")
    endif()
endfunction()

function(_package_generate lib_name)
    include (InstallRequiredSystemLibraries)
    set(CPACK_GENERATOR "ZIP")
    set(_lib_version_file "version.txt")
    set(_current_path_version_file ${CMAKE_CURRENT_SOURCE_DIR}/${_lib_version_file})
    set(_module_version "0.0.0")
    _get_version(_module_version ${_current_path_version_file})
    message(STATUS "test_version: ${_module_version}")
    set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
    set(CPACK_PACKAGE_NAME ${lib_name}_${_compiler_name})
    set(CPACK_PACKAGE_VERSION ${_module_version})
    set(CPACK_CONFIGURATION_TYPES "Debug" CACHE STRING "List of supported configurations.")
    set(CMAKE_CONFIGURATION_TYPES ${CPACK_CONFIGURATION_TYPES} CACHE STRING "Configurations" FORCE)
    include(CPack)
endfunction()