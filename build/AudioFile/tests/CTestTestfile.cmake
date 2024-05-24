# CMake generated Testfile for 
# Source directory: C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests
# Build directory: C:/Users/Patryk/Desktop/PROJEKTX/build/AudioFile/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(Tests "C:/Users/Patryk/Desktop/PROJEKTX/build/AudioFile/tests/Debug/Tests.exe")
  set_tests_properties(Tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;11;add_test;C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(Tests "C:/Users/Patryk/Desktop/PROJEKTX/build/AudioFile/tests/Release/Tests.exe")
  set_tests_properties(Tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;11;add_test;C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(Tests "C:/Users/Patryk/Desktop/PROJEKTX/build/AudioFile/tests/MinSizeRel/Tests.exe")
  set_tests_properties(Tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;11;add_test;C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(Tests "C:/Users/Patryk/Desktop/PROJEKTX/build/AudioFile/tests/RelWithDebInfo/Tests.exe")
  set_tests_properties(Tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;11;add_test;C:/Users/Patryk/Desktop/PROJEKTX/AudioFile/tests/CMakeLists.txt;0;")
else()
  add_test(Tests NOT_AVAILABLE)
endif()
