#ifndef OPENGLPLAYGROUND_CORE_H
#define OPENGLPLAYGROUND_CORE_H

#include <memory>

template<typename T>
using Ref=std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif //OPENGLPLAYGROUND_CORE_H
