# CMake generated Testfile for 
# Source directory: /Users/akhil/files/projects/prototype-trading-platform/tests
# Build directory: /Users/akhil/files/projects/prototype-trading-platform/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[SimpleTest]=] "/opt/homebrew/bin/cmake" "-E" "echo" "CTest is working")
set_tests_properties([=[SimpleTest]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/akhil/files/projects/prototype-trading-platform/tests/CMakeLists.txt;8;add_test;/Users/akhil/files/projects/prototype-trading-platform/tests/CMakeLists.txt;0;")
subdirs("libraries")
