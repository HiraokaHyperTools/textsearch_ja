#!/usr/bin/env python
# coding: utf-8

# Runs great on Python 3.13.5 (x64)

import os
import json
from datetime import datetime
import errno

template = """\
pushd %%~dp0
(
 cmake.exe ^
  -G "%(generatorName)s" ^
  -A "%(arch)s" ^
  -T %(toolSetName)s ^
  -D PGVER=%(PGVER)s ^
  -D CPU=%(CPU)s ^
  -D CMAKE_GENERATOR_INSTANCE="%(CMAKE_GENERATOR_INSTANCE)s" ^
  ..
) && (
 msbuild textsearch_ja.sln /p:Configuration=Release
) || (
 pause
)
popd
"""


def makeDirs(path):
    try:
        os.makedirs(path)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise


def writeTextFile(path, body):
    print("Writing %s" % (path,))
    f = open(path, "w")
    f.write(body)
    f.close()


buildAll = []
packer = []

combinations = (
    ("PG96", "9.6", "x86"),
    ("PG96", "9.6", "x64"),
    ("PG100", "10.0", "x86"),
    ("PG100", "10.0", "x64"),
    ("PG110", "11.0", "x64"),
    ("PG120", "12.0", "x64"),
    ("PG130", "13.0", "x64"),
    ("PG140", "14.0", "x64"),
    ("PG150", "15.0", "x64"),
    ("PG160", "16.0", "x64"),
    ("PG170", "17.0", "x64"),
)

for PGVER, PGVERDIR, CPU in combinations:
    for toolSetName in ("v143",):
        buildDir = "%s_%s_%s" % (PGVER, CPU, toolSetName)
        makeDirs(buildDir)
        batFilePath = "%s/build.cmd" % (buildDir,)
        dict = {
            "generatorName": "Visual Studio 17 2022",
            "toolSetName": toolSetName,
            "PGVER": PGVER,
            "CPU": CPU,
            "CMAKE_GENERATOR_INSTANCE": "H:/Program Files/Microsoft Visual Studio/2022/Professional",
            "arch": "x64" if CPU == "x64" else "Win32",
        }
        writeTextFile(batFilePath, template % dict)
        buildAll.append("call %s" % (batFilePath,))
        packDir = "pack/textsearch_ja-9.0.0-postgresql-%s-%s" % (PGVERDIR, CPU)
        releaseDir = ((CPU == "x64") and "%s/Release" or "%s/Release") % (buildDir,)
        packer.append(
            {
                "task": "copy",
                "param": {
                    "from": "%s/textsearch_ja.dll" % (releaseDir,),
                    "to": "%s/lib/" % (packDir,),
                },
            }
        )
        packer.append(
            {
                "task": "copy",
                "param": {
                    "from": "mecabrc",
                    "to": "%s/bin/" % (packDir,),
                },
            }
        )
        packer.append(
            {
                "task": "copy",
                "param": {
                    "from": "textsearch_ja.control",
                    "to": "%s/share/extension/" % (packDir,),
                },
            }
        )
        packer.append(
            {
                "task": "copy",
                "param": {
                    "from": "textsearch_ja-%s.sql" % (PGVERDIR,),
                    "to": "%s/share/extension/textsearch_ja--9.0.0.sql" % (packDir,),
                },
            }
        )

writeTextFile("buildAll.cmd", "\n".join(buildAll) + "\n")

packer.append(
    {
        "task": "7z",
        "param": {
            "rawFrom": "*",
            "to": datetime.now().strftime("packs/%Y%m%d.7z"),
            "wd": "pack",
        },
    }
)

writeTextFile("packer.json", json.dumps(packer, indent=2))
