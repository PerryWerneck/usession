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
 #include <systemd/sd-login.h>

 using namespace std;

 namespace User {

	class USESSION_API LoginDSession : public User::Session {
	private:
		string sid;					///< @brief LoginD session ID.
		string username;			///< @brief Username.
		uid_t uid = (uid_t) -1;		///< @brief Session user id.

		/// @brief Call logind method, convert result to std::string
		/// @return Return from logind, empty if failed or invalid.
		string get(int (*callback)(const char *, const char **)) {

			if(sid.empty()) {
				return "";
			}

			char *value = NULL;

			int rc = callback(sid.c_str(),&value);
			if(rc < 0 || !value) {
				return "";
			}

			std::string str{value};
			free(value);
			return str;
		}

	public:
		LoginDSession(const char *username) {
		}

		~LoginDSession() {
		}

		uid_t id() const {

			if(this->uid != (uid_t) -1) {
				return this->uid;
			}

			LoginDSession *session = const_cast<LoginDSession *>(this);

			int rc = sd_session_get_uid(session->sid.c_str(), &session->uid);

			if(rc < 0) {
				throw system_error(-rc,system_category(),string{"Cant get UID for session '"} + session->sid + "'");
			}

			return session->uid;

		}

		const char * name(bool refresh) const override {

			if(refresh || username.empty()) {

				LoginDSession *session = const_cast<LoginDSession *>(this);

				if(session->uid < 0) {

					// Get user id
					if(sd_session_get_uid(sid.c_str(), &session->uid)) {
						session->uid = -1;
						return string{"@"} + sid;
					}

				}

				// Get username from userid
				int bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
				if (bufsize < 0)
					bufsize = 16384;

				string rc;
				char * buf = new char[bufsize];

				struct passwd     pwd;
				struct passwd   * result;
				if(getpwuid_r(uid, &pwd, buf, bufsize, &result)) {
					session->username = "@";
					session->username += sid;
				} else {
					session->username = buf;
				}
				delete[] buf;

			}

			return username;

		}

		State state() const override {
		}

		bool remote() const override {
			// https://www.carta.tech/man-pages/man3/sd_session_is_remote.3.html
			int rc = sd_session_is_remote(sid.c_str());
			if(rc < 0) {
				throw std::system_error(-rc,std::system_category());
			}
			return rc > 0;
		}

		bool locked() const override {
		}

		bool active() const override {
			int rc = sd_session_is_active(sid.c_str());
			if(rc < 0) {
				if(rc == -ENXIO) {
					return false;
				}
				throw std::system_error(-rc,std::system_category());
			}
			return rc > 0;
		}

		virtual bool system() const override {
			return id() < 1000;
		}

		std::string display() const override {
			return get(sd_session_get_display);
		}

		std::string type() const override {
			return get(sd_session_get_type);
		}

		std::string service() const override {
			return get(sd_session_get_service);
		}

		std::string classname() const override {
			return get(sd_session_get_class);
		}

		std::string path() const override {
		}

		std::string getenv(const char *varname) const override {
		}

		void call(const std::function<void()> exec) const override {
		}

	}

	std::shared_ptr<Session> Session::factory(const char *username) {
		return make_shared<SystemDSession>(username);
	}

 }

