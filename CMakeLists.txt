cmake_minimum_required(VERSION 3.12)
project(ext-vanillagenerator)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_LIST FOLLOW_SYMLINKS *.cpp)
file(GLOB_RECURSE HEAD_FILES FOLLOW_SYMLINKS *.h)

set(PHP_SOURCE C:/pocketmine-php-sdk/php-src)
include_directories(BEFORE ./ ${PHP_SOURCE} ${PHP_SOURCE}/main ${PHP_SOURCE}/sapi ${PHP_SOURCE}/TSRM ${PHP_SOURCE}/Zend ${PHP_SOURCE}/ext/chunkutils2/src ${PHP_SOURCE}/ext/chunkutils2 ${PHP_SOURCE}/ext/chunkutils2/gsl/include ${PHP_SOURCE}/ext/morton/libmorton/libmorton/include)

add_library(ext-vanillagenerator SHARED ${SRC_LIST} ${HEAD_FILES})