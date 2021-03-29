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
		"�������˒��̒Z���G�ɁA\n�U�����d�|����ꂽ���A\n�搧�U��",
		"�����}�X�ɂ���G�ɁA�搧�U���������A�_���[�W�{�T",
		"�G�̖h��͂𖳎����ă_���[�W��^����"
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
		catch (std::out_of_range&) {}
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
		catch (std::out_of_range&) {}
		return "";
	}

}