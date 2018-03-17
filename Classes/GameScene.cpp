#include "GameScene.h"
#include "GameScene1.h"
#include "HelpScene.h"
#include<iostream>
#include<strstream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <algorithm>
#pragma execution_character_set("utf-8")
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
void GameScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0.0f));
	// Debug 模式
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	// 添加调度器
	schedule(schedule_selector(GameScene::judgeWin), 0.01f, kRepeatForever, 0.1f);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	lastCid = 'A';

	auto labelRound = Label::createWithTTF("Round 1", "fonts/arial.TTF", 40);
	labelRound->setColor(Color3B::WHITE);
	labelRound->setPosition(Vec2(150 + origin.x / 2, visibleSize.height / 2 + origin.y + 200));
	this->addChild(labelRound, 1);


	//导入地图
	// Tmx文件和屏幕的适配

	CCTMXTiledMap* tmx = TMXTiledMap::create("map1new.tmx");

	tmx->setPosition(500, 300);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	//this->addChild(tmx, 0);

	auto bg = Sprite::create("bg.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);
	bg->setScale(3.5f);

	//获取对象层
	TMXObjectGroup *object = tmx->objectGroupNamed("object1");
	// 获取对象
	auto obj = object->objectNamed("sprite");

	addPlayer();

	//auto frame0 = texture;
	addBox();

	addWall();

	//创建箱子位置
	light1 = Sprite::create("light.png");
	light1->setPosition(420, 280); //第左
	addChild(light1, 3);

	// 设置重玩按钮
	auto restart = MenuItemImage::create(
		"restart.png",
		"restart.png",
		CC_CALLBACK_1(GameScene::restartCallback, this));
	restart->setScale(0.1f);
	auto rmenu = Menu::create(restart, NULL);
	rmenu->setPosition(750 + origin.x / 2, origin.y + 400);
	this->addChild(rmenu, 1);

	// 运动动画(帧数：8帧，高：101，宽：68）
	//auto texture0 = Director::getInstance()->getTextureCache()->addImage("3.png");
	// Todo
	//for (int i = 1; i < 20; i++) {
	for (int i = 1; i < 1; i++) {
		stringstream s;
		s << i;
		s << ".png";
		auto frame = SpriteFrame::create(s.str(), Rect(0, 0, 70, 50));
		run.pushBack(frame);
	}


	// 创建TTFConfig管理Label属性
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/arial.ttf";
	labelConfig.fontSize = 36;

	// 创建labels
	auto label1 = Label::createWithTTF(labelConfig, "W");
	auto label2 = Label::createWithTTF(labelConfig, "S");
	auto label3 = Label::createWithTTF(labelConfig, "A");
	auto label4 = Label::createWithTTF(labelConfig, "D");

	// 创建menuItem
	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_1(GameScene::moveUp, this));           // W
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_1(GameScene::moveDown, this));           // S
	auto item3 = MenuItemLabel::create(label3, CC_CALLBACK_1(GameScene::moveLeft, this));           // A
	auto item4 = MenuItemLabel::create(label4, CC_CALLBACK_1(GameScene::moveRight, this));           // D

																									 // A
	item3->setPosition(Vec2(origin.x + item3->getContentSize().width, origin.y + item2->getContentSize().height));

	// S
	item2->setPosition(Vec2(item3->getPosition().x + item2->getContentSize().width + 10, origin.y + item3->getContentSize().height));

	// D
	item4->setPosition(Vec2(item2->getPosition().x + item4->getContentSize().width + 10, origin.y + item3->getContentSize().height));

	// W
	item1->setPosition(Vec2(item3->getPosition().x + item2->getContentSize().width + 10, item2->getContentSize().height + item3->getContentSize().height));

	auto menu = Menu::create(item1, item2, item3, item4, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);

	addKeyboardListener();
	return true;
}

