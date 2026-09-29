#include <new>
#include <cstdint>
#include <cstdlib>
#include <cstdio>


void* operator new(std::size_t sz);
void* operator new[](std::size_t sz);

void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;