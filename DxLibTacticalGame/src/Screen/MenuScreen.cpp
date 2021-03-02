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

		constexpr int PADDING_LEFT = 100; //! 左側余白
		constexpr int PADDING_TOP = 100; //! 上側余白
		constexpr int BUTTON_TOP = 250; //! ボタンY座標
		constexpr int BUTTON_MARGIN_Y = 20; //! ボタンの縦余白
		constexpr int BUTTON_WIDTH = 400; //! ボタンの幅
		constexpr int BUTTON_HEIGHT_S = 85; //! ボタン高さ（Sサイズ）
		constexpr int BUTTON_HEIGHT_L = 110; //! ボタンの高さ（Lサイズ）


		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());
		objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("Spirit Wars", PADDING_LEFT, PADDING_TOP, FontType::BLACK_S48, ColorType::TITLE_TEXT));

		// メニューボタン
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, make_shared<Entity::MenuScreenButton>("キャンペーン", PADDING_LEFT, BUTTON_TOP, BUTTON_WIDTH, BUTTON_HEIGHT_L));
		objectsControl.addObject(Layer::UI, UIid::NETWORK_BUTTON, make_shared<Entity::MenuScreenButton>("通信対戦", PADDING_LEFT, BUTTON_TOP + (BUTTON_HEIGHT_L + BUTTON_MARGIN_Y) * 1, BUTTON_WIDTH, BUTTON_HEIGHT_L));
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, make_shared<Entity::MenuScreenButton>("終了", PADDING_LEFT, BUTTON_TOP + (BUTTON_HEIGHT_L + BUTTON_MARGIN_Y) * 2, BUTTON_WIDTH, BUTTON_HEIGHT_S));


		// オーバーレイセット
		createOverlay(true);
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
					nextScreen_ = new SelectScreen();
					createOverlay(false); // （アニメーション後に）セレクト画面に画面遷移
				}
				else if (objId == UIid::NETWORK_BUTTON) // キャンペーンボタン
				{
					nextScreen_ = new NetworkScreen();
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

		if (isCloseOverlayEnded() && nextScreen_ != nullptr)
		{
			// セレクト画面に画面遷移
			FrameWork::Game::getInstance().setScreen(nextScreen_);
		}
		
	}
}