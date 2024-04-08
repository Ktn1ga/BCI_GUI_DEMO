cmake_minimum_required(VERSION 3.5)

project(SHUBCI_1 VERSION 0.1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

#添加PrintSupport,QCusomPlot需要
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS PrintSupport REQUIRED)

#添加frame文件路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/frame/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/frame/src)

#添加qcustomplot文件路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/qcustomplot/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/qcustomplot/src)

#添加driver 文件路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/src)

#添加 data manager 文件路径
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/src)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        # 添加frame
        ${CMAKE_CURRENT_LIST_DIR}/frame/include/frame.h
        ${CMAKE_CURRENT_LIST_DIR}/frame/src/frame.cpp
        # 添加qcustomer
        ${CMAKE_CURRENT_LIST_DIR}/qcustomplot/include/qcustomplot.h
        ${CMAKE_CURRENT_LIST_DIR}/qcustomplot/src/qcustomplot.cpp
        # 添加drivers
        ${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/include/driver_eeg.h
        ${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/include/driver_blue_eeg.h
        ${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/src/driver_eeg.c
        ${CMAKE_CURRENT_LIST_DIR}/../../module/drivers/driver_eeg/src/driver_blue_eeg.c
        # 添加data_ manager
        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/include/data_manager.h
        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/src/data_manager.c

        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/include/data_load_ebdf.h
        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/src/data_load_ebdf.c

        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/include/data_store_ebdf.h
        ${CMAKE_CURRENT_LIST_DIR}/../../module/data_manager/src/data_store_edbf.c
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SHUBCI_1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SHUBCI_1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SHUBCI_1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SHUBCI_1
            ${PROJECT_SOURCES}
        )
    endif()
endif()


# 链接动态库
target_link_libraries(SHUBCI_1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SHUBCI_1 PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)


# 添加dll-自建
include_directories("${CMAKE_CURRENT_LIST_DIR}/../libs/include")
#target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/data_manager.lib")
#target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/driver_eeg.lib")
target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/device_manager.lib")
target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/trial_manager.lib")
target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/osal.lib")
target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/pthreadVC3.lib")
target_link_libraries(SHUBCI_1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/bin/msvc/cjson.lib")

#target_link_libraries(SHUBCI_1 PRIVATE wsock32 "D:/Apps/MatlabR2019b/sys/lcc64/lcc64/lib64/wsock32.lib")
#target_link_libraries(SHUBCI_1 PRIVATE ws2_32 "D:/Apps/MatlabR2019b/sys/lcc64/lcc64/lib64/ws2_32.lib")

set_target_properties(SHUBCI_1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS SHUBCI_1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SHUBCI_1)
endif()

