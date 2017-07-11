include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS +=    

SOURCES +=     main.cpp \
    test_md5.cpp \
    test_sha1.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../hash_algorithms/release/ -lhash_algorithms
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../hash_algorithms/debug/ -lhash_algorithms
else:unix: LIBS += -L$$OUT_PWD/../hash_algorithms/ -lhash_algorithms

INCLUDEPATH += $$PWD/../hash_algorithms
DEPENDPATH += $$PWD/../hash_algorithms

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../hash_algorithms/release/libhash_algorithms.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../hash_algorithms/debug/libhash_algorithms.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../hash_algorithms/release/hash_algorithms.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../hash_algorithms/debug/hash_algorithms.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../hash_algorithms/libhash_algorithms.a
