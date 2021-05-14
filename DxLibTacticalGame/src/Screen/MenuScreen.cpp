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

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		constexpr int BUTTON_TOP = 320; //! ボタンY座標
		constexpr int BUTTON_MARGIN_Y = 25; //! ボタンの縦余白


		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());

		// タイトル
		
		// objectsControl.addFigure(Layer::UI, UIid::TITLE, make_shared<Entity::Text>("Spirit Wars", PADDING_LEFT, PADDING_TOP, (int)FontType::BLACK_S48, (int)ColorType::TITLE_TEXT));

		shared_ptr<ImageView> titleImage = make_shared<ImageView>();
		titleImage->setImageId(rm.getImage(ImageType::IMAGE, ImageId::TITLE_IMAGE_ID));
		titleImage->setPos(TITLE_X, TITLE_Y);
		objectsControl.addFigure(Layer::UI, UIid::TITLE, titleImage);


		// メニューボタン
		shared_ptr<MenuScreenButton> campaignBtn = make_shared<MenuScreenButton>();
		campaignBtn->setY(BUTTON_TOP);
		campaignBtn->setText("キャンペーン");
		objectsControl.addObject(Layer::UI, UIid::CAMPAIN_BUTTON, campaignBtn);

		shared_ptr<MenuScreenButton> netMatchBtn = make_shared<MenuScreenButton>();
		netMatchBtn->setY(BUTTON_TOP + (MenuScreenButton::H + BUTTON_MARGIN_Y) * 1);
		netMatchBtn->setText("ローカル通信対戦");
		objectsControl.addObject(Layer::UI, UIid::NETWORK_BUTTON, netMatchBtn);

		shared_ptr<MenuScreenButton> quitBtn = make_shared<MenuScreenButton>();
		quitBtn->setY(BUTTON_TOP + (MenuScreenButton::H + BUTTON_MARGIN_Y) * 2);
		quitBtn->setText("終了");
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, quitBtn);

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

		// イベント対象オブジェクトがない場合、終了
		if (!hitObjSp)
		{
			return;
		}

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