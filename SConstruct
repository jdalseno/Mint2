import os

env = Environment(ENV=os.environ)

QFT_INC = os.environ['QFT_PATH'] + '/include'
QFT_LIB = os.environ['QFT_PATH'] + '/lib'

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
      print '\033[91m' 'ERROR: ' '\033[0m' 'Mint2 needs to be built first'
      quit()
   if not os.path.exists(install):
       os.makedirs(install)
   os.system('cp -r Mint ' + install)
   os.system('cp -r lib ' + install)
   print 'Installation complete'
