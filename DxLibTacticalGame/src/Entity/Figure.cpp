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
	 * �`�揈��
	 */
	void Figure::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * shape_�̃T�C�Y�ɍ��킹���`�揈��
	 */
	void Figure::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * �A�j���[�V����ID���Z�b�g����Ă��Ȃ��Ƃ��̂݃Z�b�g�\
	 * @return �Z�b�g�ł����ꍇ��true��Ԃ�
	 */
	bool Figure::setAnimationId(int animationId)
	{
		if (!isAnimation())
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