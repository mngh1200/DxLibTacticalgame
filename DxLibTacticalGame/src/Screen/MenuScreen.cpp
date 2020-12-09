#include "MenuScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void MenuScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("GAME TITLE", 100, 100, FontType::TITLE, ColorType::TITLE_TEXT));
		/*
		Entity::TextButton templateTextButton = Entity::TextButton()
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, make_shared<Entity::TextButton>("キャンペーン", 100, 250, 400, 100));
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, make_shared<Entity::MenuScreenButton>("終了", 100, 375, 400, 70));
		*/

		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void MenuScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			// ボタンのクリックイベント
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == UIid::CAMPAIN_BUTTON) // キャンペーンボタン
				{
					createOverlay(false); // （アニメーション後に）セレクト画面に画面遷移
				}
				else if (objId == UIid::QUIT_BUTTON) // 終了ボタン
				{
					FrameWork::Game::getInstance().finish();
				}
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void MenuScreen::updateByAnimation()
	{
		isOpenOverlayEnded();

		if (isCloseOverlayEnded())
		{
			// セレクト画面に画面遷移
			FrameWork::Game::getInstance().setScreen(new SelectScreen());
		}
		
	}
}