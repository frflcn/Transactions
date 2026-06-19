cmake_minimum_required(VERSION 3.16)

project(Transactions VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(Transactions
    src/Main.cpp
    src/LibreOffice.cpp
    src/PlaidAPI.cpp
    src/PlaidJson.cpp
    include/LibreOffice.hpp
    include/PlaidAPI.hpp
    include/PlaidJson.hpp
)


set_target_properties(Transactions PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(LibreOffice.cmake)
target_include_directories(Transactions PRIVATE 
    include/
    ${LIBREOFFICE_INCLUDE_DIRS})

target_link_directories(Transactions PRIVATE
    ${LIBREOFFICE_LIB_DIRS}
)

target_link_libraries(Transactions
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Concurrent
    PRIVATE ${LIBREOFFICE_LIBS}
    PRIVATE cpr
    PRIVATE curl
    PRIVATE mpdec++
    PRIVATE mpdec
)

include(GNUInstallDirs)

configure_file(
    ${CMAKE_SOURCE_DIR}/Transactions.sh.in
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
