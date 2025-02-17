#-----------------------------------------------------------------------------
#
#  TSDuck - The MPEG Transport Stream Toolkit
#  Copyright (c) 2005-2023, Thierry Lelegard
#  BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
#
#  Common definitions for using qmake.
#  Defined only for Unix systems (Linux, macOS).
#
#-----------------------------------------------------------------------------

# Do not use Qt, we just use Qt Creator as a general-purpose C++ IDE.
CONFIG *= thread
CONFIG *= largefile
CONFIG *= c++17
CONFIG *= unversioned_libname
CONFIG *= no_plugin_name_prefix
CONFIG -= qt
CONFIG -= debug_and_release
CONFIG -= app_bundle
CONFIG += sdk_no_version_check
DEFINES -= UNICODE

# Define the symbol DEBUG in debug mode.
CONFIG(debug, debug|release):DEFINES += DEBUG

# Project directories.
PROJROOT = $$_PRO_FILE_PWD_/../../..
SRCROOT  = $$PROJROOT/src

# Enforce compilation warnings.
CONFIG -= warn_off
CONFIG *= warn_on

# Required on macOS to get C++17 features (C++17 standard library no supported before 10.15).
mac: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

# The other system-specific directories to exclude
linux: NOSYSDIR = mac
mac:   NOSYSDIR = linux

# TSDuck configuration files
TS_CONFIG_FILES += $$system("ls $$SRCROOT/libtsduck/config/*.names $$SRCROOT/libtsduck/config/*.xml")

# Shared object suffix
linux: SO = .so
mac:   SO = .dylib

# Other configuration.
LIBS += -ledit
linux|mac|mingw {
    QMAKE_CXXFLAGS_WARN_ON = -Werror -Wall -Wextra
    QMAKE_CXXFLAGS += -fno-strict-aliasing -fstack-protector-all
}
linux|mingw {
    # GCC options. Some of them depend on the compiler version.
    GCC_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    GCC_FIELDS = $$split(GCC_VERSION, ".")
    GCC_MAJOR = $$member(GCC_FIELDS, 0)
    QMAKE_CXXFLAGS_WARN_ON += -Wundef -Wcast-align -Wstrict-null-sentinel -Wformat-security \
        -Wswitch-default -Wuninitialized -Wno-unused-parameter -Wfloat-equal -Wpointer-arith \
        -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor \
        -Woverloaded-virtual -Wzero-as-null-pointer-constant
    greaterThan(GCC_MAJOR, 4): QMAKE_CXXFLAGS_WARN_ON += -Wpedantic -Weffc++ -Wshadow
}
linux {
    QMAKE_CXXFLAGS += -I/usr/include/PCSC
    LIBS += -lpcsclite -lrt -latomic -ldl
}
mac {
    # LLVM options. Some of them depend on the compiler version.
    LLVM_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    LLVM_FIELDS = $$split(LLVM_VERSION, ".")
    LLVM_MAJOR = $$member(LLVM_FIELDS, 0)
    QMAKE_CXXFLAGS_WARN_ON += -Weverything -Wno-c++98-compat-pedantic
    greaterThan(LLVM_MAJOR, 11): QMAKE_CXXFLAGS_WARN_ON += -Wno-poison-system-directories
    exists(/usr/local/include): QMAKE_CXXFLAGS += -I/usr/local/include
    exists(/opt/homebrew/include): QMAKE_CXXFLAGS += -I/opt/homebrew/include
    LIBS += -framework PCSC
    exists(/usr/local/lib): LIBS += -L/usr/local/lib
    exists(/opt/homebrew/lib): LIBS += -L/opt/homebrew/lib
    DEFINES += TS_NO_DTAPI=1
}
exists(/usr/include/srt/*.h) | exists(/usr/local/include/srt/*.h) | exists(/opt/homebrew/include/srt/*.h) {
    LIBS += -lsrt
}
else {
    DEFINES += TS_NO_SRT=1
}
exists(/usr/include/librist/*.h) | exists(/usr/local/include/librist/*.h) | exists(/opt/homebrew/include/librist/*.h) {
    LIBS += -lrist
}
else {
    DEFINES += TS_NO_RIST=1
}
tstool {
    # TSDuck tools shall use "CONFIG += tstool"
    CONFIG += libtsduck
    TEMPLATE = app
    SOURCES += $$SRCROOT/tstools/$${TARGET}.cpp
}
util {
    # TSDuck utils shall use "CONFIG += util"
    CONFIG += libtsduck
    TEMPLATE = app
    SOURCES += $$SRCROOT/utils/$${TARGET}.cpp
}
tsplugin {
    # TSP plugins shall use "CONFIG += tsplugin"
    CONFIG += libtsduck plugin
    TEMPLATE = lib
    SOURCES += $$SRCROOT/tsplugins/$${TARGET}.cpp
    QMAKE_POST_LINK += mkdir -p ../tsp $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += cp $${TARGET}$$SO ../tsp $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += mkdir -p ../tsprofiling $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += cp $${TARGET}$$SO ../tsprofiling $$escape_expand(\\n\\t)
}
libtsduck {
    # Applications using libtsduck shall use "CONFIG += libtsduck".
    linux:QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN/../libtsduck\'
    LIBS = ../libtsduck/libtsduck$$SO $$LIBS
    PRE_TARGETDEPS += ../libtsduck/libtsduck$$SO
    DEPENDPATH += ../libtsduck
    INCLUDEPATH += $$system("find $$SRCROOT/libtsduck -type d ! -name windows ! -name bsd ! -name freebsd ! -name netbsd ! -name openbsd ! -name dragonflybsd ! -name $$NOSYSDIR ! -name private ! -name __pycache__")
    QMAKE_POST_LINK += cp $$TS_CONFIG_FILES . $$escape_expand(\\n\\t)
}
