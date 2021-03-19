#include "executor.h"
#include "parser.h"

#include <iostream>
#include <vector>

Executor::Executor(std::filesystem::path inputDir, 
                   const std::string&    outFile, 
                   uint8_t               maxCountThread):
      _inputDir      ( std::move(inputDir) ),
      _out           ( outFile ),
      _maxCountThread( maxCountThread ? maxCountThread : std::max(std::thread::hardware_concurrency(), 1U) )
{

}

int Executor::run()
{
    if ( !_out.is_open() )
    {
        std::cerr << "Cannot open the output file" << std::endl;
        return EXIT_FAILURE;
    }

    auto future = std::async(std::launch::async, [this](){
        for ( const auto& path: std::filesystem::recursive_directory_iterator(_inputDir) )
        {
            _data.push_back( path.path().string() );
            while ( !_queue.push(_data.back()) );
        }

        _stopped = true;
    });

    std::vector<std::future<void>> futures;

    for (uint8_t i = 0; i < _maxCountThread; ++i)
        futures.push_back( startWorker() );

    future.wait();

    try
    {
        future.get();
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    for (auto& value : futures)
    {
        value.wait();

        try
        {
            value.get();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

std::future<void> Executor::startWorker()
{
    auto future = std::async(std::launch::async | std::launch::deferred, [this](){
        while (!_queue.empty() || !_stopped)
        {
            std::filesystem::path filename;
            while ( _queue.pop(filename) )
            {
                if ( !std::filesystem::exists(filename) )
                {
                    std::cerr << filename.string() << " does not exist" << std::endl;
                    continue;
                }

                Parser parser(filename);
                {
                    std::lock_guard lk(_mutexOut);
                    _out << parser.parse();
                }
            }
        }
    });

    return future;
}
