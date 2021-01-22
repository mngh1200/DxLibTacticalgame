#include "UnitInfo.h"

namespace Entity {
	const vector<string> UnitInfo::LEN_TEXT = { "�Z", "��", "��" };

	/**
	 * @fn
	 * �˒��̃e�L�X�g��Ԃ�
	 * @return �e�L�X�g
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