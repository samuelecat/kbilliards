#! /usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2005 (ita)

import os, md5, types, sys, string, stat, imp
import Params

g_trace=1
g_debug=1
g_error=1

def error(msg):
	Params.niceprint(msg, 'ERROR', 'Configuration')

def h_md5_file(filename):
	f = file(filename,'rb')
	m = md5.new()
	readBytes = 1024 # read 1024 bytes per time
	while (readBytes):
		readString = f.read(readBytes)
		m.update(readString)
		readBytes = len(readString)
	f.close()
	return m.hexdigest()

def h_md5_str(str):
	m = md5.new()
	m.update( str )
	return m.hexdigest()

def h_md5_lst(lst):
	m = md5.new()
	for el in lst: m.update(str(el))
	return m.hexdigest()

# --

def h_simple_file(filename):
	f = file(filename,'rb')
	s = f.read().__hash__()
	f.close()
	return s
	#return os.stat(filename).st_mtime

def h_simple_str(str):
	return str.__hash__()

def h_simple_lst(lst):
	val = reduce( lambda a,b : a.__hash__() ^ b.__hash__(), ['']+lst )
	return val+1

def reset():
	import Params
	Params.g_build = None

def options(**kwargs):
	pass

## index modules by absolute path
g_loaded_modules={}
## the main module is special
g_module=None

# this function requires an absolute path
def load_module(file_path, name='wscript'):
	try: return g_loaded_modules[file_path]
	except: pass

	module = imp.new_module(name)

	try:
		file = open(file_path, 'r')
		code = file.read()
	except:
		Params.fatal('The file %s could not be opened!' % file_path)

	exec code in module.__dict__
	if file: file.close()

	g_loaded_modules[file_path] = module

	return module

def set_main_module(file_path):
	# Load custom options, if defined
	global g_module
	g_module = load_module(file_path, 'wscript_main')
	
	# remark: to register the module globally, use the following:
	# sys.modules['wscript_main'] = g_module

def fetch_options(file_path):
	import Options
	# Load custom options, if defined
	file = open(file_path, 'r')
	name = 'wscript'
	desc = ('', 'U', 1)

	module = imp.load_module(file_path, file, name, desc)
	try:
		Options.g_custom_options.append(module.set_options)
	finally:
		if file: file.close()

