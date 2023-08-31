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
  * @brief Implement linux session.
  */

 #include <private/defs.h>
 #include <usession/user.h>
 #include <string>
 #include <memory>

 namespace User {

	class USESSION_API Win32Session : public User::Session {
	private:

	public:
		Win32Session(const char *username) {
		}

		~Win32Session() {
		}

		/// @brief Get username
		std::string name() const override {
		}

		/// @brief Get current session state.
		State state() const override {
		}

		/// @brief Is this session a remote one?
		virtual bool remote() const override {
		}

		/// @brief Is this session locked?
		virtual bool locked() const override {
		}

		/// @brief Is this session active?
		virtual bool active() const noexcept override {
		}

		/// @brief Is this a 'system' session?
		virtual bool system() const override {
		}

		/// @brief Get user's domain
		std::string domain() const override {
		}

		/// @brief Get X11 display of the session.
		std::string display() const override {
		}

		/// @brief Get session type.
		std::string type() const override {
		}

		/// @brief The name of the service (as passed during PAM session setup) that registered the session.
		std::string service() const override {
		}

		/// @brief The class of the session.
		std::string classname() const override {
		}

		/// @brief The D-Bus session path.
		std::string path() const override {
		}

		/// @brief Get environment value from user session.
		std::string getenv(const char *varname) const override {
		}

		/// @brief Execute function as user's effective id.
		void call(const std::function<void()> exec) const override {
		}

	}

	std::shared_ptr<Session> Session::factory(const char *username) {
		return make_shared<Win32Session>(username);
	}

 }

