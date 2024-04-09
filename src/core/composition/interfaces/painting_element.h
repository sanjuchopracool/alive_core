#ifndef PAINTING_ELEMENT_H
#define PAINTING_ELEMENT_H

namespace inae::model {
struct PaintContext;
class PaintingElement
{
public:
    virtual ~PaintingElement() {}
    virtual void draw(PaintContext &context) const = 0;
    virtual bool is_repeater() const { return false; }
};

} // namespace inae::model

#endif // PAINTING_ELEMENT_H
