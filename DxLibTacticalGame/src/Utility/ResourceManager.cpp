#include "ResourceManager.h"
#include "Battle/CheckWin.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager::~ResourceManager()
	{
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		for (auto&& mItr = image_.begin(); mItr != image_.end(); ++mItr)
		{
			vector<int*> vec = mItr->second;
			for (auto itr = vec.begin(); itr != vec.end(); ++itr)
			{
				delete* itr;
			}
		}
	}

	/**
	 * @fn
	 * リソースのロード
	 * @return 終了フラグ（0が正常）
	 */
	int ResourceManager::load()
	{
		int ret = 0;
		loadImages();
		loadFonts();
		loadColors();
		loadSounds();

		// mapList_[MapId::STAGE1] = loadMapCsv("resource/map/stage1.csv");

		return ret;
	}
	/**
	 * @fn
	 * 指定されたオブジェクト取得
	 * @param (type) リソースタイプ
	 * @param (kind) リソースの種類
	 * @param (pos) リソースの位置
	 * @return 対象リソースのID
	 */
	int ResourceManager::getImage(int type, int kind, int pos) const
	{
		return image_.at(type).at(kind)[pos];
	}


	int ResourceManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}
	int ResourceManager::getColor(int kind) const
	{
		return colorType_[kind];
	}
	int ResourceManager::getSound(int kind) const
	{
		return sounds_[kind];
	}

	void ResourceManager::playSound(int kind) {
		DxLib::PlaySoundMem(getInstance().sounds_[kind], DX_PLAYTYPE_BACK);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType, UNIT_KIND_LEN * 2, UNIT_KIND_LEN, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourceName, const char* resourceType, int* handle) {
		*handle = DxLib::LoadGraphToResource(resourceName, resourceType);
		THROWS_EX(*handle, resourceName);
	}

	/**
	 * @fn
	 * 複数枚絵の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (allNum) 画像数
	 * @param (xNum) 画像列数
	 * @param (yNum) 画像行数
	 * @param (w) 画像１枚あたりの幅
	 * @param (h) 画像１枚あたりの高さ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadDivImage(const char* resourceName, const char* resourceType, int allNum, int xNum, int yNum, int w, int h, int* handle)
	{
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType, allNum, xNum, yNum, w, h, handle);
		THROWS_EX(ret, resourceName);
	}

	int ResourceManager::loadImages()
	{
		int ret = 0;
		image_.insert(std::make_pair(ImageType::IMAGE, vector<int*>()));

		for (int i = 0; i < ImageId::IMAGE_ID_LEN; i++)
		{
			image_.at(ImageType::IMAGE).push_back(new int[1]);
		}
		// 背景
		loadImage(MAKEINTRESOURCE(BACKGROUND_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::BACKGROUND_MENU));

		// マス効果
		loadImage(MAKEINTRESOURCE(MASS_ATACKABLE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::MASS_ATACK));
		loadImage(MAKEINTRESOURCE(MASS_MOVABLE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::MASS_MOVE));

		image_.insert(std::make_pair(ImageType::MAP, vector<int*>()));
		image_.at(ImageType::MAP).push_back(new int[8]);
		//loadMapchip("resource/image/map/mapchip.png", image_.at(ImageType::MAP).at(0));
		loadMapchip(MAKEINTRESOURCE(MAP_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::MAP).at(0));

		// プレイヤー画像の読み込み
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[UNIT_KIND_LEN * 2]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::NORMAL));
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[UNIT_KIND_LEN * 2]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_ACTED_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::ACTED));

		// ダメージエフェクト
		image_.insert(std::make_pair(ImageType::EFFECT, vector<int*>()));
		image_.at(ImageType::EFFECT).push_back(new int[DMAGE_EFFECT_LEN]);
		loadDivImage(MAKEINTRESOURCE(EFFECT_DAMAGE_IMAGE), MAKEINTRESOURCE(IMAGE_FILE),
			DMAGE_EFFECT_LEN, DMAGE_EFFECT_LEN, 1, DMAGE_EFFECT_W, DMAGE_EFFECT_H,
			image_.at(ImageType::EFFECT).at(EffectId::DAMAGE));
		
		image_.at(ImageType::EFFECT).push_back(new int[1]);
		loadImage(MAKEINTRESOURCE(EFFECT_MISS_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::EFFECT).at(EffectId::MISS));

		// 弾丸
		image_.at(ImageType::EFFECT).push_back(new int[BULLET_POS_LEN]);
		loadDivImage(MAKEINTRESOURCE(BULLET_IMAGE), MAKEINTRESOURCE(IMAGE_FILE),
			BULLET_POS_LEN, BULLET_POS_LEN, 1, CHIP_SIZE / 4, CHIP_SIZE /4,
			image_.at(ImageType::EFFECT).at(EffectId::BULLET));

		loadFlag = true;
		return ret;
	}

	int ResourceManager::loadFonts()
	{
		int ret = 0;

		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		hdlFont_[FontType::NORMAL_S18] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 18, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S48] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 48, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S64] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 64, 3, DX_FONTTYPE_ANTIALIASING_8X8);

		// フォントの行間
		DxLib::SetFontLineSpaceToHandle(24, hdlFont_[FontType::NORMAL_S18]);
		DxLib::SetFontLineSpaceToHandle(30, hdlFont_[FontType::NORMAL_S24]);

		return ret;
	}

	int ResourceManager::loadColors()
	{
		int ret = 0;
		// 色取得
		colorType_[ColorType::WHITE] = DxLib::GetColor(255, 255, 255);

		colorType_[ColorType::MAIN_COLOR] = DxLib::GetColor(250, 244, 232);
		colorType_[ColorType::MAIN_COLOR_ON] = DxLib::GetColor(254, 213, 175);
		colorType_[ColorType::MAIN_COLOR_ON_DARK] = 0xFB8B24;

		colorType_[ColorType::SUB_COLOR] = DxLib::GetColor(124, 104, 95);
		colorType_[ColorType::SUB_COLOR_LITE] = DxLib::GetColor(224, 215, 204);
		colorType_[ColorType::SUB_COLOR_DARK] = DxLib::GetColor(76, 64, 59);

		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);

		colorType_[ColorType::PLAYER_COLOR] = DxLib::GetColor(35, 87, 137);
		colorType_[ColorType::PLAYER_COLOR_LITE] = DxLib::GetColor(186, 197, 204);

		colorType_[ColorType::ENEMY_COLOR] = DxLib::GetColor(209, 6, 6);
		colorType_[ColorType::ENEMY_COLOR_LITE] = DxLib::GetColor(233, 183, 177);

		colorType_[ColorType::POSITIVE_COLOR] = DxLib::GetColor(116, 142, 84);
		colorType_[ColorType::POSITIVE_LITE_COLOR] = DxLib::GetColor(185, 202, 165);

		colorType_[ColorType::NEGATIVE_COLOR] = DxLib::GetColor(139, 137, 130);

		colorType_[ColorType::ACCENT_COLOR] = DxLib::GetColor(204, 75, 194);

		return ret;
	}

	int ResourceManager::loadSounds()
	{
		int ret = 0;
		// 効果音取得
		sounds_[SoundKind::CLICK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CLICK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(210, sounds_[SoundKind::CLICK]);

		sounds_[SoundKind::CHECK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CHECK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(220, sounds_[SoundKind::CHECK]);

		sounds_[SoundKind::BACK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BACK_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::BACK]);

		sounds_[SoundKind::BORN] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BORN_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(170, sounds_[SoundKind::BORN]);

		sounds_[SoundKind::ATTACK_CLOSE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(ATTACK_CLOSE_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(140, sounds_[SoundKind::ATTACK_CLOSE]);

		sounds_[SoundKind::ATTACK_FIRE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(ATTACK_FIRE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(150, sounds_[SoundKind::ATTACK_FIRE]);

		sounds_[SoundKind::MOVE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(MOVE_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(130, sounds_[SoundKind::MOVE]);

		sounds_[SoundKind::WAIT] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(WAIT_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::WAIT]);

		sounds_[SoundKind::SELECT_UNIT] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(SELECT_UNIT_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(120, sounds_[SoundKind::SELECT_UNIT]);

		sounds_[SoundKind::WIN] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(WIN_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(180, sounds_[SoundKind::WIN]);

		sounds_[SoundKind::LOSE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(LOSE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::LOSE]);

		sounds_[SoundKind::MESSAGE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(MESSAGE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::MESSAGE]);

		return ret;
	}

	void ResourceManager::loadFont(const LPCSTR fontFilePath)
	{
		if (AddFontResource(fontFilePath) > 0) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
	}

	void ResourceManager::unloadFont(const LPCSTR fontFilePath)
	{
		if (RemoveFontResource(fontFilePath)) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}


	/**
	 * @fn
	 * ステージデータのロード
	 * @param (csvFilePath) ファイルパス
	 */
	void ResourceManager::loadStageData(const string stageKind, const int id, string* title, string* hint, std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H >* mapData, vector<int>* checkWinData, vector<vector<int>>* units)
	{
		std::string str_buf;
		std::string str_conma_buf;
		for (int i = 0; i < MAP_MASS_H; i++) {
			for (int j = 0; j < MAP_MASS_W; j++) {
				(*mapData)[i][j] = -1;
			}
		}

		// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
		string str = "resource/map/" + stageKind + to_string(id) + ".csv";
		const LPCSTR csvFilePath = str.c_str();
		std::ifstream ifs_csv_file(csvFilePath);

		getline(ifs_csv_file, str_buf); // ステージタイトル
		*title = str_buf;

		getline(ifs_csv_file, str_buf); // ヒント
		*hint = regex_replace(str_buf, regex("\\\\n"), "\n");

		getline(ifs_csv_file, str_buf); // 空行

		// 勝敗ルール読み込み
		getline(ifs_csv_file, str_buf);
		std::istringstream i_stream(str_buf);
		while (getline(i_stream, str_conma_buf, ','))
		{
			(*checkWinData).push_back(stoi(str_conma_buf));
		}

		getline(ifs_csv_file, str_buf); // 空行を読み込む想定


		int lineCount = 0;
		// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
		while (getline(ifs_csv_file, str_buf) && lineCount < MAP_MASS_H)
		{
			if (str_buf == "")
			{
				break; // 空行の場合終了
			}

			// 「,」区切りごとにデータを読み込むためにistringstream型にする
			std::istringstream i_stream(str_buf);
			int colCount = 0;
			// 「,」区切りごとにデータを読み込む
			while (getline(i_stream, str_conma_buf, ','))
			{
				// csvファイルに書き込む
				(*mapData)[lineCount][colCount] = stoi(str_conma_buf);
				++colCount;
			}
			++lineCount;
		}

		// 設置ユニットを読み込む
		while (getline(ifs_csv_file, str_buf))
		{
			// 「,」区切りごとにデータを読み込むためにistringstream型にする
			std::istringstream i_stream(str_buf);
			vector<int> unitData;

			// 「,」区切りごとにデータを読み込む
			while (getline(i_stream, str_conma_buf, ','))
			{
				unitData.push_back(stoi(str_conma_buf));
			}
			units->push_back(unitData);
		}

		ifs_csv_file.close();
	}

	boolean ResourceManager::isLoaded() const {
		return loadFlag;
	}

}