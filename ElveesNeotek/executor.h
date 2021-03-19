#pragma once

#include <deque>
#include <mutex>
#include <future>
#include <filesystem>
#include <fstream>

#include <boost/lockfree/queue.hpp>

class Executor
{
public:
    Executor(std::filesystem::path inputDir, 
             const std::string&    outFile, 
             uint8_t               maxCountThread = 0);

    int run();

private:
    std::filesystem::path                    _inputDir;
    std::ofstream                            _out;
    std::mutex                               _mutexOut;
    uint8_t                                  _maxCountThread;
    std::atomic<bool>                        _stopped{false};
    std::deque<std::string>                  _data;
    boost::lockfree::queue<std::string_view> _queue{32};

    std::future<void> startWorker();
};
