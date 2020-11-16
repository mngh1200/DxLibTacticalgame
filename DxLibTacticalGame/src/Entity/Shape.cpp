#include "Shape.h"

namespace Entity
{
	/**
	 * @fn
	 * �����蔻��i��Ƀ}�E�X�C�x��W�g�p�Ɏg�p�j
	 * @param (mouseX) �}�E�X��x���W
	 * @param (mouseY) �}�E�X��y���W
	 */
	bool Shape::isHit(int x, int y) const
	{
		if (type == RECT) 
		{
			return this->x <= x && x <= (this->x + this->w) && this->y <= y && y <= (this->y + this->h);
		}
		return false;
	}

	/**
	 * @fn
	 * �ʒu�Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void Shape::set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	/**
	 * @fn
	 * �ʒu�ƃT�C�Y�Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	void Shape::set(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	/**
	 * @fn
	 * �ʒu�ƃT�C�Y�ƌ`��Z�b�g
	 * @param (x) x���W
	 * @param (y) y���W
	 * @param (w) ��
	 * @param (h) ����
	 * @param (type) �`��
	 */
	void Shape::set(int x, int y, int w, int h, int type)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->type = type;
	}

	/**
	 * @fn
	 * �T�C�Y�Z�b�g
	 * @param (w) ��
	 * @param (h) ����
	 */
	void Shape::setSize(int w, int h)
	{
		this->w = w;
		this->h = h;
	}
}