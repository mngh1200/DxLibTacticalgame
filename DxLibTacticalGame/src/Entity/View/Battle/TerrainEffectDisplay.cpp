#include "TerrainEffectDisplay.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TerrainEffectDisplay::TerrainEffectDisplay() 
		: animation_{}
	{
		shape_.set(BATTLE_UI_AREA_MARGIN, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN, WIDTH, LINE_HEIGHT * LINE_COUNT);
	}

	/**
	 * @fn
	 * �`��
	 */
	void TerrainEffectDisplay::render() const
	{
		if (targetMass_ && targetMass_->getKind() != Mass::Kind::OUT_OF_MAP)
		{

			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// �G���A
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);

			// �e�s�̕\��
			for (int i = 0; i < LINE_COUNT; i++)
			{
				int y = shape_.y + LINE_HEIGHT * i;

				if (i == 0)
				{
					DxLib::DrawBox(shape_.x + LINE_MARGIN, y + LINE_MARGIN, shape_.getX2() - LINE_MARGIN, y + LINE_HEIGHT - LINE_MARGIN, rm.getColor(ColorType::SUB_COLOR_LITE), TRUE);
				}

				string text = targetMass_->getText(i);
				if (text != "")
				{
					const int space = LINE_MARGIN + LINE_PADDING;
					DxLib::DrawStringToHandle(shape_.x + space, y + space, text.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S18));
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
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN);
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V��������
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool TerrainEffectDisplay::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::DISPLAY)
		{
			shape_.y = ANIMATION_Y0;
			animation_ = Animation(800, 0, 1, 0, Easing::InOutBounce<float>);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �Ώۃ}�X�w��
	 * @param (mass) �Ώ̃}�X
	 */
	void TerrainEffectDisplay::setTargetMass(shared_ptr<Mass> mass)
	{
		if (!mass || targetMass_ == mass) // �I���ς݂̃}�X�̏ꍇ
		{
			return;
		}

		// �O��l
		int prevKind = -1;
		
		if (targetMass_)
		{
			prevKind = targetMass_->getKind();
		}

		targetMass_ = mass;

		if (mass->getKind() != prevKind) // �n�`�̎�ނ��\���ʒu���ς���Ă���ꍇ�̂݃A�j���[�V�������{
		{
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