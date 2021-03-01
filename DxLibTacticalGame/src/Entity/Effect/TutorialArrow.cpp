#include "TutorialArrow.h"
#include "Screen/BattleScreen.h"

namespace Entity {

	/**
	 * @fn
	 * �_���[�W�G�t�F�N�g�𐶐�
	 * @param (x) ���̑Ώۍ��Wx
	 * @param (y) ���̑Ώۍ��Wy
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
	 * ��������
	 * @param (x) ���̑Ώۍ��Wx
	 * @param (y) ���̑Ώۍ��Wy
	 */
	void TutorialArrow::start(int x, int y)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �ʒu�ƃT�C�Y�Z�b�g
		shape_.set(x - W / 2, y - H);
		shape_.setSize(W, H);

		baseY_ = shape_.y;

		joinAnimationList(AnimationKind::UP_DOWN);
	}

	/**
	 * @fn
	 * �`��
	 */
	void TutorialArrow::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawGraph(shape_.x, shape_.y, rm.getImage(ImageType::EFFECT, EffectId::ARROW), TRUE);
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool TutorialArrow::animationUpdate()
	{
		if (animationId_ == AnimationKind::UP_DOWN)
		{
			
			if (animation_.update(&shape_.y, baseY_, baseY_ + MOVE_Y))
			{
				// �\���I��
				destroy();
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
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