/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2023 Perry Werneck <perry.werneck@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 /**
  * @brief Common definitions for user session libarary.
  */

 #pragma once

  #if defined(_WIN32)

	#define USESSION_API		__declspec (dllexport)
	#define USESSION_PRIVATE

 #else

	#define USESSION_API		__attribute__((visibility("default")))
	#define USESSION_PRIVATE	__attribute__((visibility("hidden")))

 #endif

 #ifdef __cplusplus
 namespace User {


 }
 #endif // __cplusplus
