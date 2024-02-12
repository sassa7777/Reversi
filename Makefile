PROJECT_NAME = othello.xcodeproj
TARGET_NAME = othello
CONFIGURATION = Release
ARCH = arm64
SDK = macosx
CPU_COUNT := $(shell sysctl -n hw.ncpu)
BUILD_DIR = $(shell pwd)/build

build:
		@rm -rf build Product
		@mkdir build
		@xcodebuild -jobs $(CPU_COUNT) -project $(PROJECT_NAME) -scheme $(TARGET_NAME) -configuration $(CONFIGURATION) -arch $(ARCH) -sdk $(SDK) -derivedDataPath $(BUILD_DIR)
		@ln -s build/Build/Products Product

cbuild:
		@rm -rf build Product
		@mkdir build
		@xcodebuild -jobs $(CPU_COUNT) -project $(PROJECT_NAME) -scheme $(TARGET_NAME) -configuration $(CONFIGURATION) -arch $(ARCH) -sdk $(SDK) -derivedDataPath $(BUILD_DIR) clean build
		@ln -s build/Build/Products Product
clean:
		@rm -rf build
