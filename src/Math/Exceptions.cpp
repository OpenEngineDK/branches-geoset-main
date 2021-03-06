// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Math/Exceptions.h>
#include <Utils/Convert.h>

namespace OpenEngine {
namespace Math {

using OpenEngine::Utils::Convert;

IndexOutOfBounds::IndexOutOfBounds(int i, int l, int u)
    : out_of_range("Index " + Convert::ToString(i) +
                   " not in range: " + Convert::ToString(l) +
                   " to " + Convert::ToString(u) + ".") {
}

} // Math
} // OpenEngine
