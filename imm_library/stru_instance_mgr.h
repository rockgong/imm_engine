////////////////
// stru_instance_mgr.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef STRU_INSTANCE_MGR_H
#define STRU_INSTANCE_MGR_H
#include "stru_model_mgr.h"
namespace imm
{
////////////////
// instance_mgr
////////////////
////////////////
template <typename T_app>
struct instance_mgr
{
	instance_mgr();
	void init(T_app *app_in);
	void reload();
	void reload_scene_instance_relate();
	template <typename instance, typename get_pos>
	void push_back(
		const std::vector<instance> &v_inst, instance_stat &inst_stat, size_t &k,
		const get_pos &get_pos_f,
		std::vector<std::string> &name);
	template <typename instance, typename get_pos>
	void push_back_pntt(
		const std::vector<instance> &v_inst, instance_stat &inst_stat, size_t &k,
		const get_pos &get_pos_f,
		std::vector<std::string> &name);
	instance_stat &get(int ix);
	int get_index(const std::string &name);
	void bound_update();
	void collision_update(float dt_every);
	void update_skinned(const float &dt);
	void remove_all();
	model_mgr m_Model;
	std::vector<instance_stat> m_Stat;
	// Bounding only use BoundingBox for lazy develop!!
	phy_bound_mgr m_BoundL;
	phy_bound_mgr m_BoundW;
	bool m_IsLoading;
	std::map<std::string, std::size_t> m_NameMap;
	int m_SceneGroundIx;
	T_app *m_App;
};
//
template <typename T_app>
instance_mgr<T_app>::instance_mgr():
	m_Model(),
	m_BoundL(),
	m_BoundW(),
	m_IsLoading(false),
	m_SceneGroundIx(0),
	m_App(nullptr)
{
	;
}
//
template <typename T_app>
void instance_mgr<T_app>::init(T_app *app_in)
{
	m_App = app_in;
}
//
template <typename T_app>
void instance_mgr<T_app>::reload()
{
	assert(!m_IsLoading);
	m_IsLoading = true;
	std::string scene_ix(m_App->m_Scene.scene_ix);
	std::wstring load_done(str_to_wstr(scene_ix));
	if (!m_Model.load(m_App->m_D3DDevice, scene_ix)) {
		load_done = L"> Scene "+load_done+L" not found.\n";
		m_App->m_Cmd.input += load_done;
		return;
	}
	m_App->m_Cmd.input += L"> Loading...\n";
	m_App->m_Cmd.input += L"> Please wait...\n";	
	remove_all();
	instance_stat inst_stat;
	size_t k = 0;
	inst_stat.type = basic;
	push_back(
		m_Model.m_InstBasic, inst_stat, k,
		[](const pos_normal_tex_tan2 &x) {return &x.pos;},
		m_Model.m_NameBasic);
	push_back(
		m_Model.m_InstBasicAlpha, inst_stat, k,
		[](const pos_normal_tex_tan2 &x) {return &x.pos;},
		m_Model.m_NameBasicAlpha);
	inst_stat.type = skinned;
	push_back(
		m_Model.m_InstSkinned, inst_stat, k,
		[](const pos_normal_tex_tan_skinned &x) {return &x.pos;},
		m_Model.m_NameSkinned);
	inst_stat.type = simple_b32;
	push_back(
		m_Model.m_InstB32, inst_stat, k,
		[](const basic32 &x) {return &x.pos;}, 
		m_Model.m_NameB32);
	inst_stat.type = simple_pntt;
	push_back_pntt(
		m_Model.m_InstPNTT, inst_stat, k,
		[](const pos_normal_tex_tan &x) {return &x.pos;},
		m_Model.m_NamePNTT);
	//
	m_SceneGroundIx = get_index(m_App->m_Scene.get_misc["ground"]);
	reload_scene_instance_relate();
	m_IsLoading = false;
	load_done = L"> Scene "+load_done+L" load done\n";
	m_App->m_Cmd.input.assign(load_done);
}
//
template <typename T_app>
void instance_mgr<T_app>::reload_scene_instance_relate()
{
	m_App->m_Control.rebuild_player();
	m_App->m_Attack.rebuild_bbox_from_instance();
	m_App->m_Scene.phy_wire.rebuild_buffer();
}
//
template <typename T_app>
template <typename instance, typename get_pos>
void instance_mgr<T_app>::push_back(
	const std::vector<instance> &v_inst, instance_stat &inst_stat, size_t &k,
	const get_pos &get_pos_f,
	std::vector<std::string> &name)
{
	for (size_t ix = 0; ix != v_inst.size(); ++ix) {
		m_NameMap[name[ix]] = k;
		inst_stat.p = &v_inst[ix];
		m_Stat.push_back(inst_stat);
		m_BoundL.push_back_empty(phy_bound_type::box);
		m_BoundW.push_back_empty(phy_bound_type::box);
		phy_set_aabb(
			m_BoundL.b1[k],
			v_inst[ix].model->m_Vertices,
			get_pos_f);
		++k;
	}
}
//
template <typename T_app>
template <typename instance, typename get_pos>
void instance_mgr<T_app>::push_back_pntt(
	const std::vector<instance> &v_inst, instance_stat &inst_stat, size_t &k,
	const get_pos &get_pos_f,
	std::vector<std::string> &name)
{
	for (size_t ix = 0; ix != v_inst.size(); ++ix) {
		m_NameMap[name[ix]] = k;
		inst_stat.p = &v_inst[ix];
		m_Stat.push_back(inst_stat);
		m_BoundL.push_back_empty(phy_bound_type::box);
		m_BoundW.push_back_empty(phy_bound_type::box);
		auto vert_range = v_inst[ix].model->get_VertexRange(v_inst[ix].subid);
		phy_set_aabb(
			m_BoundL.b1[k],
			v_inst[ix].model->m_Vertices,
			get_pos_f,
			vert_range.first,
			vert_range.second);
		++k;
	}
}
//
template <typename T_app>
instance_stat &instance_mgr<T_app>::get(int ix)
{
	if (m_App->m_Cmd.is_preparing) assert(false);
	size_t sz_ix = 0;
	if (ix > 0) sz_ix = ix;
	return m_Stat[sz_ix];
}
//
template <typename T_app>
int instance_mgr<T_app>::get_index(const std::string &name)
{
	if (!m_NameMap.count(name)) return -1;
	return static_cast<int>(m_NameMap[name]);
}
//
template <typename T_app>
void instance_mgr<T_app>::bound_update()
{
	if (m_App->m_Cmd.is_preparing) return;
	XMMATRIX world;
	for (size_t ix = 0; ix != m_Stat.size(); ++ix) {
		world = XMLoadFloat4x4(m_Stat[ix].get_World());
		m_BoundL.transform(ix, m_BoundW, world);
	}
}
// should use octree, temporary not implement
template <typename T_app>
void instance_mgr<T_app>::collision_update(float dt_every)
{
	if (m_App->m_Cmd.is_preparing) return;
	if (m_SceneGroundIx == -1) return;
	// if runtime stun
	if (dt_every > PHY_MAX_DELTA_TIME) dt_every = PHY_MAX_DELTA_TIME;
	for (size_t ix = 0; ix != m_Stat.size(); ++ix) {
		if (static_cast<int>(ix) == m_SceneGroundIx) continue;
		size_t ix_gro = m_SceneGroundIx;
		if (m_Stat[ix].phy.stand_from >= 0) ix_gro = m_Stat[ix].phy.stand_from;
		m_Stat[ix].phy.is_touch_ground = m_BoundW.intersects(ix_gro, ix);
		phy_position_update(
			dt_every,
			*(m_Stat[ix].get_World()),
			m_Stat[ix].phy,
			m_Stat[ix_gro].phy,
			m_BoundW.center(ix),
			m_Stat[ix].phy.is_touch_ground,
			m_BoundW.half_y(ix));
	}
	//
	for (size_t ix = 0; ix != m_Stat.size()-1; ++ix) {
		for (size_t ix2 = ix+1; ix2 != m_Stat.size(); ++ix2) {
			if (static_cast<int>(ix) == m_SceneGroundIx || static_cast<int>(ix2) == m_SceneGroundIx) continue;
			phy_impulse_casual(dt_every,
				*(m_Stat[ix].get_World()),
				*(m_Stat[ix2].get_World()),
				m_Stat[ix].phy,
				m_Stat[ix2].phy,
				m_BoundW.center(ix),
				m_BoundW.center(ix2),
				m_BoundW.intersects(ix, ix2));
		}
	}
}
//
template <typename T_app>
void instance_mgr<T_app>::update_skinned(const float &dt)
{
	if (m_App->m_Cmd.is_preparing) return;
	// should be multithread
	for (auto &skinned: m_Model.m_InstSkinned) skinned.update(dt);
}
//
template <typename T_app>
void instance_mgr<T_app>::remove_all()
{
	m_BoundL.remove_all();
	m_BoundW.remove_all();
	m_Stat.clear();
	m_Stat.shrink_to_fit();
}
//
}
#endif