cmake_minimum_required(VERSION 3.5)

project(planewar LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
#set(RCC_FILES resource.rcc)
#find_package(Qt5 COMPONENTS Widgets REQUIRED)
#qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})

SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


#link_directories(E:\Qt\Qt5.14.2\Tools\QtCreator\bin\plugins\imageformats)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(planewar SHARED
    main.cpp
    mainscene.cpp
    mainscene.h
    background.h
    background.cpp
    my_plane.h
    my_plane.cpp
    config.h
  )
else()
  add_executable(planewar
    main.cpp
    mainscene.cpp
    mainscene.h
    background.h
    background.cpp
    my_plane.h
    my_plane.cpp
    config.h
    #     ${QRC_FILES}//这里和上面省去的四行 "file(GLOB_RECURSE..." 是配置qrc用的
  )
endif()

target_link_libraries(planewar PRIVATE Qt5::Widgets)
