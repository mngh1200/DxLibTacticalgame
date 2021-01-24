#include "BattleScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * 初期処理
	*/
	void BattleScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;
		objectsControl.setLayer(Layer::LEN);

		// 背景
		shared_ptr<Entity::Back> back = make_shared<Entity::Back>(Entity::Back::Screen::BATTLE);
		objectsControl.addObject(Layer::BACK, 0, back);

		// システムメニュ-
		systemMenu_ = make_shared<Entity::ContextMenu>();
		objectsControl.addObject(Layer::CONTEXT_MENU, 0, systemMenu_);

		systemMenu_->addMenuButton(SystemMenuKey::TURN_END, "ターン終了");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_SELECT_SCREEN, "セレクト画面に戻る");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_MENU_SCREEN, "メニュー画面に戻る");

		// マップ（マス）
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>(MapId::STAGE1);
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// バトル管理用クラスの初期処理
		btlMng_.init(map);
		playerBtlCont_.init(map);
		enemyBtlCont_.init(map);

		// ユニット設置(テスト)
		map->setUnit(4, 9, UnitKey::LANCER);
		map->setUnit(5, 9, UnitKey::GUNNER);
		map->setUnit(3, 9, UnitKey::CAVALRY);
		map->setUnit(4, 7, UnitKey::LANCER, true);
		map->setUnit(5, 7, UnitKey::GUNNER, true);
		map->setUnit(3, 7, UnitKey::CAVALRY, true);

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
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン
			{
				playerBtlCont_.updateByEvents(&btlMng_, hitObjSp, x, y, button, eventType);

				// システムメニュー関連イベント
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);

				if (systemMenuKey == SystemMenuKey::TURN_END)
				{
					// ターンエンド処理
					turnEnd();
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN || systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN)
				{
					// 特定画面に戻る
					openScreen_ = systemMenuKey;
					// Utility::ResourceManager::playSound(SoundKind::BACK);
					createOverlay(false);
				}
				else if (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_))
				{
					systemMenu_->hide();

					if (button == MOUSE_INPUT_RIGHT) // 右マウスダウン
					{
						systemMenu_->show(x, y);
					}
				}
				
				
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					// ターン終了ボタン
					if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::TURN_END_BUTTON)
					{
						turnEnd();
					}
				}
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void BattleScreen::updateByAnimation()
	{
		btlMng_.animationCheck(); // バトル管理系の処理

		if (nowScene_ == Scene::ENEMY_TURN) // 敵ターン
		{
			if (enemyBtlCont_.update(&btlMng_))
			{
				turnEnd();
			}
		}


		if (isOpenOverlayEnded()) // オーバーレイ開く
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		
		if (isCloseOverlayEnded()) // オーバレイ閉じる
		{
			if (openScreen_ == SystemMenuKey::BACK_MENU_SCREEN)
			{
				// メニュー画面に戻る
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
			else if (openScreen_ == SystemMenuKey::BACK_SELECT_SCREEN)
			{
				// セレクト画面に戻る
				FrameWork::Game::getInstance().setScreen(new SelectScreen());
			}
		}
	}

	/**
	 * @fn
	 * ターン終了時（別ターンに切替時）
	*/
	void BattleScreen::turnEnd()
	{
		Utility::ResourceManager::playSound(SoundKind::CLICK);

		if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン終了
		{
			nowScene_ = Scene::ENEMY_TURN;
			btlMng_.onStartTurn(false);
		}
		else if (nowScene_ == Scene::ENEMY_TURN) // 敵ターン終了
		{
			nowScene_ = Scene::PLAYER_TURN;
			btlMng_.onStartTurn(true);
		}
	}
}