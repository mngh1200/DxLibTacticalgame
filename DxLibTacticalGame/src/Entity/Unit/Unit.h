#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Utility/FontManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ���j�b�g�̃C���^�[�t�F�[�X�N���X
 */

namespace Entity
{
	class Unit : public Object
	{
	public:
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), imageId_(0), animation_{},
			hp_(0), hpm_(0), atk_(0), def_(0), mov_(0),
			isEnemy_(false), state_(State::NORMAL) {};
		virtual ~Unit() {};

		//! �󋵂̎��
		enum class State
		{
			NORMAL,
			MOVE,
			SELECTED,
			ACTED
		};

		void init(int x, int y, bool isEnemy);

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void damage(int damage);

		void dead();

		bool select(bool isSelect);

		/**
		 * @fn
		 * �U���͂�Ԃ�
		*/
		virtual int getAtk() const { return atk_; } ;

	protected:
		bool createAnimation(int animationId);

		void setPos(int x, int y);

		//! �摜
		int imageId_;

		//! �ʒux
		int x_;

		//! �ړ���x
		int baseX_;

		//! �ʒuy
		int y_;

		//! �ړ���y
		int baseY_;

		//! �e��X�e�[�^�X
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;



		//! ��
		State state_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		// �A�j���[�V�����̎��
		enum AnimationKind
		{
			MOVE,
			DAMAGE,
			ATACK,
			FIRE,
			DESTROY
		};
	private:
		bool isEnemy_;
	};


}