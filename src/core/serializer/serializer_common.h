#ifndef SERIALIZER_COMMON_H
#define SERIALIZER_COMMON_H

namespace alive {
namespace core {
class Serializer;
}

namespace script {
class ExpressionEngine;
}

class LottieParser;
} // namespace alive

#define ADD_FRIEND_SERIALIZERS \
    friend class alive::core::Serializer; \
    friend class alive::script::ExpressionEngine;

#endif // SERIALIZER_COMMON_H
