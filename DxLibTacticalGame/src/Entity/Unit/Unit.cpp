#include "Unit.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ テスト処理
	 */
	Unit::Unit() : 
		x_(0), y_(0), imageId_(0), animation_{},
		hp_(0), hpm_(0), atk_(0), def_(0), mov_(0),
		isSelected_(false)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		
		imageId_ = rm.getResource(Utility::ResourceManager::ResourceType::PLAYER, rm.LANCER, 0);

		shape_.set(x_ * CHIP_SIZE, y_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE);
	}

	/**
	 * @fn
	 * 描画
	 */
	void Unit::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool Unit::animationUpdate()
	{
		return false;
	}

	/**
	 * @fn
	 * 移動
	 */
	void Unit::move(int x, int y)
	{
		x_ = x;
		y_ = y;
		joinAnimationList(AnimationKind::MOVE);
	}

	void Unit::damage(int damage)
	{
		hp_ -= damage;

		if (hp_ <= 0)
		{
			hp_ = 0;
			dead();
		}
		else
		{
			joinAnimationList(AnimationKind::DAMAGE);
		}
	}

	void Unit::dead()
	{
		joinAnimationList(AnimationKind::DESTROY);
	}

	void Unit::select(bool isSelect)
	{
		isSelected_ = isSelect;
	}

	bool Unit::createAnimation(int animationId)
	{
		return false;
	}

}