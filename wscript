#! /usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2006 (ita)

APPNAME='kbilliards'
VERSION='0.8.7'

# these variables are mandatory,
srcdir = '.'
blddir = '_build_'
cachedir = '_build_/cache'

def set_options(opt):
	opt.add_option('--prefix', type='string', help='installation prefix', dest='prefix')
	opt.add_option('--destdir', type='string', help='destination for install (when creation rpms or debs)', dest='destdir')
	opt.add_option('--want-rpath', action='store_true', dest='want_rpath', default=True, help='turn rpath off or on')

	for i in "execprefix datadir libdir kdedir kdeincludes kdelibs qtdir qtincludes qtlibs libsuffix".split():
		opt.add_option('--'+i, type='string', default='', dest=i)

def configure(conf):
	conf.checkTool(['g++', 'KDE3'])
	conf.env['CXXFLAGS_QT']=['-DQT_THREAD_SUPPORT']
	#conf.checkHeader('dlfcn.h','HAVE_DLFCN_H')
	conf.addDefine('TEST_DEFINE', '345')
	conf.addDefine('VERSION', '"'+VERSION+'"')
        # WAFBUILD=1 to include .moc files
	conf.addDefine('WAFBUILD', '1')

	conf.env.appendUnique('LINKFLAGS', ['-lbz2 -lartskde'])

	# TODO until the config.h is included by main.cpp
        # WAFBUILD=1 to include .moc files
	conf.env['CXXFLAGS_QT']=['-DQT_THREAD_SUPPORT', '-DVERSION=\'\"0.8.7\"\'', '-DWAFBUILD']

	conf.writeConfigHeader('config.h')

def build(bld):
	for dir in ['po', 'src', 'media']:
		bld.add_subdirs(dir)

