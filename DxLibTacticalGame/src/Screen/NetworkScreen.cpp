#include "NetworkScreen.h"

namespace Screen
{
	/**
	 * @fn
	 * 初期処理
	*/
	void NetworkScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		objectsControl.setLayer(Layer::LEN);

		// 背景
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());

		// 部屋を作るボタン
		shared_ptr<Entity::TextButton> createRoomButton = make_shared<Entity::TextButton>();
		createRoomButton->setText("部屋を作る", FontType::BLACK_S48);
		createRoomButton->setShape(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		createRoomButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::CREATE_ROOM_BUTTON, createRoomButton);

		// 部屋を探すボタン
		shared_ptr<Entity::TextButton> searchRoomButton = make_shared<Entity::TextButton>();
		searchRoomButton->setText("部屋を探す", FontType::BLACK_S48);
		searchRoomButton->setShape(BUTTON_X, BUTTON_Y + BUTTON_H + BUTTON_MARGIN, BUTTON_W, BUTTON_H);
		searchRoomButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::SEARCH_ROOM_BUTTON, searchRoomButton);

		// 戻るボタン
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>();
		backBtn->setShape(BACK_BTN_X, BACK_BTN_Y, BACK_BTN_W, BACK_BTN_H);
		backBtn->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setText("戻る", FontType::BLACK_S32);
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, backBtn);

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
	void NetworkScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (nowScene_ == Scene::HOST) // 部屋を作る（小画面表示時処理）
		{
			int result = hostManager_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == NetworkHost::Result::CANCEL) // キャンセル
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == NetworkHost::Result::SET_RULE) // ルール設定
			{
				nowScene_ = Scene::RULE_SET;
				ruleSetting_.start(hostManager_.getNetHandle());
			}
			return;
		}
		if (nowScene_ == Scene::RULE_SET) // ルール設定（小画面表示時処理）
		{
			int result = ruleSetting_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == RuleSetting::Result::CANCEL) // 閉じる or 切断
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == RuleSetting::Result::START_BATTLE) // バトル開始
			{
				startBattle(ruleSetting_.getNetHandle(), ruleSetting_.getRuleData(), true);
			}
			return;
		}
		else if (nowScene_ == Scene::CLIENT) // 部屋を探す（小画面表示時処理）
		{
			int result = clientManager_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == NetworkClient::Result::CANCEL)
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == NetworkClient::Result::START_BATTLE)
			{
				startBattle(clientManager_.getNetHandle(), clientManager_.getRuleData(), false);
			}
			return;
		}

		// イベント対象オブジェクトがない場合、終了
		if (!hitObjSp)
		{
			return;
		}

		// ボタンのクリックイベント
		if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			int objId = hitObjSp->getObjectId();

			if (nowScene_ == Scene::INIT)
			{
				if (objId == UIid::CREATE_ROOM_BUTTON) // 「部屋を作る」ボタン
				{
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					nowScene_ = Scene::HOST;
					hostManager_.start();
				}
				else if (objId == UIid::SEARCH_ROOM_BUTTON) // 「部屋を探す」ボタン
				{
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					nowScene_ = Scene::CLIENT;
					clientManager_.start();
				}
				else if (objId == UIid::QUIT_BUTTON) // 終了ボタン
				{
					// 画面遷移
					Utility::ResourceManager::playSound(SoundKind::BACK);
					nextScreen_ = new MenuScreen();
					createOverlay(false);
				}
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void NetworkScreen::updateByAnimation()
	{
		if (isOpenOverlayEnded())
		{
			nowScene_ = initScene_;
			if (nowScene_ == Scene::RULE_SET)
			{
				ruleSetting_.start(inheritNetHandle_, inheritMapId_, inheritUnitNum_);
			}
			else if (nowScene_ == Scene::CLIENT)
			{
				clientManager_.startByRetry(inheritNetHandle_);
			}
		}

		if (isCloseOverlayEnded())
		{
			FrameWork::Game::getInstance().setScreen(nextScreen_); // 画面遷移
		}
		
	}

	/**
	 * @fn
	 * 再戦時に必要な情報をセット
	 * @param (netHandle) ネットハンドル
	 * @param (isServer) サーバー側であるか
	 * @param (mapId) マップID
	 * @param (unitNum) ユニット数
	*/
	void NetworkScreen::setRetryParam(bool isServer, int netHandle, int mapId, int unitNum)
	{
		inheritNetHandle_ = netHandle;
		inheritMapId_ = mapId;
		inheritUnitNum_ = unitNum;

		if (isServer) // サーバー側
		{
			initScene_ = Scene::RULE_SET;
		}
		else // クライアント側
		{
			initScene_ = Scene::CLIENT;
		}
	}

	/**
	 * @fn
	 * バトル画面への遷移処理
	 * @param (ruleData) ルール設定データ
	*/
	void NetworkScreen::startBattle(int netHandler_, const RuleData& ruleData, bool isServer)
	{
		BattleScreen* bs = new BattleScreen();
		bs->prepareNetMatch(netHandler_, isServer, ruleData.mapId, ruleData.unitNum, isServer != ruleData.isClientFirst);
		nextScreen_ = bs;

		createOverlay(false);
	}

}