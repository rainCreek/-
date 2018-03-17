#include "MenuScene.h"
#include "GameScene.h"
#include "GameScene3.h"
#include "HelpScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

// 获取单例
auto audio = SimpleAudioEngine::getInstance();

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto bg = Sprite::create("menupic.jpg");
    bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bg, 0);
    bg->setScale(2.0f);

	//title
	auto title = Sprite::create("title.jpg");
	title->setPosition(Vec2(570 + origin.x, visibleSize.height / 2 + origin.y - 100));
	this->addChild(title, 2);
	title->setScale(0.6f);

    //help
    auto help = MenuItemImage::create(
        "help.png",
        "help.png",
        CC_CALLBACK_1(MenuScene::helpReplaceScene, this));
    help->setScale(0.5f);
    auto menu = Menu::create(help, NULL);
    menu->setPosition(Vec2(150 + origin.x / 2, visibleSize.height / 2 + origin.y + 200));
    this->addChild(menu, 1);

    //start 
    auto start = MenuItemImage::create(
        "start0.png",
        "start0.png",
        CC_CALLBACK_1(MenuScene::startReplaceScene, this));
    start->setScale(0.4f);
    auto menu2 = Menu::create(start, NULL);
    menu2->setPosition(Vec2(650 + origin.x, visibleSize.height / 2 + origin.y + 100));
    this->addChild(menu2, 1);

	preloadMusic();     // 预加载音乐

    return true;
}

void MenuScene::helpReplaceScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/kaca.mp3", false);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(1, HelpScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MenuScene::startReplaceScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/music2.mp3", false);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(1, GameScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//预加载音乐文件
void MenuScene::preloadMusic() {
	// 预加载音效
	audio->preloadEffect("music/music2.mp3");
	audio->preloadEffect("music/kaca.mp3");
}


