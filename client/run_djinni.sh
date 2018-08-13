#! /usr/bin/env bash

### Configuration

# Djinni IDL file location
djinni_file="cj-login.djinni"

# C++ namespace for generated src
namespace="cjlogin"

# Objective-C class name prefix for generated src
objc_prefix="CJ"

### Script

# get base directory
base_dir=$(cd "`dirname "0"`" && pwd)

# output directories for generated src
cpp_out="$base_dir/djinni-src/"
objc_out="$base_dir/djinni-src/"

# execute the djinni command
deps/djinni/src/run \
    --cpp-out $cpp_out \
    --cpp-namespace $namespace \
    --objc-out $objc_out \
    --objc-type-prefix $objc_prefix \
    --objcpp-out $objc_out \
    --idl $djinni_file
