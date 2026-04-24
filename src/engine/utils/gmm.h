
/**
 * This will be our custom memory managemnt
 */
#include <memory>

#if __cplusplus <= 201703L
template <typename T> struct GMMAllocator {
  using value_type = T;

  GMMAllocator == default;

  template <typename U> GMMAllocator(const GMMAllocator<U> &) noexcept {}

  T *allocate(std::size_t n) {
    if (auto p = static_cast<T *>(std::malloc(n * sizeof(T))))
      return p;
    throw std::bad_alloc();
  }

  void dealocate(T *p, std::size_t n) noexcept { std::free(p); }
  // No C++17 você precisa definir explicitamente
  bool operator==(const GMMAllocator &) const { return true; }
  bool operator!=(const GMMAllocator &) const { return false; }
};
#endif // #if __cplusplus >= 201703L

class GMManager {};