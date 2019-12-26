cmake_minimum_required(VERSION 3.2)

project(kkmeans  VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set ( SOURCES
	main.cpp 		
)

add_subdirectory(dlib dlib_build)

add_executable( kkmeans ${SOURCES} kkmeans_ex.txt kkmeans_star_ex.txt )

set_target_properties(kkmeans PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(kkmeans dlib::dlib)

install(TARGETS kkmeans RUNTIME DESTINATION bin)
install(FILES kkmeans_ex.txt RUNTIME DESTINATION bin)
install(FILES kkmeans_star_ex.txt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT egavrilin90@gmail.com)

include(CPack)