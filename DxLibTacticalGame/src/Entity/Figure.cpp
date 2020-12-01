#include "Figure.h"

namespace Entity {
	/**
	 * @fn
	 * ����������
	 * @param (layerId) �������C���[
	 * @param (objectId) �����I�u�W�F�N�g
	 * @param (shape) �ʒu�A�T�C�Y
	 */
	void Figure::init(int layerId, int objectId)
	{
		layerId_ = layerId;
		objectId_ = objectId;
	}

	/**
	 * @fn
	 * �ʂ̃A�j���[�V����ID�������ꍇ�̂݃Z�b�g
	 * @return �Z�b�g�ł����ꍇ��true��Ԃ�
	 */
	bool Figure::setAnimationId(int animationId)
	{
		if (animationId_ != animationId)
		{
			animationId_ = animationId;
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����̒ǉ��\��
	 * @return �ǉ��ς݂̏ꍇ false
	 */
	bool Figure::isAnimation()
	{
		return animationId_ != -1;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜(�\��)
	 */
	void Figure::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g���폜�\�񂳂�Ă��邩����
	 * @return �폜����ꍇtrue
	 */
	bool Figure::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * �������C���[ID���擾
	 * @return �������C���[ID
	 */
	int Figure::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int Figure::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int Figure::getType() const
	{
		return type_;
	}
}