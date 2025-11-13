#pragma once

enum class AttackType { Punch, Summer, HeadDrop, Sliding, Rush, Donut };

struct AttackInfo
{
	AttackType type;
	int32 id;

	bool operator==(const AttackInfo& right)const = default;

	//bool operator!=(const AttackInfo& right)const
	//{
	//	return type != right.type || id != right.id;
	//}
};

template<>
struct std::hash<AttackInfo>
{
	size_t operator()(const AttackInfo& value) const noexcept
	{
		return Hash::FNV1a(value);
	}
};
