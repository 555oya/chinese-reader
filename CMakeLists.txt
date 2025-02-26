cmake_minimum_required(VERSION 3.16)

project(chinese-reader-mac VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chinese-reader-mac
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        customtextedit.cpp customtextedit.h edittermdialog.cpp edittermdialog.h edittermdialog.ui exitdialog.cpp exitdialog.h exitdialog.ui   text.cpp text.h worddata.cpp worddata.h wordhighlighter.cpp wordhighlighter.h
        cppjieba/limonp/ArgvContext.hpp cppjieba/limonp/Closure.hpp cppjieba/limonp/Colors.hpp cppjieba/limonp/Condition.hpp cppjieba/limonp/Config.hpp cppjieba/limonp/ForcePublic.hpp cppjieba/limonp/LocalVector.hpp cppjieba/limonp/Logging.hpp cppjieba/limonp/NonCopyable.hpp cppjieba/limonp/StdExtension.hpp cppjieba/limonp/StringUtil.hpp cppjieba/DictTrie.hpp cppjieba/FullSegment.hpp cppjieba/HMMModel.hpp cppjieba/HMMSegment.hpp cppjieba/Jieba.hpp cppjieba/KeywordExtractor.hpp cppjieba/MixSegment.hpp cppjieba/MPSegment.hpp cppjieba/PosTagger.hpp cppjieba/PreFilter.hpp cppjieba/QuerySegment.hpp cppjieba/SegmentBase.hpp cppjieba/SegmentTagged.hpp cppjieba/TextRankExtractor.hpp cppjieba/Trie.hpp cppjieba/Unicode.hpp dict/pos_dict/char_state_tab.utf8 dict/pos_dict/prob_emit.utf8 dict/pos_dict/prob_start.utf8 dict/pos_dict/prob_trans.utf8 dict/hmm_model.utf8 dict/idf.utf8 dict/jieba.dict.utf8 dict/README.md dict/stop_words.utf8 dict/user.dict.utf8
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET chinese-reader-mac APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(chinese-reader-mac SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(chinese-reader-mac
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(chinese-reader-mac PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chinese-reader-mac)
endif()
set_target_properties(chinese-reader-mac PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS chinese-reader-mac
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Копирование папки dict в выходной каталог сборки
file(COPY dict/ DESTINATION ${CMAKE_BINARY_DIR}/chinese-reader-mac.app/Contents/MacOS/dict)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chinese-reader-mac)
endif()
