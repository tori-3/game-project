#pragma once
# include"Common.h"
#include"MiniGameSceneBase.h"

namespace AnnaMusicGame {

	std::pair<Array<OffsetCircular>, Array<OffsetCircular>> LoadNotes(const FilePath& path, double s);


	Array<String> LoadDates(const FilePath& path);

	struct Judge : IEffect
	{
		int32 m_jud;
		int32 m_rg;
		struct Star
		{
			Vec2 start;
			Vec2 velocity;
			ColorF color;
		};



		Array<Star> m_stars;
		Array<Star> m_stars_p;
		const Font m_f{ FontMethod::MSDF,50 };

		explicit Judge(const int32& jud, const int32& rg);

		bool update(double t)override;

	};

	class SpriteSheetAnimation
	{
	private:
		Texture m_spriteSheet;
		int m_frameCountX;
		int m_frameCountY;
		int m_frameWidth;
		int m_frameHeight;
		int m_totalFrames;
		double m_frameDuration;
		double m_elapsedTime;
		bool m_loop;


	public:
		SpriteSheetAnimation(const FilePath& spriteSheetPath, int frameCountX, int frameCountY, int frameWidthX, int frameheightY, double frameDuration, bool loop = true)
			: m_spriteSheet(spriteSheetPath), m_frameCountX(frameCountX), m_frameCountY(frameCountY),
			m_frameWidth(frameWidthX), m_frameHeight(frameheightY),
			m_totalFrames(frameCountX* frameCountY), m_frameDuration(frameDuration), m_elapsedTime(0.0),
			m_loop(loop)
		{
		}

		void update(double deltaTime)
		{
			m_elapsedTime += deltaTime;

			if (!m_loop && m_elapsedTime >= m_totalFrames * m_frameDuration)
			{
				m_elapsedTime = m_totalFrames * m_frameDuration;

			}
		}

		void reset()
		{
			m_elapsedTime = 0.0;

		}

		void draw(const Vec2& pos) const;

	};




	//名前を変更してください
	class AnnaMusicGame : public MiniGameSceneBase
	{
	public:
		//ここに変数などを宣言
		double s = 9.0;


		int32 combo = 0;
		int32 maxcombo = 0;

		int32 jud = 0;

		int32 perfect = 0;
		int32 miss = 0;

		Effect effect;


		Array<String> detas = LoadDates(U"MiniGameAsset/AnnaMusicGame/{}_dates.txt"_fmt(getData().mini_mode));
		int32 bpm = Parse<double>(detas[0]);

		int32 st = Parse<double>(detas[1]);

		String credit = detas[2];

		std::pair<Array<OffsetCircular>, Array<OffsetCircular>> notes = LoadNotes(U"MiniGameAsset/AnnaMusicGame/{}_notes.txt"_fmt(getData().mini_mode), s);
		Array<OffsetCircular>& rcs = notes.first;
		Array<OffsetCircular>& gcs = notes.second;

		const Audio music{ U"MiniGameAsset/AnnaMusicGame/{}.mp3"_fmt(getData().mini_mode) ,Loop::No };

		const Font font{ FontMethod::MSDF,50 };

		const Circle C0{ 550,600,40 };
		const Circle C1{ 550, 600, 35 };
		const Circle C2{ 550,600,30 };
		const Circle C3{ OffsetCircular{ Point{-1450,600},2000,(100_deg) },130 };

		const Audio SE{ U"MiniGameAsset/AnnaMusicGame/Tambourine02-01(Open).mp3" };
		Stopwatch stopwatch{ StartImmediately::Yes };

		Texture candyr{ U"MiniGameAsset/AnnaMusicGame/candyr.png" };
		Texture candyg{ U"MiniGameAsset/AnnaMusicGame/candyg.png" };

		int32 rg = 0;

		double frameduration = 1.0 / (bpm / 60) / 10;

		Array<SpriteSheetAnimation>anime =
		{
			SpriteSheetAnimation(U"MiniGameAsset/AnnaMusicGame/anime_00.png", 10, 4, 400, 400, frameduration, true),
			SpriteSheetAnimation(U"MiniGameAsset/AnnaMusicGame/anime_11.png", 10, 4, 400, 400, frameduration, true),
			SpriteSheetAnimation(U"MiniGameAsset/AnnaMusicGame/anime_22.png", 5, 1, 400, 400, frameduration, true)

		};
		int32 nowanimation = 1;


		Vec2 pos = { 100,300 };


		double t = 0;
		bool change = false;


	public:

		RenderTexture background{ Scene::Size()/4 };
		bool saveFlg = false;

		void onPauseStart()override
		{
			stopwatch.pause();
			music.pause();
		}

		void onGameStart()override
		{
			if(music.isPaused())
			{
				music.play(BGMMixBus);
			}
			stopwatch.resume();
		}

		AnnaMusicGame(const InitData& init);

		void gameUpdate() override;

		void gameDraw() const override;
	};
}
