#include "SaveManager.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	SaveManager& SaveManager::getInstance()
	{
		static SaveManager instance;
		return instance;
	}

	SaveManager::~SaveManager()
	{
	}

	/**
	 * @fn
	 * �Z�[�u�f�[�^�t�@�C���̃��[�h
	 * @return �I���t���O�i0������j
	 */
	int SaveManager::load()
	{
		FILE* fp;
		errno_t error;
		if ((error = fopen_s(&fp, filename, "rb")) != 0) {
			DxLib::printfDx("�Z�[�u�f�[�^�����݂��܂���B\n�V�K�쐬���܂��B");
			createSavefile();
		}
		else {
			fread(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);

		}

		return 0;
	}

	/**
     * @fn
     * �Z�[�u�f�[�^�t�@�C���̐V�K�쐬
     * @return �I���t���O�i0������j
     */
	void SaveManager::createSavefile()
	{
		FILE* fp;
		errno_t error;
		// �X�e�[�W�����N�̏�����
		for (int i = 0; i < MAX_STAGE; i++) {
			savedata.stageRanks[i] = 0;
		}

		if ((error = fopen_s(&fp, filename, "wb")) != 0) {
			DxLib::printfDx("�Z�[�u�f�[�^���쐬�ł��܂���ł����B");
		}
		else {
			fwrite(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}

		return;
	}

	/**
     * @fn
     * �Z�[�u�f�[�^�t�@�C���̍X�V
     * @return �I���t���O�i0������j
     */
	int SaveManager::save()
	{
		FILE* fp;
		errno_t error;
		if ((error = fopen_s(&fp, filename, "wb")) != 0) {
			DxLib::printfDx("�Z�[�u�f�[�^���쐬�ł��܂���ł����B");
		}
		else {
			fwrite(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}

		return 0;
	}

	/**
�@�@ * @fn
�@�@ * �X�e�[�W�����N�̍X�V
�@�@ * @return �I���t���O�i0������j
�@�@ */
	int SaveManager::updateRank(int stageKey, int stageRank)
	{
		savedata.stageRanks[stageKey] = stageRank;
		return 0;
	}
	/**
�@�@ * @fn
�@�@ * �X�e�[�W�����N�̍X�V
�@�@ * @return �I���t���O�i0������j
�@�@ */
	int SaveManager::getRank(int stageKey)
	{
		return savedata.stageRanks[stageKey];
	}
}