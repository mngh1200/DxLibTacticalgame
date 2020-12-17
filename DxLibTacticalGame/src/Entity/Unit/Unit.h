#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"

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
		Unit();
		virtual ~Unit() {};

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void damage(int damage);

		void dead();

		/**
		 * @fn
		 * �U���͂�Ԃ�
		*/
		virtual int getAtk() const { return atk_; } ;

		void select(bool isSelect);

	protected:
		bool createAnimation(int animationId);

		//! �摜
		int imageId_;

		//! �ʒux
		int x_;

		//! �ʒuy
		int y_;

		//! �A�j���[�V�����N���X
		Animation animation_;

		//! �e��X�e�[�^�X
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;

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
		bool isSelected_;
	};


}