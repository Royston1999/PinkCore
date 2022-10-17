#pragma once

#include "custom-types/shared/macros.hpp"
#include "GlobalNamespace/ColorSchemeView.hpp"
#include "UnityEngine/RectTransform.hpp"

#include "bsml/shared/BSML/Components/ModalView.hpp"
#include "bsml/shared/macros.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastVector3.hpp"

DECLARE_CLASS_CODEGEN(PinkCore, ColorsUI, Il2CppObject,
    DECLARE_INSTANCE_FIELD(GlobalNamespace::ColorSchemeView*, colorSchemeView);
    DECLARE_INSTANCE_FIELD_DEFAULT(Sombrero::FastColor, voidColor, Sombrero::FastColor(0.5, 0.5, 0.5, 0.25));
    DECLARE_INSTANCE_FIELD(Sombrero::FastVector3, modalPosition);
    DECLARE_INSTANCE_FIELD(BSML::ModalView*, modal);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, selectedColor);

    DECLARE_BSML_PROPERTY(bool, colors);

    DECLARE_DEFAULT_CTOR();
    public:
        static ColorsUI* get_instance();
    private:
        static SafePtr<ColorsUI> instance;
)