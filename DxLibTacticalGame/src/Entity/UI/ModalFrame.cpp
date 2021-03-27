#include "ModalFrame.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	ModalFrame::ModalFrame() : color_(-1), isOverlay_(true), title_("")
	{
		setValidOverlay(isOverlay_);
	}

	/**
	 * @fn
	 * �{�^���ʒu�ƃT�C�Y�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 * @param (w) ��
	 * @param (h) ����
	 */
	void ModalFrame::setShape(int x, int y, int w, int h)
	{
		setPos(x, y);
		setSize(w, h);
	}

	/**
	 * @fn
	 * �{�^���ʒu�w��
	 * @param (x) X���W
	 * @param (y) Y���W
	 */
	void ModalFrame::setPos(int x, int y)
	{
		// �e�L�X�g�ƃ{�^���̑��΍��W���v�Z
		frameBodyShape_.x = x;
		frameBodyShape_.y = y;

		if (!isOverlay_)
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * �{�^���T�C�Y�w��
	 * @param (w) ��
	 * @param (h) ����
	 */
	void ModalFrame::setSize(int w, int h)
	{
		frameBodyShape_.w = w;
		frameBodyShape_.h = h;

		if (!isOverlay_)
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * �^�C�g���Z�b�g
	 * @param (title) �^�C�g������
	 */
	void ModalFrame::setTitle(string title)
	{
		title_ = title;
	}

	/**
	 * @fn
	 * �I�[�o�[���C�̗L�������Z�b�g
	 * @param (isVald) �L���̏ꍇ true�A�����̏ꍇ false
	 */
	void ModalFrame::setValidOverlay(bool isValid)
	{
		isOverlay_ = isValid;

		if (isValid)
		{
			shape_ = Shape(0, 0, WIN_W, WIN_H); // �I�[�o�[���C������ꍇ�͉�ʑS�̂ɓ����蔻����L����
		}
		else
		{
			shape_ = frameBodyShape_;
		}
	}

	/**
	 * @fn
	 * �F�w��
	 * @param (colorType) �J���[�^�C�v 
	 */
	void ModalFrame::setColor(int colorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		color_ = rm.getColor(colorType);
	}
	/**
	 * @fn
	 * �`��
	 */
	void ModalFrame::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �I�[�o�[���C
		if (isOverlay_)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, OVERLAY_ALPHA);
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), 0, TRUE);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// �t���[���{��
		int x1 = frameBodyShape_.x;
		int x2 = frameBodyShape_.getX2();
		int y1 = frameBodyShape_.y;
		int y2 = frameBodyShape_.getY2();

		// �w�b�_�[�^�C�g��
		if (title_ != "")
		{
			DxLib::DrawBox(x1, y1, x2, y1 + HEAD_H, rm.getColor(ColorType::SUB_COLOR), TRUE);
			DxLib::DrawStringToHandle(x1 + HEAD_PADDING, y1 + HEAD_PADDING, title_.c_str(), rm.getColor(ColorType::MAIN_COLOR), rm.getHdlFont(TITLE_FONT_TYPE));

			// �t���[���ʒu�����炷
			y1 += HEAD_H;
		}

		DxLib::DrawBox(x1, y1, x2, y2, color_, TRUE);
	}
}