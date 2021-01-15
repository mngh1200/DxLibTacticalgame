#pragma once
#include <climits>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

/**
 * @file Ability.h
 * @brief ユニットのスキル
 */

namespace Entity
{

	class Ability
	{
	public:
		enum Kind
		{
			NONE,
			AMBUSH, // 迎撃
			RUSH, // 突撃
			THROUGH // 貫通
		};

		const static vector<string> LABELS; //! 各スキルのラベル
		const static vector<string> DESCRIPTIONS; //! 各スキルの説明文

		Ability() : kind(Kind::NONE) {};
		Ability(int _kind) : kind(_kind) {};
		~Ability() {};

		//! スキルの種類
		int kind;

		string getLabel();

		string getDescription();

	private:
	};


}