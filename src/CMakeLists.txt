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

