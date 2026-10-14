cmake_minimum_required(VERSION 3.16)

project(cpp-playground CXX)                          # ← 改①:项目名

set(CMAKE_CXX_STANDARD 17)                  # ← 改②:C++ 标准(17 / 20 / 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)               # 用 -std=c++17,而不是 -std=gnu++17

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)       # 生成 compile_commands.json(编辑器靠它消除波浪线)

add_library(mymath STATIC 
    src/myadd.cpp
    src/myswap.cpp
)        # ← 改③:目标名 + 源文件清单

target_include_directories(mymath PUBLIC include)


add_executable(myapp                        # ← 改③:目标名 + 源文件清单
    src/main.cpp

)
target_link_libraries(myapp PRIVATE mymath)



add_library(project_warnings INTERFACE)

if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
else()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()


target_link_libraries(mymath PRIVATE project_warnings)
target_link_libraries(myapp  PRIVATE project_warnings)


# ── 测试 ─────────────────────────────────────────────────────────
# enable_testing() 让 CMake 在 build/ 里生成 CTest 需要的元数据文件。
# 没有它，ctest 一条测试都找不到。
enable_testing()

add_executable(test_mymath tests/test_mymath.cpp)
target_link_libraries(test_mymath PRIVATE mymath project_warnings)

# 登记一条测试：名字叫 test_mymath，内容就是【运行 test_mymath 这个程序】。
# CTest 不关心你断言了什么，它只看退出码：0 = 通过，非 0 = 失败。
add_test(NAME test_mymath COMMAND test_mymath)