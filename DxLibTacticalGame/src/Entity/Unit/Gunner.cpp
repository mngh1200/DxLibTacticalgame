#include "Gunner.h"

namespace Entity {
	/**
	 * @fn
	 * �U���\�ł��邩
	 * @return �U���\�ȏꍇtrue
	 */
	bool Gunner::isAtackable() const
	{
		return isLoaded_;
	}

	/**
	 * @fn
	 * �U���A�j���[�V�����̍쐬����
	 */
	void Gunner::createAtackAnimation()
	{
		isAtacked_ = true;
		isBulletExist_ = true;
		animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS), Animation::Direction::AlTERNATE, 2);
		bulletAnime_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS / 2));

		Utility::ResourceManager::playSound(SoundKind::ATTACK_FIRE);
	}

	/**
	 * @fn
	 * �U���A�j���[�V�����̍X�V����
	 */
	bool Gunner::updateAtackAnimation()
	{
		bool isFin = false;

		// ���j�b�g�A�j���[�V����
		int baseX = Map::getRealX(x_);
		int baseY = Map::getRealY(y_);
		isFin = animation_.update(&shape_.x, baseX, baseX - CHIP_SIZE / 2);


		// �e�ۃA�j���[�V����

		// �����y�яI���ʒu�̓��j�b�g�̒��S
		int adjustCenterPos = CHIP_SIZE / 2 - CHIP_SIZE / 4;

		int bulletBaseX = baseX + adjustCenterPos;
		int bulletBaseY = baseY + adjustCenterPos;
		int targetUnitX = targetRealX_ + adjustCenterPos;
		int targetUnitY = targetRealY_ + adjustCenterPos;

		if (bulletAnime_.update(&bulletX_, &bulletY_, bulletBaseX, bulletBaseY, targetUnitX, targetUnitY))
		{
			isBulletExist_ = false;
		}
		else
		{
			isFin = false;
		}

		if (isFin)
		{
			isLoaded_ = false;
			return true;
		}
		
		return false;
	}

	/**
	 * @fn
	 * �{����ŗL�̕`�揈��
	 */
	void Gunner::renderExtra() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (!isLoaded_)
		{
			// �����U�\�L
			DxLib::DrawGraph(shape_.getX2() - CHIP_SIZE / 4 - UNLOADED_MARGIN_RIGHT, shape_.y + HP_Y - CHIP_SIZE / 4 - UNLOADED_MARGIN_BOTTOM,
				rm.getImage(ImageType::EFFECT, EffectId::BULLET, BulletPos::UNLOADED), TRUE);
		}

		if (isBulletExist_)
		{
			// �e��
			DxLib::DrawGraph(bulletX_, bulletY_, rm.getImage(ImageType::EFFECT, EffectId::BULLET, BulletPos::BULLET_BODY), TRUE);
		}
	}

	/**
	 * @fn
	 * �^�[���I�����̏���
	 */
	void Gunner::turnEndExtra(bool isOwnEnd)
	{
		if (!isOwnEnd)
		{
			return;
		}

		if (!isAtacked_)
		{
			isLoaded_ = true; // �U�������Ƀ^�[���I�������ꍇ�A���U
		}
		isAtacked_ = false;
	}

	void Gunner::getExtraStatusListExtra(vector<pair<string, string>>& list) const
	{
		if (isLoaded_)
		{
			list.push_back(make_pair("���U��", "�U���\"));
		}
		else
		{
			list.push_back(make_pair("�����U", "�U���s��\n�����^�[���U���\"));
		}
	}
}