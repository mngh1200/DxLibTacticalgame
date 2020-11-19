#include "Controller.h"


namespace FrameWork
{
	/**
	 * @fn
	 * �V���O���g���ɂ��邽�߂̃C���X�^���X�֐�
	 */
	Controller& Controller::getInstance()
	{
		static Controller instance;
		return instance;
	}

	/**
	 * @fn
	 * �S�ẴL�[�̏�Ԃ��擾
	 * @return �f�t�H���g: 0 , �Q�[���I������: -1
	 */
	int Controller::getAllEvents()
	{
		// �S�ẴL�[�̏�Ԏ擾
		if (DxLib::GetHitKeyStateAll(key_) != 0) {
			return -1;
		}

		// �L�[�̏�Ԓ���
		for (int i = 0; i < KEY_LEN; i++)
		{
			if (prevKey_[i] == NOT_PRESSED && key_[i] == PRESSED)
			{
				key_[i] = PRESSED_NOW;  // if change state, set pressed state
			}
			prevKey_[i] = key_[i];
		}

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

		Game& game = Game::getInstance();

		//! �ڐG�I�u�W�F�N�g����������
		bool isFoundHitObj = false;
		
		// �}�E�X�C�x���g
		for (auto&& layer : game.layerObjList)
		{
			for (auto&& objMap : layer)
			{
				shared_ptr<Entity::Object> obj = objMap.second;
				if (obj->checkMouseEvent(x, y, button, &eventType, isFoundHitObj)) {
					// �}�E�X�ڐG
					isFoundHitObj = true;

					// �e�X�g�i�{�^���������I���j
					if (obj->objectType == Entity::Object::ObjectType::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
					{
						return -1;
					}
				}
			}
		}


		return 0;
	}

	/**
	 * @fn
	 * �ΏۃL�[��������������
	 * @return true:������
	 */
	bool Controller::isKeyPressed(int input) const
	{
		return getKey(input) != KeyState::NOT_PRESSED;
	}

	/**
	 * @fn
	 * �ΏۃL�[�����������u�Ԃ�����
	 * @return true:���������u��
	 */
	bool Controller::isKeyPressedNow(int input) const
	{
		return getKey(input) == KeyState::PRESSED_NOW;
	}

	/**
	 * @fn
	 * �ΏۃL�[�̏�Ԏ擾
	 * @return �ΏۃL�[�̏��
	 */
	char Controller::getKey(int input) const
	{
		return key_[input];
	}
}