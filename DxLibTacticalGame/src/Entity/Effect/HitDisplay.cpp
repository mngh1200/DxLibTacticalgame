#include "HitDisplay.h"
#include "Screen/BattleScreen.h"

namespace Entity {
	/**
	 * @fn
	 * �\��
	 * @param (atkUnit) �U�������j�b�g
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (hitRate) ������
	 */
	void HitDisplay::show(Fight* fight)
	{
		const FightData& actData = fight->getFightData(true);
		const FightData& psvData = fight->getFightData(false);

		if (actData.unit->isEnemy())
		{
			return; // �v���C���[���j�b�g�łȂ��ꍇ�́A�\�����Ȃ�
		}

		isShow_ = true;
		hitRate_ = actData.hitRate;

		// �\���ʒu�v�Z
		Shape defUnitShape = psvData.unit->getShape();
		Shape atkUnitShape = actData.unit->getShape();

		shape_.x = defUnitShape.x;
		shape_.y = defUnitShape.y - MARGIN - H;

		shape_.w = LABEL_W + VALUE_W;
		shape_.h = H;

		// �㑤�ɕ\���ł��Ȃ��ꍇ
		if (shape_.isHit(atkUnitShape) || shape_.y < 0)
		{
			// �E���ɕ\��
			shape_.x = defUnitShape.getX2() + MARGIN;
			shape_.y = defUnitShape.y + MARGIN; 

			// �E���ɕ\���ł��Ȃ��ꍇ
			if (shape_.isHit(atkUnitShape) || shape_.getX2() > WIN_W)
			{
				shape_.x = defUnitShape.x - MARGIN - shape_.w; // �����ɕ\��

				if (shape_.isHit(atkUnitShape) || shape_.x < 0) // �����ɕ\���ł��Ȃ��ꍇ
				{
					// �����ɕ\��
					shape_.x = defUnitShape.x;
					shape_.y = defUnitShape.getY2() + MARGIN;
				}
			}
		}

		// ��܂��͉����ɐݒu�����ꍇ�ɁA�E�����͂ݏo���Ă���ꍇ�A�����ɂ��炷
		if (shape_.x == defUnitShape.x && shape_.getX2() > WIN_W)
		{
			shape_.x = WIN_W - MARGIN - shape_.w;
		}
	}

	/**
	 * @fn
	 * ��\����
	 */
	void HitDisplay::hide()
	{
		isShow_ = false;
	}

	/**
	 * @fn
	 * �`��
	 */
	void HitDisplay::render() const
	{
		if (!isShow_)
		{
			return;
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int valX = shape_.x + LABEL_W; // �l�\��������x���W

		// �w�i
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + LABEL_W, shape_.getY2(), rm.getColor(ColorType::SUB_COLOR), TRUE);
		DxLib::DrawBox(valX, shape_.y, valX + VALUE_W, shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ����
		int colorType = ColorType::SUB_COLOR;
		if (hitRate_ < 100)
		{
			colorType = ColorType::ENEMY_COLOR;
		}

		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, "Hit", rm.getColor(ColorType::MAIN_COLOR), rm.getHdlFont(FontType::NORMAL_S14));
		DxLib::DrawStringToHandle(valX + PADDING, shape_.y + PADDING, (to_string(hitRate_) + "%").c_str(), rm.getColor(colorType), rm.getHdlFont(FontType::NORMAL_S18));
	}
}