#include "CustomTypes/ColorsUI.hpp"

DEFINE_TYPE(PinkCore, ColorsUI);

namespace PinkCore {
    SafePtr<ColorsUI> ColorsUI::instance;
    ColorsUI* ColorsUI::get_instance() {
        if (!instance) {
            instance = ColorsUI::New_ctor();
        }
        return instance.ptr();
    }

}