#include "ViewObject.h"

namespace Entity {
	/**
	 * @fn
	 * ����������
	 * @param (layerId) �������C���[
	 * @param (objectId) �����I�u�W�F�N�g
	 * @param (shape) �ʒu�A�T�C�Y
	 */
	void ViewObject::init(int layerId, int objectId, Shape shape)
	{
		layerId_ = layerId;
		objectId_ = objectId;
		shape_ = shape;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void ViewObject::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imagePath_, isTrans_);
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g�폜(�\��)
	 */
	void ViewObject::destroy()
	{
		isReserveDelete_ = true;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�g���폜�\�񂳂�Ă��邩����
	 * @return �폜����ꍇtrue
	 */
	bool ViewObject::isDelete() const
	{
		return isReserveDelete_;
	}

	/**
	 * @fn
	 * �������C���[ID���擾
	 * @return �������C���[ID
	 */
	int ViewObject::getLayerId() const
	{
		return layerId_;
	}

	/**
	 * @fn
	 * �I�u�W�F�N�gID���擾
	 * @return �I�u�W�F�N�gID
	 */
	int ViewObject::getObjectId() const
	{
		return objectId_;
	}

	/**
	 * @fn
	 * shape_�̃T�C�Y�ɍ��킹���`�揈��
	 */
	void ViewObject::renderExtend() const
	{
		DxLib::DrawExtendGraph(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), imagePath_, isTrans_);
	}
}