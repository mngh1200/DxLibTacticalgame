#include "StartScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void StartScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::TEXT, 0, make_shared<Entity::Text>("GAME TITLE", WIN_W / 2, 200, FontType::BLACK_S48, ColorType::TITLE_TEXT, Entity::Text::Align::CENTER));
		objectsControl.addFigure(Layer::TEXT, 1, make_shared<Entity::Text>("PRESS START", WIN_W / 2, 500, FontType::NORMAL_S24, ColorType::SUB_COLOR, Entity::Text::Align::CENTER));
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	*/
	void StartScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->getLayerId() == Layer::BACK && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			// オーバーレイ生成（画面遷移予約）
			createOverlay(false);
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void StartScreen::updateByAnimation()
	{
		if (isCloseOverlayEnded())
		{
			// オーバーレイが閉じたあとに画面遷移
			FrameWork::Game::getInstance().setScreen(new MenuScreen());
		}
		
	}
}