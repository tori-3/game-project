#pragma once
class TalkWindow
{
public:
	class TalkInfo
	{
	public:
		String name;
		String text;
	};

	Array<TalkInfo>talks;
	size_t index = 0;
	TalkWindow()
	{
		//FontAsset::Register(U"WindowFont", 30);
	}

	void setTalk(const Array<TalkInfo>& list)
	{
		talks = list;
		m_accumlater = 0;
		index = 0;
		m_lastLength = 0;
	}

	void setTalk(Array<TalkInfo>&& list)
	{
		talks = std::move(list);
		m_accumlater = 0;
		index = 0;
		m_lastLength = 0;
	}

	void update(bool backFlg, bool nextFlg);

	//描画：RectF 表示する位置を指定,SizeF 名前ウィンドウのサイズ
	void draw(const RectF& rect, const SizeF& nameSize = { 250,50 },bool hideMode=false,StringView keyName=U"")const;

	//次に進む
	void next()
	{
		m_accumlater = 0;
		++index;
	}

	//前に戻る
	void back()
	{
		if (0 < index) {
			m_accumlater = 0;
			--index;
		}
	}

	//表示されている名前
	String getName()const
	{
		return talks[index].name;
	}

	//会話中のテキスト
	String getText()const
	{
		return talks[index].text;
	}

	//継続中かどうか(全ての会話が終了したらfalseを返す)
	bool isContinue()const
	{
		return index < talks.size();
	}

private:
	double m_accumlater = 0;

	size_t m_lastLength = 0;
};
