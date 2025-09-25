#!/usr/bin/env python
# coding: utf-8

# Runs great on Python 3.13.5 (x64)

import os
import json
import shutil
import subprocess
import errno

def makeDirs(path):
	try:
		os.makedirs(path)
	except OSError as e:
		if e.errno != errno.EEXIST:
			raise

def getDirFromCopyTo(path):
	path = path.replace("/", "\\")
	pos = path.rfind('\\')
	if pos == -1:
		return "."
	return path[0:pos]

def appendFileNameIfNot(copyTo, copyFrom):
	if copyTo[-1] in ("/", "\\"):
		return copyTo + os.path.basename(copyFrom)
	return copyTo

workDir = os.getcwd()

f = open(os.path.join(workDir, "packer.json"), "r")
packer = json.load(f)
f.close()

for task in packer:
	param = task["param"]
	if task["task"] == "copy":
		print(u"Copying")
		src = os.path.abspath(os.path.join(workDir, param["from"]))
		dst = os.path.abspath(os.path.join(workDir, appendFileNameIfNot(param["to"], param["from"])))
		makeDirs(os.path.dirname(dst))
		shutil.copy(src, dst)
	elif task["task"] == "7z":
		print(u"7z-ing")
		src = param["rawFrom"]
		dst = os.path.abspath(os.path.join(workDir, param["to"]))
		wd = os.path.abspath(os.path.join(workDir, param["wd"]))
		p = subprocess.Popen(("7z.exe", "a", "-r", dst, src), cwd = wd)
		p.wait()
