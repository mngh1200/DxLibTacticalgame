#include "TerrainEffectDisplay.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	TerrainEffectDisplay::TerrainEffectDisplay() 
		: animation_{}
	{
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
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

			// ���x��
			BUI::drawLabel(shape_.x, shape_.y, targetMass_->getText(0), shape_.w - (BUI_LINE_MARGIN + BUI_LINE_PADDING) * 2);
			
			// ������
			BUI::drawValue(shape_.x, shape_.y + BUI_LINE_HEIGHT, targetMass_->getText(1), shape_.w);
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
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BUI_PADDING);
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
			animation_ = Animation(500, 0, 1, 0, Easing::InOutBounce<float>);
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