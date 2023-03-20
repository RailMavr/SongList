cmake_minimum_required (VERSION 3.8)
set(PRJ_NAME songlist)

project(${PRJ_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

# Формирование списка заголовочных файлов
#file(GLOB_RECURSE SRC_H src/*.h)

# Формирование списка заголовочных файлов
file(GLOB_RECURSE SRC_H src/*.h)
qt5_wrap_cpp(MOC_SRC_H ${SRC_H})

# Формирование списка файлов с исходным кодом
file(GLOB_RECURSE SRC_C src/*.cpp)

file(GLOB_RECURSE SRC_UI src/*.ui)
qt5_wrap_ui(SRC_UI1 ${SRC_UI})

file(GLOB_RECURSE SRC_RCC src/*.qrc)
qt5_add_resources(SRC_RES ${SRC_RCC})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_PATH})

add_executable(${PRJ_NAME} ${SRC_C} ${SRC_H}  ${SRC_UI1} ${SRC_RES} ${MOC_SRC_H} )

target_link_libraries(
	${PRJ_NAME}
	Qt5::Core
	Qt5::Widgets 
	Qt5::Gui 
        Qt5::Sql
	Qt5::Xml
)


include(GenerateSourceGroups.cmake)
GroupSources(src)


set_property(TARGET ${PRJ_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_property(TARGET ${PRJ_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_PREFIX_PATH}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PRJ_NAME})


