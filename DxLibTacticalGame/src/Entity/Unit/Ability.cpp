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
		"自分より射程の短い敵に、\n攻撃を仕掛けられた時、\n先制攻撃",
		"草原マスにいる敵に、先制攻撃した時、ダメージ＋５",
		"敵の防御力を無視してダメージを与える"
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
		catch (std::out_of_range&) {}
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
		catch (std::out_of_range&) {}
		return "";
	}

}