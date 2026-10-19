// W2：手写 unique_ptr / shared_ptr 的测试。
//
// 现在只有冒烟测试 —— 确认这条链路是通的：
//   find_package(GTest) → 链接 GTest::gtest_main → 编译 → CTest 登记 → ctest 跑起来
// 链路通了再往里填真正的内容。
//
// 注意这个文件【没有 main()】—— GTest::gtest_main 已经提供了一个，
// 它会自动把下面所有 TEST 找出来挨个跑，全部通过才返回 0。

#include <gtest/gtest.h>

// TEST(套件名, 用例名)
// 同一个套件的用例在输出里会分到一组，哪个模块坏了看得清清楚楚。
TEST(Smoke, GTestIsWiredUp) {
    EXPECT_EQ(1 + 1, 2);
}

// ── 接下来要写的东西（W2 剩余部分）─────────────────────────
//
// unique_ptr：
//   - 独占所有权，移动后源指针变空
//   - 不能拷贝 —— 这是【编译期】性质，GTest 测不了，要用 static_assert：
//         static_assert(!std::is_copy_constructible_v<MyUniquePtr<int>>);
//   - 自定义删除器
//
// shared_ptr：
//   - 控制块里有【强、弱两个计数】
//   - 循环引用会泄漏 → 用 weak_ptr 破
//
// 记忆点：EXPECT_EQ 失败时会打印两边的实际值（"Which is: 4"），
// 比只告诉你"表达式是假的"有用得多。
