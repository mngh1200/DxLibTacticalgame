#include "Ability.h"

namespace Entity {
	// スキルのラベル
	const vector<string> Ability::LABELS = {
		"",
		"迎撃",
		"突撃",
		"貫通"
	};

	// スキルの説明文
	const vector<string> Ability::DESCRIPTIONS = {
		"",
		"自分より射程の短い敵に攻撃された場合、先制攻撃する",
		"草原で自分から攻撃を仕掛けて先制攻撃した場合、ダメージ+5",
		"相手の防御力を無視してダメージを与える"
	};

	/**
	 * @fn
	 * スキル名（ラベル）を返す
	 * @return スキル名
	 */
	string Ability::getLabel() const
	{
		try
		{
			return LABELS[kind];
		}
		catch (std::out_of_range& oor) {}
		return "";
		
	}

	/**
	 * @fn
	 * スキルの説明文を返す
	 * @return スキルの説明文
	 */
	string Ability::getDescription() const
	{
		try
		{
			return DESCRIPTIONS[kind];
		}
		catch (std::out_of_range& oor) {}
		return "";
	}

}