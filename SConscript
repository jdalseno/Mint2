Import('*')

env.SConscript('src/Mint/SConscript')
env.SConscript('src/Mojito/SConscript')

Import('*')

lib_mint = env.SharedLibrary('lib/Mint2', src_mint + src_mojito)

Export(['env', 'lib_mint'])
