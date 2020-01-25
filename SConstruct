import os
from sys import platform

if platform == "darwin": # OS X
  env = Environment(ENV=os.environ, CC='llvm-gcc', CXX="llvm-g++")
else:
  env = Environment(ENV=os.environ)

QFT_INC = os.environ['QFT_PATH'] + '/include'
QFT_LIB = os.environ['QFT_PATH'] + '/lib'

if platform == "darwin":
  env.ParseConfig("root-config --libs --cflags --ldflags")
  env.Append(CPPPATH = ['.', QFT_INC])
  env.Append(CXXFLAGS = ['-g', '-O0', '-Wall', '-Wextra', '-Wno-deprecated', '-pedantic', '-Xpreprocessor', '-fopenmp'])
  #env.Append(CXXFLAGS = ['-g', '-Wall', '-Wextra', '-Wno-deprecated-copy', '-pedantic', '-fopenmp'])
  env.Append(LIBPATH = [QFT_LIB])
  env.Append(LIBS = ['Minuit2', 'MathMore', 'qft++', 'omp'])
else:
  env.Append(CPPPATH = ['.', QFT_INC])
  env.Append(CXXFLAGS = ['-O3', '-Wall', '-Wextra', '-Werror', '-pedantic', '-std=c++11', '-fopenmp'])
  #env.Append(CXXFLAGS = ['-g', '-Wall', '-Wextra', '-Werror', '-pedantic', '-std=c++11', '-fopenmp'])
  #env.Append(CXXFLAGS = ['-O3', '-Wall', '-fprofile-arcs', '-ftest-coverage', '-Wextra', '-Werror', '-pedantic', '-std=c++11', '-fopenmp'])
  env.ParseConfig("root-config --libs --cflags --ldflags")
  env.Append(LIBPATH = [QFT_LIB])
  env.Append(LIBS = ['Minuit2', 'MathMore', 'qft++'])
  env.Append(LINKFLAGS = ['-fopenmp'])
  #env.Append(LINKFLAGS = ['-fopenmp', ' -fprofile-arcs'])

#Export environment
Export('env')

#dict = env.Dictionary()
#keys = dict.keys()
#for key in keys:
#    if "c++11" in str(dict[key]):
#         print ("construction variable = '%s', value = '%s'" % (key, dict[key]))
#dict['CCFLAGS'].remove('-stdlib=libc++')
#dict['CXXFLAGS'].remove('-std=c++11')

print ("CC is:", env['CC'])
print ("CXX is:", env['CXX'])
print ("CCFLAGS is:", env['CCFLAGS'])
print ("CXXFLAGS is:", env['CXXFLAGS'])
print ("LINKFLAGS is:", env['LINKFLAGS'])

#Run SConscript files
SConscript('SConscript')

# Set up installation environment
AddOption('--install',
	  dest='install',
          type='string',
          nargs=1,
          action='store',
          metavar='DIR',
          help='Installation directory')

install = GetOption('install')

if install != None:
   if os.path.isfile('lib/libMint2.so') is False:
      print ('\033[91m', 'ERROR: ', '\033[0m', 'Mint2 needs to be built first')
      quit()
   if not os.path.exists(install):
       os.makedirs(install)
   os.system('cp -r Mint ' + install)
   os.system('cp -r lib ' + install)
   os.system('cp -r share ' + install)
   print ('Installation complete')
