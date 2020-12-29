#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
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
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), imageId_(0), animation_{},
			hp_(0), hpm_(0), atk_(0), def_(0), mov_(0), range_(1),
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

		void back();

		void damage(int damage);

		void dead();

		bool select(bool isSelect);

		int atack(int targetRealX, int targetRealY);

		void setPos(int x, int y);

		int getMassX() const { return x_; }; // x���W��Ԃ�
		int getMassY() const { return y_; }; // y���W��Ԃ�
		int getBaseX() const { return baseX_; }; // �ړ���x���W��Ԃ�
		int getBaseY() const { return baseY_; }; // �ړ���y���W��Ԃ�
		virtual int getAtk() const { return atk_; } ; // �U���͂�Ԃ�
		int getMove() const { return mov_; }; // �ړ��͂�Ԃ�
		int getRange() const { return range_; }; // �U���͈͂�Ԃ�
		bool isEnemy() const { return isEnemy_; } // �G���j�b�g�ł��邩��Ԃ�

	protected:
		bool createAnimation(int animationId);

		

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

		//! �U�����x���W
		int targetRealX_;

		//! �U�����y���W
		int targetRealY_;

		//! �e��X�e�[�^�X
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;
		int range_;



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

		constexpr static int HP_PADDING = 5; //! HP�o�[�̗]��
		constexpr static int HP_Y = 55; //! HP�o�[�̑��Έʒuy
		constexpr static int HP_H = 5;  //! HP�o�[�̍���

		constexpr static int ANIME_ATACK_MS = 400;					//! �U���A�j���[�V�����̎���
		constexpr static int ANIME_DAMAGE_MS = ANIME_ATACK_MS / 2;	//! �_���[�W�A�j���V�����̎���
		constexpr static int ANIME_DAMAGE_MOVE = 10;				//! �_���[�W�A�j���V�����̓���͈�

	};


}