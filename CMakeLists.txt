cmake_minimum_required(VERSION 3.16)

project(Transactions VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Concurrent ${USER_PLUGIN_QT_COMPONENTS})

qt_standard_project_setup(REQUIRES 6.8)

if(NOT USER_PLUGIN_SOURCES)
    set(USER_PLUGIN_SOURCES include/Categories.hpp)
endif()

qt_add_executable(Transactions
    src/Main.cpp
    src/LibreOffice.cpp
    src/PlaidAPI.cpp
    src/PlaidJson.cpp
    src/Config.cpp
    src/Categorize.cpp
    include/LibreOffice.hpp
    include/PlaidAPI.hpp
    include/PlaidJson.hpp
    include/Config.hpp
    include/Parser.hpp
    include/Categorize.hpp
    ${USER_PLUGIN_SOURCES}
)


set_target_properties(Transactions PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(LibreOffice.cmake)
target_include_directories(Transactions PRIVATE 
    ${USER_PLUGIN_INCLUDE_DIRECTORIES}
    include/
    ${LIBREOFFICE_INCLUDE_DIRS})

target_link_directories(Transactions PRIVATE
    ${USER_PLUGIN_LINK_DIRECTORIES}
    ${LIBREOFFICE_LIB_DIRS}
)

target_link_libraries(Transactions
    ${USER_PLUGIN_LINK_LIBRARIES}
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Concurrent
    PRIVATE ${LIBREOFFICE_LIBS}
    PRIVATE cpr
    PRIVATE curl
    PRIVATE mpdec++
    PRIVATE mpdec
    PRIVATE hs
)

include(GNUInstallDirs)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Transactions.sh.in
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Transactions.sh
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                     GROUP_READ GROUP_EXECUTE
                     WORLD_READ
    @ONLY
)

install(TARGETS Transactions
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
