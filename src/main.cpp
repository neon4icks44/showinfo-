#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    // Объявляем методы как методы экземпляра
    void onCopyLevelID(CCObject* sender) {
        auto level = GameManager::sharedState()->getPlayLayer()->m_level;
        if (level) {
            geode::utils::clipboard::write(std::to_string(level->m_levelID));
            geode::createQuickPopup(
                "Success", "Level ID copied to clipboard!", "OK", nullptr, nullptr
            );
        } else {
            log::warn("Failed to get level ID: level is null");
        }
    }

    void onViewComments(CCObject* sender) {
        // Заглушка для комментариев
        geode::createQuickPopup(
            "Comments", "Viewing comments is not implemented yet.", "OK", nullptr, nullptr
        );
        // В будущем здесь можно использовать GJCommentListLayer или API сервера
    }

    void customSetup() {
        PauseLayer::customSetup();

        // Получаем ID уровня
        auto level = GameManager::sharedState()->getPlayLayer()->m_level;
        std::string levelID = level ? std::to_string(level->m_levelID) : "Unknown";
        log::info("Level ID: {}", levelID);

        // Создаём текстовое поле для ID уровня
        auto label = CCLabelBMFont::create(("ID: " + levelID).c_str(), "goldFont.fnt");
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