FIND_LIBRARY (OMP_LIBS gomp
    HINTS
        /usr/local/gcc-${GCC_VERSION}/lib64/
        /usr/lib64/gcc/x86_64-pc-linux-gnu/${GCC_VERSION}/
        /usr/lib/gcc/x86_64-linux-gnu/${GCC_VERSION}/
    PATHS
        /usr/local/gcc-*/lib64/
        /usr/lib64/gcc/x86_64-pc-linux-gnu/*/
        /usr/lib/gcc/x86_64-linux-gnu/*/
)

INCLUDE (FindPackageHandleStandardArgs)

IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    FIND_PATH (IOMP omp.h
        HINTS
            /usr/local/gcc-${GCC_VERSION}/
            /usr/local/gcc-${GCC_VERSION}/lib/gcc/x86_64-unknown-linux-gnu/${GCC_VERSION}/
            /usr/lib64/gcc/x86_64-pc-linux-gnu/${GCC_VERSION}/
            /usr/lib/gcc/x86_64-linux-gnu/${GCC_VERSION}/include/
        PATHS
            /usr/local/gcc-*/lib64/
            /usr/local/gcc-*/lib/gcc/x86_64-unknown-linux-gnu/*/
            /usr/lib64/gcc/x86_64-pc-linux-gnu/*/
            /usr/lib/gcc/x86_64-linux-gnu/*/
        PATH_SUFFIXES
            include
    )

    FIND_PACKAGE_HANDLE_STANDARD_ARGS (OMP DEFAULT_MSG OMP_LIBS IOMP)
    ADD_DEFINITIONS (-idirafter "${IOMP}")
ELSE()
    FIND_PACKAGE_HANDLE_STANDARD_ARGS (OMP DEFAULT_MSG OMP_LIBS)
ENDIF()
