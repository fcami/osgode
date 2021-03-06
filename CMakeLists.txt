CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)




PROJECT(osgODE)




SET(LOCAL_PROJECT_NAME osgODE)



SET(OSGODE_SOVERSION 35)
SET(OSGODE_MAJOR_VERSION 2)
SET(OSGODE_MINOR_VERSION 0)
SET(OSGODE_PATCH_VERSION 0)
SET(OSGODE_VERSION ${OSGODE_MAJOR_VERSION}.${OSGODE_MINOR_VERSION}.${OSGODE_PATCH_VERSION})



IF(NOT WIN32)
    IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET(LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
        MARK_AS_ADVANCED(LIB_POSTFIX)
    ENDIF()

    SET(PKGCONFIG_FILES
            osgODE
            osgODEUtil
    )

    FOREACH(PKGCONFIG_FILE ${PKGCONFIG_FILES})
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc.in
        ${PROJECT_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc
        @ONLY
        )
    INSTALL(FILES ${PROJECT_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc DESTINATION lib${LIB_POSTFIX}/pkgconfig)
    ENDFOREACH(PKGCONFIG_FILE)
ENDIF(NOT WIN32)






CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/src/osgODE/Version.cpp.in" "${PROJECT_SOURCE_DIR}/src/osgODE/Version.cpp")







OPTION(OSGODE_BUILD_PLATFORM_ANDROID "" OFF)
IF(OSGODE_BUILD_PLATFORM_ANDROID)

    SET(ANDROID TRUE)
    SET(ANDROID_PLATFORM 5)

    SET(OSGODE_ANDROID_TEMPLATES "${CMAKE_SOURCE_DIR}/PlatformSpecifics/Android")

ENDIF(OSGODE_BUILD_PLATFORM_ANDROID)









INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)



IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)






OPTION(OSGODE_BUILD_DEBUG "" OFF)

IF(OSGODE_BUILD_DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(OSGODE_BUILD_DEBUG)



IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(OSGODE_BUILD_DEBUG ON)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")


MESSAGE(STATUS "osgODE build: ${CMAKE_BUILD_TYPE}")


IF(OSGODE_BUILD_DEBUG)

    SET(PS_ASSERT_ENABLED "1")
    SET(PS_DEFAULT_LOG_LEVEL "LOG_LEVEL_DEBUG2")

ELSE(OSGODE_BUILD_DEBUG)

    SET(PS_ASSERT_ENABLED "0")
    SET(PS_DEFAULT_LOG_LEVEL "LOG_LEVEL_MESSAGE")

ENDIF(OSGODE_BUILD_DEBUG)



OPTION(OSGODE_BUILD_VERBOSE "" ON)

IF(OSGODE_BUILD_VERBOSE)
    SET(PS_VERBOSE "1")

ELSE(OSGODE_BUILD_VERBOSE)
    SET(PS_VERBOSE "0")
    SET(PS_DEFAULT_LOG_LEVEL "LOG_LEVEL_NONE")

ENDIF(OSGODE_BUILD_VERBOSE)




OPTION(OSGODE_REPORT_FILE_AND_LINE "" OFF)
IF(OSGODE_REPORT_FILE_AND_LINE)
    SET(PS_REPORT_FILE_AND_LINE "1")
ELSE(OSGODE_REPORT_FILE_AND_LINE)
    SET(PS_REPORT_FILE_AND_LINE "0")
ENDIF(OSGODE_REPORT_FILE_AND_LINE)




OPTION(OSGODE_USE_SINGLE_PRECISION "" OFF)

MARK_AS_ADVANCED(OSGODE_USE_SINGLE_PRECISION)

IF(OSGODE_USE_SINGLE_PRECISION)
    SET(OSGODE_SINGLE_PRECISION "1")
    SET(OSGODE_REAL "float")

ELSE(OSGODE_USE_SINGLE_PRECISION)
    SET(OSGODE_SINGLE_PRECISION "0")
    SET(OSGODE_REAL "double")

ENDIF(OSGODE_USE_SINGLE_PRECISION)

MESSAGE(STATUS "osgODE precision: ${OSGODE_REAL}")




OPTION(OSGODE_USE_ALIAS_OO "" OFF)

MARK_AS_ADVANCED(OSGODE_USE_ALIAS_OO)

IF(OSGODE_USE_ALIAS_OO)
    MESSAGE(STATUS "osgODE: using alias")
    SET(OSGODE_SET_ALIAS "    namespace osgODE {}\n    namespace oo = osgODE ;\n\n    namespace osgODEUtil {}\n    namespace ooUtil = osgODEUtil ;")

ELSE(OSGODE_USE_ALIAS_OO)
    MESSAGE(STATUS "osgODE: NOT using alias")
    SET(OSGODE_SET_ALIAS "//     namespace osgODE {}\n//     namespace oo = osgODE ;\n\n//     namespace osgODEUtil {}\n//     namespace ooUtil = osgODEUtil ;")

ENDIF(OSGODE_USE_ALIAS_OO)





IF( WIN32 )
    ADD_DEFINITIONS(-W4 -DOSG_LIBRARY)
ELSE( WIN32 )
    ADD_DEFINITIONS(-Wall -DOSG_LIBRARY -fPIC)
ENDIF( WIN32 )




CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/include/osgODE/Config.in" "${PROJECT_SOURCE_DIR}/include/osgODE/Config")





IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

    ADD_DEFINITIONS(-DIS_X86_64=1)

ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )

    ADD_DEFINITIONS(-DIS_X86_64=0)

ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )






