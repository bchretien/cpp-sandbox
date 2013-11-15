# Test 1: using Boost Fusion with Eigen maps to C-style arrays
ADD_EXECUTABLE(boost_fusion_eigen "${CMAKE_CURRENT_SOURCE_DIR}/boost_fusion_eigen.cpp")
PKG_CONFIG_USE_DEPENDENCY(boost_fusion_eigen eigen3)

# Test 2: using Boost Fusion with Eigen maps to STL vectors holding C-style arrays
ADD_EXECUTABLE(boost_fusion_eigen_vector "${CMAKE_CURRENT_SOURCE_DIR}/boost_fusion_eigen_vector.cpp")
PKG_CONFIG_USE_DEPENDENCY(boost_fusion_eigen_vector eigen3)

