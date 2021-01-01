#include "UnitStatusDisplay.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}
	{
		shape_.set(X, BATTLE_UI_AREA_Y + BATTLE_UI_AREA_MARGIN, WIDTH, LINE_HEIGHT * LINE_COUNT);
	}
	/**
	 * @fn
	 * �`��
	 */
	void UnitStatusDisplay::render() const
	{
		if (targetUnit_)
		{

			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// �G���A
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool UnitStatusDisplay::animationUpdate()
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
	 */
	bool UnitStatusDisplay::createAnimation(int animationId)
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
	 * �Ώۃ��j�b�g�w��
	 */
	void UnitStatusDisplay:: setTargetUnit(shared_ptr<Unit> unit)
	{
		if (!unit || targetUnit_ == unit) // �I���ς݂̃��j�b�g�̏ꍇ
		{
			return;
		}

		targetUnit_ = unit;
		changeAnimation(AnimationKind::DISPLAY);
	}

	/**
	 * @fn
	 * �Ώۃ��j�b�g�N���A
	 */
	void UnitStatusDisplay::clear()
	{
		targetUnit_.reset();
		animation_.reset();
	}


}