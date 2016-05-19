////////////////
// phy_ai_probe.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015-2016 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef PHY_AI_PROBE_H
#define PHY_AI_PROBE_H
#include "phy_prepare.h"
namespace imm
{
////////////////
// ai_bound
////////////////
////////////////
struct ai_bound
{
	ai_bound();
	void init(
		CXMMATRIX &rot_rev,
		const float &radius_inst_in,
		const float &radius_add_in,
		const float &extents_z_add_in);
	void transform(CXMMATRIX &world);
	BoundingSphere SphL;
	BoundingSphere SphW;
	BoundingOrientedBox ObbL;
	BoundingOrientedBox ObbW;
	bool is_active;
	float radius_inst;
	float radius_add;
	float extents_z_add;
};
//
ai_bound::ai_bound():
	is_active(false),
	radius_inst(0.0f),
	radius_add(1.0f),
	extents_z_add(2.0f)
{
	;
}
//
void ai_bound::init(
	CXMMATRIX &rot_rev,
	const float &radius_inst_in,
	const float &radius_add_in,
	const float &extents_z_add_in)
{
	radius_inst = radius_inst_in;
	radius_add = radius_add_in;
	SphL.Radius = radius_inst+radius_add;
	SphL.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	SphW = SphL;
	//
	ObbL.Center = XMFLOAT3(0.0f, 0.0f, -extents_z_add_in-radius_inst);
	ObbL.Extents = XMFLOAT3(radius_inst, radius_inst, radius_inst+extents_z_add_in);
	ObbL.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	// Blender's mesh is right-hander, after export to left-handed,
	// mesh will face toward ground
	ObbL.Transform(ObbW, rot_rev);
	ObbL = ObbW;
}
//
void ai_bound::transform(CXMMATRIX &world)
{
	SphL.Transform(SphW, world);
	ObbL.Transform(ObbW, world);
}
////////////////
// ai_probe
////////////////
////////////////
template <typename T_app>
struct ai_probe
{
	ai_probe();
	void init(T_app *app_in);
	void reset();
	void rebuild();
	void update();
	void set_active(const size_t &ix, const float &radius_add);
	std::map<size_t, ai_bound> geometry;
	T_app *app;
};
//
template <typename T_app>
ai_probe<T_app>::ai_probe():
	app(nullptr)
{
	;
}
//
template <typename T_app>
void ai_probe<T_app>::init(T_app *app_in)
{
	app = app_in;
}
//
template <typename T_app>
void ai_probe<T_app>::reset()
{
	geometry.clear();
}
//
template <typename T_app>
void ai_probe<T_app>::rebuild()
{
	reset();
	for (auto &stat: app->m_Inst.m_Stat) {
		if (stat.property & MODEL_IS_CONTROLLABLE) {
			size_t ix = &stat - &app->m_Inst.m_Stat[0];
			XMMATRIX rot_front = XMLoadFloat4x4(app->m_Inst.m_Stat[ix].get_RotFront());
			XMVECTOR out_scale, out_trans, out_rot;
			assert(XMMatrixDecompose(&out_scale, &out_rot, &out_trans, rot_front));
			out_rot = XMQuaternionInverse(out_rot);
			XMMATRIX rot_rev = XMMatrixRotationQuaternion(out_rot);
			//
			float radius_inst = (app->m_Inst.m_BoundL.extents_x(ix)+app->m_Inst.m_BoundL.extents_z(ix))/2.0f;
			geometry[ix].init(rot_rev, radius_inst, 1.0f, 5.0f);
		}
	}
}
//
//
template <typename T_app>
void ai_probe<T_app>::update()
{
	for (auto &geo: geometry) {
		if (!geo.second.is_active) continue;
		geo.second.transform(XMLoadFloat4x4(app->m_Inst.m_Stat[geo.first].get_World()));
		for (size_t ix = 0; ix != app->m_Inst.m_Stat.size(); ++ix) {
			if (app->m_Inst.m_Stat[ix].property & MODEL_IS_LAND) continue;
			if (!app->m_Inst.m_Stat[ix].is_invoke_physics()) continue;
			bool is_touch = app->m_Inst.m_BoundW.intersects(ix, geo.second.SphW);
			app->m_Inst.m_Steering[geo.first].touch[ix] = is_touch;
		}
	}
}
//
template <typename T_app>
void ai_probe<T_app>::set_active(const size_t &ix, const float &radius_add)
{
	geometry[ix].is_active = true;
	geometry[ix].SphL.Radius_add = geometry[ix].radius_inst+radius_add;
}
//
}
#endif