IF( WIN32 )
    SET(OSGODE_OSG_INCLUDE "$ENV{PROGRAMFILES}\\OpenSceneGraph\\include" CACHE PATH "OSG Headers")
    SET(OSGODE_ODE_INCLUDE "$ENV{PROGRAMFILES}\\OpenDynamicsEngine\\include" CACHE PATH "ODE Headers")

ELSE( WIN32 )

    IF( NOT ANDROID )

        EXECUTE_PROCESS(COMMAND pkg-config openscenegraph-osg --cflags-only-I OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

        SET(OSGODE_OSG_INCLUDE ${tmp_var} CACHE PATH "OSG Headers")




        EXECUTE_PROCESS(COMMAND pkg-config ode --cflags-only-I OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

        SET(OSGODE_ODE_INCLUDE ${tmp_var} CACHE PATH "ODE Headers")

    ENDIF( NOT ANDROID )

ENDIF( WIN32 )


MESSAGE(STATUS "OSG include: ${OSGODE_OSG_INCLUDE}")
MESSAGE(STATUS "ODE include: ${OSGODE_ODE_INCLUDE}")





IF( WIN32 )
    SET(OSGODE_OSG_LIBRARIES "$ENV{PROGRAMFILES}\\OpenSceneGraph\\lib" CACHE PATH "OSG Libraries")
    SET(OSGODE_ODE_LIBRARIES "$ENV{PROGRAMFILES}\\OpenDynamicsEngine\\lib" CACHE PATH "ODE Libraries")

ELSE( WIN32 )

    IF( NOT ANDROID )

        EXECUTE_PROCESS(COMMAND pkg-config openscenegraph-osg --libs-only-L OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

        SET(OSGODE_OSG_LIBRARIES ${tmp_var} CACHE PATH "OSG libraries")



        EXECUTE_PROCESS(COMMAND pkg-config ode --libs-only-L OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

        SET(OSGODE_ODE_LIBRARIES ${tmp_var} CACHE PATH "ODE libraries")

    ENDIF( NOT ANDROID )

ENDIF( WIN32 )


MESSAGE(STATUS "OSG libraries: ${OSGODE_OSG_LIBRARIES}")
MESSAGE(STATUS "ODE libraries: ${OSGODE_ODE_LIBRARIES}")





IF( WIN32 )
    SET(OSGODE_OSG_BINARIES "$ENV{PROGRAMFILES}\\OpenSceneGraph\\bin" CACHE PATH "OSG binaries")
    MESSAGE(STATUS "OSG binaries: ${OSGODE_OSG_BINARIES}")
ENDIF( WIN32 )




IF(NOT ANDROID)
    INCLUDE_DIRECTORIES(${OSGODE_OSG_INCLUDE})
    INCLUDE_DIRECTORIES(${OSGODE_ODE_INCLUDE})

    LINK_DIRECTORIES( ${OSGODE_OSG_LIBRARIES} )
    LINK_DIRECTORIES( ${OSGODE_ODE_LIBRARIES} )
ENDIF(NOT ANDROID)





CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")





SET(OSGODE_DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/doc" CACHE PATH "Doxygen output directory")



SET(OSGODE_TEMP_DOXYGEN_INPUT "${PROJECT_BINARY_DIR}/doc/osgode_doxygen_input_files.txt")
MARK_AS_ADVANCED(OSGODE_TEMP_DOXYGEN_INPUT)

FILE(WRITE ${OSGODE_TEMP_DOXYGEN_INPUT} "${PROJECT_SOURCE_DIR}/README\n")
FILE(APPEND ${OSGODE_TEMP_DOXYGEN_INPUT} "${PROJECT_SOURCE_DIR}/INSTALL_WINDOWS\n")


FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
    ADD_CUSTOM_TARGET(  doc
                        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen"
                        VERBATIM
    )
ENDIF(DOXYGEN_FOUND)




SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(osgODEMacros)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)


IF( NOT ANDROID )
    OPTION(OSGODE_BUILD_EXAMPLES "Build osgODE examples" OFF)
    MESSAGE(STATUS "osgODE examples: ${OSGODE_BUILD_EXAMPLES}")

    OPTION(OSGODE_BUILD_PSOUND_EXAMPLES "Build pSound based osgODE examples" OFF)
    MESSAGE(STATUS "osgODE pSound examples: ${OSGODE_BUILD_PSOUND_EXAMPLES}")


    IF( OSGODE_BUILD_EXAMPLES )

        SET(OSGODE_SAMPLE_DATA "${CMAKE_SOURCE_DIR}/../data-2.0.0" CACHE PATH "osgODE sample data")
        MESSAGE(STATUS "osgODE sample data: ${OSGODE_SAMPLE_DATA}")

        ADD_SUBDIRECTORY( examples )

    ENDIF( OSGODE_BUILD_EXAMPLES )
ENDIF( NOT ANDROID )


IF( NOT ANDROID )
    OPTION(OSGODE_BUILD_APPLICATIONS "Build osgODE applications" ON)
    MESSAGE(STATUS "osgODE applications: ${OSGODE_BUILD_APPLICATIONS}")


    IF( OSGODE_BUILD_APPLICATIONS )
        ADD_SUBDIRECTORY( applications )
    ENDIF( OSGODE_BUILD_APPLICATIONS )
ENDIF( NOT ANDROID )




FILE(REMOVE ${OSGODE_TEMP_DOXYGEN_INPUT})




MESSAGE(STATUS "${PROJECT_NAME} install prefix: ${CMAKE_INSTALL_PREFIX}")





IF(ANDROID)
    SET(OSGODE_SOURCE_DIR "${PROJECT_SOURCE_DIR}")

    CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/Android.mk.master.in" "${CMAKE_BINARY_DIR}/Android.mk")
    CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/Application.mk.master.in" "${CMAKE_BINARY_DIR}/Application.mk")
    CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/AndroidManifest.xml.master.in" "${CMAKE_BINARY_DIR}/AndroidManifest.xml")

    MESSAGE(STATUS "Android build: use <PATH_TO_NDK>/ndk-build NDK_APPLICATION_MK=Application.mk")
ENDIF(ANDROID)









IF(OSGODE_USE_ALIAS_OO)

IF(WIN32)

    INSTALL( CODE
                "EXECUTE_PROCESS(
                    COMMAND
                        mklink /d ${CMAKE_INSTALL_PREFIX}/include/oo ${CMAKE_INSTALL_PREFIX}/include/osgODE
                )")

    INSTALL( CODE
                "EXECUTE_PROCESS(
                    COMMAND
                        mklink /d ${CMAKE_INSTALL_PREFIX}/include/ooUtil ${CMAKE_INSTALL_PREFIX}/include/osgODEUtil
                )")

ELSE(WIN32)

    INSTALL( CODE
                "EXECUTE_PROCESS(
                    COMMAND
                        ln -sf ${CMAKE_INSTALL_PREFIX}/include/osgODE  ${CMAKE_INSTALL_PREFIX}/include/oo
                )")

    INSTALL( CODE
                "EXECUTE_PROCESS(
                    COMMAND
                        ln -sf ${CMAKE_INSTALL_PREFIX}/include/osgODEUtil  ${CMAKE_INSTALL_PREFIX}/include/ooUtil
                )")



    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

        INSTALL( CODE
                    "EXECUTE_PROCESS(
                        COMMAND
                            ln -sf ${CMAKE_INSTALL_PREFIX}/lib64/libosgODE.so  ${CMAKE_INSTALL_PREFIX}/lib64/liboo.so
                    )")

        INSTALL( CODE
                    "EXECUTE_PROCESS(
                        COMMAND
                            ln -sf ${CMAKE_INSTALL_PREFIX}/lib64/libosgODEUtil.so  ${CMAKE_INSTALL_PREFIX}/lib64/libooUtil.so
                    )")



    ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )

        INSTALL( CODE
                    "EXECUTE_PROCESS(
                        COMMAND
                            ln -sf ${CMAKE_INSTALL_PREFIX}/lib/libosgODE.so  ${CMAKE_INSTALL_PREFIX}/lib/liboo.so
                    )")

        INSTALL( CODE
                    "EXECUTE_PROCESS(
                        COMMAND
                            ln -sf ${CMAKE_INSTALL_PREFIX}/lib/libosgODEUtil.so  ${CMAKE_INSTALL_PREFIX}/lib/libooUtil.so
                    )")



    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )



ENDIF(WIN32)

ENDIF(OSGODE_USE_ALIAS_OO)
