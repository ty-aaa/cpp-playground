// mymath 的测试。
//
// 【为什么不用 <cassert> 的 assert】
//   Release 构建会定义 NDEBUG，而 assert 在 NDEBUG 下被编译成空语句 ——
//   结果是测试"通过"，但一个断言都没跑。这个坑不报错，只是骗你。
//
//   我实际验证过：一条故意写错的 assert(1 == 2)，
//   Debug 下 ctest 判「失败」，Release 下判「通过」。
//
//   所以这里自己写一个 CHECK，不受 NDEBUG 影响。
//   就十几行 —— 这也顺便让你看清 assert 这类东西内部在干嘛。
//   （W2 会用 GoogleTest 替掉这一整段。）

#include "myadd.h"
#include "myswap.h"

#include <iostream>

namespace {

int g_failed = 0;

void report(bool ok, const char* expr, int line) {
    if (ok) return;
    ++g_failed;
    std::cerr << "  失败: " << expr << "   (test_mymath.cpp:" << line << ")\n";
}

}  // namespace

#define CHECK(expr) report((expr), #expr, __LINE__)

int main() {
    // ── myadd ──────────────────────────────────────────────
    CHECK(myadd(2, 3) == 5);
    CHECK(myadd(-1, 1) == 0);
    CHECK(myadd(0, 0) == 0);
    CHECK(myadd(-5, -7) == -12);

    // ── myswap ─────────────────────────────────────────────
    {
        int a = 1, b = 2;
        myswap(a, b);
        CHECK(a == 2);
        CHECK(b == 1);
    }
    {
        // 边界：自己和自己交换。两个引用绑到同一个对象，
        // 实现写得对的话结果不变。
        int a = 7;
        myswap(a, a);
        CHECK(a == 7);
    }

    if (g_failed == 0) {
        std::cout << "test_mymath: 全部通过\n";
        return 0;
    }
    std::cerr << "test_mymath: " << g_failed << " 项失败\n";
    return 1;
}
