#pragma once
#include "Define.h"
#include "DxLib.h"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @file Save Manager.h
 * @brief セーブを管理するクラス
 */

namespace Utility
{
	class SaveManager
	{
	public:
		typedef struct {
			int stageRanks[MAX_STAGE];
		}save_data_t;
		//for singleton
		SaveManager(const SaveManager&) = delete;
		SaveManager& operator=(const SaveManager&) = delete;
		SaveManager(SaveManager&&) = delete;
		SaveManager& operator=(SaveManager&&) = delete;

		static SaveManager& getInstance();

		int load();

		int updateRank(int stageKey, int stageRank);

		int save();

		int getRank(int stageKey);

	private:
		SaveManager()
			:savedata{}
		{};
		~SaveManager();

		void createSavefile();

		save_data_t savedata;

		const char* filename = "savedata.dat";
	};
}