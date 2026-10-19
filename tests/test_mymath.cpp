// mymath（myadd / myswap）的单元测试。
//
// 之前用自己写的 CHECK 宏 + 手写 main，现在换成 GoogleTest：
//   - 不用写 main（GTest::gtest_main 提供）
//   - 不用自己数失败个数、拼输出
//   - 失败时自动打印【两边的实际值】，不是只告诉你"表达式是假的"
//
// 但省掉的只是脚手架 —— 【用例本身】还是得自己想。
// 谁决定 2+3 应该等于 5？是你。GTest 只负责检查你说了算不算数。

#include "myadd.h"
#include "myswap.h"

#include <gtest/gtest.h>

// ── myadd ──────────────────────────────────────────────────────

TEST(MyAdd, PositiveNumbers) {
    EXPECT_EQ(myadd(2, 3), 5);
    EXPECT_EQ(myadd(100, 200), 300);
}

TEST(MyAdd, NegativeNumbers) {
    EXPECT_EQ(myadd(-5, -7), -12);
    EXPECT_EQ(myadd(-1, -1), -2);
}

// 混合符号是最容易暴露 bug 的一类：实现里把 + 写成 -、
// 或者符号处理写反，这里立刻就会红。
TEST(MyAdd, MixedSigns) {
    EXPECT_EQ(myadd(-3, 5), 2);
    EXPECT_EQ(myadd(5, -3), 2);
    EXPECT_EQ(myadd(3, -5), -2);
}

// ⚠️ 注意：光有下面这几条【区分不出加法还是减法】——
// 0+0 和 0-0 都是 0，7+0 和 7-0 都是 7。
// 如果整个文件只剩这个用例，把 myadd 的 + 改成 - 也照样全绿。
// （我们实际验证过：把 a+b 改成 a-b，4 条用例里只有这条不挂。）
TEST(MyAdd, ZeroIsIdentity) {
    EXPECT_EQ(myadd(0, 0), 0);
    EXPECT_EQ(myadd(7, 0), 7);
    EXPECT_EQ(myadd(0, -7), -7);
}

// 关于溢出：myadd(INT_MAX, 1) 是【有符号溢出】，属于未定义行为。
// 这里故意不写这条用例 —— 因为 UB 没法断言"应该得到什么"。
// 但它是个真实的设计问题：这个函数要不要处理溢出？等你写库的时候再决定。

// ── myswap ─────────────────────────────────────────────────────

TEST(MySwap, SwapsTwoDistinctValues) {
    int a = 1, b = 2;
    myswap(a, b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);
}

TEST(MySwap, WorksWithNegativeAndZero) {
    int a = -10, b = 0;
    myswap(a, b);
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, -10);
}

// 边界：自己和自己交换。两个引用绑到同一个对象，
// 实现写对的话结果不变（写成 a = b; b = temp; 也对）。
TEST(MySwap, SelfSwapIsNoOp) {
    int a = 7;
    myswap(a, a);
    EXPECT_EQ(a, 7);
}
