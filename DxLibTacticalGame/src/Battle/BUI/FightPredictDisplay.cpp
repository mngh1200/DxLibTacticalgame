#include "FightPredictDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	FightPredictDisplay::FightPredictDisplay() 
		: animation_{}
	{
		fight_ = nullptr;
		shape_.set(BUI_PADDING, BATTLE_UI_AREA_Y + BUI_PADDING);
		shape_.setSize(AREA_WIDTH * 2 + CENTER_WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * �`��
	 */
	void FightPredictDisplay::render() const
	{
		if (fight_ != nullptr && fight_->isPrepared())
		{
			Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

			// �퓬�\���f�[�^�擾
			const Battle::FightData* playerFightData; 
			const Battle::FightData* enemyFightData;

			Battle::FightData actFightData = fight_->getFightData(true);
			Battle::FightData psvFightData = fight_->getFightData(false);

			if (!actFightData.unit->isEnemy())
			{
				playerFightData = &actFightData;
				enemyFightData = &psvFightData;
			}
			else
			{
				playerFightData = &psvFightData;
				enemyFightData = &actFightData;
			}

			// �퓬�\���f�[�^�`��
			drawFightPredict(shape_.x, enemyFightData, false); // �G��
			drawFightPredict(shape_.x + PLAYER_X, playerFightData, true); // �v���C���[��

			// �U�������`��
			if (playerFightData->isAtk)
			{
				if (!enemyFightData->isAtk)
				{
					drawAtackDirection(shape_.y + shape_.h / 4, true); // �U��������
				}
				else if (fight_->isActSideFirst()) // �U�����s����U
				{
					drawAtackDirection(shape_.y, true);
					drawAtackDirection(shape_.y + shape_.h / 2, false);
				}
				else if (fight_->isActSideFirst()) // �h�䑤��U
				{
					drawAtackDirection(shape_.y, false);
					drawAtackDirection(shape_.y + shape_.h / 2, true);
				}
			}
			else if (enemyFightData->isAtk)
			{
				drawAtackDirection(shape_.y + shape_.h / 4, false); // �h�䑤����
			}
		}
	}

	/**
	 * @fn
	 * �Е��̃��j�b�g�̐퓬�\�����`��
	 * @param (areaX) �\���G���A��x���W
	 * @param (fightData) �퓬���
	 * @param (isPlayer) �v���C���[���̃��j�b�g�ł��邩
	 */
	void FightPredictDisplay::drawFightPredict(int areaX, const Battle::FightData* fightData, bool isPlayer) const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		
		// �G���A
		DxLib::DrawBox(areaX, shape_.y, areaX + AREA_WIDTH, shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);


		

		/* ��s�� �������� */

		int x = areaX;
		UnitInfo info = fightData->unit->getInfo();

		// ���O�\��
		int nameColorType = isPlayer ? ColorType::PLAYER_COLOR : ColorType::ENEMY_COLOR;
		x = BUI::drawLabel(x, shape_.y, info.name, BUI::getZenW(3), nameColorType, ColorType::WHITE);

		// �������
		for (auto itr = fightData->extraEffects.begin(); itr != fightData->extraEffects.end(); ++itr)
		{
			x += EXTRA_EFFECT_MARGIN;
			int len = (*itr).size() / 2;
			x = BUI::drawLabel(x, shape_.y, (*itr), BUI::getZenW(len), ColorType::MAIN_COLOR_ON, ColorType::SUB_COLOR);
		}



		/* ��s�� �������� */

		x = areaX;
		int y = shape_.y + BUI_LINE_HEIGHT;

		// HP
		x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
		x = BUI::drawValue(x, y, to_string(info.hp) + " / " + to_string(info.hpm), BUI::getHanW(7));

		// �^�_���[�W
		string value = !fightData->isAtk ? "-" : to_string(fightData->damage);

		x = BUI::drawLabel(x, y, "�_���[�W", BUI::getZenW(4));
		x = BUI::drawValue(x, y, value, BUI::getHanW(3));

		// ������
		value = !fightData->isAtk ? "-" : to_string(fightData->hitRate) + "%";
		x = BUI::drawLabel(x, y, "������", BUI::getZenW(3));
		x = BUI::drawValue(x, y, value, BUI::getHanW(4));
	}

	/**
	 * @fn
	 * �U�������̕`��
	 * @param (y) y���W
	 * @param (isPlayer) �v���C���[���̃��j�b�g�ł��邩
	 */
	void FightPredictDisplay::drawAtackDirection(int y, bool isPlayer) const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		float backX = (float)(shape_.x + AREA_WIDTH + CENTER_PADDING + DIRECTION_W); // ���̌���x���W
		float forwardX = (float)(shape_.x + AREA_WIDTH + CENTER_PADDING); // ���̐��x���W

		float yf = (float)y;

		if (!isPlayer)
		{
			float tmp = backX;
			backX = forwardX;
			forwardX = tmp;
		}

		int color = isPlayer ? rm.getColor(ColorType::PLAYER_COLOR_LITE) : rm.getColor(ColorType::ENEMY_COLOR_LITE);

		DxLib::DrawTriangleAA(backX, yf, forwardX, (float)(yf + shape_.h / 4), backX, (float)(yf + shape_.h / 2), color, TRUE);
	}



	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool FightPredictDisplay::animationUpdate()
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
	bool FightPredictDisplay::createAnimation(int animationId)
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
	 * �퓬�\������
	 * @return �퓬�\����V�����Z�b�g�����Ƃ��̂�true��Ԃ�
	 */
	void FightPredictDisplay::emplaceFight(const Battle::Fight* fight)
	{
		fight_ = fight;
		changeAnimation(AnimationKind::DISPLAY);
		shape_.disabledHit = false;
	}

	/**
	 * @fn
	 * ���Z�b�g
	 */
	void FightPredictDisplay::reset()
	{
		fight_ = nullptr;
		animation_.reset();
		shape_.disabledHit = true;
	}


}