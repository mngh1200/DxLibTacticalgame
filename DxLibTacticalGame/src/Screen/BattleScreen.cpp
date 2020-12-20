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

		// マップ（マス）
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);

		// ユニット設置(テスト)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, false);
		objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);
		map->setUnit(4, 9, playerUnit);

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 0, true);
		objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);
		map->setUnit(4, 0, enemy);

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
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			if (nowScene_ == Scene::PLAYER_TURN)
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					if (hitObjSp->getLayerId() == Layer::MAP) // マップクリック
					{
						shared_ptr<Entity::Map> map = dynamic_pointer_cast<Entity::Map>(hitObjSp);
						map->onClickMass(x, y);
					}
					else if (hitObjSp->getLayerId() == Layer::PLAYER_UNIT) // ユニットクリック
					{
						shared_ptr<Entity::Map> map = dynamic_pointer_cast<Entity::Map>(hitObjSp);
						map->onClickPlayerUnit(x, y);
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
		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		isCloseOverlayEnded();
	}
}