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
		objectsControl.addObject(Layer::MAP, 0, make_shared<Entity::Map>());

		// ユニット設置(テスト)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, false);
		objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 0, true);
		objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);

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

						shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
						if (selectedUnitSp)
						{
							// 移動
							selectedUnitSp->move(Entity::Map::getMassX(x), Entity::Map::getMassY(y));
						}

					}
					else if (hitObjSp->getLayerId() == Layer::PLAYER_UNIT) // ユニットクリック
					{
						if (deselectUnit())
						{
							// ユニット選択
							shared_ptr<Entity::Unit> unit = dynamic_pointer_cast<Entity::Unit>(hitObjSp);
							if (unit->select(true))
							{
								selectedUnit_ = unit;
							}
						}
					}
					else
					{
						deselectUnit();
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

	/**
	 * @fn
	 * ユニットの選択状態を解除
	 * @return 解除できた場合と選択済みのユニットがない場合はtrueを返す
	*/
	bool BattleScreen::deselectUnit()
	{
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				return true;
			}
			return false;
		}

		// 選択済みのユニットがない場合もtureを返す
		return true;
	}

}