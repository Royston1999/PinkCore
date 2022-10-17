#pragma once

#include "custom-types/shared/macros.hpp"

#include "bsml/shared/BSML/Components/CustomListTableData.hpp"
#include "bsml/shared/BSML/Components/ModalView.hpp"
#include "bsml/shared/macros.hpp"

#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "Tweening/TweeningManager.hpp"

#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastVector3.hpp"

DECLARE_CLASS_CODEGEN(PinkCore, RequirementsUI, Il2CppObject,
    DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, list);
    DECLARE_INSTANCE_FIELD(BSML::ModalView*, modal);

    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, root);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, infoButton);
    DECLARE_INSTANCE_FIELD(Sombrero::FastVector3, modalPosition);

    DECLARE_INSTANCE_FIELD(Tweening::TweeningManager*, tweenyManager);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::StandardLevelDetailViewController*, standardLevel);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, buttonBG);
    DECLARE_INSTANCE_FIELD(Sombrero::FastColor, originalColor0);
    DECLARE_INSTANCE_FIELD(Sombrero::FastColor, originalColor1);

    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, haveReqIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, missingReqIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, haveSuggestionIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, missingSuggestionIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, warningIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, infoIcon);
    DECLARE_INSTANCE_FIELD(UnityEngine::Sprite*, colorsIcon);
    
    DECLARE_INSTANCE_FIELD(GlobalNamespace::CustomPreviewBeatmapLevel*, level);

    DECLARE_BSML_PROPERTY(bool, buttonGlow);
    DECLARE_BSML_PROPERTY(bool, buttonInteractable);

    DECLARE_INSTANCE_METHOD(void, Setup);
    DECLARE_INSTANCE_METHOD(void, GetIcons);
    DECLARE_INSTANCE_METHOD(void, ShowRequirements);

    DECLARE_INSTANCE_METHOD(void, Select, HMUI::TableView* _, int index);
    DECLARE_INSTANCE_METHOD(void, SetRainbowColors, bool shouldSet, bool firstPulse);

    DECLARE_DEFAULT_CTOR();

    public:
        static RequirementsUI* get_instance();
    private:
        static SafePtr<RequirementsUI> instance; 
        static inline const std::string_view BUTTON_BSML = "<bg id='root'><action-button id='infoButton' text='?' active='~buttonGlow' interactable='~buttonInteractable' anchor-pos-x='31' anchor-pos-y='0' pref-width='12' pref-height='9' on-click='ShowRequirements'/></bg>";
)