void GameScene::addPlayer() {
	//使用第一帧创建精灵
	player = Sprite::create("3.png");
	player->setPosition(480, 400);
	addChild(player, 3);

	// 设置角色刚体属性
	player->setPhysicsBody(PhysicsBody::createBox(Size(48, 48), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	player->getPhysicsBody()->setCategoryBitmask(2);
	player->getPhysicsBody()->setCollisionBitmask(2);
	player->getPhysicsBody()->setContactTestBitmask(2);
	player->getPhysicsBody()->setRotationEnable(false);  // 防止小人旋转
}

void GameScene::addBox() {
	// 创建箱子
	box1 = Sprite::create("box1.png");
	box1->setPosition(480, 362); //第二下
	box1->setPhysicsBody(PhysicsBody::createBox(box1->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	box1->getPhysicsBody()->setCategoryBitmask(3);
	box1->getPhysicsBody()->setCollisionBitmask(3);
	box1->getPhysicsBody()->setContactTestBitmask(3);
	box1->getPhysicsBody()->setRotationEnable(false);
	addChild(box1, 3);

	box2 = Sprite::create("box1.png");
	box2->setPosition(442, 400);
	box2->setPhysicsBody(PhysicsBody::createBox(box2->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	box2->getPhysicsBody()->setCategoryBitmask(3);
	box2->getPhysicsBody()->setCollisionBitmask(3);
	box2->getPhysicsBody()->setContactTestBitmask(3);
	box2->getPhysicsBody()->setRotationEnable(false);
	addChild(box2, 3);

	light2 = Sprite::create("light.png");
	light2->setPosition(530, 280); //第三左
	addChild(light2, 3);
}

void GameScene::addWall() {
	// 创建墙体
	//左墙体
	for (int i = 0; i < 10; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300, 162 + 50 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//右墙体
	for (int i = 0; i < 6; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(580, 162 + 50 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//右墙体
	for (int i = 8; i < 10; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(580, 162 + 50 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//右墙体
	for (int i = 5; i < 9; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(615, 162 + 50 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//下墙体
	for (int i = 1; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 50 * 0.7*i, 162);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//中间墙体
	for (int i = 4; i < 5; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 50 * 0.7*i, 232);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//中间墙体
	for (int i = 1; i < 4; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 50 * 0.7*i, 318);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//下墙体
	for (int i = 6; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 50 * 0.7*i, 318);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//上墙体
	for (int i = 1; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 50 * 0.7*i, 477);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(35, 35), PhysicsMaterial(10000.0f, 1.0f, 1.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(2);
		wall->getPhysicsBody()->setCollisionBitmask(2);
		wall->getPhysicsBody()->setContactTestBitmask(2);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
}

// 上
void GameScene::moveUp(Ref* sender) {
	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;
	player->runAction(MoveTo::create(0.2, Vec2(x, y + 43)));

	player->runAction(animate);
}
// 下
void GameScene::moveDown(Ref* sender) {

	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;

	player->runAction(MoveTo::create(0.2, Vec2(x, y - 43)));


	player->runAction(animate);
}
// 左
void GameScene::moveLeft(Ref* sender) {
	if (lastCid != 'A') {
		player->setFlipX(false);
	}
	lastCid = 'A';
	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;

	player->runAction(MoveTo::create(0.2, Vec2(x - 42, y)));

	player->runAction(animate);
}
// 右
void GameScene::moveRight(Ref* sender) {
	if (lastCid != 'D') {
		player->setFlipX(true);
	}
	lastCid = 'D';
	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;

	player->runAction(MoveTo::create(0.2, Vec2(x + 42, y)));

	player->runAction(animate);
}

// 添加键盘事件监听器
void GameScene::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		moveLeft(NULL);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		moveRight(NULL);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		moveUp(NULL);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		moveDown(NULL);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		//isMove = false;
		break;
	}
}

void GameScene::judgeWin(float dt) {
	if (light1 != nullptr && light2 != nullptr) {
		if 	(((box1->getBoundingBox().intersectsRect(light1->getBoundingBox()))&&
			(box2->getBoundingBox().intersectsRect(light2->getBoundingBox())))||
			((box1->getBoundingBox().intersectsRect(light2->getBoundingBox()))&&
			(box2->getBoundingBox().intersectsRect(light1->getBoundingBox())))) {

			// 关闭调度器
			unschedule(schedule_selector(GameScene::judgeWin));
			// 设置下一关按钮
			auto next = MenuItemImage::create(
				"next.png",
				"next.png",
				CC_CALLBACK_1(GameScene::nextCallback, this));
			next->setScale(0.1f);
			auto menu3 = Menu::create(next, NULL);
			menu3->setPosition(750 + origin.x / 2, origin.y + 300);
			this->addChild(menu3, 1);

		}
	}
}


// 下一关按钮响应函数.
void GameScene::nextCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(GameScene1::createScene());
}
// 重玩按钮响应函数.
void GameScene::restartCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}
