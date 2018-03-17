#include "GameScene.h"
#include "GameScene4.h"
#include "HelpScene.h"
#include "MenuScene.h"
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
void GameScene4::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* GameScene4::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0.0f));
	// Debug 模式
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// 'layer' is an autorelease object
	auto layer = GameScene4::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene4::init()
{

	if (!Layer::init())
	{
		return false;
	}

	// 添加调度器
	schedule(schedule_selector(GameScene4::judgeWin), 0.01f, kRepeatForever, 0.1f);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	lastCid = 'A';

	auto labelRound = Label::createWithTTF("Final Round", "fonts/arial.TTF", 40);
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

	// 提示语
	auto labeltips = Label::createWithTTF("Double Boxes.", "fonts/arial.TTF", 40);
	labeltips->setColor(Color3B::BLACK);
	auto Btn = MenuItemLabel::create(labeltips, NULL);
	Menu* tip = Menu::create(Btn, NULL);
	tip->setPosition(visibleSize.width / 2, origin.y + 550);
	this->addChild(tip);

	// 设置重玩按钮
	auto restart = MenuItemImage::create(
		"restart.png",
		"restart.png",
		CC_CALLBACK_1(GameScene4::restartCallback, this));
	restart->setScale(0.1f);
	auto rmenu = Menu::create(restart, NULL);
	rmenu->setPosition(750 + origin.x / 2, origin.y + 400);
	this->addChild(rmenu, 1);

	/*
	light3 = Sprite::create("light.png");
	light3->setPosition(568, 280); //第右
	addChild(light3, 3);

	light4 = Sprite::create("light.png");
	light4->setPosition(560, 200); //最右
	addChild(light4, 3);
	*/

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
	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_1(GameScene4::moveUp, this));           // W
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_1(GameScene4::moveDown, this));           // S
	auto item3 = MenuItemLabel::create(label3, CC_CALLBACK_1(GameScene4::moveLeft, this));           // A
	auto item4 = MenuItemLabel::create(label4, CC_CALLBACK_1(GameScene4::moveRight, this));           // D

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
	addContactListener();
	return true;
}

void GameScene4::addPlayer() {
	//使用第一帧创建精灵
	player = Sprite::create("3.png");
	player->setPosition(360, 400);
	addChild(player, 4);
	//player->setAnchorPoint(Vec2(1, 0));

	// 设置角色刚体属性
	player->setPhysicsBody(PhysicsBody::createBox(Size(48, 48), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	player->getPhysicsBody()->setCategoryBitmask(6);
	player->getPhysicsBody()->setCollisionBitmask(6);
	player->getPhysicsBody()->setContactTestBitmask(6);
	player->getPhysicsBody()->setRotationEnable(false);  // 防止小人旋转
}

void GameScene4::addBox() {
	// 创建箱子
	box1 = Sprite::create("box1.png");
	box1->setPosition(500, 362); //第二下
	box1->setPhysicsBody(PhysicsBody::createBox(box1->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 1.001f)));
	box1->getPhysicsBody()->setCategoryBitmask(3);
	box1->getPhysicsBody()->setCollisionBitmask(3);
	box1->getPhysicsBody()->setContactTestBitmask(3);
	box1->getPhysicsBody()->setRotationEnable(false);
	addChild(box1, 3);

	box2 = Sprite::create("box1.png");
	box2->setPosition(500, 396);
	box2->setPhysicsBody(PhysicsBody::createBox(box2->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 1.001f)));
	box2->getPhysicsBody()->setCategoryBitmask(3);
	box2->getPhysicsBody()->setCollisionBitmask(3);
	box2->getPhysicsBody()->setContactTestBitmask(3);
	box2->getPhysicsBody()->setRotationEnable(false);
	addChild(box2, 3);


	//创建箱子位置
	light1 = Sprite::create("light.png");
	light1->setPhysicsBody(PhysicsBody::createBox(light1->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 10.001f)));
	light1->getPhysicsBody()->setCategoryBitmask(4);
	light1->getPhysicsBody()->setCollisionBitmask(4);
	light1->getPhysicsBody()->setContactTestBitmask(4);
	light1->getPhysicsBody()->setRotationEnable(false);
	light1->setPosition(420, 280); //第左
	addChild(light1, 3);


	light2 = Sprite::create("light.png");
	light2->setPosition(500, 397); //第三左
	addChild(light2, 3);

	/*
	box3 = Sprite::create("box1.png");
	box3->setPosition(556, 362);  //最右
	box3->setPhysicsBody(PhysicsBody::createBox(box3->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	box3->getPhysicsBody()->setCategoryBitmask(3);
	box3->getPhysicsBody()->setCollisionBitmask(3);
	box3->getPhysicsBody()->setContactTestBitmask(3);
	box3->getPhysicsBody()->setRotationEnable(false);
	addChild(box3, 3);

	box4 = Sprite::create("box1.png");
	box4->setPosition(480, 324);  // 最下
	box4->setPhysicsBody(PhysicsBody::createBox(box4->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	box4->getPhysicsBody()->setCategoryBitmask(3);
	box4->getPhysicsBody()->setCollisionBitmask(3);
	box4->getPhysicsBody()->setContactTestBitmask(3);
	box4->getPhysicsBody()->setRotationEnable(false);
	addChild(box4, 3);

	box5 = Sprite::create("box1.png");
	box5->setPosition(518, 400);  //第二右
	box5->setPhysicsBody(PhysicsBody::createBox(box5->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.001f)));
	box5->getPhysicsBody()->setCategoryBitmask(3);
	box5->getPhysicsBody()->setCollisionBitmask(3);
	box5->getPhysicsBody()->setRotationEnable(false);
	addChild(box5, 3);
	*/
}

