#ifndef __GAME_SCENE4_H__
#define __GAME_SCENE4_H__

#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC;

class GameScene4 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld * world);

	void addPlayer();
	void addBox();
	void addWall();
	void moveUp(Ref* sender);
	void moveDown(Ref* sender);
	void moveLeft(Ref* sender);
	void moveRight(Ref* sender);
	void addKeyboardListener();
	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);
	void judgeWin(float dt);
	void nextCallback(Ref * pSender);
	void restartCallback(Ref * pSender);
	void addContactListener();
	bool onConcactBegin(PhysicsContact & contact);
	//bool onConcactSeparate(PhysicsContact & contact);
	CREATE_FUNC(GameScene4);
	//void preloadMusic();
	void helpReplaceScene(cocos2d::Ref* pSender);

private:
	cocos2d::Size visibleSize;
	PhysicsWorld* m_world;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	int dtime;
	cocos2d::ProgressTimer* pT;
	char lastCid;
	cocos2d::Sprite* player;
	cocos2d::Sprite* box1;
	cocos2d::Sprite* box2;
	//cocos2d::Sprite* box3;
	//cocos2d::Sprite* box4;
	//cocos2d::Sprite* box5;

	cocos2d::Sprite* light1;
	cocos2d::Sprite* light2;
	//cocos2d::Sprite* light3;
	//cocos2d::Sprite* light4;
	//cocos2d::Sprite* light5;
	cocos2d::Vector<SpriteFrame*> run;


};

#endif // __GAME_SCENE_H__

