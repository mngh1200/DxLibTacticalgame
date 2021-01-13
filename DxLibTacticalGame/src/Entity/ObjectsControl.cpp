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
		vector<map<int, shared_ptr<Figure>>>().swap(layerViewObjList_);
		clearAnimation();
		// レイヤー追加
		for (int i = 0; i < size; i++)
		{
			layerObjList_.push_back(map<int, shared_ptr<Entity::Object>>());
			layerViewObjList_.push_back(map<int, shared_ptr<Entity::Figure>>());
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
	 * 指定されたViewオブジェクト取得
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 * @return 対象オブジェクト
	 */
	weak_ptr<Figure> ObjectsControl::getFigureWp(int layerId, int objectId) const
	{
		weak_ptr<Figure> wp;

		try
		{
			map<int, shared_ptr<Figure>> tmpMap = layerViewObjList_.at(layerId);

			wp = tmpMap[objectId];
		}
		catch (out_of_range&)
		{
			DxLib::AppLogAdd("Error:ObjectsControl::getFigureWp: out_of_range\n");
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
	void ObjectsControl::addObject(int layerId, int objectId, shared_ptr<Object> objSp)
	{
		if (0 <= layerId && (unsigned int)layerId < layerObjList_.size()) // 存在するレイヤーであるかチェック
		{
			objSp->init(layerId, objectId);
			auto mapItr = layerObjList_.begin() + layerId;				 
			(*mapItr).emplace(objectId, objSp); // 新規追加のみ
		}
	}

	/**
	 * @fn
	 * Viewオブジェクト追加
	 * @param (layaerId) レイヤーのID
	 * @param (objectId) オブジェクトのID
	 * @param (objSp)    追加オブジェクト
	 */
	void ObjectsControl::addFigure(int layerId, int objectId, shared_ptr<Figure> objSp)
	{
		if (0 <= layerId && (unsigned int)layerId < layerObjList_.size()) // 存在するレイヤーであるかチェック
		{
			objSp->init(layerId, objectId);
			auto mapItr = layerViewObjList_.begin() + layerId;
			(*mapItr).emplace(objectId, objSp); // 新規追加のみ
		}
	}

	/**
	 * @fn
	 * オブジェクト追加（オブジェクトIDは自動採番）
	 * @param (layaerId) レイヤーのID
	 * @param (objSp)    追加オブジェクト
	 */
	void ObjectsControl::addObject(int layerId, shared_ptr<Object> objSp)
	{
		if (0 <= layerId && (unsigned int)layerId < layerObjList_.size()) // 存在するレイヤーであるかチェック
		{
			int autoObjectId = 0;
			auto mapItr = layerObjList_.begin() + layerId;

			// 重複しないオブジェクトIDを探索
			for (int i = 0; i < MAX_AUTO_OBJECT_ID; ++i)
			{
				if (!mapItr->count(autoObjectId))
				{
					break;
				}
				++autoObjectId;
			}

			addObject(layerId, autoObjectId, objSp);
		}
	}

	/**
	 * @fn
	 * Viewオブジェクト追加（オブジェクトIDは自動採番）
	 * @param (layaerId) レイヤーのID
	 * @param (objSp)    追加Viewオブジェクト
	 */
	void ObjectsControl::addFigure(int layerId, shared_ptr<Figure> viewObjSp)
	{
		if (0 <= layerId && (unsigned int)layerId < layerViewObjList_.size()) // 存在するレイヤーであるかチェック
		{
			int autoObjectId = 0;
			auto mapItr = layerViewObjList_.begin() + layerId;

			// 重複しないオブジェクトIDを探索
			for (int i = 0; i < MAX_AUTO_OBJECT_ID; ++i)
			{
				if (!mapItr->count(autoObjectId))
				{
					break;
				}
				++autoObjectId;
			}

			addFigure(layerId, autoObjectId, viewObjSp);
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
		if (0 <= layerId && (unsigned int)layerId < layerObjList_.size()) // 存在するレイヤーであるかチェック
		{
			auto mapItr = layerObjList_.begin() + layerId;
			(*mapItr).erase(objectId);
		}

	}

	/**
	 * @fn
	 * オブジェクト削除
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 */
	void ObjectsControl::removeFigure(int layerId, int objectId)
	{
		if (0 <= layerId && (unsigned int)layerId < layerViewObjList_.size()) // 存在するレイヤーであるかチェック
		{
			auto mapItr = layerViewObjList_.begin() + layerId;
			(*mapItr).erase(objectId);
		}
	}

	/**
	 * @fn
	 * 全オブジェクトのマウスイベント
	 * @param (x) マウスのx座標の参照
	 * @param (y) マウスのy座標の参照
	 * @param (button) どのボタンが対象か返すための参照
	 * @param (eventType) イベントタイプを返すための参照
	 * @return 終了判定のとき -1 を返す
	 */
	weak_ptr<Object> ObjectsControl::checkMouseEvent(int* x, int* y, int* button, int* eventType)
	{
		if (DxLib::GetMouseInputLog2(button, x, y, eventType, true) == -1)
		{
			// マウスイベントログがなかった場合、マウスの位置だけ取得
			*eventType = -1;
			*button = -1;
			if (DxLib::GetMousePoint(x, y) == -1)
			{
				return weak_ptr<Object>();
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
				if (obj->checkMouseEvent(*x, *y, *button, eventType, isFoundHitObj)) {
					// マウス接触
					isFoundHitObj = true;
					hitObjWp = obj;
				}
			}
		}

		return hitObjWp;
	}


	/**
	 * @fn
	 * 全オブジェクト描画および削除予約されているオブジェクトの削除
	 */
	void ObjectsControl::renderAndDelete()
	{
		// layerViewObjList_用のイテレータ
		auto layerViewItr = rbegin(layerViewObjList_);
		auto layerVIewItrEnd = rend(layerViewObjList_);

		for (auto layerItr = rbegin(layerObjList_); layerItr != rend(layerObjList_); ++layerItr)
		{
			// layerObjList_
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

			// layerViewObjList_
			if (layerViewItr == layerVIewItrEnd)
			{
				continue;
			}

			for (auto objMapItr = layerViewItr->rbegin(); objMapItr != layerViewItr->rend(); )
			{
				shared_ptr<Figure> obj = (*objMapItr).second;

				if (obj->isDelete()) // 削除
				{
					(*layerViewItr).erase((*objMapItr).first);
				}
				else // 描画
				{
					obj->render();
					objMapItr++;
				}
			}
			++layerViewItr;
		}
	}

	/**
	 * @fn
	 * IDで指定されたオブジェクトをアニメーション中のアブジェクトリストに追加
	 * @param (animationId) セットするアニメーションID
	 * @param (layaerId) 対象レイヤーのID
	 * @param (objectId) 対象オブジェクトのID
	 * @param (isView)   true: layerViewObjList_から取得, false: layerObjList_から取得
	 */
	void ObjectsControl::addAnimationObj(int animationId, int layerId, int objectId, bool isView)
	{
		weak_ptr<Figure> objWp;
		if (isView)
		{
			objWp = getFigureWp(layerId, objectId);
		}
		else
		{
			objWp = getObjectWp(layerId, objectId);
		}
		

		shared_ptr<Figure> objSP = objWp.lock();

		if (objSP)
		{
			// 既にアニメーション中の場合は追加しない
			if (objSP->createAnimation(animationId))
			{
				objSP->animationId_ = animationId;
				animationObjList_.push_front(objWp);
			}
		}
	}

	/**
	 * @fn
	 * 1フレーム分のオブジェクトのアニメーション処理を実行
	 */
	void ObjectsControl::updateAnimation()
	{
		for (auto objItr = animationObjList_.begin(); objItr != animationObjList_.end();)
		{
			bool isRemove = false;
			shared_ptr<Figure> objSp = (*objItr).lock();
			if (objSp)
			{
				isRemove = objSp->animationUpdate();

				if (isRemove)
				{
					// アニメーションIDを未指定状態にする
					objSp->animationId_ = -1;
				}
			}
			else
			{
				// 存在しない個体を参照している場合
				isRemove = true;
			}

			if (isRemove)
			{
				// リストから削除
				objItr = animationObjList_.erase(objItr);
			}
			else
			{
				++objItr;
			}
		}
	}

	/**
	 * @fn
	 * アニメーションを強制的にすべて終了
	 */
	void ObjectsControl::clearAnimation()
	{
		list<weak_ptr<Figure>>().swap(animationObjList_);
	}


}