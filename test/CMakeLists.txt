SET(BLOOM_FILTER_TEST_SRCS
  string_bloom.cpp
  )

SET(HAT_TRIE_TEST_SRCS
  hat_set_test.cpp array_hash_test.cpp
  )



ADD_EXECUTABLE(bloom_filter_example01 ${BLOOM_FILTER_TEST_SRCS})
TARGET_LINK_LIBRARIES(bloom_filter_example01 boost_serialization)

ADD_EXECUTABLE(hat_set_test ${HAT_TRIE_TEST_SRCS} )
TARGET_LINK_LIBRARIES(hat_set_test boost_unit_test_framework)
