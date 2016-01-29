////////////////
// ai_attribute.h
// This file is a portion of the immature engine.
// It is distributed under the BSD license.
// Copyright 2015-2016 Huang Yiting (http://endrollex.com)
////////////////
////////////////
#ifndef AI_ATTRIBUTE_H
#define AI_ATTRIBUTE_H
#include <string>
#include <map>
namespace imm
{
////////////////
// ai_points
////////////////
////////////////
struct ai_points
{
	ai_points();
	int hp_max;
	int mp_max;
	int hp;
	int mp;
	int str;
	int mgc;
	int def;
	int res;
};
//
ai_points::ai_points():
	hp_max(20),
	mp_max(20),
	hp(13),
	mp(20),
	str(5),
	mgc(5),
	def(5),
	res(5)
{
	;
}
////////////////
// ui_attr
////////////////
////////////////
template <typename T_app>
struct ui_attr
{
	ui_attr();
	void init(T_app *app_in);
	void reset();
	void update();
	T_app *app;
	int p1_hp;
	int p1_mp;
};
//
template <typename T_app>
ui_attr<T_app>::ui_attr():
	app(nullptr),
	p1_hp(1),
	p1_mp(1)
{
	;
}
//
template <typename T_app>
void ui_attr<T_app>::init(T_app *app_in)
{
	app = app_in;
}
//
template <typename T_app>
void ui_attr<T_app>::reset()
{
	p1_hp = 1;
	p1_mp = 1;
}
//
template <typename T_app>
void ui_attr<T_app>::update()
{
	if (!app->m_UiMgr.status.m_IsActive) return;
	if (p1_hp != app->m_AiAttr.points[app->m_Control.player1].hp) {
		p1_hp = app->m_AiAttr.points[app->m_Control.player1].hp;
		float z = static_cast<float>(p1_hp) /
			static_cast<float>(app->m_AiAttr.points[app->m_Control.player1].hp_max);
		z = 1.0f - z;
		app->m_UiMgr.status.m_Rect[app->m_UiMgr.status.m_MapID["hp_bar"]].margin.z = z;
		app->m_UiMgr.status.on_resize();
	}
}
////////////////
// ai_attr
////////////////
////////////////
template <typename T_app>
struct ai_attr
{
	ai_attr();
	void init(T_app *app_in);
	void reset();
	void rebuild();
	void rebuild_points();
	void rebuild_troll();
	void update();
	T_app *app;
	ui_attr<T_app> ui;
	std::map<size_t, ai_points> points;
	std::map<std::string, size_t> points_name;
};
//
template <typename T_app>
ai_attr<T_app>::ai_attr():
	app(nullptr),
	ui(),
	points(),
	points_name()
{
	;
}
//
template <typename T_app>
void ai_attr<T_app>::init(T_app *app_in)
{
	app = app_in;
	ui.init(app);
}
//
template <typename T_app>
void ai_attr<T_app>::reset()
{
	points.clear();
	points_name.clear();
	ui.reset();	
}
//
template <typename T_app>
void ai_attr<T_app>::rebuild()
{
	reset();
	rebuild_points();
	rebuild_troll();
}
//
template <typename T_app>
void ai_attr<T_app>::rebuild_points()
{
	size_t ix = 0;
	for (auto &stat: app->m_Inst.m_Stat) {
		if (stat.type == MODEL_SKINNED) {
			points[ix];
			points_name[*(app->m_Inst.m_Stat[ix].get_ModelName())] = ix;
		}
		++ix;
	}
	if (points.count(app->m_Control.player1)) app->m_UiMgr.status.m_IsActive = true;
	else app->m_UiMgr.status.m_IsActive = false;
}
//
template <typename T_app>
void ai_attr<T_app>::rebuild_troll()
{
	;
}
//
template <typename T_app>
void ai_attr<T_app>::update()
{
	ui.update();
}
//
}
#endif