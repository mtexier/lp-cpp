################################################################################
#                                                                              #
#                        Lp project CMake modules                              #
#                                                                              #
#           Author: Julien Peeters <contact@julienpeeters.net>                 #
#                  Licence: GNU GPL version 3 or later                         #
#                                                                              #
################################################################################

set (Boost_ADDITIONAL_VERSIONS "1.46.1" "1.47.0" )
include ( FindBoost )
find_package ( Boost 1.47.0 )

if ( Boost_FOUND )
  message ( STATUS "Boost library found in system" )
  link_directories ( ${Boost_LIBRARY_DIRS} )
  include_directories ( ${Boost_INCLUDE_DIRS} )
else ()
  message ( STATUS "Boost library found in home" )
  link_directories ( $ENV{HOME}/Work/libs/boost/1.46/lib )
  include_directories ( $ENV{HOME}/Work/libs/boost/1.46/include )
endif ()
