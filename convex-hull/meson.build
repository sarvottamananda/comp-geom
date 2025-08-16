project('chull', 'c',
  version : '0.1',
  license : 'GPL-2.0-or-later',
  default_options : ['warning_level=3', 'c_std=c23'])

hdrs = [
]

srcs = files(
  'chull.c',
  'stack.c',
  'qsort.c',
  'data.c'
)

deps = declare_dependency(
    sources : hdrs,
    include_directories : include_directories('.')
)

executable(
    'chull',
    sources : srcs,
    dependencies : deps
)
