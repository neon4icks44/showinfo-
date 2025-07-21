cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Настройка архитектур в зависимости от платформы
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    set(CMAKE_ANDROID_ARCH_ABI "arm64-v8a") # Можно изменить на armeabi-v7a для старых устройств
    set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
    set(CMAKE_ANDROID_STL_TYPE c++_shared)
elseif (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
else()
    set(CMAKE_OSX_ARCHITECTURES "x86_64") # Для Windows
endif()

project(ShowInfo VERSION 1.0.4)

# Находим исходные файлы
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Проверка Geode SDK
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# Подключаем Geode SDK
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Настройка Geode мода
setup_geode_mod(${PROJECT_NAME})

# Настройка Android NDK (если платформа Android)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    if (NOT DEFINED ENV{ANDROID_NDK_HOME})
        message(WARNING "ANDROID_NDK_HOME not set, trying default path")
        set(ANDROID_NDK "C:/android-ndk-r27d") # Замени на свой путь
    else()
        set(ANDROID_NDK $ENV{ANDROID_NDK_HOME})
    endif()
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK})
    if (NOT EXISTS ${CMAKE_ANDROID_NDK})
        message(FATAL_ERROR "Android NDK not found at ${CMAKE_ANDROID_NDK}. Please install NDK or set ANDROID_NDK_HOME correctly.")
    endif()
    set(CMAKE_SYSTEM_NAME Android)
    set(CMAKE_ANDROID_PLATFORM android-21) # Минимальная версия Android
endif()

# Настройка iOS (если платформа iOS)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
    set(CMAKE_OSX_SYSROOT "iphoneos")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
endif()