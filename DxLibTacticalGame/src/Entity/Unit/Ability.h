#pragma once
#include <climits>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

/**
 * @file Ability.h
 * @brief ���j�b�g�̃X�L��
 */

namespace Entity
{

	class Ability
	{
	public:
		enum Kind
		{
			NONE,
			AMBUSH, // �}��
			RUSH, // �ˌ�
			THROUGH // �ђ�
		};

		const static vector<string> LABELS; //! �e�X�L���̃��x��
		const static vector<string> DESCRIPTIONS; //! �e�X�L���̐�����

		Ability() : kind(Kind::NONE) {};
		Ability(int _kind) : kind(_kind) {};
		~Ability() {};

		//! �X�L���̎��
		int kind;

		string getLabel();

		string getDescription();

	private:
	};


}