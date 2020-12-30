#include "TerrainEffectDisplay.h"

namespace Entity {
	TerrainEffectDisplay::TerrainEffectDisplay() 
		: animation_{}, x0(0), x1(0)
	{
		shape_.set(0, Y, WIDTH, LINE_HEIGHT * LINE_COUNT);
	}
	/**
	 * @fn
	 * �`��
	 */
	void TerrainEffectDisplay::render() const
	{
		if (targetMass_)
		{
			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// �\��
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.y + LINE_HEIGHT, rm.getColor(ColorType::SUB_COLOR), TRUE);
			DxLib::DrawStringToHandle(shape_.x + LINE_PADDING, shape_.y + LINE_PADDING, "�n�`����", rm.getColor(ColorType::MAIN_COLOR), rm.getHdlFont(FontType::NORMAL_S20));

			// ������
			for (int i = 1; i <= LINE_COUNT; i++)
			{
				string text = targetMass_->getText(i);
				if (text != "")
				{
					int y = shape_.y + LINE_HEIGHT * i;
					DxLib::DrawBox(shape_.x, y, shape_.getX2(), y + LINE_HEIGHT, rm.getColor(ColorType::MAIN_COLOR), TRUE);
					DxLib::DrawStringToHandle(shape_.x + LINE_PADDING, y + LINE_PADDING, text.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S20));
				}
			}

		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool TerrainEffectDisplay::animationUpdate()
	{
		if (animationId_ == AnimationKind::DISPLAY)
		{
			return animation_.update(&shape_.x, x0, x1);
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V��������
	 */
	bool TerrainEffectDisplay::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::DISPLAY)
		{
			animation_ = Animation(200);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �Ώۃ}�X�w��
	 */
	void TerrainEffectDisplay::setTargetMass(shared_ptr<Mass> mass, int realX)
	{
		if (!mass || targetMass_ == mass) // �I���ς݂̃}�X�̏ꍇ
		{
			return;
		}

		// �O��̃}�X�̎�ނƒn�`���ʕ\���ʒu���擾
		int prevX0 = x0;
		int prevKind = -1;
		
		if (targetMass_)
		{
			prevKind = targetMass_->getKind();
		}

		targetMass_ = mass;

		x0 = - WIDTH;
		x1 = 0;

		if (realX < WIDTH + CHIP_SIZE) // �}�X�̈ʒu�����[�̏ꍇ�͉E���ɕ\��
		{
			x0 = WIN_W;
			x1 = WIN_W - WIDTH;
		}

		if (x0 != prevX0 || mass->getKind() != prevKind) // �n�`�̎�ނ��\���ʒu���ς���Ă���ꍇ�̂݃A�j���[�V�������{
		{
			shape_.x = x0;
			changeAnimation(AnimationKind::DISPLAY);
		}
	}

	/**
	 * @fn
	 * �Ώۃ}�X�N���A
	 */
	void TerrainEffectDisplay::clear()
	{
		targetMass_.reset();
		animation_.reset();
	}


}