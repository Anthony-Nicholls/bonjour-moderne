
#===============================================================================
# setup
#===============================================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(cmake-utils)

cmake_minimum_required(VERSION 3.12)
project(bonjour-moderne VERSION 1.0.0)


#===============================================================================
# primary target
#===============================================================================

add_library(bonjour-moderne STATIC)

target_sources(bonjour-moderne
PRIVATE
    include/bonjour_moderne/advertisable_service.h
    include/bonjour_moderne/advertised_service.h
    include/bonjour_moderne/bonjour_moderne.h
    include/bonjour_moderne/daemon_api_version.h
    include/bonjour_moderne/discoverable_service.h
    include/bonjour_moderne/discovered_service.h
    include/bonjour_moderne/resolved_service.h
    include/bonjour_moderne/service_advertiser.h
    include/bonjour_moderne/service_browser.h
    include/bonjour_moderne/service_domain.h
    include/bonjour_moderne/service_fullname.h
    include/bonjour_moderne/service_host.h
    include/bonjour_moderne/service_hostname.h
    include/bonjour_moderne/service_interface.h
    include/bonjour_moderne/service_name.h
    include/bonjour_moderne/service_port.h
    include/bonjour_moderne/service_protocol.h
    include/bonjour_moderne/service_resolver.h
    include/bonjour_moderne/service_txt_record.h
    include/bonjour_moderne/service_type.h

    source/daemon_api_version.cpp
    source/dns_service.cpp
    source/dns_service.h
    source/service_advertiser.cpp
    source/service_browser.cpp
    source/service_domain.cpp
    source/service_fullname.cpp
    source/service_host.cpp
    source/service_hostname.cpp
    source/service_interface.cpp
    source/service_name.cpp
    source/service_port.cpp
    source/service_protocol.cpp
    source/service_resolver.cpp
    source/service_txt_record.cpp
    source/service_type.cpp
    source/shared_resource.h
    source/string.cpp
    source/string.h
)

target_include_directories(bonjour-moderne PUBLIC include)
target_compile_features(bonjour-moderne PUBLIC cxx_std_17)
target_enable_recommended_warnings(bonjour-moderne)
target_enable_warnings_as_errors(bonjour-moderne)
target_clangformat_setup(bonjour-moderne)

#===============================================================================
# example target
#===============================================================================

add_executable(bonjour-moderne-example)

target_sources(bonjour-moderne-example
PRIVATE
    example/main.cpp
)

target_link_libraries(bonjour-moderne-example
PRIVATE
    bonjour-moderne
)

target_enable_recommended_warnings(bonjour-moderne-example)
target_enable_warnings_as_errors(bonjour-moderne-example)
target_clangformat_setup(bonjour-moderne-example)


#===============================================================================
# test target
#===============================================================================

add_subdirectory(lib/Catch2)
add_executable(bonjour-moderne-tests)

target_sources(bonjour-moderne-tests
PRIVATE
    tests/main.cpp
)

target_link_libraries(bonjour-moderne-tests
PRIVATE
    bonjour-moderne
    Catch2::Catch2WithMain
)

target_enable_recommended_warnings(bonjour-moderne-tests)
target_enable_warnings_as_errors(bonjour-moderne-tests)
target_clangformat_setup(bonjour-moderne-tests)
