﻿#include "HelloWorldScene.h"
#include "FanCalculator/FanCalculatorScene.h"
#include "ScoreSheet/ScoreSheetScene.h"
#include "FanTable/FanTable.h"
#include "Other/OtherScene.h"
#include "MahjongTheory/MahjongTheoryScene.h"
#include "widget/AlertView.h"
#include "common.h"

USING_NS_CC;

static void showAbout();

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (UNLIKELY(!Layer::init())) {
        return false;
    }

    UserDefault::getInstance()->deleteValueForKey("night_mode");

    LayerColor *background = LayerColor::create(Color4B(245, 245, 245, 255));
    this->addChild(background, -100);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event *unused_event) {
        AlertView::showWithMessage("提示", "是否确定退出国标小助手？", []() {
            Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }, nullptr);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    ui::Button *button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("算番器");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f + 100));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(FanCalculatorScene::createScene());
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("计分器");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f + 60));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(ScoreSheetScene::createScene());
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("番种表");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f + 20));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(FanTableScene::createScene());
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("牌理");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f - 20));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(MahjongTheoryScene::createScene());
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("其他");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f - 60));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(OtherScene::createScene());
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(75.0, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("关于");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f - 100));
    button->addClickEventListener([](Ref *) {
        showAbout();
    });

    Label *label = Label::createWithSystemFont("Built  " __DATE__ "  " __TIME__, "Arial", 10);
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    label->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + 10));

    return true;
}

void showAbout() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    const float width = visibleSize.width * 0.8f - 10;

    Label *label = Label::createWithSystemFont(
        "1. 特别鸣谢：逍遥宫国标麻将群。\n"
        "2. BUG反馈请发邮件至「wohaaitinciu@163.com」。\n"
        "3. 如果您觉得好用，可多多帮我推广。\n"
        "4. 本软件开源，高端玩家可下载源代码自行编译。\n"
        "5. 由于作者无力承担苹果上架相关费用，没有推出iOS版本，您可以使用源代码自己打包出iOS版本。\n"
        "6. 本项目源代码地址：https://github.com/summerinsects/ChineseOfficialMahjongHelper",
        "Arial", 10);
    label->setColor(Color3B::BLACK);
    label->setDimensions(width, 0);
    AlertView::showWithNode("关于", label, nullptr, nullptr);
}
