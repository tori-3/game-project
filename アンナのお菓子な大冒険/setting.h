#pragma once
//プレイヤーがどの高さまで来たら画面を切り替えるか
//constexpr int change_hieght = -100;
constexpr int change_hieght = -1000;//実質無効


//画面のどこにプレイヤーを描画するか
constexpr int draw_x = 400;
constexpr int draw_y = 570;

constexpr int rect_size = 70;


//重力加速度
constexpr double gravity = 10;

//減速の割合
constexpr double deceleration = 0.8;

