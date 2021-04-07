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
		x_ = x;
		y_ = y;
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
		DxLib::DrawGraph(x_, y_, imageHandle_, TRUE);
	}
}