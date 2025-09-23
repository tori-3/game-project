#pragma once
#include"Common.h"
#include"MiniGameSceneBase.h"

namespace ManjuRush {

	struct BlockEffect : IEffect {

		Array<Vec2>m_pos;
		Array<Vec2>m_v;

		explicit BlockEffect(const Vec2& p_pos)
			:m_pos{ 5 }, m_v{ 5 }
		{
			for (auto& pos : m_pos) {
				pos = p_pos + RandomVec2(10.0);
			}
			for (auto& v : m_v) {
				v = RandomVec2(1.0) * Random(40) * Vec2(2.5, 1);
			}
		}

		bool update(double t) override
		{
			const double e = EaseOutExpo(t);

			for (int i = 0; i < m_pos.size(); i++) {

				const Vec2 pos = m_pos[i] + m_v[i] * Vec2(1, 1.1) * t;

				RectF{ pos,8 }.draw(Palette::Mediumvioletred);
			}
			return(t < 0.5);
		}
	};

	//名前を変更してください
	class ManjuRush : public MiniGameSceneBase
	{
	public:
		const Font font{ 100 };
		Texture anna{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃんver2右.png" };
		const Texture anna2_j1{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j1.png" };
		const Texture anna2_j2{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j2.png" };
		const Texture anna2_j3{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2_j3.png" };
		const Texture anna2_d{ U"MiniGameAsset/まんじゅうラッシュ素材/アンナちゃん2右_d.png" };
		Texture manju{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう3.png" };
		Texture renga{ U"MiniGameAsset/まんじゅうラッシュ素材/レンガピンク.png" };
		Texture renga80{ U"MiniGameAsset/まんじゅうラッシュ素材/レンガピンク80.png" };

		Texture yama_s{ U"MiniGameAsset/まんじゅうラッシュ素材/山小.png" };
		Texture manju_s{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山小.png" };
		Texture manju_m{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山中.png" };
		Texture manju_l{ U"MiniGameAsset/まんじゅうラッシュ素材/まんじゅう山大.png" };
		Texture yama_m{ U"MiniGameAsset/まんじゅうラッシュ素材/山中.png" };
		Texture yama_l{ U"MiniGameAsset/まんじゅうラッシュ素材/山大.png" };
		Texture kumo{ U"MiniGameAsset/まんじゅうラッシュ素材/くも.png" };
		Texture kumokumo{ U"MiniGameAsset/まんじゅうラッシュ素材/くもくも.png" };

		Audio BGM = AudioAsset{ U"MiniGameBGM" };
		Audio Clear{ U"MiniGameAsset/Common/クリア2.wav" };
		Audio Touch{ U"MiniGameAsset/まんじゅうラッシュ素材/8bitダメージ8.mp3" };
		Audio Defeat{ U"MiniGameAsset/Common/やられた.wav" };
		Audio Score{ U"MiniGameAsset/まんじゅうラッシュ素材/スコア.wav" };

		RectF player{ 70, 500, 70,100 };
		Array<RectF>enemys;
		Stopwatch jumptime;

		double g = 15;
		double v = 0;
		double speed = 5;
		double rad1 = 0;
		double rad3 = 0;
		double x_e = 0;
		double score = 0;
		bool touch = false;
		bool jump = false;
		int level = 1;
		bool flag = false;
		bool clear = false;
		bool defeat = false;

		Stopwatch stopwatch{ StartImmediately::Yes };


		Effect effect;

		void onPauseStart()override;

		void onGameStart()override;

		ManjuRush(const InitData& init);

		~ManjuRush()
		{
			BGM.stop();
		}

		void gameUpdate() override;

		void gameDraw() const override;

	};

}
