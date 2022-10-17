#include "CustomTypes/RequirementsUI.hpp"
#include "UnityEngine/GameObject.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "bsml/shared/Helpers/delegates.hpp"
#include "assets.hpp"
#include "config.hpp"

#include "UnityEngine/Resources.hpp"
#include "Tweening/FloatTween.hpp"
#include "GlobalNamespace/EaseType.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"

#include "Utils/RequirementUtils.hpp"
#include "Utils/SongUtils.hpp"
#include "Utils/ContributorUtils.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "System/IO/Path.hpp"

#include <fmt/format.h>

using namespace Tweening;
DEFINE_TYPE(PinkCore, RequirementsUI);

namespace PinkCore {
    SafePtr<RequirementsUI> RequirementsUI::instance; 
    RequirementsUI* RequirementsUI::get_instance() {
        if (!instance) {
            instance = RequirementsUI::New_ctor();
        } 
        return instance.ptr();
    }

    void RequirementsUI::Setup() {
        GetIcons();
        standardLevel = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::StandardLevelDetailViewController*>().FirstOrDefault();
        tweenyManager = UnityEngine::Resources::FindObjectsOfTypeAll<Tweening::TweeningManager*>().FirstOrDefault();
        BSML::parse_and_construct(BUTTON_BSML, standardLevel->get_transform()->Find("LevelDetail"), this);
        auto infoButtonTransform = infoButton->get_transform();
        infoButtonTransform->set_localScale(infoButtonTransform->get_localScale() * 0.7f); //no scale property in bsml as of now so manually scaling it
        auto favToggle = standardLevel->get_transform()->Find("LevelDetail/FavoriteToggle");
        reinterpret_cast<UnityEngine::RectTransform*>(favToggle)->set_anchoredPosition({3, -2});
        buttonBG = infoButtonTransform->Find("BG")->GetComponent<HMUI::ImageView*>();
        originalColor0 = buttonBG->get_color0();
        originalColor1 = buttonBG->get_color1();
    }

    void RequirementsUI::GetIcons() {
        if (!missingReqIcon)
            missingReqIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::RequirementMissing_png);

        if (!haveReqIcon)
            haveReqIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::RequirementFound_png);

        if (!haveSuggestionIcon)
            haveSuggestionIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::SuggestionFound_png);

        if (!missingSuggestionIcon)
            missingSuggestionIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::SuggestionMissing_png);

        if (!warningIcon)
            warningIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::WIP_png);

        if (!infoIcon)
            infoIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::Info_png);

        if (!colorsIcon)
            colorsIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::Colors_png);
    }

    void RequirementsUI::ShowRequirements() {
        if (!modal || !modal->m_CachedPtr.m_value) {
            BSML::parse_and_construct(IncludedAssets::requirements_bsml, root, this);
            modalPosition = modal->get_transform()->get_localPosition();
        }

        auto& mapdata = SongUtils::SongInfo::get_mapData();

        modal->get_transform()->set_localPosition(modalPosition);
        modal->Show();
        list->data->Clear();

        //Requirements
        if (mapdata.dataIsValid) {
            for (const auto& req : mapdata.currentRequirements) {
                list->data->Add(!RequirementUtils::GetRequirementInstalled(req)
                    ? BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Missing Requirement", missingReqIcon)
                    : BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Requirement", haveReqIcon));
            }
        }

        //Contributors
        if (mapdata.dataIsValid && !mapdata.currentRequirements.empty()) {
            for (const auto& author : mapdata.currentContributors) {
                if (!author.iconPath.empty()) {
                    auto path = static_cast<std::string>(System::IO::Path::Combine(level->get_customLevelPath(), author.iconPath));
                    auto spr = QuestUI::BeatSaberUI::FileToSprite(path);
                    list->data->Add(BSML::CustomCellInfo::construct(author.name, author.role, spr == nullptr ? spr : infoIcon));
                } else {
                    list->data->Add(BSML::CustomCellInfo::construct(author.name, author.role, infoIcon));
                }
            }
        }

        //WIP Check
        if (mapdata.dataIsValid && mapdata.isWIP) {
            list->data->Add(BSML::CustomCellInfo::construct("<size=70%>WIP Song. Please Play in Practice Mode</size>", "Warning", warningIcon));
        }

        //Additional Diff Info
        if (mapdata.dataIsValid)
        {
            if (mapdata.hasCustomColours) {
                list->data->Add(BSML::CustomCellInfo::construct("<size=75%>Custom Colors Available</size>", fmt::format("Click here to preview & {} it.", config.enableCustomSongColours ? "disable" : "enable"), colorsIcon));
            }

            if (!mapdata.currentWarnings.empty()) {
                for (const auto& req : mapdata.currentWarnings) {
                    list->data->Add(BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Warning", warningIcon));
                }
            }
            if (!mapdata.currentInformation.empty()) {
                for (const auto& req : mapdata.currentInformation) {
                    list->data->Add(BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Info", infoIcon));
                }
            }

            if (mapdata.dataIsValid && !mapdata.currentSuggestions.empty())
            {
                for (const auto& req : mapdata.currentSuggestions) {
                    list->data->Add(!!RequirementUtils::GetRequirementInstalled(req)
                        ? BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Missing Suggestion", missingSuggestionIcon)
                        : BSML::CustomCellInfo::construct(fmt::format("<size=75%>{}</size>", req), "Suggestion", haveSuggestionIcon));
                }
            }
        }

        list->tableView->ReloadData();
        list->tableView->ScrollToCellWithIdx(0, HMUI::TableView::ScrollPositionType::Beginning, false);
    }

    void RequirementsUI::Select(HMUI::TableView* _, int index) {
        list->tableView->ClearSelection();
        auto& mapdata = SongUtils::SongInfo::get_mapData();
        if (mapdata.dataIsValid && list->data[index]->icon == colorsIcon) {
            //modal->HMUI::ModalView::Hide(false, BSML::MakeSystemAction(std::bind(&ColorsUI::ShowColors, ColorsUI::get_instance(), mapdata)));
        }
    }

    void RequirementsUI::SetRainbowColors(bool shouldSet, bool firstPulse) {
        tweenyManager->KillAllTweens(buttonBG);
        if (shouldSet) {
            auto tweenDelegate = BSML::MakeSystemAction(std::function<void(float)>([buttonBG = this->buttonBG](float val){
                buttonBG->set_color0({1 - val, val, 0.0f, 1.0f});
                buttonBG->set_color1({0.0f, 1 - val, val, 1.0f});
                buttonBG->SetAllDirty();
            }));
            auto tween = FloatTween::New_ctor(!firstPulse, firstPulse, tweenDelegate, 5.0f, GlobalNamespace::EaseType::InOutSine, 0.0f);

            tweenyManager->AddTween(tween, buttonBG);
            tween->onCompleted = BSML::MakeSystemAction(std::bind(&RequirementsUI::SetRainbowColors, this, true, !firstPulse));
        }
        else {
            buttonBG->set_color0(originalColor0);
            buttonBG->set_color1(originalColor1);
        }
    }
    
    bool RequirementsUI::get_buttonGlow() {
        return _buttonGlow;
    }

    void RequirementsUI::set_buttonGlow(bool value) {
        _buttonGlow = value;
        infoButton->get_gameObject()->SetActive(value);
    }

    bool RequirementsUI::get_buttonInteractable() {
        return _buttonInteractable;
    }

    void RequirementsUI::set_buttonInteractable(bool value) {
        _buttonInteractable = value;
        infoButton->set_interactable(value);
    }
}