cmake_minimum_required(VERSION 3.16)

project(charactree VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(RC_FILE app.rc)

if(WIN32)
    # Для Windows добавляем ресурсы через RC файл
    set(PROJECT_SOURCES
        ${PROJECT_SOURCES}
        ${RC_FILE}
        app.manifest
    )
endif()

set(RESOURCE_FILES
    resources.qrc
    icon/icon.ico
)

# Добавьте поиск Windows API
if(WIN32)
    find_package(WindowsSDK)
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(charactree
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        cppjieba/DictTrie.hpp cppjieba/FullSegment.hpp cppjieba/HMMModel.hpp cppjieba/HMMSegment.hpp cppjieba/Jieba.hpp cppjieba/KeywordExtractor.hpp cppjieba/MixSegment.hpp cppjieba/MPSegment.hpp cppjieba/PosTagger.hpp cppjieba/PreFilter.hpp cppjieba/QuerySegment.hpp cppjieba/SegmentBase.hpp cppjieba/SegmentTagged.hpp cppjieba/TextRankExtractor.hpp cppjieba/Trie.hpp cppjieba/Unicode.hpp
        cppjieba/limonp/ArgvContext.hpp cppjieba/limonp/Closure.hpp cppjieba/limonp/Colors.hpp cppjieba/limonp/Condition.hpp cppjieba/limonp/Config.hpp cppjieba/limonp/ForcePublic.hpp cppjieba/limonp/LocalVector.hpp cppjieba/limonp/Logging.hpp cppjieba/limonp/NonCopyable.hpp cppjieba/limonp/StdExtension.hpp cppjieba/limonp/StringUtil.hpp
        wordhighlighter.h wordhighlighter.cpp
        customtextedit.h customtextedit.cpp

        edittermdialog.h edittermdialog.cpp edittermdialog.ui
        worddata.h worddata.cpp
        exitdialog.h exitdialog.cpp exitdialog.ui
        text.h text.cpp

        library.h library.cpp library.ui
        dbmanager.cpp dbmanager.h
        dictionarydialog.h dictionarydialog.cpp dictionarydialog.ui
        settingsdialog.h settingsdialog.cpp settingsdialog.ui
        resources.qrc



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET chinese-reader APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(chinese-reader SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(chinese-reader
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(charactree PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chinese-reader)
endif()
set_target_properties(charactree PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS charactree
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY cppjieba/dict/ DESTINATION ${CMAKE_INSTALL_PREFIX}/dict)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(charactree)
endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # Установите иконку для исполняемого файла
    set_target_properties(charactree PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE_ICON_FILE ":/icon/ico_256.png"
    )

    # Для полной статической сборки
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()
