import os

env = Environment(ENV=os.environ)

env.Append(CPPPATH = ['.'])
env.Append(CXXFLAGS = ['-O3', '-Wall', '-Wextra', '-Werror', '-pedantic', '-std=c++11', '-fopenmp'])
#env.Append(CXXFLAGS = ['-g', '-Wall', '-Wextra', '-Werror', '-pedantic', '-std=c++11', '-fopenmp'])
env.ParseConfig("root-config --libs --cflags --ldflags")
#env.Append(LIBPATH = [QFT_LIB])
#env.Append(LIBS = ['Minuit2', 'MathMore', 'qft++'])
env.Append(LINKFLAGS = ['-fopenmp'])

#Export environment
Export('env')

#Run SConscript files
SConscript('SConscript')
