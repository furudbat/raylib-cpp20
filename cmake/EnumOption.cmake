# https://github.com/raysan5/raylib/blob/master/cmake/EnumOption.cmake
macro(enum_option var values description)
  set(${var}_VALUES ${values})
  list(LENGTH "${ARGN}" extra_count)
  if(${ARGC} GREATER 4)
    list(GET ARGV 3 default_value)
  else()
    list(GET ${var}_VALUES 0 default)
  endif()
  set(${var}
      "${default}"
      CACHE STRING "${description}")
  set_property(CACHE ${var} PROPERTY STRINGS ${${var}_VALUES})
  if(NOT ";${${var}_VALUES};" MATCHES ";${${var}};")
    message(FATAL_ERROR "Unknown value ${${var}}. Only -D${var}=${${var}_VALUES} allowed.")
  endif()
endmacro()

# Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
#
# This software is provided "as-is", without any express or implied warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute
# it freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this
#   software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
#
# 1. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
#
# 1. This notice may not be removed or altered from any source distribution.
