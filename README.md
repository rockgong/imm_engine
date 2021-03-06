Dragon's Ingot
==============
Dragon's Ingot source code, immature engine.

Introduction:
-------------
[Dragon's Ingot](http://dragonsingot.com/) is an indie 3D action game. The game is in early development.
This open source project is just a technic showcase.

Features:
---------
* Command-Line Build ([cl.exe](https://msdn.microsoft.com/en-us/library/9s7c9wdw.aspx)).
* The fundamental 3D rendering technique is derived from 
the [DirectX 11 book](http://www.amazon.com/dp/1936420228/) by Frank Luna.
* No visualization scene editor.
* The platform only supports Microsoft Windows.
* Little document support.
* Single core optimization.
* Mixing game data and engine code directly. (Very high coupling)

Compile Environment:
--------------------
Win 10  
Visual Studio 2017 (require Desktop development for C++)  
Windows SDK for Windows 10  
DirectX SDK (June 2010)  

Runtime Environment:
--------------------
Win 7.1 / Win 8.x / Win 10  
DirectX End-User Runtimes (June 2010)  
Visual C++ Redistributable for Visual Studio 2017  
Update KB2670838 (Windows 7.1 situation)  
DirectX 11 capable GPU  
1024X768 minimum display resolution  
DirectX 9.0c compliant sound card  

Dependent .Lib Files for compiling:
---------------------
Those files should be in VC++ Library Directories, 
notice corresponding x64/x86, Debug/Release version.
The .lib files you can get from each corresponding project.
* **DirectXTK.lib**: [DirectX Tool Kit](https://github.com/Microsoft/DirectXTK) (April 24, 2017)
* **DirectXTKAudioDX.lib**: [DirectX Tool Kit](https://github.com/Microsoft/DirectXTK)  (April 24, 2017)
* **Effects11.lib**: [Effects for Direct3D 11](https://github.com/Microsoft/FX11) (March 10, 2017), Debug version please rename.
* **lua.lib**: [Lua](https://www.lua.org/) 5.3.4
* **XInput1_3.lib**: Its orgin name is XInput.lib, from 
[DirectX SDK (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* **X3DAudio.lib**: [DirectX SDK (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* **XAPOFX.lib**: [DirectX SDK (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

How to Compile:
---------------
In sample\ folder, every subfolder include a signle sample.
The compile option see bat_tool\1.bat.
* **init_d3d**: A basic window draw empty.
* **m3dtob3m**: Command line tool, convert text .m3d model file to binary .b3m file. 
Those two types both can be read by engine.
* **simple_scene**: A very simple scene draw 4 basic geometrics. 
[(Screen Capture)](https://github.com/endrollex/imm_engine/tree/master/sample/simple_scene/simple_scene.png)

Runtime Folder Tree:
--------------------
See sample\simple_scene\, represents following tree.

	 |--ast_media
	 |--ast_model
	 |--ast_shader
	 |--ast_texture
	 |--script
         |--library
	     |--scene_common
	     |--scene00
	     |--text_en


Asset Tools:
------------
* **[immature Blender export](https://github.com/endrollex/imm_blender_export)**: 
Export Blender model data to .m3d file format for immature engine.
* **[m3dtob3m.exe](https://github.com/endrollex/imm_engine/tree/master/sample/m3dtob3m)**: 
Convert .m3d file to binary .b3m file. 
Notice .b3m file corresponding x64/x86 version.
* **CSV based scene data**: 
Unfortunately, this engine has not a scene editor let you can "drag and drop".
You must write a game object by handwriting, this limits the complexity of a scene.

Copyright and License:
----------------------
* Copyright 2015-2017 Huang Yiting (http://endrollex.com)
* immature engine is distributed under the the terms of the BSD license.
* immature engine's work not include the code in folders of suffix "_3rd_party".
* The complete game Dragon's Ingot is proprietary and retains intellectual property rights.