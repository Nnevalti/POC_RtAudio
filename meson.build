project('POC_RtAudio', 'cpp',
    default_options: ['cpp_std=c++17'])
    
compiler = meson.get_compiler('cpp')

inc = include_directories(
    './include'
)

defines = [
    '-g',
    '-Wall',
    '-Wno-deprecated-declarations',
    '-D__MACOSX_CORE__'
]

deps = [
    dependency('CoreAudio'),
    dependency('CoreFoundation'),
    dependency('threads')
]	

srcs =  [
    'src/main.cpp'
]

executable('POC_RtAudio', 
    srcs,
    include_directories: inc,
    dependencies: deps,
    link_args : ['-L' + meson.current_source_dir() + '/libs', '-l' + 'rtaudio', '-l' + 'pthread'],
    cpp_args: defines
)
