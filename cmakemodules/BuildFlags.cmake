IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    SET (CMAKE_BUILD_TYPE RELEASE)
    SET (CMAKE_BUILD_TYPE RELEASE CACHE STRING "Build type" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
MESSAGE ("[!] Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE ("[!] Compiler: ${CMAKE_CXX_COMPILER_ID}")

INCLUDE (CheckCXXSymbolExists)

IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
 OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Опция для печати всех вызовов программ из gcc
    OPTION (USE_GCC_WRAPPER "Turn on to use gcc internal calls wrapper which outputs it to stdin" OFF)
    IF (USE_GCC_WRAPPER)
        SET (WRAPPER_FLAG "-wrapper ${CMAKE_SOURCE_DIR}/CMakeModules/gcc_wrapper.sh")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WRAPPER_FLAG}")
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WRAPPER_FLAG}")
    ENDIF (USE_GCC_WRAPPER)

    OPTION (GCC_ALWAYS_COLOR "turns -fdiagnostics-color=always in gcc" OFF)
    IF (GCC_ALWAYS_COLOR)
        ADD_DEFINITIONS (-fdiagnostics-color=always)
    ELSE()
        ADD_DEFINITIONS (-fdiagnostics-color=auto)
    ENDIF()

    OPTION (GCC_TIME_REPORT "Enables reporting of compilation times" OFF)
    IF (GCC_TIME_REPORT)
        ADD_DEFINITIONS (-ftime-report)
    ENDIF()

    # Общие для всех конфигураций опции
    ADD_DEFINITIONS (-pipe)
    #    ADD_DEFINITIONS (-fPIC)
    ADD_DEFINITIONS (-Wall)
    #    ADD_DEFINITIONS (-Wextra)
        ADD_DEFINITIONS (-Wempty-body)
        #   ADD_DEFINITIONS (-Wunused-parameter)
        ADD_DEFINITIONS (-Wignored-qualifiers)
        ADD_DEFINITIONS (-Wtype-limits)
    ADD_DEFINITIONS (-Winit-self)
    ADD_DEFINITIONS (-pedantic)
    # Эти опцию включаются только с -pedantic
    ADD_DEFINITIONS (-Wno-long-long)
    ADD_DEFINITIONS (-Wno-variadic-macros)
    ADD_DEFINITIONS (-Wformat=2)
    ADD_DEFINITIONS (-Wdate-time)
    ADD_DEFINITIONS (-Wmissing-braces)

    IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        ADD_DEFINITIONS (-Wno-mismatched-tags)
        ADD_DEFINITIONS (-Wno-format-nonliteral)
    ENDIF()

    ADD_DEFINITIONS (-fstack-protector)

    SET (FLAGS_C_LANG "")
    #SET (FLAGS_CXX_LANG "-Wno-deprecated -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual")
    # Здесь был флаг -fopenmp, но с ним сборка будет с динамической libgomp, т.к. флаг будет передан в линкер.
    # Чтобы этого избежать, флаги передаем специально файлам, где используется OpenMP, а библиотеку ищем сами вручную статическую,
    # и явно линкуем.
    SET (FLAGS_CXX_LANG "-std=c++14 -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual")

    # Но в LLVM libomp не поставляется статическая библиотека, поэтому для Clang передаем флаг в компилятор
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        SET (FLAGS_C_LANG "${FLAGS_C_LANG} -fopenmp")
        SET (FLAGS_CXX_LANG "${FLAGS_CXX_LANG} -fopenmp")
    ENDIF()

    SET (FLAGS_RELEASE          "-O2 -fomit-frame-pointer -funroll-loops -DNDEBUG")
    SET (FLAGS_RELWITHDEBINFO   "-ggdb -g3 -O2 -DNDEBUG")
    SET (FLAGS_DEBUG            "-ggdb -g3")

