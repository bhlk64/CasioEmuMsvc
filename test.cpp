#include <cstring>
#include <type_traits>
struct ImGuiStyle { int x; };
template <typename T> bool is_mem_equal(const T& a, const T& b) { return std::memcmp(&a, &b, sizeof(T)) == 0; }
int main() { ImGuiStyle s; return is_mem_equal(s, {}); }
