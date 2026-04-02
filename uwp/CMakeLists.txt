cmake_minimum_required(VERSION 3.14)
include(FetchContent)

# Setup UWP platform
set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0.19041.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch shared UWP deps
FetchContent_Declare(UwpLibs
    GIT_REPOSITORY https://github.com/worleydl/uwp-dep.git
    GIT_TAG 8b9c5a5488153b37b7d82b975580c8413bc7cce1
)
FetchContent_MakeAvailable(UwpLibs)

# Define project
project(love2d-uwp LANGUAGES CXX)
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})

# Config for main love build
set(BUILD_TYPE "Release") # debug isn't functioning currently
set(LOVE_DIR CACHE PATH "")

add_executable(${PROJECT_NAME} WIN32
	main.cpp
)

set(BinDeps
	# libuwp/mesa/openal/sdl
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/dxil.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/libgallium_wgl.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/libuwp.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/OpenAL32.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/opengl32.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/SDL2.dll
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/bin/z-1.dll

	${LOVE_DIR}/love/${BUILD_TYPE}/love.dll
	${LOVE_DIR}/love/${BUILD_TYPE}/lua51.dll
	${LOVE_DIR}/love/${BUILD_TYPE}/mpg123.dll
	#${LOVE_DIR}/love/${BUILD_TYPE}/OpenAL32.dll # build config doesn't work

	Package.appxmanifest
)
source_group("BinDeps" FILES ${BinDeps})
set_source_files_properties(${BinDeps} PROPERTIES
	VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "."
)

set(PkgAssets
	Assets/LockScreenLogo.scale-200.png
	Assets/Square44x44Logo.targetsize-24_altform-unplated.png
	Assets/Square44x44Logo.scale-200.png
	Assets/Square150x150Logo.scale-200.png
	Assets/StoreLogo.png
)
source_group("PkgAssets" FILES ${PkgAssets})
set_source_files_properties(${PkgAssets} PROPERTIES
	#VS_COPY_TO_OUT_DIR Always
	VS_DEPLOYMENT_CONTENT TRUE
	VS_DEPLOYMENT_LOCATION "Assets"
)

# Setup data deployment
file(GLOB_RECURSE DATA_FILES
	RELATIVE ${CMAKE_SOURCE_DIR}/..
	${CMAKE_SOURCE_DIR}/../assets/*
)

foreach(ASSET ${DATA_FILES})
	set(FULL_PATH "${CMAKE_SOURCE_DIR}/../${ASSET}")

	get_filename_component(DEST_DIR ${ASSET} DIRECTORY)

	set_source_files_properties("${FULL_PATH}" PROPERTIES
		VS_COPY_TO_OUT_DIR Always
		VS_DEPLOYMENT_CONTENT TRUE
		VS_DEPLOYMENT_LOCATION "${DEST_DIR}" # preserves folder structure
	)

	target_sources(${PROJECT_NAME} PRIVATE "${FULL_PATH}")
	source_group("BinData" FILES "${FULL_PATH}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE
	${BinDeps}
	${PkgAssets}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/include
)

set(BinLibs
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/lib/libuwp.lib
	${CMAKE_CURRENT_BINARY_DIR}/_deps/uwplibs-src/x64/lib/SDL2.lib

	${LOVE_DIR}/bin/lua51.lib
	${LOVE_DIR}/love/${BUILD_TYPE}/liblove.lib
	${LOVE_DIR}/love/${BUILD_TYPE}/lovestatic.lib

	WindowsApp.lib
	OneCore.lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${BinLibs})
