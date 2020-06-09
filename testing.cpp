#include "Engine.h"

#include <thread>
#include <limits>

int main() {
    CsvLogger logger("thread_ids.csv");

    auto logthread = [&]() -> void {
        logger.log(fmt::format("{}", std::this_thread::get_id()));
    };

    std::vector<std::thread> threads;
    for (;;) {
        report("cycle!");
        report("creating");
        for (int i = 0; i < 20; ++i)
            threads.push_back(std::thread(logthread));
        report("joining");
        for (auto& thread : threads) {
            if (thread.joinable())
                thread.join();
        }
    }
}
