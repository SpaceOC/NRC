/*
    Copyright (C) 2024-2024  SpaceOC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifdef _WIN32
    /*
     * This C++ library  allows a C++ console program to use all
     * common C++ input commands to input with UTF-8 in Windows.
     *
     * For more information, visit:
     * https://github.com/Jalopy-Tech/WUTF8Console
     *
     */
    #include <wutf8console.h>
#endif
#include "Core/main.h"

void core::main::fixNOW() {
    #ifdef _WIN32
        wutf8console::setupConsole();
    #endif
}