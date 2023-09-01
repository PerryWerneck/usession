/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2021 Perry Werneck <perry.werneck@gmail.com>
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

 #include "private.h"
 #include <usession/defs.h>
 #include <private/defs.h>
 #include <usession/user.h>

 #include <dirent.h>
 #include <cstdlib>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <system_error>
 #include <sys/mman.h>
 #include <cstring>
 #include <systemd/sd-login.h>

 using namespace std;

 namespace User {

	/// @brief File on /proc/[PID]/environ
	class USESSION_PRIVATE Environ {
	private:
		int descriptor = -1;
		struct stat st;

	public:
		Environ(DIR * dir, const char *name) : descriptor(openat(dirfd(dir),(string{name} + "/environ").c_str(),O_RDONLY)) {

			memset(&st,0,sizeof(st));
			if(fstat(descriptor,&st) < 0) {
				st.st_uid = (uid_t) -1;
			}

		}

		~Environ() {

			if(descriptor > 0) {
				::close(descriptor);
			}
		}

		operator bool() const {
			return descriptor >= 0;
		}

		inline uid_t uid() {
			return st.st_uid;
		}

		inline int fd() {
			return this->descriptor;
		}

		std::string load() {

			std::string text;
			char buffer[8192];

			int loaded = -1;
			do {

				loaded = ::read(this->descriptor,buffer,8192);
				if(loaded < 0) {
					throw std::system_error(errno, std::system_category());
				} else if(loaded > 0) {
					text.append(buffer,loaded);
				}

			} while(loaded >  0);

			return text;
		}

	};


	string User::LoginDSession::getenv(const char *varname) const {

		// This is designed to find the user's dbus-session allowing
		// subscription of gnome session's lock/unlock signals from
		// system daemons

		// This would be far more easier with the fix of the issue
		// https://gitlab.gnome.org/GNOME/gnome-shell/-/issues/741#

		string value;
		size_t szName = strlen(varname);
		uid_t uid = this->id();

		// https://stackoverflow.com/questions/6496847/access-another-users-d-bus-session
		DIR * dir = opendir("/proc");
        if(!dir) {
			throw std::system_error(errno, std::system_category());
        }

        try {

			struct dirent *ent;
			while((ent=readdir(dir))!=NULL) {

				Environ environ(dir,ent->d_name);

				if(!environ || environ.uid() != uid) {
					continue;
				}

				// Check if it's the required session
				{
					char *sname = nullptr;

					// Ignome process not associated to any session.
					if(sd_pid_get_session(atoi(ent->d_name), &sname) == -ENODATA || !sname)
						continue;

					int sval = strcmp(sid.c_str(),sname);
					free(sname);

					if(sval)
						continue;
				}


				// It's an user environment, scan it.
				{
					std::string text{environ.load()};
					for(const char *ptr = text.c_str(); *ptr; ptr += (strlen(ptr)+1)) {
						if(strncmp(ptr,varname,szName) == 0 && ptr[szName] == '=') {
							value.assign(ptr+szName+1);
							break;
						}
					}
				}
			}

        } catch(...) {

			closedir(dir);
			throw;

        }

		closedir(dir);

		return value;

	}

 }
