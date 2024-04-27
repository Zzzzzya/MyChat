# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building IM.Login.

cmake_minimum_required(VERSION 3.20)

project(DataServer C CXX)

# 找到spdlog库 openssl库
find_package(spdlog REQUIRED)
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(../cmake/common.cmake)

# -------------------------------------Data Proto file------------------------------------- #
# Data Proto file
get_filename_component(mc_proto_data "../protos/Data/MC.Data.proto" ABSOLUTE)
get_filename_component(mc_proto_data_path "${mc_proto_data}" PATH)

# Generated sources for MC.Data.proto
set(mc_proto_data_srcs "${CMAKE_CURRENT_BINARY_DIR}/MC.Data.pb.cc")
set(mc_proto_data_hdrs "${CMAKE_CURRENT_BINARY_DIR}/MC.Data.pb.h")
set(mc_grpc_data_srcs "${CMAKE_CURRENT_BINARY_DIR}/MC.Data.grpc.pb.cc")
set(mc_grpc_data_hdrs "${CMAKE_CURRENT_BINARY_DIR}/MC.Data.grpc.pb.h")

# Custom command for MC.Data.proto
add_custom_command(
    OUTPUT "${mc_proto_data_srcs}" "${mc_proto_data_hdrs}" "${mc_grpc_data_srcs}" "${mc_grpc_data_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${mc_proto_data_path}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${mc_proto_data}"
    DEPENDS "${mc_proto_data}"
)

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${PROJECT_SOURCE_DIR}/include)

# mc_grpc_proto
add_library(mc_grpc_proto
    ${mc_grpc_data_srcs}
    ${mc_grpc_data_hdrs}
    ${mc_proto_data_srcs}
    ${mc_proto_data_hdrs}

    # ${mc_token_grpc_srcs}
    # ${mc_token_grpc_hdrs}
    # ${mc_token_proto_srcs}
    # ${mc_token_proto_hdrs}
)
target_link_libraries(mc_grpc_proto
    absl::check
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# Targets greeter_[async_](client|server)
foreach(_target DataServerMain)
    add_executable(${_target} "main/${_target}.cpp"
        "src/DataServer.cc"
        "src/log.cc"
        "src/mysqlpool.cc")
    target_link_libraries(${_target}
        mc_grpc_proto
        absl::check
        absl::flags
        absl::flags_parse
        absl::log
        spdlog::spdlog
        OpenSSL::SSL
        OpenSSL::Crypto
        mysqlcppconn
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
endforeach()
