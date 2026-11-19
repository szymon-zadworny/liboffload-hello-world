project('offload-hello-world', 'c', default_options: ['c_std=c23', 'buildtype=release', 'optimization=3'])

cc = meson.get_compiler('c')

# Assemble the textual SPIR-V into a binary module.
spirv = custom_target('spirv',
  input: 'src/vector-add.spvasm',
  output: 'vector-add.spv',
  command: [find_program('spirv-as'), '-o', '@OUTPUT@', '@INPUT@'],
)

# Device image consumed by the #embed directive in src/main.c.
image = custom_target('image',
  input: spirv,
  output: 'image.bin',
  command: [find_program('llvm-offload-binary'), '-o', '@OUTPUT@', '--image=file=@INPUT@,triple=spirv64-intel'],
)

exe = executable('offload-hello-world', 'src/main.c', image,
  c_args: '--embed-dir=' + meson.current_build_dir(),
  dependencies: cc.find_library('LLVMOffload'),
)
