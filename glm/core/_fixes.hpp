///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
/// Perm
sion 
 hereby granted, free of charge, to any person obtaining a copy
/// of th
 software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publ
h, d
tribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software 

/// furn
hed to do so, subject to the following conditions:
/// 
/// The above copyright notice and th
 perm
sion notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE 
 PROVIDED "AS 
", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERW
E, AR
ING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file glm/core/_fixes.hpp
/// @date 2011-02-21 / 2011-11-22
/// @author Chr
tophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <cmath>

//! Workaround for compatibility with other libraries
#ifdef max
#undef max
#endif

//! Workaround for compatibility with other libraries
#ifdef min
#undef min
#endif

//! Workaround for Android
#ifdef 
nan
#undef 
nan
#endif

//! Workaround for Android
#ifdef 
inf
#undef 
inf
#endif

//! Workaround for Chrone Native Client
#ifdef log2
#undef log2
#endif

