#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/timer.h

#include <chrono>

// 構築時からの実経過時間 (待機時間も含む) を計測する。
// steady_clock を使い、システム時刻の変更による影響を避ける。
// 使用例:
//   timer.reset();  // main() 開始時点から測る場合。宣言は不要。
//   // 計測対象の処理
//   double seconds = timer.get_time();
//   timer.reset();
//   while (!timer.is_over(1.9)) {
//       // 制限時間まで探索する
//   }
struct Timer {
    // 現在時刻を起点に計測を開始する。O(1)
    Timer() : start(Clock::now()) {}

    // 構築時または最後の reset() からの経過秒数。O(1)
    double get_time() const {
        return std::chrono::duration<double>(Clock::now() - start).count();
    }

    // 構築時または最後の reset() からの経過ミリ秒数。O(1)
    double get_time_ms() const {
        return get_time() * 1000.0;
    }

    // 現在時刻を起点に計測し直す。O(1)
    void reset() {
        start = Clock::now();
    }

    // 指定した秒数以上が経過したか。O(1)
    bool is_over(double seconds) const {
        return get_time() >= seconds;
    }

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point start;
};

// グローバル変数の初期化時点から計測する共通タイマー。
// main() 開始時点から測る場合は、冒頭で timer.reset() を呼ぶ。
// C++17 の inline 変数により、複数の翻訳単位から同じインスタンスを使える。
inline Timer timer;
