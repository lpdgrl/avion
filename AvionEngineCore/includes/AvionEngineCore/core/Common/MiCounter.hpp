#ifndef AVION_CORE_COMMON_MI_COUNTER_H 
#define AVION_CORE_COMMON_MI_COUNTER_H 1

#include <cstdint>
#include <limits>
#include <concepts>

namespace avion::core::common
{
  // Simple Monotonously Increasing Counter 
  template <std::unsigned_integral T>
  class MiCounter
  {
    public:
      MiCounter() = default;
      
      MiCounter(const MiCounter& other) = delete;
      MiCounter(MiCounter&& other) = delete;
      MiCounter& operator=(const MiCounter& rhs) = delete;
      MiCounter& operator=(MiCounter&& rhs) = delete;
      
      ~MiCounter() = default;

      inline auto GetUnchangedValue() noexcept -> T
      {
        return m_value;
      }
      
      inline auto GetValueAfterIncrease() noexcept -> T
      {
        Increase();
        return m_value;
      }

      inline auto GetMaxValue() noexcept -> T
      {
        return m_max;
      }
    
    private:
      inline auto Increase() noexcept -> void
      {
        if (m_value < m_max)
        {
          m_value = m_value + 1;
        }
      }

    private:
      T m_max = std::numeric_limits<T>::max();
      T m_value{};

  };
} // namespace avion::core::common

#endif