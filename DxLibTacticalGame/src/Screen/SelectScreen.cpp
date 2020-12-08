#include "MenuScreen.h"
#include "SelectScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void SelectScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>(Entity::Back::Screen::SELECT));
		
		/*
		objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("GAME TITLE", 100, 100, FontType::TITLE, ColorType::TITLE_TEXT));
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, make_shared<Entity::MenuScreenButton>("キャンペーン", 100, 250, 400, 100));


		objectsControl.addObject(Layer::UI, 0, make_shared<Entity::ChapterButton>("チュートリアル", Entity::ChapterButton::H));

		for (int i = 1; i < 10; i++)
		{
			objectsControl.addObject(Layer::UI, i, make_shared<Entity::ChapterButton>("Chapter." + to_string(i), Entity::ChapterButton::H * (i + 1)));
		}
		*/
		


		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void SelectScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			/*
			// ボタンのクリックイベント
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				// 終了ボタン
				if (objId == UIid::QUIT_BUTTON)
				{
					FrameWork::Game::getInstance().finish();
				}
			}
			*/
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void SelectScreen::updateByAnimation()
	{
		isOpenOverlayEnded();
	}

}