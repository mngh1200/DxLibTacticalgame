#include "ObjectsControl.h"

namespace Entity {
	/**
	 * @fn
	 * レイヤーセット
	 * @param (size) レイヤーの枚数
	 */
	void ObjectsControl::setLayer(int size)
	{
		// すべてクリア
		vector<map<int, shared_ptr<Object>>>().swap(layerObjList_);
		// レイヤー追加
		for (int i = 0; i < size; i++)
		{
			layerObjList_.push_back(map<int, shared_ptr<Entity::Object>>());
		}
	}

	/**
	 * @fn
	 * 指定されたオブジェクト取得
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 * @return 対象オブジェクト
	 */
	weak_ptr<Object> ObjectsControl::getObjectWp(int layerId, int objectId) const
	{
		// TODO: 例外処理
		map<int, shared_ptr<Object>> tmpMap = layerObjList_.at(layerId);
		weak_ptr<Object> wp = tmpMap[objectId];

		return wp;
	}

	/**
	 * @fn
	 * オブジェクト追加
	 * @param (layaerId) レイヤーのID
	 * @param (objectId) オブジェクトのID
	 * @param (objSp)    追加オブジェクト
	 */
	void ObjectsControl::addObject(int layerId, int objectId, shared_ptr<Object> objSp)
	{
		// TODO: 例外処理
		auto mapItr = layerObjList_.begin() + layerId;
		if ((*mapItr)[objectId] == nullptr)
		{
			(*mapItr)[objectId] = objSp;
		}
	}

	/**
	 * @fn
	 * オブジェクト削除
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 */
	void ObjectsControl::removeObject(int layerId, int objectId)
	{
		// TODO: 例外処理
		auto mapItr = layerObjList_.begin() + layerId;
		if ((*mapItr)[objectId] == nullptr)
		{
			(*mapItr).erase(objectId);
		}
	}

	/**
	 * @fn
	 * 全オブジェクト描画
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 * @return 終了判定のとき -1 を返す
	 */
	int ObjectsControl::checkMouseEvent()
	{
		// マウスイベント取得
		int x, y, button, eventType;

		if (DxLib::GetMouseInputLog2(&button, &x, &y, &eventType, true) == -1)
		{
			// マウスイベントログがなかった場合、マウスの位置だけ取得
			eventType = -1;
			button = -1;
			if (DxLib::GetMousePoint(&x, &y) == -1)
			{
				return -1;
			}
		}

		//! 接触オブジェクトを見つけたか
		bool isFoundHitObj = false;
		weak_ptr<Object> hitObjWp;

		// マウスイベント
		for (auto&& layer : layerObjList_)
		{
			for (auto&& objMap : layer)
			{
				shared_ptr<Entity::Object> obj = objMap.second;
				if (obj->checkMouseEvent(x, y, button, &eventType, isFoundHitObj)) {
					// マウス接触
					isFoundHitObj = true;
					hitObjWp = obj;
				}
			}
		}

		// テスト用コード
		shared_ptr<Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->objectType == Entity::Object::ObjectType::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			return -1;
		}

		return 0;
	}


	/**
	 * @fn
	 * 全オブジェクト描画
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 */
	void ObjectsControl::render()
	{
		for (auto layerItr = rbegin(layerObjList_); layerItr != rend(layerObjList_); ++layerItr)
		{
			for (auto objMapItr = layerItr->rbegin(); objMapItr != layerItr->rend(); objMapItr++)
			{
				shared_ptr<Object> obj = (*objMapItr).second;
				obj->render();
			}
		}
	}


}