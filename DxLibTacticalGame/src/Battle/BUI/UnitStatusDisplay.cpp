#include "UnitStatusDisplay.h"


namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	UnitStatusDisplay::UnitStatusDisplay() 
		: animation_{}, extraStatusHoverId_ (EXTRA_STATUS_ID_NONE)
	{
		shape_.set(X, WIN_H, WIDTH, BUI_LINE_HEIGHT * BUI_LINE_COUNT + BUI_LINE_MARGIN);
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

			UnitInfo info = targetUnit_->getInfo();

			// ���O�\��
			int nameColorType = targetUnit_->isEnemy() ? ColorType::ENEMY_COLOR : ColorType::PLAYER_COLOR;
			BUI::drawLabel(shape_.x, shape_.y, info.name, NAME_W, nameColorType, ColorType::WHITE);

			// ����X�e�[�^�X
			for (auto itr = extraStatusList_.begin(); itr != extraStatusList_.end(); ++itr)
			{
				BUI::drawLabel(itr->shape.x, shape_.y, itr->label, itr->shape.w - BUI_LINE_PADDING * 2);
			}

			/* ��s�� �������� */

			int x = shape_.x;
			int y = shape_.y + BUI_LINE_HEIGHT;

			// HP
			x = BUI::drawLabel(x, y, "HP", BUI::getHanW(2));
			x = BUI::drawValue(x, y, to_string(info.hp) + " / " + to_string(info.hpm), BUI::getHanW(7));

			// �U����
			x = BUI::drawLabel(x, y, "�U", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(info.atk), BUI::getHanW(3));

			// �h��
			x = BUI::drawLabel(x, y, "�h", BUI::getZenW(1));
			x = BUI::drawValue(x, y, to_string(info.def), BUI::getHanW(3));

			// �˒�
			x = BUI::drawLabel(x, y, "�˒�", BUI::getZenW(2));
			x = BUI::drawValue(x, y, info.getLenText(), BUI::getHanW(2));
		}
	}

	/**
	 * @fn
	 * �}�E�X�C�x���g�ɂ��X�V����
	 */
	void UnitStatusDisplay::updateByEvents(int x, int y)
	{
		// ����X�e�[�^�X�̃c�[���`�b�v�̕\��/��\������
		bool isHoverExtraStatus = false;
		if (isMouseOver_)
		{
			int count = 0;
			for (auto itr = extraStatusList_.begin(); itr != extraStatusList_.end(); ++itr)
			{
				if (itr->shape.isHit(x, y))
				{
					isHoverExtraStatus = true;
					if (extraStatusHoverId_ != count)
					{
						// �c�[���`�b�v�\��
						extraStatusHoverId_ = count;

						if (!tooltip_)
						{
							// �c�[���`�b�v�I�u�W�F�N�g�ǉ�
							FrameWork::Game& game = FrameWork::Game::getInstance();
							Entity::ObjectsControl& objectsControl = game.objectsControl;

							tooltip_ = make_shared<Tooltip>();
							objectsControl.addFigure(getLayerId(), tooltip_);
						}

						tooltip_->show(shape_.x, shape_.y - BUI_PADDING - 5, itr->description);
					}
					break;
				}
				++count;
			}
		}

		if (!isHoverExtraStatus)
		{
			// �c�[���`�b�v��\��
			extraStatusHoverId_ = EXTRA_STATUS_ID_NONE;
			if (tooltip_)
			{
				tooltip_->hide();
			}
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
	 * @param (animationId) �A�j���[�V�����̎��
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
	 * @param (unit) �Ώۃ��j�b�g
	 */
	void UnitStatusDisplay::setTargetUnit(shared_ptr<Unit> unit)
	{
		if (!unit || targetUnit_ == unit) // �I���ς݂̃��j�b�g�̏ꍇ
		{
			return;
		}

		targetUnit_ = unit;

		// ����X�e�[�^�X�̕\���f�[�^����
		extraStatusList_.clear();

		vector<pair<string, string>> extraStatusTexts;
		unit->getExtraStatusList(extraStatusTexts);

		int x = shape_.x + NAME_W + BUI_LINE_PADDING * 2 + BUI_LINE_MARGIN + EXTRA_STATUS_MARGIN;
		const int y = shape_.y + BUI_LINE_MARGIN;
		const int h = BUI_FONT_SIZE + BUI_LINE_PADDING * 2;

		for (auto itr = extraStatusTexts.begin(); itr != extraStatusTexts.end(); ++itr)
		{
			int w = BUI::getZenW((itr->first).size() / 2) + BUI_LINE_PADDING * 2;
			extraStatusList_.push_back(ExtraStatus{ itr->first, itr->second, Shape(x, y ,w ,h)});
			x = x + w + EXTRA_STATUS_MARGIN;
		}

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