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
  * @brief Declare linux related classes.
  */

 #pragma once

 #include <private/defs.h>
 #include <usession/user.h>
 #include <unistd.h>
 #include <string>
 #include <memory>

 using namespace std;

 namespace User {

	class USESSION_API LoginDSession : public User::Session {
	private:
		string sid;					///< @brief LoginD session ID.
		string username;			///< @brief Username.
		string dbpath;
		uid_t uid = (uid_t) -1;		///< @brief Session user id.

		/// @brief Call logind method, convert result to std::string
		/// @return Return from logind, empty if failed or invalid.
		string get(int (*callback)(const char *, char **)) const;

	public:
		LoginDSession(const char *username);

		~LoginDSession();

		uid_t id() const;

		const char * name(bool refresh) const override;

		State state() const override;

		void lock() const override;

		void unlock() const override;

		bool remote() const override;

		bool locked() const override;

		bool active() const override;

		bool system() const override;

		std::string display() const override;

		std::string type() const override;

		std::string service() const override;

		std::string classname() const override;

		std::string path() const override;

		std::string getenv(const char *varname) const override;

		void call(const std::function<void()> exec) const override;

	};

 }

