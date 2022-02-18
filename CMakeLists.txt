#
# Copyright (C) 2015 Cybernetica
#
# Research/Commercial License Usage
# Licensees holding a valid Research License or Commercial License
# for the Software may use this file according to the written
# agreement between you and Cybernetica.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl-3.0.html.
#
# For further information, please contact us at sharemind@cyber.ee.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindModSpdzFrescoEmu VERSION 1.2.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.6 REQUIRED)

FIND_PACKAGE(Boost 1.62 REQUIRED)
FIND_PACKAGE(LogHard 0.6.0 REQUIRED)
FIND_PACKAGE(SharemindCHeaders 1.3.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders 0.9.0 REQUIRED)
FIND_PACKAGE(SharemindLibConfiguration 0.5.0 REQUIRED)
FIND_PACKAGE(SharemindLibEmulatorProtocols 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindLibExecutionModelEvaluator 0.4.0 REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler 0.5.0 REQUIRED)
FIND_PACKAGE(SharemindModuleApis 1.1.0 REQUIRED)
FIND_PACKAGE(SharemindPdkHeaders 0.5.0 REQUIRED)


FILE(GLOB_RECURSE SharemindModSpdzFrescoEmu_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE SharemindModSpdzFrescoEmu_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
SharemindAddSharedLibrary(ModSpdzFrescoEmu
    MODULE
    OUTPUT_NAME "sharemind_mod_spdz_fresco_emu"
    SOURCES
        ${SharemindModSpdzFrescoEmu_SOURCES}
        ${SharemindModSpdzFrescoEmu_HEADERS}
)
IF(SHAREMIND_NETWORK_STATISTICS)
    TARGET_COMPILE_DEFINITIONS(ModSpdzFrescoEmu
        PRIVATE "SHAREMIND_NETWORK_STATISTICS_ENABLE")
ENDIF()
TARGET_LINK_LIBRARIES(ModSpdzFrescoEmu
    PRIVATE
        Boost::boost
        LogHard::LogHard
        Sharemind::CHeaders
        Sharemind::CxxHeaders
        Sharemind::LibConfiguration
        Sharemind::LibEmulatorProtocols
        Sharemind::LibExecutionModelEvaluator
        Sharemind::LibExecutionProfiler
        Sharemind::ModuleApis
        Sharemind::PdkHeaders
    )

# Configuration files:
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/packaging/configs/sharemind/"
        DESTINATION "/etc/sharemind/"
        COMPONENT "lib"
        EXCLUDE_FROM_ALL)

# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-mod-spdz-fresco-emu"
    DESCRIPTION "Sharemind SPDZ Fresco emulation module"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libloghard (>= 0.5.0)"
        "libsharemind-configuration (>= 0.5.0)"
        "libsharemind-executionmodelevaluator (>= 0.3.0)"
        "libsharemind-executionprofiler (>= 0.4.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
    DEB_EXTRA_CONTROL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian/conffiles"
)
SharemindAddComponentPackage("debug"
    NAME "libsharemind-mod-spdz-fresco-emu-dbg"
    DESCRIPTION "Sharemind SPDZ Fresco emulation module debug symbols"
    DEB_SECTION "debug"
    DEB_DEPENDS
        "libsharemind-mod-spdz-fresco-emu (= ${SharemindModSpdzFrescoEmu_DEB_lib_PACKAGE_VERSION})"
)
SharemindPackagingFinalize()
