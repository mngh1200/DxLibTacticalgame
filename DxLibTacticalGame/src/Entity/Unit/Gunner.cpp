#include "Gunner.h"

namespace Entity {
	/**
	 * @fn
	 * 攻撃可能であるか
	 * @return 攻撃可能な場合true
	 */
	bool Gunner::isAtackable() const
	{
		return isLoaded_;
	}

	/**
	 * @fn
	 * 攻撃アニメーションの作成処理
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
	 * 攻撃アニメーションの更新処理
	 */
	bool Gunner::updateAtackAnimation()
	{
		bool isFin = false;

		// ユニットアニメーション
		int baseX = Map::getRealX(x_);
		int baseY = Map::getRealY(y_);
		isFin = animation_.update(&shape_.x, baseX, baseX - CHIP_SIZE / 2);


		// 弾丸アニメーション

		// 初期及び終了位置はユニットの中心
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
	 * 本兵種固有の描画処理
	 */
	void Gunner::renderExtra() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (!isLoaded_)
		{
			// 未装填表記
			DxLib::DrawGraph(shape_.getX2() - CHIP_SIZE / 4 - UNLOADED_MARGIN_RIGHT, shape_.y + HP_Y - CHIP_SIZE / 4 - UNLOADED_MARGIN_BOTTOM,
				rm.getImage(ImageType::EFFECT, EffectId::BULLET, BulletPos::UNLOADED), TRUE);
		}

		if (isBulletExist_)
		{
			// 弾丸
			DxLib::DrawGraph(bulletX_, bulletY_, rm.getImage(ImageType::EFFECT, EffectId::BULLET, BulletPos::BULLET_BODY), TRUE);
		}
	}

	/**
	 * @fn
	 * ターン終了時の処理
	 */
	void Gunner::turnEndExtra(bool isOwnEnd)
	{
		if (!isOwnEnd)
		{
			return;
		}

		if (!isAtacked_)
		{
			isLoaded_ = true; // 攻撃せずにターン終了した場合、装填
		}
		isAtacked_ = false;
	}

	void Gunner::getExtraStatusListExtra(vector<pair<string, string>>& list) const
	{
		if (isLoaded_)
		{
			list.push_back(make_pair("装填済", "攻撃可能"));
		}
		else
		{
			list.push_back(make_pair("未装填", "攻撃不可\n※次ターン攻撃可能"));
		}
	}
}