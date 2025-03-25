#pragma once
#include"Common.h"
#include"MiniGameSceneBase.h"

namespace HawkDropOut {

	class HawkDropOut : public MiniGameSceneBase
	{
	public:

		int32 clearScore = 0;

		Audio clearAudio{ U"MiniGameAsset/Common/クリア2.wav" };
		Audio defeatAudio{ U"MiniGameAsset/Common/やられた.wav" };

		bool clear = false;
		bool gameover = false;
		Timer gameOverTimer{ 2s };
		bool tornadoFlg = false;
		double groundTime = 0;

		bool gameOverTimerFlg = false;

		bool ButtonUpdate(const Rect& rect, double scale, int32 xdistance, int32 ydistance, const Texture& texture, const Font& font, int32 count, bool enabled, bool enemy, bool att)
		{
			return(enabled && !enemy);
		}

		void ButtonDraw(const Rect& rect, double scale, int32 xdistance, int32 ydistance, const Texture& texture, const Font& font, int32 count, bool enabled, bool enemy, bool att)const
		{
			if (enabled && !enemy && att)
			{
				texture.scaled(scale).drawAt(rect.x + xdistance, rect.y + ydistance);

				if (rect.mouseOver())
				{
					Cursor::RequestStyle(CursorStyle::Hand);
				}
			}
			else
			{
				rect.draw(Palette::Black);

				rect.drawFrame(2, 2, ColorF(0.5));
			}
		}

		void onPauseStart()override;

		void onGameStart()override;

		HawkDropOut(const InitData& init);

		void gameUpdate() override;

		void gameDraw() const override;

	private:

		int32 j = 0;

		bool attack = false;

		double upTime = 0.0;

		int32 numofmeat = 0;

		double plasdropspeed = 0.0;

		double Deltadistance = 0.0;

		int32 a = 0;

		int32 b = 0;

		int32 c = 0;

		int32 d = 0;

		int32 e = 0;

		double dropspeed = 10;

		bool receive = false;

		double SumTime = 0.0;

		double stopTime = 2.0;

		double around = 0.0;

		bool enemy = false;

		double cookienumber = 500;

		double astowndistance = -240;

		double bstowndistance = -500;

		double cstowndistance = -1000;

		double dstowndistance = -1600;

		double estowndistance = -2800;

		double a_stownspeed = 150;

		double b_stownspeed = 150;

		double c_stownspeed = 150;

		double d_stownspeed = 150;

		double e_stownspeed = 150;
		// 回転角度
		double angle = 0.0;

		// 回転速度（1秒あたりの回転角度）
		const double rotateSpeed = 360.0; // 1秒あたり360度

		const double aroundSpeed = 30.0;

		// タイマー
		double timer = 0.0;

		Image image{ U"MiniGameAsset/HawkDropOut/StrawberrySoldier.png" };

		Texture ishi{ image };

		Image feather{ U"MiniGameAsset/HawkDropOut/wing.png" };

		Texture wing{ feather };

		Texture attackfunction{ U"💥"_emoji };

		Texture meat{ U"🍪"_emoji };

		// クッキーの絵文字
		Texture texture{ U"🦅"_emoji };

		// 農場の絵文字
		Texture upEmoji{ U"↖️"_emoji };

		Texture tornadoEmoji{ U"🌪️"_emoji };

		// 工場の絵文字
		Texture downEmoji{ U"↙️"_emoji };

		Texture cloudEmoji{ U"☁️"_emoji };

		Texture groundTexture{ U"StageTexture/ChocolateWall.png" };

		// フォント
		Font font{ FontMethod::MSDF, 48, Typeface::Bold };

		// クッキーの表示サイズ（倍率）
		double cookieScale = 0.9;

		// 農場の所有数
		int32 farmCount = 0;

		// 工場の所有数
		int32 factoryCount = 0;

		// 農場の価格
		int32 farmCost = 0;

		// 工場の価格
		int32 factoryCost = 0;

		int32 speed = 50;

		double Aclouddistance = 100;

		double Bclouddistance = 300;

		double Cclouddistance = 600;

		double Dclouddistance = -200;

		double Eclouddistance = -300;

		int32 FlyDistance = 0;

		// クッキーの毎秒の生産量
		int32 cps = 0;

		// ゲームの経過時間の蓄積
		double accumulatedTime = 0.0;

		// クッキーのクリック円
		Circle cookieCircle{ 500, 280, 50 };

		Circle cookieCirclea{ 500, 180, 70 };

		Circle a_enemyCircle{ -200, 80, 20 };

		Circle b_enemyCircle{ -460, 160, 20 };

		Circle c_enemyCircle{ -960, 240, 20 };

		Circle d_enemyCircle{ -1560, 320, 20 };

		Circle e_enemyCircle{ -2760, 400, 20 };

		Circle meatCircle{ 1000, 360, 30 };

		Circle attackball{ 0, 0, 40 };

	};

}
