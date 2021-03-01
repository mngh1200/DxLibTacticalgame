#include "TutorialArrow.h"
#include "Screen/BattleScreen.h"

namespace Entity {

	/**
	 * @fn
	 * ダメージエフェクトを生成
	 * @param (x) 矢印の対象座標x
	 * @param (y) 矢印の対象座標y
	 */
	shared_ptr<TutorialArrow> TutorialArrow::makeTutorialArrow(int x, int y)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		shared_ptr<TutorialArrow> objSp = make_shared<TutorialArrow>();
		objectsControl.addFigure(Screen::BattleScreen::Layer::EFFECT, objSp);

		objSp->start(x, y);

		return objSp;
	}

	/**
	 * @fn
	 * 初期処理
	 * @param (x) 矢印の対象座標x
	 * @param (y) 矢印の対象座標y
	 */
	void TutorialArrow::start(int x, int y)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 位置とサイズセット
		shape_.set(x - W / 2, y - H);
		shape_.setSize(W, H);

		baseY_ = shape_.y;

		joinAnimationList(AnimationKind::UP_DOWN);
	}

	/**
	 * @fn
	 * 描画
	 */
	void TutorialArrow::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawGraph(shape_.x, shape_.y, rm.getImage(ImageType::EFFECT, EffectId::ARROW), TRUE);
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool TutorialArrow::animationUpdate()
	{
		if (animationId_ == AnimationKind::UP_DOWN)
		{
			
			if (animation_.update(&shape_.y, baseY_, baseY_ + MOVE_Y))
			{
				// 表示終了
				destroy();
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool TutorialArrow::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::UP_DOWN)
		{
			animation_ = Animation(ANIME_MS, Animation::Direction::AlTERNATE, ANIMATION_COUNT);
			return true;
		}
		return false;
	}
}