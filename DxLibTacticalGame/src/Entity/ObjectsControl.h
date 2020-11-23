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
		weak_ptr<ViewObject> getViewObjectWp(int layerId, int objectId) const;
		void addObject(int layerId, int objectId, shared_ptr<Object> objSp);
		void addViewObject(int layerId, int objectId, shared_ptr<ViewObject> viewObjSp);
		void removeObject(int layerId, int objectId);
		void removeViewObject(int layerId, int objectId);

		int checkMouseEvent();
		void renderAndDelete();

		void addAnimationObj(int animationId, int layerId, int objectId, bool isView = false);
		void updateAnimation();
		void clearAnimation();

	private:
		//! �}�E�X�C�x���g�Ώۂ̃I�u�W�F�N�g�Q���i�[
		vector<map<int, shared_ptr<Object>>> layerObjList_;

		//! �}�E�X�C�x���g�ΏۈȊO�̃I�u�W�F�N�g�Q���i�[
		vector<map<int, shared_ptr<ViewObject>>> layerViewObjList_;

		//! �A�j���[�V�������̃I�u�W�F�N�g���i�[�ilayerObjList_�����Q�Ɓj
		list<weak_ptr<ViewObject>> animationObjList_;
	};
}