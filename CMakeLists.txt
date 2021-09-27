cmake_minimum_required(VERSION 3.21)

set(VERBOSE)

# Names
set(PROJECT_NAME "TagViewer 3" CACHE STRING "Project Name")
set(EXECUTABLE_NAME tagviewer CACHE STRING "Executable Name")

# Qt Modules
set(QT_MODULES Core Gui Widgets Sql CACHE STRING "Qt Modules")
set(CMAKE_AUTOMOC True)
set(CMAKE_INCLUDE_CURRENT_DIR True)
set(CMAKE_AUTORCC True)
set(CMAKE_AUTOUIC True)

# Dirs/Files
set(DIR_SRC src CACHE PATH "Source Directory")
set(DIR_DIST dist CACHE PATH "Binary Directory")
set(DIR_INCLUDE include CACHE PATH "Include Directory")
set(DIR_SYSINC sysinc CACHE PATH "System Include Directory")
set(DIR_TRANSLATIONS translations CACHE PATH "Translations Directory")
set(DIR_RESOURCES resources CACHE PATH "Resources Directory")
set(DIR_LIB lib CACHE PATH "Libraries Directory")
set(FILE_RESOURCES ${DIR_RESOURCES}/resources.qrc CACHE FILEPATH "Resources File")

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_POSITION_INDEPENDENT_CODE True)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Linker/Compiler Flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
	-fuse-ld=lld \
	")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
	-Wall -Wextra -Wundef -Wno-unknown-pragmas -pedantic \
	-Wno-enum-compare -Wno-gnu-zero-variadic-macro-arguments \
	-Wno-c++11-extensions -Wno-c++14-extensions \
	-Wno-c++17-extensions -Wno-c++20-extensions \
	-fno-rtti -fno-exceptions \
	-Wno-unused-command-line-argument \
	-fcolor-diagnostics \
	")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)
if(${FORCE_COLORED_OUTPUT})
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options(-fdiagnostics-color=always)
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		add_compile_options(-fcolor-diagnostics)
	endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
	-fno-omit-frame-pointer -fstack-protector-strong -Og \
	-ggdb3 -Werror -Wfatal-errors \
	")

# Optional Linker/Compiler Flags
option(USE_ASAN "Use Address Sanitizer (Debug Only)" ON)
if(${USE_ASAN})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
		-finstrument-functions \
		-fsanitize=address -fsanitize=undefined -fsanitize=leak \
		")
endif()

option(USE_GLIBCXX_DEBUG "STL Debug Mode (Debug Only)" ON)
if(${USE_GLIBCXX_DEBUG})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
		-D_GLIBCXX_DEBUG \
		")
endif()

# Release Defines
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} \
	-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT -DNDEBUG \
	")

foreach(module ${QT_MODULES})
	find_package(Qt5${module} REQUIRED)
endforeach()

function(genqrc)
	cmake_parse_arguments(
		"GENQRC_ARG"
		""
		"QRC_PATH"
		"FILES"
		${ARGN}
	)
	if(NOT DEFINED GENQRC_ARG_QRC_PATH)
		message(FATAL_ERROR ".qrc path is required")
	endif()
	if(NOT DEFINED GENQRC_ARG_FILES)
		message(FATAL_ERROR "at least one file is required")
	else()
		list(LENGTH ${GENQRC_ARG_FILES} num_files)
		if (${num_files} LESS_EQUALS 0)
			message(FATAL_ERROR "at least one file required")
		endif()
	endif()

	set(genqrc_output "<!DOCTYPE RCC>\n\
		<!-- ${CMAKE_CURRENT_FUNCTION} -->\n\
		<!-- This is a generated file. -->\n\
		<RCC version=\"1.0\">\n\
		<qresource prefix=\"/\">\n\
		")
	foreach(file ${GENQRC_ARG_FILES})
		set(genqrc_output "${genqrc_output}\t<file>${file}</file>\n")
	endforeach()
	set(genqrc_output "${genqrc_output}\
		</qresource>\n\
		</RCC>\n\
		")
	file(WRITE ${GENQRC_ARG_QRC_PATH} "${genqrc_output}")
endfunction()