void GameScene4::addWall() {
	// 创建墙体
	//左墙体
	for (int i = 0; i < 10; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300, 162 + 55 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//右墙体
	for (int i = 0; i < 6; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(608, 162 + 55 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//右墙体
	for (int i = 8; i < 10; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(608, 162 + 55 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//右墙体
	for (int i = 5; i < 9; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(646.5, 162 + 55 * 0.7*i);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//中间墙体
	for (int i = 1; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 55 * 0.7*i, 162);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//中间墙体
	for (int i = 4; i < 5; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 55 * 0.7*i, 239);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//下墙体
	for (int i = 1; i < 4; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 55 * 0.7*i, 316);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
	//下墙体
	for (int i = 6; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 55 * 0.7*i, 316);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}

	//上墙体
	for (int i = 1; i < 8; i++) {
		auto wall = Sprite::create("wall.jpg");
		wall->setScale(0.7f);
		wall->setPosition(300 + 55 * 0.7*i, 508.5);
		wall->setPhysicsBody(PhysicsBody::createBox(Size(55, 55), PhysicsMaterial(10000.0f, 1.0f, 10.0f)));
		wall->getPhysicsBody()->setCategoryBitmask(6);
		wall->getPhysicsBody()->setCollisionBitmask(6);
		wall->getPhysicsBody()->setContactTestBitmask(6);
		wall->getPhysicsBody()->setRotationEnable(false);  // 防止旋转
		addChild(wall, 1);
	}
}

// 上
void GameScene4::moveUp(Ref* sender) {
	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;
	player->runAction(MoveTo::create(0.2, Vec2(x, y + 43)));

	player->runAction(animate);
}
// 下
void GameScene4::moveDown(Ref* sender) {

	auto animation = Animation::createWithSpriteFrames(run, 0.05f);
	auto animate = Animate::create(animation);
	float x = player->getPosition().x;
	float y = player->getPosition().y;

	player->runAction(MoveTo::create(0.2, Vec2(x, y - 43)));


	player->runAction(animate);
}
// 左
void GameScene4::moveLeft(Ref* sender) {
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
void GameScene4::moveRight(Ref* sender) {
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
void GameScene4::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene4::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene4::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene4::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
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

void GameScene4::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
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


void GameScene4::addContactListener() {
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene4::onConcactBegin, this);
	//contactListener->onContactSeparate = CC_CALLBACK_1(GameScene4::onConcactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameScene4::onConcactBegin(PhysicsContact & contact) {
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();
	auto BitmaskA = shapeA->getCollisionBitmask();
	auto BitmaskB = shapeB->getCollisionBitmask();

	if (BitmaskA == 3 && BitmaskB == 3) {
		auto joint1 = PhysicsJointDistance::construct(box1->getPhysicsBody(), box2->getPhysicsBody(), box1->getAnchorPoint(), box2->getAnchorPoint());
		if (m_world)
			m_world->addJoint(joint1);
	}
return true;
}



void GameScene4::judgeWin(float dt) {
	if (light1 != nullptr && light2 != nullptr) {
		if (((box1->getBoundingBox().intersectsRect(light1->getBoundingBox())) &&
			(box2->getBoundingBox().intersectsRect(light2->getBoundingBox()))) ||
			((box1->getBoundingBox().intersectsRect(light2->getBoundingBox())) &&
			(box2->getBoundingBox().intersectsRect(light1->getBoundingBox())))) {

			// 关闭调度器
			unschedule(schedule_selector(GameScene4::judgeWin));
			// 设置下一关按钮
			auto next = MenuItemImage::create(
				"next.png",
				"next.png",
				CC_CALLBACK_1(GameScene4::nextCallback, this));
			next->setScale(0.1f);
			auto menu3 = Menu::create(next, NULL);
			menu3->setPosition(750 + origin.x / 2, origin.y + 300);
			this->addChild(menu3, 1);

			auto labelReturn = Label::createWithTTF("Congratulations!  \nYou have passed ALL six rounds! \nClick to return to the Menu", "fonts/arial.TTF", 18);
			labelReturn->setColor(Color3B::WHITE);
			labelReturn->setPosition(800 + origin.x / 2, origin.y + 200);
			this->addChild(labelReturn);
		}
	}
}


// 下一关按钮响应函数.
void GameScene4::nextCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(MenuScene::createScene());
}

// 重玩按钮响应函数.
void GameScene4::restartCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(GameScene4::createScene());
}
