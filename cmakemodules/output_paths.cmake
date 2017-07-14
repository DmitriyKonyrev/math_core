SET (INSTALL_DIR ${CMAKE_BINARY_DIR})
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR}/lib)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR})

SET (EXAMPLES_OUTPUT_DIR ${INSTALL_DIR}/examples)
SET (MANUAL_TESTS_OUTPUT_DIR ${INSTALL_DIR}/manual_tests)

SET (AUTO_DIR ${PROJECT_BINARY_DIR}/auto)
SET (AUTO_INCLUDE_DIR ${AUTO_DIR}/include)
SET (AUTO_SRC_DIR ${AUTO_DIR}/src)

SET (AUTO_EXP_DIR ${CMAKE_BINARY_DIR}/auto)
SET (AUTO_EXP_CONF_DIR ${AUTO_EXP_DIR}/include)

INCLUDE_DIRECTORIES (BEFORE ${AUTO_INCLUDE_DIR})
INCLUDE_DIRECTORIES (BEFORE ${AUTO_EXP_CONF_DIR})