#   Debug options to prevent buffer overflows (http://gcc.gnu.org/ml/gcc-patches/2004-09/msg02055.html)
#   They add some extra checking code
    # If it is already set by default (like in Gentoo) we will get a lot of warnings about redifinition
    # Starting from glibc 2.22 it emits a warning if using FORTIFY_SOURCE without optimization,
    # so adding it only to optimized builds
    SET (FLAGS_RELEASE          "${FLAGS_RELEASE}        -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2")
    SET (FLAGS_RELWITHDEBINFO   "${FLAGS_RELWITHDEBINFO} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2")

    # Информация о системе, нужна для вывода в build_config
    EXECUTE_PROCESS (COMMAND uname -a OUTPUT_VARIABLE SYSTEM_INFO)
    STRING (REPLACE "\n" "" SYSTEM_INFO "${SYSTEM_INFO}")

    # Настройки, специфичные для разных версий компилятора
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        SET (GCC_MINIMUM_VERSION_REQUIRED 6.1)
        IF (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${GCC_MINIMUM_VERSION_REQUIRED})
            MESSAGE (FATAL_ERROR "Found compiler version ${CMAKE_CXX_COMPILER_VERSION}, need at least ${GCC_MINIMUM_VERSION_REQUIRED}")
        ENDIF ()
    ENDIF()

    IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
        AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "3.6.0")
        ADD_DEFINITIONS (-Wno-inconsistent-missing-override)
    ENDIF()

    SET (CMAKE_C_FLAGS_RELEASE          "${FLAGS_RELEASE}           ${FLAGS_C_LANG}")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO   "${FLAGS_RELWITHDEBINFO}    ${FLAGS_C_LANG}")
    SET (CMAKE_C_FLAGS_DEBUG            "${FLAGS_DEBUG}             ${FLAGS_C_LANG}")

    SET (CMAKE_CXX_FLAGS_RELEASE        "${FLAGS_RELEASE}           ${FLAGS_CXX_LANG}")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${FLAGS_RELWITHDEBINFO}    ${FLAGS_CXX_LANG}")
    SET (CMAKE_CXX_FLAGS_DEBUG          "${FLAGS_DEBUG}             ${FLAGS_CXX_LANG}")

    SET (CMAKE_EXE_LINKER_FLAGS         "-static-libstdc++ -static-libgcc")

    # Некоторые определения
    ADD_DEFINITIONS (-D__STDC_FORMAT_MACROS)    # включает printf-макросы из <inttypes.h> в C++ коде
    ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64)    # включает 64-битный off_t для работы с большими файлами
    ADD_DEFINITIONS (-DJUDYERROR_NOTEST)        # включает в макросах libJudy режим игнорирования ошибок
    ADD_DEFINITIONS (-DRAPIDJSON_HAS_STDSTRING=1) # включает поддержку std::string в rapidjson

    OPTION (USE_LTO               "Use link time optimization" ON)
    OPTION (USE_GOLD              "Use gold linker" ON)
    OPTION (USE_ADDRESS_SANITIZER "Use compiler address sanitizer" OFF)
    OPTION (USE_THREAD_SANITIZER  "Use compiler thread  sanitizer" OFF)
    OPTION (USE_UB_SANITIZER      "Use compiler  undefined behavioud sanitizer" OFF)
    # Avoid new ABI (https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html)
    OPTION (USE_CXX11_ABI         "Use new GCC 5.1 ABI" OFF)

    IF (USE_GOLD)
        # I don't know do we really need it but I know that Clang blames for this since version 3.8.0
        IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            ADD_DEFINITIONS (-fuse-ld=gold)
        ENDIF()
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
        MESSAGE (STATUS "gold is enabled")
    ELSE()
        MESSAGE (STATUS "gold is disabled")
    ENDIF()

    IF (USE_LTO)
        INCLUDE (ProcessorCount)
        PROCESSORCOUNT (NPROCS)

        ADD_DEFINITIONS (-flto)
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto=${NPROCS}")

        IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            ADD_DEFINITIONS (-fno-fat-lto-objects)
            ADD_DEFINITIONS (-fuse-linker-plugin)
            SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-linker-plugin -fno-fat-lto-objects")

            FIND_PACKAGE (LTOBinUtils ${CMAKE_CXX_COMPILER_VERSION} REQUIRED)

            SET (CMAKE_AR       "${CUSTOM_CMAKE_AR}")
            SET (CMAKE_RANLIB   "${CUSTOM_CMAKE_RANLIB}")
            SET (CMAKE_NM       "${CUSTOM_CMAKE_NM}")
        ENDIF ()

        SET (CMAKE_EXE_LINKER_FLAGS_RELEASE         "${CMAKE_EXE_LINKER_FLAGS} -O2")
        SET (CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_EXE_LINKER_FLAGS} -O2")

        MESSAGE (STATUS "LTO is enabled")
    ENDIF()

    IF (USE_ADDRESS_SANITIZER)
        ADD_DEFINITIONS (-fsanitize=address)
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libasan")
        ENDIF ()
    ENDIF()

    IF (USE_THREAD_SANITIZER)
        ADD_DEFINITIONS (-fsanitize=thread)
        ADD_DEFINITIONS (-fpie)
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -pie")
        IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libtsan")
        ENDIF ()
    ENDIF()

    IF (USE_UB_SANITIZER)
        ADD_DEFINITIONS (-fsanitize=undefined)
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
        IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libubsan")
        ENDIF ()
    ENDIF()

    IF (USE_CXX11_ABI)
        ADD_DEFINITIONS (-D_GLIBCXX_USE_CXX11_ABI=1)
    ELSE()
        ADD_DEFINITIONS (-D_GLIBCXX_USE_CXX11_ABI=0)
    ENDIF()

    GET_DIRECTORY_PROPERTY(COMPILE_DEFINITIONS_VAR DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
ENDIF ()
