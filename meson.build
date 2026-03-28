project('falling-sand',  'cpp',
        meson_version:   '>=1.4.0',
        version:         '0.0.1',
        license:         'GPL-3.0-or-later',
        license_files:   [ 'COPYING' ],
        default_options: [ 'warning_level=3',
                           'cpp_std=c++23' ])

deps = [ dependency('SDL3'), dependency('SDL3_image'), ]
subdir('src')

executable('falling-sand', src,
           cpp_args: [ '-fno-omit-frame-pointer' ],
           include_directories: include_directories('include'),
           dependencies:        deps)