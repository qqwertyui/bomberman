include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

file(GLOB MESSAGES_PROTO "*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HEADERS ${MESSAGES_PROTO})

add_library(messages ${PROTO_SRCS})
target_link_libraries(messages ${PROTOBUF_LIBRARIES})