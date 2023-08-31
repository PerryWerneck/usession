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
  * @brief Declare the user session.
  */

 #pragma once

 #include <usession/defs.h>
 #include <memory>
 #include <string>

 namespace User {

	/// @brief Session state, as reported by logind.
	/// @see sd_session_get_state
	enum State : uint8_t {
		SessionInBackground,		///< @brief Session logged in, but session not active, i.e. not in the foreground
		SessionInForeground,		///< @brief Session logged in and active, i.e. in the foreground
		SessionIsOpening,			///< @brief Session is opening.
		SessionIsClosing,			///< @brief Session nominally logged out, but some processes belonging to it are still around.

		SessionInUnknownState,		///< @brief Session in unknown state.
	};

	USESSION_API State StateFactory(const char *statename);

	class USESSION_API Session {
	public:
		virtual ~Session();

		/// @brief Get session by user name.
		std::shared_ptr<Session> factory(const char *username);

		/// @brief Get username
		virtual const char * name(bool refresh = false) const;

		/// @brief Get current session state.
		virtual State state() const = 0;

		/// @brief Is this session a remote one?
		virtual bool remote() const = 0;

		/// @brief Is this session locked?
		virtual bool locked() const = 0;

		/// @brief Is this session active?
		virtual bool active() const noexcept = 0;

		/// @brief Is this a 'system' session?
		virtual bool system() const = 0;

		/// @brief Get user's domain
		virtual std::string domain() const;

		/// @brief Get X11 display of the session.
		virtual std::string display() const;

		/// @brief Get session type.
		virtual std::string type() const;

		/// @brief The name of the service (as passed during PAM session setup) that registered the session.
		virtual std::string service() const;

		/// @brief The class of the session.
		virtual std::string classname() const;

		/// @brief The D-Bus session path.
		virtual std::string path() const;

		/// @brief Get environment value from user session.
		virtual std::string getenv(const char *varname) const;

		/// @brief Execute function as user's effective id.
		virtual void call(const std::function<void()> exec) const;

	};

 }

 namespace std {

	inline const string to_string(const Udjat::User::Session &session) noexcept {
		return session.to_string();
	}

	inline const string to_string(std::shared_ptr<User::Session> session) noexcept {
		return session->name();
	}

	inline ostream& operator<< (ostream& os, const User::Session &session) {
		return os << session.name();
	}

	inline ostream& operator<< (ostream& os, const std::shared_ptr<User::Session> session) {
		return os << session->name();
	}

	UDJAT_API const char * to_string(const User::State state) noexcept;

	inline ostream& operator<< (std::ostream& os, const User::State state) {
		return os << to_string(state);
	}

 }
