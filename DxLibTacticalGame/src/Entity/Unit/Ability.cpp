#include "Ability.h"

namespace Entity {
	// �X�L���̃��x��
	const vector<string> Ability::LABELS = {
		"",
		"�}��",
		"�ˌ�",
		"�ђ�"
	};

	// �X�L���̐�����
	const vector<string> Ability::DESCRIPTIONS = {
		"",
		"�������˒��̒Z���G�ɍU�����ꂽ�ꍇ�A�搧�U������",
		"�����Ŏ�������U�����d�|���Đ搧�U�������ꍇ�A�_���[�W+5",
		"����̖h��͂𖳎����ă_���[�W��^����"
	};

	/**
	 * @fn
	 * �X�L�����i���x���j��Ԃ�
	 * @return �X�L����
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
	 * �X�L���̐�������Ԃ�
	 * @return �X�L���̐�����
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