#pragma once
#include <climits>
#include "Unit.h"

using namespace std;

/**
 * @file Gunner.h
 * @brief �e�����j�b�g�̃N���X
 */

namespace Entity
{

	class Gunner : public Unit
	{
	public:
		Gunner() : 
			isLoaded_(true), 
			isAtacked_(false),
			isBulletExist_(false),
			bulletX_(0),
			bulletY_(0),
			bulletAnime_{}
		{};
		~Gunner() {};

		constexpr static int ATTENUATION = 15; //! �������̋�������

		bool isAtackable() const override; // �U���\�ł��邩

	protected:
		void createAtackAnimation() override;
		bool updateAtackAnimation() override;

		void renderExtra() const override;
		void turnEndExtra(bool isOwnEnd) override;
		void getExtraStatusListExtra(vector<pair<string, string>>& list) const override;

	private:
		constexpr static int UNLOADED_MARGIN_BOTTOM = 3; //! �����U�`��̗]��
		constexpr static int UNLOADED_MARGIN_RIGHT = CHIP_SIZE / 4; //! �����U�`��̗]��

		bool isAtacked_; //! ���̃^�[���U��������
		bool isLoaded_; //! ���U�ςł��邩
		bool isBulletExist_; //! �e�ۈړ����ł��邩

		Animation bulletAnime_; //! �e�ۗp�̃A�j���[�V�����N���X

		int bulletX_, bulletY_; //! �e�ۂ̍��W
	};


}