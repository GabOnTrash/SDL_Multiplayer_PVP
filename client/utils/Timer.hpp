#include <chrono>
#include <functional>
#include <iostream>
#include <tuple>

class Timer
{
  public:
    Timer(int64_t durationMS, std::function<void()> onTimeout = nullptr) 
        : duration(static_cast<int64_t>(durationMS * 1000)), onTimeout(onTimeout)
    {
    }
    operator bool() const
    {
        return isRunning;
    }
    operator int64_t() const
    {
        return ElapsedMs();
    }
    friend std::ostream& operator << (std::ostream& os, const Timer& timer)
    {
        return os << timer.ElapsedMs();
    }

    void Start()
    {
        isRunning = true;
        initialTime = Clock::now();
    }
    void Stop()
    {
        isRunning = false;
    }
    double ElapsedMs() const
    {
        if (isRunning)
            return std::chrono::duration_cast<Microseconds>(Clock::now() - initialTime).count() / 1000.0;

        return 0;
    }
    void SetCallback(std::function<void()> callback)
    {
        onTimeout = std::move(callback);
    }
    void Update()
    {
        if (!isRunning)
            return;

        if (Clock::now() - initialTime >= duration)
        {
            isRunning = false;
            if (onTimeout)
                onTimeout();
        }
    }

  private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Microseconds = std::chrono::microseconds;

    Microseconds duration;
    TimePoint initialTime;

    std::function<void()> onTimeout;
    bool isRunning = false;
};