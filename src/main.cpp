#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    void onCopyLevelID(CCObject* sender) {
        auto pl = PlayLayer::get();
        if (!pl) return;
        auto level = pl->m_level;
        if (level) {
            std::string id = std::to_string(level->m_levelID.value());
            geode::utils::clipboard::write(id);
            Notification::create("Level ID copied to clipboard!")->show();
        } else {
            log::warn("Failed to get level ID: level is null");
        }
    }

    void onShowHistory(CCObject* sender) {
        Notification::create("History feature removed in this version")->show();
    }

    void customSetup() {
        log::info("Setting up custom PauseLayer");
        PauseLayer::customSetup();
        
        auto background = this->getChildByIDRecursive("background");
        if (!background) {
            log::error("Failed to find background");
            return;
        }

        auto pl = PlayLayer::get();
        if (!pl) return;
        auto level = pl->m_level;
        std::string levelID = level ? std::to_string(level->m_levelID.value()) : "Unknown";
        log::info("Level ID: {}", levelID);

        auto menu = CCMenu::create();
        if (!menu) {
            log::error("Failed to create menu for Level ID");
            return;
        }
        menu->setID("level-id-menu"_spr);

        auto label = CCLabelBMFont::create(("ID: " + levelID).c_str(), "goldFont.fnt");
        if (!label) {
            log::error("Failed to create label for Level ID");
            return;
        }
        label->setID("level-id-label"_spr);
        
        auto button = CCMenuItemSpriteExtra::create(label, this, menu_selector(MyPauseLayer::onCopyLevelID));
        if (!button) {
            log::error("Failed to create button for Level ID");
            return;
        }
        button->setID("level-id-button"_spr);
        button->setAnchorPoint({1.0f, 0.0f});

        menu->setContentSize(button->getContentSize());
        menu->setPosition({background->getContentSize().width - 10.0f, 10.0f});
        menu->setAnchorPoint({0.0f, 1.0f});
        menu->setScale(0.5f);
        menu->addChild(button);
        this->addChild(menu);
    }
};

class $modify(MyMoreOptionsLayer, MoreOptionsLayer) {
    bool init() {
        if (!MoreOptionsLayer::init()) return false;
        auto winSize = CCDirector::get()->getWinSize();

        auto statsLabel = CCLabelBMFont::create("Stats", "goldFont.fnt");
        auto statsButton = CCMenuItemSpriteExtra::create(statsLabel, this, menu_selector(MyMoreOptionsLayer::onStats));
        statsButton->setPosition(winSize.width / 2, winSize.height / 2 + 50);
        statsButton->setID("stats-button"_spr);

        auto menu = this->getChildByIDRecursive("main-menu");
        if (!menu) menu = this->getChildByIDRecursive("menu");
        
        if (menu) {
            auto castedMenu = typeinfo_cast<CCMenu*>(menu);
            if (castedMenu) {
                castedMenu->addChild(statsButton);
            }
        }

        return true;
    }

    void onStats(CCObject* sender) {
        auto statsManager = GameStatsManager::sharedState();
        int levelCount = 0;
        
        if (statsManager) {
            levelCount = statsManager->getStat("1");
        }

        auto statsText = fmt::format("Levels Played: {}", levelCount);
        auto alertLayer = FLAlertLayer::create("Game Stats", statsText.c_str(), "OK");
        if (alertLayer) alertLayer->show();
    }
};
