#pragma once
class TalkWindow {

	double accumlater = 0;

	size_t lastLength = 0;


public:
	class TalkInfo {
	public:
		String name;
		String text;
	};

	Array<TalkInfo>talks;
	size_t index = 0;
	TalkWindow() {
		FontAsset::Register(U"WindowFont", 30);
	}

	void setTalk(const Array<TalkInfo>& list) {
		talks = list;
		accumlater = 0;
		index = 0;
		lastLength = 0;
	}

	void setTalk(Array<TalkInfo>&& list) {
		talks = std::move(list);
		accumlater = 0;
		index = 0;
		lastLength = 0;
	}

	//更新：bool 次に進むか,bool 音量
	void update() {

		const bool backFlg = KeyA.down() || KeyLeft.down();
		const bool nextFlg = KeyD.down() || KeyRight.down() || KeyEnter.down();

		//確認
		if (not isContinue())return;

		accumlater += Scene::DeltaTime()/0.05;

		const size_t length = static_cast<size_t>(accumlater);
		const String& text = talks[index].text;

		lastLength = length;


		if (nextFlg) {
			if (text.size() < length) {
				if (isContinue()) {
					next();
					lastLength = 0;
				}
			}
			else {
				accumlater = (double)text.size();
			}
		}
		else if (backFlg) {
			back();
		}
	}

	//描画：RectF 表示する位置を指定,SizeF 名前ウィンドウのサイズ
	void draw(const RectF& rect, const SizeF& nameSize = { 250,50 })const {

		if (not isContinue())return;

		constexpr double thickness = 5;

		const RoundRect window{ rect,20 };
		const RoundRect nameWindow{ rect.pos - Vec2::Down(nameSize.y - thickness),nameSize ,20 };
		window.draw({ Palette::Black, 0.8 }).drawFrame(thickness, 0);
		nameWindow.draw({ Palette::Black ,0.8 }).drawFrame(thickness, 0);

		//確認

		const TalkInfo& talk = talks[index];

		const size_t length = static_cast<size_t>(accumlater);
		FontAsset(U"WindowFont")(talk.text.substr(0, length)).draw(window.rect.stretched(-20));
		FontAsset(U"WindowFont")(talk.name).drawAt(nameWindow.rect.center());

		if (talk.text.size() < length) {
			Triangle{ window.rect.br().movedBy(-40, -40), 20, 180_deg }.draw(ColorF{ 1, Periodic::Sine0_1(2.0s) });
		}

		FontAsset(U"WindowFont")(U"←A 戻る　　次へ D→").drawBase(20, window.rect.bl() + Vec2{ 20,-20 });
	}

	//次に進む
	void next() {
		accumlater = 0;
		++index;
	}

	//前に戻る
	void back() {

		if (0 < index) {
			accumlater = 0;
			--index;
		}
	}

	//表示されている名前
	const String& getName()const {
		return talks[index].name;
	}

	//会話中のテキスト
	const String& getText()const {
		return talks[index].text;
	}

	//継続中かどうか(全ての会話が終了したらfalseを返す)
	bool isContinue()const {
		return index < talks.size();
	}
};
