#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    // Объявляем методы как методы экземпляра
    void onCopyLevelID(CCObject* sender) {
        auto pl = PlayLayer::get();
        if (!pl) return;
        auto level = pl->m_level;
        if (level) {
            geode::utils::clipboard::write(geode::utils::numToString(level->m_levelID.value()));
            geode::Notification::create("Level ID copied to clipboard!")->show();
        } else {
            log::warn("Failed to get level ID: level is null");
        }
    }

    void customSetup() {
        PauseLayer::customSetup();

        // Получаем ID уровня
        auto pl = PlayLayer::get();
        if (!pl) return;
        auto level = pl->m_level;
        std::string levelID = level ? geode::utils::numToString(level->m_levelID.value()) : "Unknown";
        log::info("Level ID: {}", levelID);

        // Создаём текстовое поле для ID уровня
        auto menu = CCMenu::create();
        if (!menu) {
            log::error("Failed to create menu for Level ID");
            return;
        }
        menu->setID("level-id-menu"_spr);

        auto label = CCLabelBMFont::create(fmt::format("ID: {}", levelID).c_str(), "goldFont.fnt");
        if (!label) {
            log::error("Failed to create label for Level ID");
            return;
        }
        label->setID("level-id-label"_spr);
        
        auto button = CCMenuItemSpriteExtra::create(label, this, menu_selector(MyPauseLayer::onCopyLevelID));
        if (!button) {
            log::error("Failed to create label for Level ID");
            return;
        }
        button->setID("level-id-button"_spr);
        button->setAnchorPoint({1.0f, 0.0f});

        menu->setContentSize(button->getContentSize() / 2.f);
        menu->setPosition({CCDirector::sharedDirector()->getWinSize().width - 10.0f, 10.0f});
        menu->setScale(0.5f);
        menu->addChild(button);
        this->addChild(menu);
    }
};