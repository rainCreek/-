#include "cocos2d.h"
#include "HelpScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include "network/HttpClient.h" 
using namespace cocos2d::network;
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;


Scene* HelpScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelpScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("box.png");
    bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));
    this->addChild(bg, 0);
    bg->setScale(0.2f);

    //back to menu
    auto button = Button::create();
    button->addClickEventListener(CC_CALLBACK_1(HelpScene::backButtonClick, this));
    button->setTitleText("Back");
    button->setTitleFontSize(30);
    button->setPosition(Size(3 * visibleSize.width / 4, visibleSize.height / 4 - 100));
    this->addChild(button, 2);

    //start to play
    auto playButton = Button::create();
    playButton->addClickEventListener(CC_CALLBACK_1(HelpScene::playButtonClick, this));
    playButton->setTitleText("Play");
    playButton->setTitleFontSize(30);
    playButton->setPosition(Size(visibleSize.width / 4, visibleSize.height / 4));
    //this->addChild(playButton, 2);

    //help text
    TextField * text = TextField::create("                                                                          How to play\n                                           1.Push the boxes to their correct locations.\n                                           2.Find the bulb hiden so you can get the location.\n                                           3.Enjoy yourself!", "Arial", 36);
    text->setColor(cocos2d::Color3B(255, 255, 255));
    text->setPosition(Size(visibleSize.width / 4, 3 * visibleSize.height / 4));
    this->addChild(text, 2);

}

void HelpScene::backButtonClick(Ref* pSender) {

    Director::getInstance()->replaceScene(TransitionSlideInL::create(1, MenuScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelpScene::playButtonClick(Ref* pSender) {

    Director::getInstance()->replaceScene(TransitionSlideInL::create(1, GameScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}




