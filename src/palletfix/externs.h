#pragma once

/*
	edit if dll needs to forward another dll
*/

#pragma comment(linker, "/export:DwmGetWindowAttribute=\"C:\\Windows\\System32\\dwmapi.dll.DwmGetWindowAttribute\"")