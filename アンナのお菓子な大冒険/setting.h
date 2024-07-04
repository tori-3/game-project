#pragma once
//プレイヤーがどの高さまで来たら画面を切り替えるか
//constexpr int change_hieght = -100;
constexpr static int change_hieght = -1000;//実質無効


//画面のどこにプレイヤーを描画するか
constexpr static int draw_x = 400;
constexpr static int draw_y = 570;

constexpr static int rect_size = 70;


//重力加速度
constexpr static double gravity = 10;

//減速の割合
constexpr static double deceleration = 0.8;

