#include "ImageView.h"

namespace Entity {

	/**
	 * @fn
	 * ���W�Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void ImageView::setPos(int x, int y)
	{
		type_ = IMAGE_VIEW;
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * ���W�Z�b�g
	 * @param (imageId) �C���[�WID�Z�b�g 
	 */
	void ImageView::setImageId(int imageHandle)
	{
		imageHandle_ = imageHandle;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void ImageView::render() const
	{
		if (imageHandle_ == -1) // �C���[�W�n���h�������ݒ�̏ꍇ
		{
			return;
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageHandle_, TRUE);
	}
}