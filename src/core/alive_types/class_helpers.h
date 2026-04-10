#ifndef CLASS_HELPERS_H
#define CLASS_HELPERS_H

namespace alive {

#define ONLY_CLONABLE(ClassName) \
private: \
    ClassName &operator=(const ClassName &) = delete; \
    ClassName &operator=(ClassName &&) = delete; \
    ClassName(ClassName &&) = delete;


#define NON_COPYABLE(ClassName) \
ClassName(const ClassName &) = delete; \
    ONLY_CLONABLE(ClassName) \

} // namespace alive

#endif // CLASS_HELPERS_H
