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
        auto label = CCLabelBMFont::create(fmt::format("ID: {}", levelID).c_str(), "goldFont.fnt");
        if (!label) {
            log::error("Failed to create label for Level ID");
            return;
        }
        label->setAnchorPoint({1.0f, 0.0f});
        label->setPosition({CCDirector::sharedDirector()->getWinSize().width - 10, 10});
        label->setScale(0.5f);
        this->addChild(label);
    }
};