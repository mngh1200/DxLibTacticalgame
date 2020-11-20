#include "Object.h"

namespace Entity {
	/**
	 * @fn
	 * ����������
	 * @param (layerId) �������C���[
	 * @param (objectId) �����I�u�W�F�N�g
	 * @param (shape) �ʒu�A�T�C�Y
	 */
	void Object::init(int layerId, int objectId, Shape shape)
	{
		layerId_ = layerId;
		objectId_ = objectId;
		shape_ = shape;
	}

	/**
	 * @fn
	 * �}�E�X�C�x���g
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType)
	 * @param (isOtherHit) �}�E�X��y���W
	 * @return �}�E�X���ڐG���Ă���Ƃ���true
	 */
	bool Object::checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit)
	{
		bool isHit = false;

		if (&shape_ != nullptr)
		{
			isHit = !isOtherHit && (shape_.isHit(x, y));

			// �}�E�X�|�C���^���ڐG�v
			if (isHit)
			{
				if (button == MOUSE_INPUT_LEFT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseLeftDown(); // ��������
						isMouseDown_ = true;
					}
					else
					{
						onMouseLeftUp(); // ��������
						
						if (isMouseDown_)
						{

							onMouseClick();
							// eventType���N���b�N�ɏ��������� TODO:�s�
							*eventType = MOUSE_INPUT_LOG_CLICK;


						}
							
						isMouseDown_ = false;
					}
				}
				else if (button == MOUSE_INPUT_RIGHT)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseRightDown(); // �E������
					}
					else
					{
						onMouseRightUp(); // �E������
					}
				}
				else if (button == MOUSE_INPUT_MIDDLE)
				{
					if (*eventType == MOUSE_INPUT_LOG_DOWN)
					{
						onMouseWheelDown(); // �z�C�[��������
					}
					else
					{
						onMouseWheelUp(); // �z�C�[��������
					}
				}

				if (!isMouseOver_ && !isMouseDown_) {
					isMouseOver_ = true;
					onMouseOver(); // �}�E�X�z�o�[�����u��
				}
			}
			else if (isMouseOver_)
			{
				// �}�E�X�|�C���^��ڐG���@���@1�t���[���O�܂ŐڐG���Ă����ꍇ
				onMouseOut(); // �z�o�[�A�E�g
				isMouseOver_ = false;
				isMouseDown_ = false;
			}
		}
		return isHit;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Object::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜(�\��)
	 */
	void Object::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g���폜�\�񂳂�Ă��邩����
	 * @return �폜����ꍇtrue
	 */
	bool Object::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * �������C���[ID���擾
	 * @return �������C���[ID
	 */
	int Object::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int Object::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * shape_�̃T�C�Y�ɍ��킹���`�揈��
	 */
	void Object::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), imagePath_, isTrans_);
	}
}