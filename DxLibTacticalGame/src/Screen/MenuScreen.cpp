#include "MenuScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void MenuScreen::init()
	{
		// テスト
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addObject(Layer::UI, 0, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15, 100, 30)));
		objectsControl.addObject(Layer::UI, 1, make_shared<Entity::Button>(Entity::Shape(WIN_W / 2 - 50, WIN_H / 2 - 15 + 50, 100, 30)));
		objectsControl.addFigure(Layer::UI, 2, make_shared<Entity::Text>("メニュー", WIN_W / 2, 200, DxLib::CreateFontToHandle("メイリオ", 24, 3, DX_FONTTYPE_ANTIALIASING), Entity::Text::Align::CENTER, DxLib::GetColor(124, 104, 95)));

		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void MenuScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// テスト
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
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
	void MenuScreen::updateByAnimation()
	{
		isOpenOverlayEnded();
	}
}