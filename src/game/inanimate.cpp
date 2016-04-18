#include "game/inanimate.h"

namespace bm {

InanimateObject::InanimateObject(InanimateType t): type_(t) {
    model_ = ModelId::Wood;
}

} // ns bm
