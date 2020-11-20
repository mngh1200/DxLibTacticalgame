#pragma once
#include <climits>
#include <memory>
#include <vector>
#include <map>
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
		ObjectsControl() : layerObjList_{ {} } {};
		~ObjectsControl() {};

		
		void setLayer(int size);
		weak_ptr<Object> getObjectWp(int layerId, int objectId) const;
		void addObject(int layerId, int objectId, shared_ptr<Object> objSp, Shape shape = Shape());
		void removeObject(int layerId, int objectId);

		int checkMouseEvent();
		void renderAndDelete();

	private:
		//! 全オブジェクトを格納
		vector<map<int, shared_ptr<Entity::Object>>> layerObjList_;
	};
}