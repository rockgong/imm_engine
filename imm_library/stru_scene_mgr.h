////////////////
// stru_scene_mgr.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef STRU_SCENE_MGR_H
#define STRU_SCENE_MGR_H
#include "stru_particle.h"
#include "stru_instance_mgr.h"
#include "render_sky.h"
#include "audio_dxtk.h"
#include "phy_auxiliary.h"
namespace imm
{
////////////////
// scene_mgr
////////////////
////////////////
template <typename T_app>
struct scene_mgr
{
	scene_mgr();
	~scene_mgr();
	void init_load(T_app *app_in);
	void update_atmosphere(float dt);
	void draw_d3d_atmosphere();
	void reload(const std::wstring &scene_ix);
	T_app *app;
	std::map<std::string, std::string> get_misc;
	lit_dir dir_lights[3];
	BoundingSphere bounds;
	sky *skybox;
	state_plasma plasma;
	audio_dxtk audio;
	phy_wireframe<T_app> phy_wire;
};
//
template <typename T_app>
scene_mgr<T_app>::scene_mgr():
	skybox(nullptr),
	plasma(),
	audio(),
	phy_wire()
{
	scene_dir_lights_common(dir_lights);
	bounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	bounds.Radius = sqrtf(80.0f*80.0f + 80.0f*80.0f);
}
template <typename T_app>
scene_mgr<T_app>::~scene_mgr()
{
	delete skybox;
}
//
template <typename T_app>
void scene_mgr<T_app>::init_load(T_app *app_in)
{
	app = app_in;
	plasma.init_load(app->m_D3DDevice, app->m_D3DDC);
	audio.init_load();
	phy_wire.init(app);
	app->m_Attack.init_load(app);
	reload(L"00");
}
//
template <typename T_app>
void scene_mgr<T_app>::update_atmosphere(float dt)
{
	plasma.update(dt, app->m_Timer.total_time());
	audio.update();
	app->m_Attack.update();	
}
//
template <typename T_app>
void scene_mgr<T_app>::draw_d3d_atmosphere()
{
	// Draw phy_wireframe
	phy_wire.draw();
	// Draw sky
	skybox->draw(app->m_D3DDC, app->m_Cam);
	// Restore default states, as the SkyFX changes them in the effect file.
	app->m_D3DDC->RSSetState(0);
	app->m_D3DDC->OMSetDepthStencilState(0, 0);
	// Draw particle systems last so it is blended with scene.
	plasma.draw(app->m_D3DDC, app->m_Cam);
	// Restore default states.
	app->m_D3DDC->RSSetState(0);
	app->m_D3DDC->OMSetDepthStencilState(0, 0);
	app->m_D3DDC->OMSetBlendState(0, BLEND_FACTOR_ZERO, 0xffffffff);
}
//
template <typename T_app>
void scene_mgr<T_app>::reload(const std::wstring &scene_ix)
{
	assert(!app->m_Inst.m_IsLoading);
	std::string scene_ix_str(scene_ix.begin(), scene_ix.end());
	get_misc["ground"] = "";
	get_misc["player1"] = "";
	get_misc["skybox_dds"] = "";
	get_misc["play_bgm"] = "";
	get_misc["ui_class"] = "";
	get_misc["ui_group"] = "";
	lua_reader l_reader;
	std::string describe = GLOBAL["path_lua"]+"scene"+scene_ix_str+"\\describe_instance.lua";
	l_reader.loadfile(describe);
	l_reader.map_from_global(get_misc);
	// Instance
	app->m_Control.reset();
	std::thread(
		&instance_mgr<T_app>::load,
		std::ref(app->m_Inst),
		app->m_D3DDevice,
		scene_ix_str,
		get_misc["ground"]).detach();
	// Skybox
	if (skybox != nullptr) delete skybox;
	if (csv_value_is_empty(get_misc["skybox_dds"])) {
		// Empty sky no draw
		skybox = new sky();
	}
	else {
		std::wstring path_tex(GLOBAL["path_tex"].begin(), GLOBAL["path_tex"].end());
		path_tex += str_to_wstr(get_misc["skybox_dds"]);
		skybox = new sky(app->m_D3DDevice, path_tex, 5000.0f);
	}
	// Audio
	audio.stop_bgm();
	if (!csv_value_is_empty(get_misc["play_bgm"])) audio.play_bgm(get_misc["play_bgm"]);
	// UI
	app->m_UiMgr.reload_active(get_misc["ui_class"], get_misc["ui_group"]);
}
//
}
#endif