set(sources
	FileList.cpp
	TagSpaceConfig/PropsModel.cpp
	TagSpaceConfig/PropsView.cpp
	TagSpaceConfig/TagsModel.cpp
	TagSpaceConfig/TagSpaceConfig.cpp
	TagSpaceConfig/TagsView.cpp
	TagSpaceConfig/PropTypeComboBoxDelegate.cpp
	AboutDialog.cpp
	ActionBar.cpp
	AspectRatioLabel.cpp
	CenterPanel.cpp
	DebugPanel.cpp
	definitions.cpp
	FiltersSidebar.cpp
	main.cpp
	MainWindow.cpp
	Menu.cpp
	PropertiesSidebar.cpp
	SettingsTreeView.cpp
	StatusBar.cpp
	themes.cpp
	SettingsDialog.cpp
	TagSpace/TagSpace.cpp
	TagSpace/Tag.cpp
	TagSpace/Prop.cpp
	ComboBoxHelpers.cpp
	Settings.cpp
	)
set(headers
	CenterPanel.hpp
	themes.hpp
	Action.hpp
	links.hpp
	TagSpaceConfig/DataBackedModel.hpp
	TagSpaceConfig/PropsModel.hpp
	TagSpaceConfig/PropsView.hpp
	TagSpaceConfig/TagsModel.hpp
	TagSpaceConfig/TagSpaceConfig.hpp
	TagSpaceConfig/TagsView.hpp
	TagSpaceConfig/PropTypeComboBoxDelegate.hpp
	AboutDialog.hpp
	ActionBar.hpp
	AspectRatioLabel.hpp
	DebugPanel.hpp
	FileList.hpp
	FiltersSidebar.hpp
	icons.hpp
	initptr.hpp
	MainWindow.hpp
	Menu.hpp
	PropertiesSidebar.hpp
	SettingsDialog.hpp
	SettingsTreeView.hpp
	StatusBar.hpp
	templates.hpp
	TagSpace/TagSpace.hpp
	TagSpace/Tag.hpp
	TagSpace/Prop.hpp
	pragmas.hpp
	QVariantType.hpp
	ComboBoxHelpers.hpp
	Settings.hpp
	definitions.hpp
	settings_prop_list.hpp
	DataStreamList.hpp
	custom_types.hpp
	)
list(TRANSFORM sources PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_SRC}/")
list(TRANSFORM headers PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_INCLUDE}/")

qt_add_resources(sources ${${PROJECT_NAME}_SOURCE_DIR}/${FILE_RESOURCES})

add_executable(${EXECUTABLE_NAME} ${sources} ${headers})

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_INCLUDE})
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_SYSINC})
foreach(module ${QT_MODULES})
	target_link_libraries(${EXECUTABLE_NAME} Qt5::${module})
endforeach()

function(use_library library_name)
	function(library_load) # new scope
		unset(SOURCES)
		unset(DIR)
		unset(QT_MODULES)
		unset(INCLUDE_DIR)

		include(${DIR_LIB}/${library_name}.cmake)

		# DIR and NAME are required
		if(NOT DEFINED DIR OR NOT DEFINED LIBRARY_NAME)
			message(FATAL_ERROR "libraries must set DIR and LIBRARY_NAME")
		endif()

		set(DIR ${DIR_LIB}/${DIR})
		# INCLUDE_DIR defaults to DIR
		if(NOT DEFINED INCLUDE_DIR)
			set(INCLUDE_DIR ${DIR})
		endif()
		if (DEFINED QT_MODULES)
			list(TRANSFORM QT_MODULES PREPEND "Qt5::")
		endif()

		# a header-only library would not have sources
		if(DEFINED SOURCES)
			list(TRANSFORM SOURCES PREPEND ${DIR}/)
			add_library(${LIBRARY_NAME} OBJECT ${SOURCES})
			target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIR})
		else()
			add_library(${LIBRARY_NAME} INTERFACE)
			target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIR})
		endif()

		# a library might not be Qt-specific
		if(DEFINED QT_MODULES)
			if(DEFINED SOURCES)
				target_link_libraries(${LIBRARY_NAME} ${QT_MODULES})
			else()
				target_link_libraries(${LIBRARY_NAME} INTERFACE ${QT_MODULES})
			endif()
		endif()
	endfunction()
	library_load()
	target_link_libraries(${EXECUTABLE_NAME} ${library_name})
endfunction()

use_library(ColorPicker)
use_library(phantom)
use_library(QFontIcon)
use_library(QFunctionUtils)

# Defines
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE QT_NO_CAST_FROM_ASCII QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

set_property(TARGET ${EXECUTABLE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
