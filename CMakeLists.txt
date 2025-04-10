cmake_minimum_required(VERSION 3.16)

project(QtLianliankan VERSION 0.3 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 uic 查找 ui 文件的路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/gamewindow.h src/gamewindow.cpp ui/gamewindow.ui
        src/startmenu.h src/startmenu.cpp ui/startmenu.ui
        src/basicmode.h src/basicmode.cpp ui/basicmode.ui
        src/gamemap.h src/gamemap.cpp ui/gamemap.ui
        src/itemgrid.h src/itemgrid.cpp
        src/debugwindow.h src/debugwindow.cpp ui/debugwindow.ui
        src/options.h src/options.cpp ui/options.ui
        src/help.h src/help.cpp ui/help.ui
        src/ReturnablePage.h
        res/img.qrc
        res/fonts.qrc

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtLianliankan
        res/icon/icon.rc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtLianliankan APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtLianliankan SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtLianliankan
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QtLianliankan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QtLianliankan)
endif()
set_target_properties(QtLianliankan PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QtLianliankan
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtLianliankan)
endif()
