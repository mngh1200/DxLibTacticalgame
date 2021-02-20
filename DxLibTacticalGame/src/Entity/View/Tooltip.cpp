#include "Tooltip.h"

namespace Entity {

	/**
	 * @fn
	 * �c�[���`�b�v�̏�������(��{��xy���W�̉E��ɕ\���A��ʊO�ɂȂ�ꍇ�͒���)
	 */
	void Tooltip::show(int x, int y, string text)
	{
		text_ = text;

		// �� ��`���W�v�Z
		shape_.set(x, y);
		
		// �e�L�X�g�̓��e�����ɃT�C�Y�擾
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		int lineCount = 1;

		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text.c_str(), 
			DxLib::GetStringLength(text.c_str()), rm.getHdlFont(FONT_TYPE));

		shape_.w += PADDING * 2;
		shape_.h += PADDING * 2;

		shape_.y -= shape_.h; // �E��ɕ\�����邽�߂̒���

		// ��ʊO�ɂ͂ݏo���ꍇ�̒���
		if (shape_.getX2() > WIN_W)
		{
			shape_.x = WIN_W - shape_.w;
		}

		if (shape_.y < 0)
		{
			shape_.y = 0;
		}

		// �A�j���[�V�����J�n
		joinAnimationList(AnimationKind::SHOW);
	}

	/**
	 * @fn
	 * ��\��
	 */
	void Tooltip::hide()
	{
		joinAnimationList(AnimationKind::HIDE);
	}

	/**
	 * @fn
	 * �`��
	 */
	void Tooltip::render() const
	{
		if (alpha_ == 0)
		{
			return;
		}

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �g
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR_ON_DARK), FALSE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �e�L�X�g
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, text_.c_str(),
			rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S18));
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool Tooltip::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // ��\�����A�j���[�V����
		{
			if (animation_.update(&alpha_, 0, 230))
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool Tooltip::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW) // �\�����A�j���[�V����
		{
			animation_ = Animation(ANIME_MS);
			return true;
		}
		else if (animationId == AnimationKind::HIDE) // ��\�����A�j���[�V����
		{
			animation_ = Animation(ANIME_MS, Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}