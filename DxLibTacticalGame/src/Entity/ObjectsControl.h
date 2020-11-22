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

using namespace std;

namespace Entity
{
	class ObjectsControl
	{

	public:
		ObjectsControl() : layerObjList_{ {} }, animationObjList_{ {} } {};
		~ObjectsControl() {};

		
		void setLayer(int size);
		weak_ptr<Object> getObjectWp(int layerId, int objectId) const;
		void addObject(int layerId, int objectId, shared_ptr<Object> objSp, Shape shape = Shape());
		void removeObject(int layerId, int objectId);

		int checkMouseEvent();
		void renderAndDelete();

		void addAnimationObj(int animationId, int layerId, int objectId, bool isView);
		void updateAnimation();
		void clearAnimation();

	private:
		//! 全オブジェクトを格納
		vector<map<int, shared_ptr<Object>>> layerObjList_;

		//! アニメーション中のオブジェクトを格納（layerObjList_から弱参照）
		list<weak_ptr<ViewObject>> animationObjList_;
	};
}