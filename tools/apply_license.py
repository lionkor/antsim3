#!/bin/python3.8

license_string = """\
// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

"""

import sys

print("apply_license v1.0 for GPL-3.0")

files = sys.argv[1:]

print("applying GPL-3.0 short notice to", len(files), "files")

i = 0
for filename in files:
    try:
        if filename.count("CMakeLists.txt") == 0:
            with open(filename, "r+", encoding="utf-8-sig") as file:
                content = file.readlines()
                if len(content) > 0 and content[0].startswith(license_string[:10]):
                    print("   ", filename, "already has the license notice")
                else:
                    file.seek(0,0)
                    file.write(license_string)
                    file.writelines(content)
    except Exception as e:
        print("    an error occured:", e)
    else:
        print("    notice added to", filename)
    i = i + 1 
    print(float(i) / float(len(files)) * 100, "%", sep="")

