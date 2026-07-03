#ifndef AVION_CORE_UTILS_UTILS_H
#define AVION_CORE_UTILS_UTILS_H 1

  #include <cstdint>

  namespace avion::core::utils
  {
    template <typename T, std::size_t N>
    constexpr std::size_t ArraySize(T (&)[N]) { return N; }
  } // namespace avion::core::utils

#endif 