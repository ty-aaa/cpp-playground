cmake_minimum_required(VERSION 3.16)

project(cpp-playground CXX)                          # ← 改①:项目名

set(CMAKE_CXX_STANDARD 17)                  # ← 改②:C++ 标准(17 / 20 / 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)               # 用 -std=c++17,而不是 -std=gnu++17

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)       # 生成 compile_commands.json(编辑器靠它消除波浪线)

# ── 第三方依赖 ───────────────────────────────────────────────────
# 必须放在【用到它的 target 之前】—— find_package 只做「查找并导入」，
# 不会自动对后面的 target 生效，所以要用 GTest 的地方得自己链接 GTest::xxx。
#
# 本机用 apt 装的 libgtest-dev 提供 GTestConfig.cmake，所以这里能直接找到。
# ⚠️ 但 GitHub 的 CI 机器上【没有】预装，CI 里要多一步 apt-get install。
find_package(GTest REQUIRED)

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
# 用 GoogleTest 之后，测试文件里不再需要自己写 main() ——
# GTest::gtest_main 已经提供了一个，会自动找出所有 TEST 跑一遍。
target_link_libraries(test_mymath PRIVATE mymath GTest::gtest_main project_warnings)

# 登记一条测试：名字叫 test_mymath，内容就是【运行 test_mymath 这个程序】。
# CTest 不关心你断言了什么，它只看退出码：0 = 通过，非 0 = 失败。
add_test(NAME test_mymath COMMAND test_mymath)


# ── 手写智能指针（W2）───────────────────────────────────────────
# 智能指针是【模板】→ 纯头文件，没有需要编译的 .cpp。
# INTERFACE 库正好表达这件事：「我没有源文件，只有一组使用要求」。
# 谁链接 smartptr，谁就自动获得 include/ 路径和警告选项。
add_library(smartptr INTERFACE)
target_include_directories(smartptr INTERFACE include)
target_link_libraries(smartptr INTERFACE project_warnings)

add_executable(test_smartptr tests/test_smartptr.cpp)
# GTest::gtest_main 不带测试框架本身，还【顺带提供了一个 main()】——
# 所以这个测试文件里不用自己写 main。
target_link_libraries(test_smartptr PRIVATE smartptr GTest::gtest_main)

add_test(NAME test_smartptr COMMAND test_smartptr)