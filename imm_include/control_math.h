////////////////
// control_math.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef CONTROL_MATH_H
#define CONTROL_MATH_H
#include <DirectXMath.h>
using namespace DirectX;
#include "control_state.h"
#include <string>
namespace imm {namespace math
{
//
void mouse_instance_move(const size_t &index, const float &speed);
//
void mouse_move_toward_hit(
	CXMVECTOR &hit_pos,
	const size_t &index,
	const float &speed);
//
void mouse_face_rot_y(
	XMMATRIX &W,
	XMMATRIX &RF,
	CXMVECTOR &direction);
//
void pad_face_rot_y(
	XMMATRIX &W,
	XMMATRIX &RF,
	XMVECTOR &direction,
	const float &rot_cam);
//
void mouse_hit_plane_y(XMVECTOR &hit_pos_out);
//
void mouse_hit_terrain(XMVECTOR &hit_pos_out);
//
void pad_move_toward(const size_t &index, const float &speed);
//
}}
#endif