#pragma once
#include <climits>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>
#include "Object.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file ObjectsControl.h
 * @brief すべての画面内要素を管理するクラス
 */

using namespace std;

namespace Entity
{
	class ObjectsControl
	{

	public:
		ObjectsControl() : layerObjList_{ {} }, animationObjList_{ {} } {};
		~ObjectsControl() {};

		
		void setLayer(int size);
		void clear();
		weak_ptr<Object> getObjectWp(int layerId, int objectId) const;
		weak_ptr<Figure> getFigureWp(int layerId, int objectId) const;
		void addObject(int layerId, int objectId, shared_ptr<Object> objSp);
		void addFigure(int layerId, int objectId, shared_ptr<Figure> viewObjSp);
		void addObject(int layerId, shared_ptr<Object> objSp);
		void addFigure(int layerId, shared_ptr<Figure> viewObjSp);
		void removeObject(int layerId, int objectId);
		void removeFigure(int layerId, int objectId);

		weak_ptr<Object> checkMouseEvent(int* x, int* y, int* button, int* eventType);
		void renderAndDelete();

		void addAnimationObj(int animationId, int layerId, int objectId, bool isView = false);
		void updateAnimation();
		void clearAnimation();

		//! フォーカス中のオブジェクト
		weak_ptr<Object> focusObject;

	private:
		constexpr static int MAX_AUTO_OBJECT_ID = 10000; //! 自動採番するオブジェクトIDの最大値

		//! マウスイベント対象のオブジェクト群を格納
		vector<map<int, shared_ptr<Object>>> layerObjList_;

		//! マウスイベント対象以外のオブジェクト群を格納
		vector<map<int, shared_ptr<Figure>>> layerViewObjList_;

		//! アニメーション中のオブジェクトを格納（layerObjList_から弱参照）
		list<weak_ptr<Figure>> animationObjList_;
	};
}