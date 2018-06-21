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
PROJECT(SharemindModSpdzFrescoEmu VERSION 1.1.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.2 REQUIRED)

SharemindSetProjectVersion()


FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(LogHard 0.3.0 REQUIRED)
FIND_PACKAGE(SharemindCHeaders 1.1.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders 0.4.0 REQUIRED)
FIND_PACKAGE(SharemindLibEmulatorProtocols REQUIRED)
FIND_PACKAGE(SharemindLibExecutionModelEvaluator 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler 0.2.0 REQUIRED)
FIND_PACKAGE(SharemindModuleApis 1.0.0 REQUIRED)
FIND_PACKAGE(SharemindPdkHeaders REQUIRED)


SharemindSetCxx11CompileOptions()
IF(SHAREMIND_NETWORK_STATISTICS)
    SharemindNewList(SharemindModSpdzFrescoEmu_EXTRA_DEFINITIONS
        "SHAREMIND_NETWORK_STATISTICS_ENABLE"
    )
ELSE()
    SharemindNewList(SharemindModSpdzFrescoEmu_EXTRA_DEFINITIONS)
ENDIF()
FILE(GLOB_RECURSE SharemindModSpdzFrescoEmu_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE SharemindModSpdzFrescoEmu_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
SharemindAddSharedLibrary("sharemind_mod_spdz_fresco_emu"
    MODULE
    OUTPUT_NAME "sharemind_mod_spdz_fresco_emu"
    SOURCES
        ${SharemindModSpdzFrescoEmu_SOURCES}
        ${SharemindModSpdzFrescoEmu_HEADERS}
    INCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS}
        ${LogHard_INCLUDE_DIRS}
        ${SharemindCHeaders_INCLUDE_DIRS}
        ${SharemindCxxHeaders_INCLUDE_DIRS}
        ${SharemindLibEmulatorProtocols_INCLUDE_DIRS}
        ${SharemindLibExecutionModelEvaluator_INCLUDE_DIRS}
        ${SharemindLibExecutionProfiler_INCLUDE_DIRS}
        ${SharemindModuleApis_INCLUDE_DIRS}
        ${SharemindPdkHeaders_INCLUDE_DIRS}
    COMPILE_DEFINITIONS
        ${SharemindModSpdzFrescoEmu_EXTRA_DEFINITIONS}
        "exprtk_disable_comments"
        "exprtk_disable_break_continue"
        "exprtk_disable_enhanced_features"
    LEGACY_DEFINITIONS
        ${LogHard_DEFINITIONS}
        ${SharemindCHeaders_DEFINITIONS}
        ${SharemindCxxHeaders_DEFINITIONS}
        ${SharemindLibEmulatorProtocols_DEFINITIONS}
        ${SharemindLibExecutionModelEvaluator_DEFINITIONS}
        ${SharemindLibExecutionProfiler_DEFINITIONS}
        ${SharemindPdkHeaders_DEFINITIONS}
    LINK_LIBRARIES
        ${CRYPTOPP_LIBRARIES}
        ${LogHard_LIBRARIES}
        ${SharemindCHeaders_LIBRARIES}
        ${SharemindCxxHeaders_LIBRARIES}
        ${SharemindLibExecutionModelEvaluator_LIBRARIES}
        ${SharemindLibExecutionProfiler_LIBRARIES}
        ${SharemindPdkHeaders_LIBRARIES}
)

FILE(GLOB_RECURSE SharemindModSpdzFrescoEmu_DEBUG_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/debug_files/*.cfg")
ADD_CUSTOM_TARGET("include_SharemindModSpdzFrescoEmu_DEBUG_FILES_in_IDE" SOURCES
                  ${SharemindModSpdzFrescoEmu_DEBUG_FILES})
IF(NOT (CMAKE_BUILD_TYPE STREQUAL "Release"))
    INSTALL(FILES ${SharemindModSpdzFrescoEmu_DEBUG_FILES}
            DESTINATION "bin"
            COMPONENT "debugconf")
ENDIF()


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-mod-spdz-fresco-emu"
    DESCRIPTION "Sharemind SPDZ Fresco emulation module"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libcrypto++6"
        "libloghard (>= 0.3.0)"
        "libsharemind-executionmodelevaluator (>= 0.2.0)"
        "libsharemind-executionprofiler (>= 0.2.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
)
SharemindAddComponentPackage("debug"
    NAME "libsharemind-mod-spdz-fresco-emu-dbg"
    DESCRIPTION "Sharemind SPDZ Fresco emulation module debug symbols"
    DEB_SECTION "debug"
    DEB_DEPENDS
        "libsharemind-mod-spdz-fresco-emu (= ${SharemindModSpdzFrescoEmu_DEB_lib_PACKAGE_VERSION})"
)
IF(NOT (CMAKE_BUILD_TYPE STREQUAL "Release"))
    SharemindAddComponentPackage("debugconf"
        NAME "libsharemind-mod-spdz-fresco-emu-debugconfiguration"
        DESCRIPTION "Sharemind SPDZ Fresco emulation module debug configuration"
        DEB_SECTION "debug"
        DEB_DEPENDS
            "libsharemind-mod-spdz-fresco-emu (= ${SharemindModSpdzFrescoEmu_DEB_lib_PACKAGE_VERSION})"
    )
ENDIF()
SharemindPackagingFinalize()
