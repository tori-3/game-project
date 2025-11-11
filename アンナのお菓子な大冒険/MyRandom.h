#pragma once

/// @brief 均一かつ2連続で同じ値にならないランダム
class MyRandom
{
public:

	MyRandom(int32 max)
		:m_max{ max }
	{
		reset();
	}

	int32 get()
	{
		int32 num = m_list.back();
		m_list.pop_back();

		if (m_list.isEmpty())
		{
			reset();

			if (m_list.back() == num)
			{
				std::swap(m_list.front(), m_list.back());
			}
		}

		return num;
	}

private:
	const int32 m_max;

	Array<int32>m_list;

	void reset()
	{
		for (int32 i = 0; i <= m_max; ++i)
		{
			m_list.push_back(i);
		}
		m_list.shuffle();
	}
};
