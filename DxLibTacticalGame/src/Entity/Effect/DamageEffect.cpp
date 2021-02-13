#include "DamageEffect.h"
#include "Screen/BattleScreen.h"

namespace Entity {

	/**
	 * @fn
	 * ダメージエフェクトを生成
	 * @param (unitX) 被ダメージしたユニットのX座標
	 * @param (unitY) 被ダメージしたユニットのY座標
	 * @param (damage) 被ダメージ量
	 */
	void DamageEffect::makeDamageEffect(int unitX, int unitY, int damage)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<DamageEffect> objSp = make_shared<DamageEffect>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, objSp);

		objSp->init(unitX, unitY, damage);
	}

	/**
	 * @fn
	 * 初期処理
	 * @param (unitX) 被ダメージしたユニットのX座標
	 * @param (unitY) 被ダメージしたユニットのY座標
	 * @param (damage) 被ダメージ量
	 */
	void DamageEffect::init(int unitX, int unitY, int damage)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		shape_.y = unitY - H;
		shape_.h = H;

		if (damage == MISS) // MISS表示
		{
			shape_.w = MISS_W;
			imageIds_.push_back(rm.getImage(ImageType::EFFECT, EffectId::MISS));
		}
		else // ダメージ表示
		{
			string damageStr = to_string(damage);
			int size = damageStr.length(); // 桁数

			shape_.w = size * DAMAGE_W;

			for (int i = 0; i < size; ++i )
			{
				// 特定の桁の数値取得
				char s = damageStr.at(i);
				int num = s - '0';

				imageIds_.push_back(rm.getImage(ImageType::EFFECT, EffectId::DAMAGE, num));
			}
		}

		shape_.x = unitX + CHIP_SIZE / 2 - shape_.w / 2; // 中央揃え

		if (shape_.y < 0) // 画面外の場合
		{
			shape_.y = unitY;
		}

		joinAnimationList(AnimationKind::DISPLAY);
	}

	/**
	 * @fn
	 * 描画
	 */
	void DamageEffect::render() const
	{
		if (!isDisplay_)
		{
			return;
		}

		int count = 0;
		for (auto itr = imageIds_.begin(); itr != imageIds_.end(); ++itr)
		{
			DxLib::DrawGraph(shape_.x + DAMAGE_W * count, shape_.y, *itr, TRUE);
			++count;
		}
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool DamageEffect::animationUpdate()
	{
		if (animationId_ == AnimationKind::DISPLAY)
		{
			
			if (animation_.increaseFrame())
			{
				// 表示終了
				destroy();
				return true;
			}
			else if (animation_.getDelayFinishLog_()) // 遅延開始時間終了時
			{
				isDisplay_ = true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool DamageEffect::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::DISPLAY)
		{
			animation_ = Animation(Unit::ANIME_DAMAGE_MS, 0, 1, Unit::ANIME_DAMAGE_MS);
			return true;
		}
		return false;
	}
}