#pragma once

/// @brief 透明なレンダーテクスチャに書き込むためのBlendStateを作る
/// @return 透明用のBlendState
constexpr BlendState MakeTransparentBlendState()
{
	BlendState blendState = BlendState::Additive;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Max;
	return blendState;
}

constexpr BlendState MakeBlendState()
{
	BlendState blendState = BlendState::Default2D;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Max;
	return blendState;
}
