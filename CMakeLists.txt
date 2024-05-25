
# CMakeLists.txt 
# CMake Template file. 26/Nov/2022

# Please check 'cmake --help-module-list'


#[[ Require a minimum version of cmake. ]]
cmake_minimum_required(VERSION 3.25.0)

#[[ Enable languages (CXX/C/OBJC/OBJCXX/Fortran/etc)
    The following options affect compiler selection.
    Be careful when using this option! 
]]
enable_language(CXX)
enable_language(OBJC)
enable_language(OBJCXX)

# Set the name of the project.　
project(Reversi)


#[[ Add an executable to the project using the specified source files. ]]
file(GLOB SRC "Reversi/*.cpp" "Reversi/*.m" "Reversi/*.mm")
add_executable(${PROJECT_NAME}
    MACOSX_BUNDLE
    ${SRC}
    ./Reversi/Main.storyboard
)

#[[ Add include directories to a target. ]]
if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/Reversi/include)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/Reversi/include)
endif()

#[[ Add expected compiler features to a target.]]
# cxx_std_98, cxx_std_11, cxx_std_14, cxx_std_17, cxx_std_20, cxx_std_23
# c_std_90, c_std_99, c_std_11, c_std_17, c_std_23
target_compile_features(${PROJECT_NAME} PRIVATE c_std_17 cxx_std_20)

#[[ Additional compiler options. -v ]]
target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall )

#[[ clang++ sanitizer options.
    OK:Address Sanitizer
    OK:Undefined Behavior
    NG:Memory Sanitizer : unsupported option '-fsanitize=memory' for target 'arm64-apple-darwin22.1.0'
    cmake -DSANI=true ..
]]
if( SANI AND (( CMAKE_CXX_COMPILER MATCHES "XcodeDefault") OR (CMAKE_C_COMPILER MATCHES "XcodeDefault")))
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address -g )
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
endif()


#[[ Source files can have properties that affect how they are built.]]
# set_source_files_properties(
#   ${PROJECT_SOURCE_DIR}/src/main.cpp
#     PROPERTIES COMPILE_FLAGS 
#     "-Wall"
# )

#[[ Targets can have properties that affect how they are built. ]]
# set_target_properties(
#     ${PROJECT_NAME}
#     PROPERTIES COMPILE_FLAGS 
#     "-v"
# )

#[[ Source files can have properties that affect how they are built. ]]
# set_source_files_properties(
#   ${PROJECT_SOURCE_DIR}/src/main.cpp
#   PROPERTIES COMPILE_DEFINITIONS "ARGDEFINE=\"TEST\"; NUMTEST=10"
# )

target_sources(${PROJECT_NAME} PRIVATE Reversi/Assets.xcassets)
set_source_files_properties(Reversi/Assets.xcassets PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.sasa.othello"
        XCODE_ATTRIBUTE_PRODUCT_NAME "Reversi"
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon;
        XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES YES;
        COMPILE_FLAGS "-fobjc-arc"
        #RESOURCE "${ICON_FILES}" 
)

target_link_libraries(${PROJECT_NAME}
    "-framework AppKit"
    "-framework Foundation"
)

#[[ Add a target with no output so it will always be built. ]]
# add_custom_target(-${PROJECT_NAME}- ALL
#     COMMAND "date" "-R"
#     COMMAND "lipo" "-archs" "${PROJECT_NAME}"
#     COMMAND "otool" "-L" "${PROJECT_NAME}"
# )

#[[ The best way of installing programs into /usr/local/bin.    ]]
# install(TARGETS ${PROJECT_NAME}) 

#[[ The better way of installing programs into optinal directory.   ]]
# install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/install) 

#[[ The better way of installing programs into prefixed install directory.  ]]
# set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
# install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}) 

# install (
#     DIRECTORY ${CMAKE_SOURCE_DIR}/src/include/
#     DESTINATION include
#     FILES_MATCHING PATTERN "*.h*")

#  ---------  End of template block  ---------------------------------------

