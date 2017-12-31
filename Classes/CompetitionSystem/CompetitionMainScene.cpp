﻿#include "CompetitionMainScene.h"
#include <array>
#include "../widget/AlertView.h"
#include "Competition.h"
#include "CompetitionEnrollScene.h"
#include "CompetitionRoundScene.h"
#include "CompetitionHistoryScene.h"
#include "EditBoxDelegateWrapper.hpp"

USING_NS_CC;

bool CompetitionMainScene::init() {
    if (UNLIKELY(!BaseScene::initWithTitle("比赛"))) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    ui::Button *button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(90.0f, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("新建比赛");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f + 50.0f));
    button->addClickEventListener([this](Ref *) {
        if (_competitionData->start_time != 0 && _competitionData->finish_time == 0) {
            AlertView::showWithMessage("新建比赛", "当前有未完成的比赛，新建比赛将会覆盖旧的比赛，是否继续？", 12,
                std::bind(&CompetitionMainScene::showNewCompetitionAlert, this, "", 8, 5), nullptr);
        }
        else {
            this->showNewCompetitionAlert("", 8, 5);
        }
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(90.0f, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("继续比赛");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f));
    button->addClickEventListener([this](Ref *) {
        if (_competitionData->start_time != 0 && _competitionData->finish_time == 0) {
            Scene *scene = nullptr;
            if (_competitionData->isEnrollmentOver()) {
                scene = CompetitionRoundScene::create(_competitionData, _competitionData->rounds.size() - 1);
            }
            else {
                scene = CompetitionEnrollScene::create(_competitionData);
            }
            Director::getInstance()->pushScene(scene);
        }
    });

    button->setOnEnterCallback([this, button]() {
        button->setEnabled(_competitionData->start_time != 0 && _competitionData->finish_time == 0);
    });

    button = ui::Button::create("source_material/btn_square_highlighted.png", "source_material/btn_square_selected.png");
    this->addChild(button);
    button->setScale9Enabled(true);
    button->setContentSize(Size(90.0f, 32.0f));
    button->setTitleFontSize(20);
    button->setTitleText("历史记录");
    button->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f - 50.0f));
    button->addClickEventListener([](Ref *) {
        Director::getInstance()->pushScene(CompetitionHistoryScene::create([](CompetitionData *){ }));
    });

    _competitionData = std::make_shared<CompetitionData>();
    _competitionData->associated_file = FileUtils::getInstance()->getWritablePath().append("competition.json");
    _competitionData->modify_callback = CompetitionHistoryScene::modifyData;
    _competitionData->readFromFile();

    return true;
}

void CompetitionMainScene::showNewCompetitionAlert(const std::string &name, size_t player, size_t round) {
    Node *rootNode = Node::create();
    rootNode->setContentSize(Size(220.0f, 90.0f));

    Label *label = Label::createWithSystemFont("赛事名称", "Arial", 12);
    label->setColor(Color3B::BLACK);
    rootNode->addChild(label);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Vec2(5.0f, 75.0f));

    std::array<ui::EditBox *, 3> editBoxes;

    ui::EditBox *editBox = ui::EditBox::create(Size(160.0f, 20.0f), ui::Scale9Sprite::create("source_material/btn_square_normal.png"));
    editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
    editBox->setFontColor(Color4B::BLACK);
    editBox->setFontSize(12);
    editBox->setText(name.c_str());
    editBox->setMaxLength(128);
    rootNode->addChild(editBox);
    editBox->setPosition(Vec2(135.0f, 75.0f));
    editBoxes[0] = editBox;
#if 1  // 测试代码
    editBox->setText("测试比赛");
#endif

    static const char *titleText[2] = { "参赛人数", "比赛轮数" };
    size_t value[2] = { player, round };

    for (int i = 0; i < 2; ++i) {
        label = Label::createWithSystemFont(titleText[i], "Arial", 12);
        label->setColor(Color3B::BLACK);
        rootNode->addChild(label);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        label->setPosition(Vec2(5 + 110.0f * i, 45.0f));

        char buf[32];
        snprintf(buf, sizeof(buf), "%" PRIzu, value[i]);

        editBox = ui::EditBox::create(Size(50.0f, 20.0f), ui::Scale9Sprite::create("source_material/btn_square_normal.png"));
        editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
        editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
        editBox->setReturnType(ui::EditBox::KeyboardReturnType::NEXT);
        editBox->setFontColor(Color4B::BLACK);
        editBox->setFontSize(12);
        editBox->setText(buf);
        editBox->setMaxLength(3);
        rootNode->addChild(editBox);
        editBox->setPosition(Vec2(80.0f + 110.0f * i, 45.0f));
        editBoxes[i + 1] = editBox;
    }

    // EditBox的代理，使得能连续输入
    auto delegate = std::make_shared<EditBoxDelegateWrapper>(std::vector<ui::EditBox *>(editBoxes.begin(), editBoxes.end()));
    editBoxes[0]->setDelegate(delegate.get());
    editBoxes[1]->setDelegate(delegate.get());

    label = Label::createWithSystemFont("参赛人数必须为4的倍数。移动设备毕竟不比PC，不建议设置过多人数和轮数", "Arial", 10);
    label->setColor(Color3B(0x60, 0x60, 0x60));
    rootNode->addChild(label);
    label->setPosition(Vec2(110.0f, 15.0f));
    label->setDimensions(220.0f, 0.0f);

    AlertView::showWithNode("新建比赛", rootNode, [this, editBoxes, delegate]() {
        std::string name;
        size_t player = 8, round = 5;

        const char *text = editBoxes[0]->getText();
        if (*text != '\0') {
            name = text;
        }

        text = editBoxes[1]->getText();
        if (*text != '\0') {
            player = static_cast<size_t>(atoll(text));
        }

        text = editBoxes[2]->getText();
        if (*text != '\0') {
            round = static_cast<size_t>(atoll(text));
        }

        if (name.empty()) {
            AlertView::showWithMessage("新建比赛", "请输入赛事名称", 12,
                std::bind(&CompetitionMainScene::showNewCompetitionAlert, this, name, player, round), nullptr);
            return;
        }

        if ((player & 0x3) || player == 0) {
            AlertView::showWithMessage("新建比赛", "参赛人数必须为4的倍数，且大于0", 12,
                std::bind(&CompetitionMainScene::showNewCompetitionAlert, this, name, player, round), nullptr);
            return;
        }

        if (round == 0) {
            AlertView::showWithMessage("新建比赛", "比赛轮数必须大于0", 12,
                std::bind(&CompetitionMainScene::showNewCompetitionAlert, this, name, player, round), nullptr);
            return;
        }

        Label *label = Label::createWithSystemFont(Common::format("「%s」\n%" PRIzu "人\n%" PRIzu "轮", name.c_str(), player, round), "Arial", 12);
        label->setColor(Color3B::BLACK);
        label->setHorizontalAlignment(TextHAlignment::CENTER);

        AlertView::showWithNode("新建比赛", label, [this, name, player, round]() {
            _competitionData->prepare(name, player, round);
            _competitionData->writeToFile();
            Director::getInstance()->pushScene(CompetitionEnrollScene::create(_competitionData));
        }, nullptr);
    }, nullptr);
}