#include <formats/read_file_format.hh>
#include <solver.hh>

#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

#include <unistd.h>

using std::atomic;
using std::boolalpha;
using std::cerr;
using std::condition_variable;
using std::cout;
using std::cv_status;
using std::endl;
using std::exception;
using std::function;
using std::localtime;
using std::make_pair;
using std::mutex;
using std::put_time;
using std::string;
using std::thread;
using std::unique_lock;


using std::chrono::seconds;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

/* Helper: return a function that runs the specified algorithm, dealing
 * with timing information and timeouts. */
template <typename Result_, typename Params_, typename Data_>
auto run_this_wrapped(const function<Result_ (const Data_ &, const Params_ &)> & func)
    -> function<Result_ (const Data_ &, Params_ &, bool &, int)>
{
    return [func] (const Data_ & data, Params_ & params, bool & aborted, int timeout) -> Result_ {
        /* For a timeout, we use a thread and a timed CV. We also wake the
         * CV up if we're done, so the timeout thread can terminate. */
        thread timeout_thread;
        mutex timeout_mutex;
        condition_variable timeout_cv;
        atomic<bool> abort;
        abort.store(false);
        params.abort = &abort;
        if (0 != timeout) {
            timeout_thread = thread([&] {
                    auto abort_time = steady_clock::now() + seconds(timeout);
                    {
                        /* Sleep until either we've reached the time limit,
                         * or we've finished all the work. */
                        unique_lock<mutex> guard(timeout_mutex);
                        while (! abort.load()) {
                            if (cv_status::timeout == timeout_cv.wait_until(guard, abort_time)) {
                                /* We've woken up, and it's due to a timeout. */
                                aborted = true;
                                break;
                            }
                        }
                    }
                    abort.store(true);
                    });
        }

        /* Start the clock */
        params.start_time = steady_clock::now();
        auto result = func(data, params);

        /* Clean up the timeout thread */
        if (timeout_thread.joinable()) {
            {
                unique_lock<mutex> guard(timeout_mutex);
                abort.store(true);
                timeout_cv.notify_all();
            }
            timeout_thread.join();
        }

        return result;
    };
}

/* Helper: return a function that runs the specified algorithm, dealing
 * with timing information and timeouts. */
template <typename Result_, typename Params_, typename Data_>
auto run_this(Result_ func(const Data_ &, const Params_ &)) -> function<Result_ (const Data_ &, Params_ &, bool &, int)>
{
    return run_this_wrapped(function<Result_ (const Data_ &, const Params_ &)>(func));
}

auto main(int argc, char * argv[]) -> int
{
    try {
        char const * pattern_filename = argv[1];
        char const * target_filename = argv[2];

        auto algorithm = sequential_subgraph_isomorphism;

        Params params;
        params.induced = true;
        params.enumerate = true;
        params.presolve = false; // could try true

        auto graphs = make_pair(
            read_file_format("gal", pattern_filename),
            read_file_format("gal", target_filename));


        std::chrono::time_point<std::chrono::steady_clock> start, end;
        std::chrono::milliseconds elapsed;

        start = std::chrono::steady_clock::now();

        bool aborted = false;
        auto result = run_this(algorithm)(
                graphs,
                params,
                aborted,
                0);

        end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << result.solution_count << "," << elapsed.count() << std::endl;

        return EXIT_SUCCESS;
    }
    catch (const exception & e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}

