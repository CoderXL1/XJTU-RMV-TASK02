# cmake_minimum_required(VERSION 3.22)
# project(XJTU-RMV-TASK02 VERSION 1.0 LANGUAGES CXX)
# add_executable(main main.cpp)


cmake_minimum_required(VERSION 3.16)
project(XJTU-RMV-TASK02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 找到 OpenCV
find_package(OpenCV REQUIRED)

# 2. 声明可执行文件
add_executable(main src/recognition.cpp)

# 3. 给“这个目标”加 include 路径 + 链接库 + 额外编译 flags
target_link_libraries(main PRIVATE ${OpenCV_LIBS})
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})

# 4. 如需手动再追加额外 flags，用 target_compile_options
target_compile_options(main PRIVATE
    # 警告/优化例子
    -Wall -Wextra -O3
    # 如果 pkg-config 给出了 -DXXX，也可以放这儿
    -DUSE_OPENCV4
)

# 5. 若 OpenCV 装在非系统目录，提前给 CMake 提示
# list(APPEND CMAKE_PREFIX_PATH "/usr/local")