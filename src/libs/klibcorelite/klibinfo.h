#ifndef KLIBINFO_H
#define KLIBINFO_H

#include <QQmlEngine>
#include "klibcorelite_global.h"
#define USE_KLIBCORE_AS_SUBPROJECT

class KLIBCORELITE_EXPORT KLibInfo {
    static const int static_call;
public:
    static inline const char *libname = "KLib";
    static inline const int major = 1;
    static inline const int minor = 0;
};


template<typename T>
struct KCompareEngine {
    static bool compare(const T &arg0, const T &arg1) {
        if constexpr(std::is_same<T, QJSValue>::value) {
            return false;
        } else {
            return arg0 == arg1;
        }
    }
};

template<>
struct KCompareEngine<double> {
    static bool compare(double arg0, double arg1) {
        return qFuzzyCompare(arg0, arg1);
    }
};

template<>
struct KCompareEngine<float> {
    static bool compare(float arg0, float arg1) {
        return qFuzzyCompare(arg0, arg1);
    }
};

template <typename T>
constexpr auto type_name() noexcept {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void) noexcept";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}



#endif // KLIBINFO_H
