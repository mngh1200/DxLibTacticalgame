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
     */
	void SaveManager::createSavefile()
	{
		FILE* fp;
		errno_t error;
		// ステージランクの初期化
		for (int i = 0; i < MAX_STAGE; ++i) {
			savedata.stageRanks[i] = StageRank::LOCK;
		}

		// すべてのチュートリアルステージと標準ステージ１つを開放
		for (int i = 0; i < TUTORIAL_STAGE_NUM + 1; ++i)
		{
			savedata.stageRanks[i] = StageRank::NONE;
		}

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
　　 * ステージランクの取得
　　 * @return ステージランク
　　 */
	int SaveManager::getRank(int stageKey)
	{
		if (0 <= stageKey && stageKey < MAX_STAGE)
		{
			return savedata.stageRanks[stageKey];
		}
		return NULL_STAGE;
	}
}