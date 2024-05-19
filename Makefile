PROJECT_NAME = Reversi.xcodeproj
TARGET_NAME = Reversi
CONFIGURATION = Release
SDK = macosx
CPU_COUNT := $(shell sysctl -n hw.ncpu)
BUILD_DIR = $(shell pwd)/build

build:
		@rm -rf build Product
		@mkdir build
		@xcodebuild -jobs $(CPU_COUNT) -project $(PROJECT_NAME) -scheme $(TARGET_NAME) -configuration $(CONFIGURATION) -sdk $(SDK) -derivedDataPath $(BUILD_DIR) clean build CODE_SIGNING_ALLOWED=NO
		@ln -s build/Build/Products/Release Release
clean:
		@rm -rf build
