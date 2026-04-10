#ifndef KEYPATHSEARCHABLE_H
#define KEYPATHSEARCHABLE_H

#include <QString>
#include "NodeProperty.h"

class CALayer;
class KeypathSearchable
{
public:
    virtual QString keypath_name() const {
        return QString();
    }

    virtual AnyNodeProperty* keypath_properties(const QString& key) const {
        return nullptr;
    }

    virtual QList<KeypathSearchable> child_keypaths() const {
        return QList<KeypathSearchable>();
    }

    virtual CALayer* keypath_layer() const {
        return nullptr;
    }
};

#endif // KEYPATHSEARCHABLE_H
