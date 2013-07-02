MESSAGE(STATUS "Using bundled Find Protobuf ...")

FIND_PATH(
  LIBPROTOBUF_INCLUDE_DIR
  google/protobuf/service.h
  /usr/include
  /usr/local/include/
  )

FIND_LIBRARY(
  LIBPROTOBUF_LIBRARY NAMES  protobuf
  PATHS /usr/lib/ /usr/local/lib/
  )

IF (LIBPROTOBUF_INCLUDE_DIR AND LIBPROTOBUF_LIBRARY)
  SET(PROTOBUF_FOUND 1)
  INCLUDE_DIRECTORIES(${LIBPROTOBUF_INCLUDE_DIR})
ELSE (LIBPROTOBUF_INCLUDE_DIR AND LIBPROTOBUF_LIBRARY)
  SET(PROTOBUF_FOUND 0)
ENDIF (LIBPROTOBUF_INCLUDE_DIR AND LIBPROTOBUF_LIBRARY)

IF(LIBPROTOBUF_FOUND)
INCLUDE_DIRECTORIES(${LIBPROTOBUF_INCLUDE_DIR})
ENDIF(LIBPROTOBUF_FOUND)

MARK_AS_ADVANCED(
  LIBPROTOBUF_INCLUDE_DIR
  LIBPROTOBUF_LIBRARY
  )
