//
// Created by Rafael Campos on 4/9/16.
//

#ifndef NIKITA_RENDER_CPUTIMER_H
#define NIKITA_RENDER_CPUTIMER_H

#include <tbb/tick_count.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace nikita {
class CpuTimer
{
public:
    CpuTimer() : startTime(tbb::tick_count::now()) {}

    CpuTimer(const std::string &message)
    {
        this->start(message);
    }

    inline void start() { startTime = tbb::tick_count::now(); }

    inline void start(const std::string &message)
    {
        std::cout << message << "....";
        this->start();
    }

    inline void restart(const std::string& message)
    {
        this->stop();
        this->start(message);
    }

    inline double delta() const
    {
        tbb::tick_count::interval_t dt = tbb::tick_count::now() - startTime;
        return dt.seconds();
    }

    inline void stop() const
    {
        const double t = this->delta();
        std::ostringstream out;
        out << "finished in " << std::setprecision(3) << t << " seconds." << std::endl;
        std::cout << out.str();
    }

private:
    tbb::tick_count startTime;
};
}

#endif //NIKITA_RENDER_CPUTIMER_H
