-- describe_instance.lua
-- read instance information to immature engine

-- require
require "script\\library\\csv_dump"

-- instance
-- all name need to be unique
csv_instance_basic    = read_csv("script\\scene00\\instance_basic.csv")
csv_instance_geometry = read_csv("script\\scene00\\instance_geometry.csv")
csv_instance_skinned  = read_csv("script\\scene00\\instance_skinned.csv")
csv_model_basic       = read_csv("script\\scene00\\model_basic.csv")
csv_model_geometry    = read_csv("script\\scene00\\model_geometry.csv")
csv_model_skinned     = read_csv("script\\scene00\\model_skinned.csv")

-- misc
ground = "grid"
player1 = "cylinder"
skybox_dds = ""
play_bgm = ""
ui_class = ""
ui_group = ""
terrain_info = ""
