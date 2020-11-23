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
		vector<map<int, shared_ptr<ViewObject>>>().swap(layerViewObjList_);
		clearAnimation();
		// ���C���[�ǉ�
		for (int i = 0; i < size; i++)
		{
			layerObjList_.push_back(map<int, shared_ptr<Entity::Object>>());
			layerViewObjList_.push_back(map<int, shared_ptr<Entity::ViewObject>>());
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
	 * �w�肳�ꂽView�I�u�W�F�N�g�擾
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 * @return �ΏۃI�u�W�F�N�g
	 */
	weak_ptr<ViewObject> ObjectsControl::getViewObjectWp(int layerId, int objectId) const
	{
		weak_ptr<ViewObject> wp;

		try
		{
			map<int, shared_ptr<ViewObject>> tmpMap = layerViewObjList_.at(layerId);

			wp = tmpMap[objectId];
		}
		catch (out_of_range&)
		{
			DxLib::AppLogAdd("Error:ObjectsControl::getViewObjectWp: out_of_range\n");
		}

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
		if (0 <= layerId && layerId < layerObjList_.size()) // ���݂��郌�C���[�ł��邩�`�F�b�N
		{
			objSp->init(layerId, objectId);
			auto mapItr = layerObjList_.begin() + layerId;				 
			(*mapItr).emplace(objectId, objSp); // �V�K�ǉ��̂�
		}
	}

	/**
	 * @fn
	 * View�I�u�W�F�N�g�ǉ�
	 * @param (layaerId) ���C���[��ID
	 * @param (objectId) �I�u�W�F�N�g��ID
	 * @param (objSp)    �ǉ��I�u�W�F�N�g
	 */
	void ObjectsControl::addViewObject(int layerId, int objectId, shared_ptr<ViewObject> objSp)
	{
		if (0 <= layerId && layerId < layerObjList_.size()) // ���݂��郌�C���[�ł��邩�`�F�b�N
		{
			objSp->init(layerId, objectId);
			auto mapItr = layerViewObjList_.begin() + layerId;
			(*mapItr).emplace(objectId, objSp); // �V�K�ǉ��̂�
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
		if (0 <= layerId && layerId < layerObjList_.size()) // ���݂��郌�C���[�ł��邩�`�F�b�N
		{
			auto mapItr = layerObjList_.begin() + layerId;
			(*mapItr).erase(objectId);
		}

	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 */
	void ObjectsControl::removeViewObject(int layerId, int objectId)
	{
		if (0 <= layerId && layerId < layerObjList_.size()) // ���݂��郌�C���[�ł��邩�`�F�b�N
		{
			auto mapItr = layerViewObjList_.begin() + layerId;
			(*mapItr).erase(objectId);
		}
	}

	/**
	 * @fn
	 * �S�I�u�W�F�N�g�̃}�E�X�C�x���g
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

		// �ڐG�I�u�W�F�N�g����������
		bool isFoundHitObj = false;

		// �ڐG�I�u�W�F�N�g�̏��
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
	 * �S�I�u�W�F�N�g�`�您��э폜�\�񂳂�Ă���I�u�W�F�N�g�̍폜
	 */
	void ObjectsControl::renderAndDelete()
	{
		// layerViewObjList_�p�̃C�e���[�^
		auto layerViewItr = rbegin(layerViewObjList_);
		auto layerVIewItrEnd = rend(layerViewObjList_);

		for (auto layerItr = rbegin(layerObjList_); layerItr != rend(layerObjList_); ++layerItr)
		{
			// layerObjList_
			auto& targetLayer = (*layerItr);
			for (auto objMapItr = layerItr->rbegin(); objMapItr != layerItr->rend(); )
			{
				shared_ptr<Object> obj = (*objMapItr).second;

				if (obj->isDelete()) // �폜
				{
					targetLayer.erase((*objMapItr).first);
				}
				else // �`��
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
				shared_ptr<ViewObject> obj = (*objMapItr).second;

				if (obj->isDelete()) // �폜
				{
					targetLayer.erase((*objMapItr).first);
				}
				else // �`��
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
	 * ID�Ŏw�肳�ꂽ�I�u�W�F�N�g���A�j���[�V�������̃A�u�W�F�N�g���X�g�ɒǉ�
	 * @param (animationId) �Z�b�g����A�j���[�V����ID
	 * @param (layaerId) �Ώۃ��C���[��ID
	 * @param (objectId) �ΏۃI�u�W�F�N�g��ID
	 * @param (isView)   true: layerViewObjList_����擾, false: layerObjList_����擾
	 */
	void ObjectsControl::addAnimationObj(int animationId, int layerId, int objectId, bool isView)
	{
		weak_ptr<ViewObject> objWp;
		if (isView)
		{
			objWp = getViewObjectWp(layerId, objectId);
		}
		else
		{
			objWp = getObjectWp(layerId, objectId);
		}
		

		shared_ptr<ViewObject> objSP = objWp.lock();

		if (objSP)
		{
			// ���ɃA�j���[�V�������̏ꍇ�͒ǉ����Ȃ�
			if (objSP->setAnimationId(animationId))
			{
				animationObjList_.push_front(objWp);
			}
		}
	}

	/**
	 * @fn
	 * 1�t���[�����̃I�u�W�F�N�g�̃A�j���[�V�������������s
	 */
	void ObjectsControl::updateAnimation()
	{
		for (auto objItr = animationObjList_.begin(); objItr != animationObjList_.end();)
		{
			bool isRemove = false;
			shared_ptr<ViewObject> objSp = (*objItr).lock();
			if (objSp)
			{
				isRemove = objSp->animationUpdate();

				if (isRemove)
				{
					// �A�j���[�V����ID�𖢎w���Ԃɂ���
					objSp->setAnimationId(-1);
				}
			}
			else
			{
				// ���݂��Ȃ��̂��Q�Ƃ��Ă���ꍇ
				isRemove = true;
			}

			if (isRemove)
			{
				// ���X�g����폜
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
	 * �A�j���[�V�����������I�ɂ��ׂďI��
	 */
	void ObjectsControl::clearAnimation()
	{
		list<weak_ptr<ViewObject>>().swap(animationObjList_);
	}


}