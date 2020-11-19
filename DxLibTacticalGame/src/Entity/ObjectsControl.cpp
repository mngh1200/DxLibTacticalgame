#include "ObjectsControl.h"

namespace Entity {
	/**
	 * @fn
	 * ���C���[�Z�b�g
	 * @param (size) ���C���[�̖���
	 */
	void ObjectsControl::setLayer(int size)
	{
		// ���ׂăN���A
		vector<map<int, shared_ptr<Object>>>().swap(layerObjList_);
		// ���C���[�ǉ�
		for (int i = 0; i < size; i++)
		{
			layerObjList_.push_back(map<int, shared_ptr<Entity::Object>>());
		}
	}

	/**
	 * @fn
	 * �w�肳�ꂽ�I�u�W�F�N�g�擾
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 * @return �ΏۃI�u�W�F�N�g
	 */
	weak_ptr<Object> ObjectsControl::getObjectWp(int layerId, int objectId) const
	{
		// TODO: ��O����
		map<int, shared_ptr<Object>> tmpMap = layerObjList_.at(layerId);
		weak_ptr<Object> wp = tmpMap[objectId];

		return wp;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�ǉ�
	 * @param (layaerId) ���C���[��ID
	 * @param (objectId) �I�u�W�F�N�g��ID
	 * @param (objSp)    �ǉ��I�u�W�F�N�g
	 */
	void ObjectsControl::addObject(int layerId, int objectId, shared_ptr<Object> objSp)
	{
		// TODO: ��O����
		auto mapItr = layerObjList_.begin() + layerId;
		if ((*mapItr)[objectId] == nullptr)
		{
			(*mapItr)[objectId] = objSp;
		}
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 */
	void ObjectsControl::removeObject(int layerId, int objectId)
	{
		// TODO: ��O����
		auto mapItr = layerObjList_.begin() + layerId;
		if ((*mapItr)[objectId] == nullptr)
		{
			(*mapItr).erase(objectId);
		}
	}

	/**
	 * @fn
	 * �S�I�u�W�F�N�g�`��
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 * @return �I������̂Ƃ� -1 ��Ԃ�
	 */
	int ObjectsControl::checkMouseEvent()
	{
		// �}�E�X�C�x���g�擾
		int x, y, button, eventType;

		if (DxLib::GetMouseInputLog2(&button, &x, &y, &eventType, true) == -1)
		{
			// �}�E�X�C�x���g���O���Ȃ������ꍇ�A�}�E�X�̈ʒu�����擾
			eventType = -1;
			button = -1;
			if (DxLib::GetMousePoint(&x, &y) == -1)
			{
				return -1;
			}
		}

		//! �ڐG�I�u�W�F�N�g����������
		bool isFoundHitObj = false;
		weak_ptr<Object> hitObjWp;

		// �}�E�X�C�x���g
		for (auto&& layer : layerObjList_)
		{
			for (auto&& objMap : layer)
			{
				shared_ptr<Entity::Object> obj = objMap.second;
				if (obj->checkMouseEvent(x, y, button, &eventType, isFoundHitObj)) {
					// �}�E�X�ڐG
					isFoundHitObj = true;
					hitObjWp = obj;
				}
			}
		}

		// �e�X�g�p�R�[�h
		shared_ptr<Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp && hitObjSp->objectType == Entity::Object::ObjectType::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			return -1;
		}

		return 0;
	}


	/**
	 * @fn
	 * �S�I�u�W�F�N�g�`��
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
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