////////////////
// stru_particle.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015-2017 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef STRU_PARTICLE_H
#define STRU_PARTICLE_H
#include "render_particle.h"
#include "mesh_d3d_util.h"
#include "stru_lua_help.h"
#include <list>
namespace imm
{
////////////////
// PLASMA_TYPE
////////////////
////////////////
enum PLASMA_TYPE
{
	PLASMA_FIRE,
	PLASMA_STRIKE,
	PLASMA_STRIKE2,
	PLASMA_CHARGE,
	PLASMA_LIGHTNING,
	PLASMA_BROKEN,
	PLASMA_BLOCK,
};
////////////////
// state_plasma
////////////////
////////////////
struct state_plasma
{
	state_plasma();
	~state_plasma();
	void init_load(ID3D11Device *device, ID3D11DeviceContext *context);
	void update(const float &dt, const float &total_time);
	void draw(ID3D11DeviceContext *context, const camera &cam);
	void remove_all();
	void push_back(PLASMA_TYPE type, const float &count_down, const XMFLOAT3 &pos);
	particle pt_fire;
	particle pt_strike;
	particle pt_strike2;
	particle pt_charge;
	particle pt_lightning;
	particle pt_broken;
	particle pt_block;
	std::list<inst_plasam> list_fire;
	std::list<inst_plasam> list_strike;
	std::list<inst_plasam> list_strike2;
	std::list<inst_plasam> list_charge;
	std::list<inst_plasam> list_lightning;
	std::list<inst_plasam> list_broken;
	std::list<inst_plasam> list_block;
	bool is_active;
	size_t index_fire;
	size_t index_strike;
	size_t index_strike2;
	size_t index_charge;
	size_t index_lightning;
	size_t index_broken;
	size_t index_block;
	ID3D11ShaderResourceView *tex_fire_srv;
	ID3D11ShaderResourceView *tex_strike_srv;
	ID3D11ShaderResourceView *tex_strike2_srv;
	ID3D11ShaderResourceView *tex_charge_srv;
	ID3D11ShaderResourceView *tex_lightning_srv;
	ID3D11ShaderResourceView *tex_broken_srv;
	ID3D11ShaderResourceView *tex_block_srv;
	ID3D11ShaderResourceView *tex_random_srv;
private:
	state_plasma(const state_plasma &rhs);
	state_plasma &operator=(const state_plasma &rhs);
};
//
state_plasma::state_plasma():
	is_active(false),
	index_fire(0),
	index_strike(0),
	index_strike2(0),
	index_charge(0),
	index_lightning(0),
	index_broken(0),
	index_block(0),
	tex_fire_srv(nullptr),
	tex_strike_srv(nullptr),
	tex_strike2_srv(nullptr),
	tex_charge_srv(nullptr),
	tex_lightning_srv(nullptr),
	tex_broken_srv(nullptr),
	tex_block_srv(nullptr),
	tex_random_srv(nullptr)
{
	;
}
//
state_plasma::~state_plasma()
{
	RELEASE_COM(tex_fire_srv);
	RELEASE_COM(tex_strike_srv);
	RELEASE_COM(tex_strike2_srv);
	RELEASE_COM(tex_charge_srv);
	RELEASE_COM(tex_lightning_srv);
	RELEASE_COM(tex_broken_srv);
	RELEASE_COM(tex_block_srv);
	RELEASE_COM(tex_random_srv);
}
//
void state_plasma::init_load(ID3D11Device *device, ID3D11DeviceContext *context)
{
	std::string concrete = IMM_PATH["script"]+"scene_common\\state_plasma.lua";
	std::map<std::string, std::string> get_dds;
	get_dds["plasma_fire_dds"] = "";
	get_dds["plasma_strike_dds"] = "";
	get_dds["plasma_strike2_dds"] = "";
	get_dds["plasma_charge_dds"] = "";
	get_dds["plasma_lightning_dds"] = "";
	get_dds["plasma_broken_dds"] = "";
	get_dds["plasma_block_dds"] = "";
	lua_reader l_reader;
	l_reader.loadfile(concrete);
	l_reader.map_from_string(get_dds);
	if (csv_value_is_empty(get_dds["plasma_fire_dds"]) ||
		csv_value_is_empty(get_dds["plasma_strike_dds"]) ||
		csv_value_is_empty(get_dds["plasma_strike2_dds"]) ||
		csv_value_is_empty(get_dds["plasma_charge_dds"]) ||
		csv_value_is_empty(get_dds["plasma_lightning_dds"]) ||
		csv_value_is_empty(get_dds["plasma_broken_dds"]) ||
		csv_value_is_empty(get_dds["plasma_block_dds"])) {
	//
		is_active = false;
		return;
	}
	else {
		is_active = true;
	}
	std::wstring path_tex = str_to_wstr(IMM_PATH["texture"]+"particle\\");
	tex_random_srv = create_RandomTexture1DSRV(device);
	std::vector<std::wstring> file_names;
	// Fire
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_fire_dds"]));
	tex_fire_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_fire.init(device, effects::m_PtFireFX, tex_fire_srv, tex_random_srv, 512, 16);
	// Strike
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_strike_dds"]));
	tex_strike_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_strike.init(device, effects::m_PtStrikeFX, tex_strike_srv, tex_random_srv, 512, 16);
	// Strike2
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_strike2_dds"]));
	tex_strike2_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_strike2.init(device, effects::m_PtGuardFX, tex_strike2_srv, tex_random_srv, 512, 16);
	// Charge
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_charge_dds"]));
	tex_charge_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_charge.init(device, effects::m_PtChargeFX, tex_charge_srv, tex_random_srv, 512, 16);	
	// Lightning
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_lightning_dds"]));
	tex_lightning_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_lightning.init(device, effects::m_PtLightningFX, tex_lightning_srv, tex_random_srv, 1, 16);
	// Broken
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_broken_dds"]));
	tex_broken_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_broken.init(device, effects::m_PtBrokenFX, tex_broken_srv, tex_random_srv, 32, 16);
	// Block
	file_names.clear();
	file_names.push_back(path_tex+str_to_wstr(get_dds["plasma_block_dds"]));
	tex_block_srv = create_Texture2DArraySRV(device, context, file_names);
	pt_block.init(device, effects::m_PtBlockFX, tex_block_srv, tex_random_srv, 32, 16);
}
//
void state_plasma::update(const float &dt, const float &total_time)
{
	if (!is_active) return;
	pt_fire.update(dt, total_time);
	pt_strike.update(dt, total_time);
	pt_strike2.update(dt, total_time);
	pt_charge.update(dt, total_time);
	pt_lightning.update(dt, total_time);
	pt_broken.update(dt, total_time);
	pt_block.update(dt, total_time);
	for (auto &inst: list_fire) inst.update(dt);
	for (auto &inst: list_strike) inst.update(dt);
	for (auto &inst: list_strike2) inst.update(dt);
	for (auto &inst: list_charge) inst.update(dt);
	for (auto &inst: list_lightning) inst.update(dt);
	for (auto &inst: list_broken) inst.update(dt);
	for (auto &inst: list_block) inst.update(dt);
	auto is_should_del =
		[](const inst_plasam &pla) {return (pla.count_down > -99.0f && pla.count_down < 0.0f);};
	list_fire.remove_if(is_should_del);
	list_strike.remove_if(is_should_del);
	list_strike2.remove_if(is_should_del);
	list_charge.remove_if(is_should_del);
	list_lightning.remove_if(is_should_del);
	list_broken.remove_if(is_should_del);
	list_block.remove_if(is_should_del);
	//
}
//
void state_plasma::draw(ID3D11DeviceContext *context, const camera &cam)
{
	if (!is_active) return;
	pt_fire.set_EyePos(cam.get_Position());
	pt_strike.set_EyePos(cam.get_Position());
	pt_strike2.set_EyePos(cam.get_Position());
	pt_charge.set_EyePos(cam.get_Position());
	pt_lightning.set_EyePos(cam.get_Position());
	pt_broken.set_EyePos(cam.get_Position());
	pt_block.set_EyePos(cam.get_Position());
	pt_fire.draw_list(context, cam, list_fire);
	pt_strike.draw_list(context, cam, list_strike);
	pt_strike2.draw_list(context, cam, list_strike2);
	pt_charge.draw_list(context, cam, list_charge);
	pt_lightning.draw_list(context, cam, list_lightning);
	pt_broken.draw_list(context, cam, list_broken);
	pt_block.draw_list(context, cam, list_block);
}
//
void state_plasma::remove_all()
{
	list_fire.clear();
	list_strike.clear();
	list_strike2.clear();
	list_charge.clear();
	list_lightning.clear();
	list_broken.clear();
	list_block.clear();
}
//
void state_plasma::push_back(PLASMA_TYPE type, const float &count_down, const XMFLOAT3 &pos)
{
	auto do_push_back =
		[&](size_t &myindex, std::list<inst_plasam> &mylist, const particle &mypt) {
		++myindex;
		if (myindex > mypt.get_ListSize()-1) myindex = 0;
		mylist.emplace_back();
		mylist.back().pos = pos;
		mylist.back().count_down = count_down;
		mylist.back().slot = myindex;
		};
	//
	switch(type) {
	case PLASMA_FIRE:
		do_push_back(index_fire, list_fire, pt_fire);
		break;
	case PLASMA_STRIKE:
		do_push_back(index_strike, list_strike, pt_strike);
		break;
	case PLASMA_STRIKE2:
		do_push_back(index_strike2, list_strike2, pt_strike2);
		break;
	case PLASMA_CHARGE:
		do_push_back(index_charge, list_charge, pt_charge);
		break;
	case PLASMA_LIGHTNING:
		do_push_back(index_lightning, list_lightning, pt_lightning);
		break;
	case PLASMA_BROKEN:
		do_push_back(index_broken, list_broken, pt_broken);
		break;
	case PLASMA_BLOCK:
		do_push_back(index_block, list_block, pt_block);
		break;
	}
}
//
}
#endif