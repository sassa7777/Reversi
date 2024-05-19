cmake_minimum_required (VERSION 3.7)

enable_language(OBJCXX)

set(DEVELOPMENT_PROJECT_NAME "Reversi")                     # <== Set to your project name, e.g. project.xcodeproj
set(DEVELOPMENT_TEAM_ID "AAAAAAAAAA")                       # <== Set to your team ID from Apple
set(APP_NAME "Reversi")                                     # <== Set To your app's name
set(APP_BUNDLE_IDENTIFIER "com.sasa.othello")                # <== Set to your app's bundle identifier
                                                            # /usr/bin/env xcrun security find-identity -v -p codesigning
set(DEPLOYMENT_TARGET 11.0)                                  # <== Set your deployment target version of iOS
set(DEVICE_FAMILY "1")                                      # <== Set to "1" to target iPhone, set to "2" to target iPad, set to "1,2" to target both
set(LOGIC_ONLY_TESTS 0)                                     # <== Set to 1 if you do not want tests to be hosted by the application, speeds up pure logic tests but you can not run them on real devices

project(${DEVELOPMENT_PROJECT_NAME})
include(BundleUtilities)
include(FindXCTest)

message(STATUS XCTestFound:${XCTest_FOUND})

set(PRODUCT_NAME ${APP_NAME})
set(EXECUTABLE_NAME ${APP_NAME})
set(MACOSX_BUNDLE_EXECUTABLE_NAME ${APP_NAME})
set(MACOSX_BUNDLE_INFO_STRING ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_ICON_FILE "")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright YOU")
set(MACOSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})

set(CMAKE_CXX_FLAGS "-O3 -I/usr/local/include -fobjc-arc -fobjc-weak")

set(APP_HEADER_FILES
  ./Reversi/ViewController.h
  ./Reversi/AppDelegate.h
  ./Reversi/FirstViewController.h
  ./Reversi/ResultsViewController.h
  ./Reversi/Wrapper.h
  ./Reversi/reversi.h
)

set(APP_SOURCE_FILES
  ./Reversi/ViewController.mm
  ./Reversi/AppDelegate.m
  ./Reversi/FirstViewController.mm
  ./Reversi/ResultsViewController.mm
  ./Reversi/Wrapper.mm
  ./Reversi/reversi.cpp
  ./Reversi/main.m
)

set(RESOURCES
  ./Reversi/Main.storyboard
  ./Reversi/Assets.xcassets
)

add_executable(
    ${APP_NAME}
    MACOSX_BUNDLE
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
    ${RESOURCES}
)

find_library(FOUNDATION Foundation)
target_link_libraries(${APP_NAME} ${FOUNDATION})
