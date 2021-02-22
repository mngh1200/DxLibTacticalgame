#include "Message.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Message::Message() : text_(""), displayTimeMs_(-1), animation_{}
	{
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * ���b�Z�[�W�\��
	 * @param (text) �\���e�L�X�g
	 * @param (isSound) ���ʉ���炷�ꍇ��true
	 * @param (displayTimeMs) �\������ (�f�t�H���g�͖�����)
	 */
	void Message::show(string text, bool isSound, int displayTimeMs)
	{
		shape_.disabledHit = false;

		text_ = text;

		if (isSound)
		{
			Utility::ResourceManager::playSound(SoundKind::MESSAGE);
		}
		
		// �e�L�X�g�̓��e�����ɃT�C�Y�擾
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int lineCount = 1;

		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text.c_str(), 
			DxLib::GetStringLength(text.c_str()), rm.getHdlFont(FONT_TYPE));

		shape_.w += PADDING + TEXT_PADDING_RIGHT;
		shape_.h += PADDING + PADDING_TOP + CLOSE_H;

		shape_.x = WIN_W - shape_.w;
		shape_.y = BATTLE_UI_AREA_Y - MARGIN - shape_.h; // Y���W

		// �A�j���[�V�����J�n
		joinAnimationList(AnimationKind::SHOW);

		displayTimeMs_ = displayTimeMs;
		
	}

	/**
	 * @fn
	 * ��\��
	 */
	void Message::hide()
	{
		text_ = "";
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * �}�E�X�N���b�N�C�x���g
	 * @param (x) x���W
	 * @param (y) y���W
	 */
	void Message::onMouseClick(int x, int y)
	{
		// ����{�^���������ɕ���
		int closeX;
		int closeY;
		getClosePos(&closeX, &closeY);
		if (Shape(closeX, closeY, CLOSE_SIZE, CLOSE_SIZE).isHit(x, y))
		{
			hide();
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void Message::render() const
	{
		if (text_ == "")
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);

		// �g
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON_DARK), FALSE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ����{�^��
		int closeX;
		int closeY;
		getClosePos(&closeX, &closeY);
		DxLib::DrawStringToHandle(closeX, closeY, "�~",
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S24));

		// �e�L�X�g
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING_TOP + CLOSE_H, text_.c_str(),
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FONT_TYPE));
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool Message::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::DELAY_HIDE) // �\�����A�j���[�V����
		{
			if (animation_.update(&shape_.x, WIN_W, WIN_W - shape_.w - MARGIN))
			{
				if (animationId_ == AnimationKind::SHOW)
				{
					if (displayTimeMs_ != -1) // �\�����Ԃ���������Ă���ꍇ
					{
						changeAnimation(AnimationKind::DELAY_HIDE);
						return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					hide(); // ��\��
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool Message::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW) // �\�����A�j���[�V����
		{
			animation_ = Animation(ANIME_MS);
			return true;
		}
		else if (animationId_ == AnimationKind::DELAY_HIDE) // ��莞�Ԍ��\��
		{
			animation_ = Animation(ANIME_MS, Animation::Direction::REVERSE, 1, displayTimeMs_);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * ����{�^���̋�`���W�擾
	 * @return ����{�^���̋�`���W
	 */
	void Message::getClosePos(int* x, int* y) const
	{
		*x = shape_.getX2() - PADDING - CLOSE_SIZE;
		*y = shape_.y + PADDING_TOP;
	}
}