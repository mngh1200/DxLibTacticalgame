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
	 * セーブデータファイルのロード
	 * @return 終了フラグ（0が正常）
	 */
	int SaveManager::load()
	{
		FILE* fp;
		errno_t error;
		if ((error = fopen_s(&fp, filename, "rb")) != 0) {
			// DxLib::printfDx("セーブデータが存在しません。\n新規作成します。");
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
     * セーブデータファイルの新規作成
     * @return 終了フラグ（0が正常）
     */
	void SaveManager::createSavefile()
	{
		FILE* fp;
		errno_t error;
		// ステージランクの初期化
		for (int i = 0; i < MAX_STAGE; i++) {
			savedata.stageRanks[i] = StageRank::LOCK;
		}

		// savedata.stageRanks[0] = StageRank::NONE;

		// テスト処理
		savedata.stageRanks[0] = StageRank::CLEAR;
		savedata.stageRanks[1] = StageRank::NEW;

		if ((error = fopen_s(&fp, filename, "wb")) != 0) {
			// DxLib::printfDx("セーブデータを作成できませんでした。");
		}
		else {
			fwrite(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}

		return;
	}

	/**
     * @fn
     * セーブデータファイルの更新
     * @return 終了フラグ（0が正常）
     */
	int SaveManager::save()
	{
		FILE* fp;
		errno_t error;
		if ((error = fopen_s(&fp, filename, "wb")) != 0) {
			DxLib::printfDx("セーブデータを作成できませんでした。");
		}
		else {
			fwrite(&savedata, sizeof(savedata), 1, fp);
			fclose(fp);
		}

		return 0;
	}

	/**
　　 * @fn
　　 * ステージランクの更新
　　 * @return 終了フラグ（0が正常）
　　 */
	int SaveManager::updateRank(int stageKey, int stageRank)
	{
		savedata.stageRanks[stageKey] = stageRank;
		return 0;
	}
	/**
　　 * @fn
　　 * ステージランクの更新
　　 * @return 終了フラグ（0が正常）
　　 */
	int SaveManager::getRank(int stageKey)
	{
		return savedata.stageRanks[stageKey];
	}
}