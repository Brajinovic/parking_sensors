SRCS = main.c
CFLAGS = -lglut -lGL -lX11 -lXtst
compile:
	gcc $(SRCS) -o main.out $(CFLAGS)
	./main.out

generate_dependencies:
	makedepend -- $(CFLAGS) -- $(SRCS)

key_press:
	gcc uart_handler.c -o uart_handler.out -lX11 -lXtst
	./uart_handler.out# DO NOT DELETE

main.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
main.o: /usr/include/features.h /usr/include/features-time64.h
main.o: /usr/include/bits/wordsize.h /usr/include/bits/timesize.h
main.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/bits/time64.h /usr/include/bits/types/__fpos_t.h
main.o: /usr/include/bits/types/__mbstate_t.h
main.o: /usr/include/bits/types/__fpos64_t.h /usr/include/bits/types/__FILE.h
main.o: /usr/include/bits/types/FILE.h /usr/include/bits/types/struct_FILE.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/floatn.h
main.o: /usr/include/bits/floatn-common.h /usr/include/GL/glut.h
main.o: /usr/include/GL/freeglut_std.h /usr/include/GL/gl.h
main.o: /usr/include/GL/glext.h /usr/include/KHR/khrplatform.h
main.o: /usr/include/stdint.h /usr/include/bits/wchar.h
main.o: /usr/include/bits/stdint-intn.h /usr/include/bits/stdint-uintn.h
main.o: /usr/include/GL/glu.h /usr/include/stdlib.h
main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
main.o: /usr/include/sys/types.h /usr/include/bits/types/clock_t.h
main.o: /usr/include/bits/types/clockid_t.h /usr/include/bits/types/time_t.h
main.o: /usr/include/bits/types/timer_t.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/endianness.h
main.o: /usr/include/bits/byteswap.h /usr/include/bits/uintn-identity.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/types/sigset_t.h
main.o: /usr/include/bits/types/__sigset_t.h
main.o: /usr/include/bits/types/struct_timeval.h
main.o: /usr/include/bits/types/struct_timespec.h
main.o: /usr/include/bits/pthreadtypes.h
main.o: /usr/include/bits/thread-shared-types.h
main.o: /usr/include/bits/pthreadtypes-arch.h
main.o: /usr/include/bits/atomic_wide_counter.h
main.o: /usr/include/bits/struct_mutex.h /usr/include/bits/struct_rwlock.h
main.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
main.o: /usr/include/string.h /usr/include/bits/types/locale_t.h
main.o: /usr/include/bits/types/__locale_t.h /usr/include/strings.h
main.o: /usr/include/errno.h /usr/include/bits/errno.h
main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
main.o: /usr/include/asm-generic/errno.h
main.o: /usr/include/asm-generic/errno-base.h /usr/include/sys/time.h
