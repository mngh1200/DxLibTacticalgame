#include "Back.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Back::Back()
	{
		objectType = BACKGROUND;
	}

	/**
	 * @fn
	 * �����������i�T�C�Y�͌Œ�j
	 */
	void Back::init(int layerId, int objectId, Shape shape)
	{
		Object::init(layerId, objectId, Shape(0, 0, WIN_W, WIN_H));
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Back::render() const
	{
		// �e�X�g�p
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), DxLib::GetColor(247, 241, 230), TRUE);
	}
}