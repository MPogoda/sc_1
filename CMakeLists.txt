project( virtualBartender CXX )

cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

add_definitions( -DFL_DIVISIONS=9000)
set( FUZZYLITE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/fuzzylite/fuzzylite" CACHE PATH "Path to fuzzylite lib" )
add_subdirectory( ${FUZZYLITE_PATH} )
include_directories( ${FUZZYLITE_PATH} )

find_package( Qt4 REQUIRED QtCore QtGui )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

add_definitions( -std=c++11 )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
qt4_wrap_ui( FORMS_HEADERS mainwindow.ui parametersdialog.ui )

file( GLOB virtualBartender_SRCS "*.cxx" )

add_executable( virtualBartender ${virtualBartender_SRCS} ${FORMS_HEADERS} )
target_link_libraries( virtualBartender ${QT_LIBRARIES} fl-static )
