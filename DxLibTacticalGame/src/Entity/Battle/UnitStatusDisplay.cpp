#include "UnitStatusDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}
	{
		shape_.set(X, BATTLE_UI_AREA_Y + BUI_PADDING, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
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

			/* ��s�� �������� */

			// ���O�\��
			int nameColorType = targetUnit_->isEnemy() ? ColorType::ENEMY_COLOR : ColorType::PLAYER_COLOR;
			BUI::drawLabel(shape_.x, shape_.y, targetUnit_->getName(), BUI::getZenW(3), nameColorType, ColorType::WHITE);

			/* ��s�� �������� */

			int x = shape_.x;
			int y = shape_.y + BUI_LINE_HEIGHT;

			// HP
			x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getHpv()) + " / " + to_string(targetUnit_->getHpm()), BUI::getHanW(7));

			// �U����
			x = BUI::drawLabel(x, y, "�U", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getAtk()), BUI::getHanW(3));

			// �h��
			x = BUI::drawLabel(x, y, "�h", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(targetUnit_->getDef()), BUI::getHanW(3));

			// �˒�
			x = BUI::drawLabel(x, y, "�˒�", BUI::getZenW(2));
			x = BUI::drawValue(x, y, targetUnit_->getLenText(), BUI::getHanW(2));
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
			return animation_.update(&shape_.y, ANIMATION_Y0, BATTLE_UI_AREA_Y + BUI_PADDING);
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
			animation_ = Animation(500, 0, 1, 0, Easing::InOutBounce<float>);
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