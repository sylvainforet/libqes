## Compile tests, & set up coverage
SET(COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/helpers.c)
FILE(GLOB TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test*.c)
SET(TEST_SRCS ${COMMON_SRCS} ${TEST_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/tinytest/tinytest.c)
ADD_EXECUTABLE(test_libqes ${TEST_SRCS})
TARGET_LINK_LIBRARIES(test_libqes qes ${LIBQES_DEPENDS_LIBS})
IF (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	SETUP_TARGET_FOR_COVERAGE(coverage test_libqes coverage src)
ENDIF()
ADD_TEST(NAME libqes COMMAND ${CMAKE_BINARY_DIR}/bin/test_libqes ${CMAKE_BINARY_DIR})

# Benchmarking:
ADD_EXECUTABLE(bench_libqes ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks.c ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(bench_libqes qes ${LIBQES_DEPENDS_LIBS})
ADD_TEST(NAME libqes_benchmarks
         COMMAND ${CMAKE_BINARY_DIR}/bin/bench_libqes
	  ${CMAKE_BINARY_DIR}/data/test.fastq
	  50
	  qes_seqfile_write
	  kseq_parse_fq
	  gnu_getline
	  qes_seqfile_parse_fq
	  qes_file_readline_realloc)

# Copy test files over to bin dir
ADD_CUSTOM_COMMAND(TARGET test_libqes
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_CURRENT_SOURCE_DIR}/data
		    ${CMAKE_BINARY_DIR}/data)
