#include "../../inc/screen/DC.hpp"

DC::DC(const HWND hWnd): _hWnd(hWnd), _hDc(GetDC(hWnd)) { }

HDC DC::getHDC() const { return _hDc; }

DC::~DC() { ReleaseDC(_hWnd, _hDc); }
