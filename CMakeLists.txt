cmake_minimum_required(VERSION 3.2)

set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
set( DOC_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Doc )
project(print_ip VERSION 0.0.${PROJECT_VERSION_PATCH})

configure_file(version.h.in version.h @ONLY)

set(SRC
    main.cpp
    )

set(HDR
    )

#Program print_ip
add_executable(print_ip ${SRC} ${HDR})

set_target_properties(print_ip PROPERTIES
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}
)

#Дополнительный свойста компилятора для всех проектов
set_target_properties( print_ip PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall
)

find_package(Doxygen)

IF(NOT DOXYGEN_FOUND)
    MESSAGE(STATUS "No Doxygen found. Documentation won't be built")
ELSE()
    file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp )

    CONFIGURE_FILE(Doxyfile.in Doxyfile @ONLY)

    set( DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )

    add_custom_command(OUTPUT html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS ${MARKDOWN_DOC} ${SOURCES} ${DOXYFILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../
        )

    add_custom_target(doc ALL DEPENDS html)
   # install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
   #     DESTINATION ${DOC_INSTALL_DIR}
   #     COMPONENT doc)
ENDIF()

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT aquila-2004@yandex.ru)

include(CPack)
