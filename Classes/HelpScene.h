#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include <string>
#include <vector>
using std::vector;
using std::string;
using namespace cocos2d::ui;
using namespace cocos2d::network;
#include "cocos2d.h"

class HelpScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();


	// implement the "static create()" method manually
	CREATE_FUNC(HelpScene);

	void backButtonClick(Ref* pSender);
	void playButtonClick(Ref* pSender);

};

#endif // __HELP_SCENE_H__

