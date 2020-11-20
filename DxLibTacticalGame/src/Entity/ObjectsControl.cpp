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
		weak_ptr<Object> wp;

		try
		{
			map<int, shared_ptr<Object>> tmpMap = layerObjList_.at(layerId);
			wp = tmpMap[objectId];
		}
		catch (out_of_range&)
		{
			DxLib::AppLogAdd("Error:ObjectsControl::getObjectWp: out_of_range\n");
		}

		return wp;
	}

	/**
	 * @fn
	 * オブジェクト追加
	 * @param (layaerId) レイヤーのID
	 * @param (objectId) オブジェクトのID
	 * @param (objSp)    追加オブジェクト
	 */
	void ObjectsControl::addObject(int layerId, int objectId, shared_ptr<Object> objSp, Shape shape)
	{
		if (0 <= layerId && layerId < layerObjList_.size() ) // 存在するレイヤーであるかチェック
		{
			objSp->init(layerId, objectId, shape);
			auto mapItr = layerObjList_.begin() + layerId;
			(*mapItr).emplace(objectId, objSp); // 新規追加のみ
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
		if (0 <= layerId && layerId < layerObjList_.size()) // 存在するレイヤーであるかチェック
		{
			auto mapItr = layerObjList_.begin() + layerId;
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

		// 接触オブジェクトを見つけたか
		bool isFoundHitObj = false;

		// 接触オブジェクトの情報
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

		if (hitObjSp && hitObjSp->getLayerId() == InitLayer::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			int objId = hitObjSp->getObjectId();
			if (objId == 0)
			{
				return -1;
			}
			else if (objId == 1)
			{
				hitObjSp->destroy();
			}
		}

		return 0;
	}


	/**
	 * @fn
	 * 全オブジェクト描画および削除予約されているオブジェクトの削除
	 */
	void ObjectsControl::renderAndDelete()
	{
		for (auto layerItr = rbegin(layerObjList_); layerItr != rend(layerObjList_); ++layerItr)
		{
			auto& targetLayer = (*layerItr);
			for (auto objMapItr = layerItr->rbegin(); objMapItr != layerItr->rend(); )
			{
				shared_ptr<Object> obj = (*objMapItr).second;

				if (obj->isDelete()) // 削除
				{
					targetLayer.erase((*objMapItr).first);
				}
				else // 描画
				{
					obj->render();
					objMapItr++;
				}
			}
		}
	}


}