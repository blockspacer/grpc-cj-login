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
cpp_out="$base_dir/djinni-src/cpp"
objc_out="$base_dir/djinni-src/objc"

# clean generated src dirs
rm -rf $cpp_out
rm -rf $objc_out

# execute the djinni command
deps/djinni/src/run \
    --cpp-out $cpp_out \
    --cpp-namespace $namespace \
    --objc-out $objc_out \
    --objc-type-prefix $objc_prefix \
    --objcpp-out $objc_out \
    --idl $djinni_file
