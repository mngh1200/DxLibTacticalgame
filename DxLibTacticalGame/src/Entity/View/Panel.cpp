#include "Panel.h"

namespace Entity {

	/**
	 * @fn
	 * ���W�Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void Panel::setPos(int x, int y)
	{
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * �T�C�Y�Z�b�g
	 * @param (w) ��
	 * @param (h) ����
	 */
	void Panel::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
	}

	/**
	 * @fn
	 * ��`���W�Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	void Panel::setShape(int x, int y, int w, int h)
	{
		shape_.set(x, y, w, h);
	}

	/**
	 * @fn
	 * �F�Z�b�g
	 * @param (colorType) �J���[�^�C�v
	 */
	void Panel::setColor(int colorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		color_ = rm.getColor(colorType);
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Panel::render() const
	{
		if (color_ == -1) // �F���w��
		{
			return;
		}

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_, TRUE);
	}
}