#include <catch2/catch_all.hpp>
#include <pack/pack.h>

namespace Catch {
template <typename T>
struct StringMaker<pack::expected<T>>
{
    static std::string convert(const pack::expected<T>& value)
    {
        if (value) {
            return "expected has result";
        }
        return value.error().toStdString();
    }
};
} // namespace Catch
