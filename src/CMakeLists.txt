# Test 1: using Boost Fusion with Eigen maps to C-style arrays
ADD_EXECUTABLE(boost_fusion_eigen "${CMAKE_CURRENT_SOURCE_DIR}/boost_fusion_eigen.cpp")
PKG_CONFIG_USE_DEPENDENCY(boost_fusion_eigen eigen3)

# Test 2: using Boost Fusion with Eigen maps to STL vectors holding C-style arrays
ADD_EXECUTABLE(boost_fusion_eigen_vector "${CMAKE_CURRENT_SOURCE_DIR}/boost_fusion_eigen_vector.cpp")
PKG_CONFIG_USE_DEPENDENCY(boost_fusion_eigen_vector eigen3)

# Test 3: using Boost preprocessor to access variables
ADD_EXECUTABLE(boost_preprocessor_names "${CMAKE_CURRENT_SOURCE_DIR}/boost_preprocessor_names.cpp")

# Test 4: Boost shared memory array (interprocess)
ADD_EXECUTABLE(boost_shared_memory_array "${CMAKE_CURRENT_SOURCE_DIR}/boost_shared_memory_array.cpp")
TARGET_LINK_LIBRARIES(boost_shared_memory_array pthread rt)

# Test 5: Boost shared memory vector (interprocess)
ADD_EXECUTABLE(boost_shared_memory_vector "${CMAKE_CURRENT_SOURCE_DIR}/boost_shared_memory_vector.cpp")
TARGET_LINK_LIBRARIES(boost_shared_memory_vector pthread rt)

# Test 6: Boost graph
ADD_EXECUTABLE(boost_graph "${CMAKE_CURRENT_SOURCE_DIR}/boost_graph.cpp")
ADD_CUSTOM_TARGET(
    boost_graph_run
    COMMAND ./bin/boost_graph && dot -Tpng graph.dot > graph.png && feh graph.png
    DEPENDS boost_graph
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Test 7: gcc ICE
ADD_EXECUTABLE(gcc_ice "${CMAKE_CURRENT_SOURCE_DIR}/gcc_ice.cpp")

# Test 8: Eigen QR decomposition
ADD_EXECUTABLE(eigen_qr "${CMAKE_CURRENT_SOURCE_DIR}/eigen_qr.cpp")
PKG_CONFIG_USE_DEPENDENCY(eigen_qr eigen3)

# Test 9: using Boost preprocessor to generate multiple functions
ADD_EXECUTABLE(boost_preprocessor_functions "${CMAKE_CURRENT_SOURCE_DIR}/boost_preprocessor_functions.cpp")

# Test 10: test Boost logger
ADD_EXECUTABLE(boost_logger "${CMAKE_CURRENT_SOURCE_DIR}/boost_logger.cpp")
SET_TARGET_PROPERTIES(boost_logger PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK)
TARGET_LINK_LIBRARIES(boost_logger boost_log boost_log_setup boost_system
                                   boost_filesystem boost_date_time
                                   boost_thread pthread)
ADD_CUSTOM_TARGET(
    boost_logger_run
    COMMAND ./bin/boost_logger && cat test.log
    DEPENDS boost_logger
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Test 11: bug in Eigen's polynomial solver
ADD_EXECUTABLE(eigen_poly_bug "${CMAKE_CURRENT_SOURCE_DIR}/eigen_poly_bug.cpp")
PKG_CONFIG_USE_DEPENDENCY(eigen_poly_bug eigen3)

# Test 12: parameter testing (mixin/policy-based)
ADD_EXECUTABLE(parameters "${CMAKE_CURRENT_SOURCE_DIR}/parameters.cpp")

# Test 13: multidimension array pointer/reference
ADD_EXECUTABLE(multimensional_array "${CMAKE_CURRENT_SOURCE_DIR}/multidimensional_array.cpp")

# Test 14: boost::mpl::vector asserts
ADD_EXECUTABLE(boost_mpl_vector_assert "${CMAKE_CURRENT_SOURCE_DIR}/boost_mpl_vector_assert.cpp")

# Test 15: boost metaprog sandbox
ADD_EXECUTABLE(boost_metaprog "${CMAKE_CURRENT_SOURCE_DIR}/boost_metaprog.cpp")

# Test 16: use functions as template parameters
ADD_EXECUTABLE(template_parameter_function "${CMAKE_CURRENT_SOURCE_DIR}/template_parameter_function.cpp")
PKG_CONFIG_USE_DEPENDENCY(template_parameter_function eigen3)

# Test 17: using Boost preprocessor to generate multiple structures
ADD_EXECUTABLE(boost_preprocessor_structure "${CMAKE_CURRENT_SOURCE_DIR}/boost_preprocessor_structure.cpp")
