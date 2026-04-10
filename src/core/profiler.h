#ifndef AUTOPROFILER_H
#define AUTOPROFILER_H

#include <chrono>
#include <core/logging/logger.h>
#include <string>

namespace alive {
class AutoProfiler {
 public:
  AutoProfiler(std::string name)
      : m_name(std::move(name)),
        m_beg(std::chrono::high_resolution_clock::now()) { }
  ~AutoProfiler()
  {
      auto end = std::chrono::high_resolution_clock::now();
      auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - m_beg);
      ALIVE_CORE_INFO("{} : {} usec \n", m_name, dur.count());
  }

  private:
  std::string m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_beg;
};
} // namespace alive
#endif // AUTOPROFILER_H
