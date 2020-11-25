#include "StartScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void StartScreen::init()
	{
		// テスト
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(InitLayer::LEN);
		objectsControl.addObject(InitLayer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addObject(InitLayer::BUTTON, 0, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15, 100, 30)));
		objectsControl.addObject(InitLayer::BUTTON, 1, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15 + 50, 100, 30)));
		objectsControl.addFigure(InitLayer::BUTTON, 2, make_shared<Entity::Text>("タイトル", WIN_W / 2 - 10, 200));
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void StartScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// テスト
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getLayerId() == InitLayer::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			int objId = hitObjSp->getObjectId();
			if (objId == 0)
			{
				FrameWork::Game::getInstance().finish();
			}
			else if (objId == 1)
			{
				hitObjSp->destroy();
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void StartScreen::updateByAnimation()
	{
	}
}