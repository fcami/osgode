ADD_SUBDIRECTORY(serializers)




IF(ANDROID)
    CONFIGURE_FILE("${OSGODE_ANDROID_TEMPLATES}/Android.mk.wrappers.in" "${CMAKE_CURRENT_BINARY_DIR}/Android.mk")
ENDIF()
