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
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// バトル管理クラス
		btlMng = Battle::BattleManager(map, Layer::UI);

		// ユニット設置(テスト)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, UnitKey::LANCER);
		if (map->setUnit(playerUnit))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);
		}

		shared_ptr<Entity::Unit> playerUnit2 = make_shared<Entity::Unit>();
		playerUnit2->init(5, 9, UnitKey::GUNNER);
		if (map->setUnit(playerUnit2))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 1, playerUnit2);
		}

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 7, UnitKey::LANCER, true);
		if (map->setUnit(enemy))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);
		}

		shared_ptr<Entity::Unit> enemy2 = make_shared<Entity::Unit>();
		enemy2->init(5, 7, UnitKey::LANCER, true);
		if (map->setUnit(enemy2))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 1, enemy2);
		}
		
		

		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // プレイヤーターン
			{
				btlMng.updateByEvents(hitObjSp, x, y, button, eventType);

				// システムメニュー関連イベント
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);

				if (systemMenuKey == SystemMenuKey::TURN_END)
				{
					// ターンエンド処理
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN || systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN)
				{
					// 特定画面に戻る
					openScreen_ = systemMenuKey;
					createOverlay(false);
				}

				if (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_))
				{
					systemMenu_->hide();

					if (button == MOUSE_INPUT_RIGHT) // 右マウスダウン
					{
						systemMenu_->show(x, y);
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
		btlMng.animationCheck(); // バトル管理系の処理

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
}