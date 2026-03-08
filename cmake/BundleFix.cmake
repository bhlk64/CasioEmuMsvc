include(BundleUtilities)

if(NOT DEFINED APP)
    message(FATAL_ERROR "APP variable not set")
endif()

message(STATUS "Fixing macOS bundle: ${APP}")

if(NOT EXISTS "${APP}")
    message(FATAL_ERROR "Bundle path does not exist: ${APP}")
endif()

# main executable
get_bundle_main_executable("${APP}" APP_EXEC)

message(STATUS "Main executable: ${APP_EXEC}")

# Frameworks dir
set(FRAMEWORK_DIR "${APP}/Contents/Frameworks")

file(MAKE_DIRECTORY "${FRAMEWORK_DIR}")

# gather libraries already inside bundle
set(BUNDLE_LIBS)

file(GLOB_RECURSE EXISTING_LIBS
    "${APP}/Contents/Frameworks/*.dylib"
)

list(APPEND BUNDLE_LIBS ${EXISTING_LIBS})

# run fixup
fixup_bundle("${APP}" "${BUNDLE_LIBS}" "")

# sanity check
verify_app("${APP}")

message(STATUS "Bundle fix complete.")