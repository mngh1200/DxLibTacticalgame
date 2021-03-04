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
 * @brief ���ׂẲ�ʓ��v�f���Ǘ�����N���X
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

		//! �t�H�[�J�X���̃I�u�W�F�N�g
		weak_ptr<Object> focusObject;

	private:
		constexpr static int MAX_AUTO_OBJECT_ID = 10000; //! �����̔Ԃ���I�u�W�F�N�gID�̍ő�l

		//! �}�E�X�C�x���g�Ώۂ̃I�u�W�F�N�g�Q���i�[
		vector<map<int, shared_ptr<Object>>> layerObjList_;

		//! �}�E�X�C�x���g�ΏۈȊO�̃I�u�W�F�N�g�Q���i�[
		vector<map<int, shared_ptr<Figure>>> layerViewObjList_;

		//! �A�j���[�V�������̃I�u�W�F�N�g���i�[�ilayerObjList_�����Q�Ɓj
		list<weak_ptr<Figure>> animationObjList_;
	};
}