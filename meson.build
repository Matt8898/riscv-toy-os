project('riscv-toy-os', 'c')

kernel = executable('kernel',
	'kernel/boot.S',
	'kernel/kernel.c',
	'kernel/fdt.c',
	'kernel/string.c',
	link_args: [
		'-T' + (meson.source_root() / 'kernel/kernel.x'),
		'-static'
	])
