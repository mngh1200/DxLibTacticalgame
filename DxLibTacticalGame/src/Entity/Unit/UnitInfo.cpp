#include "UnitInfo.h"

namespace Entity {
	const vector<string> UnitInfo::LEN_TEXT = { "短", "中", "長" };

	/**
	 * @fn
	 * 射程のテキストを返す
	 * @return テキスト
	 */
	string UnitInfo::getLenText() const
	{
		try
		{
			return LEN_TEXT.at(len);
		}
		catch (out_of_range&) {}

		return "";
	}
}