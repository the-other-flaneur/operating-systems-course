
# Proyecto MyBash de Sistemas Operativos FAMAF 2024 grupo 51 

## Autores:
### Felipe Tomas Pagani
### Mauro Elias Centurion
### Braian David Rodriguez

## Sobre las branchs:
En el desarrollo del lab queríamos implementar distintas branchs por cada funcionalidad distinta (parsing, execute, builtin, etc.). No las borramos 
para que se vea el progreso del desarrollo.

## Memtest:

En la ejecucion de:

```bash
$ make memtest 

gcc `pkg-config --cflags glib-2.0` -std=gnu11 -Wall -Wextra
...
--show-reachable=yes --suppressions=glib.supp ./leaktest

==11457== Memcheck, a memory error detector
==11457== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==11457== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==11457== Command: ./leaktest
==11457== 
==11457== 
==11457== HEAP SUMMARY:
==11457==     in use at exit: 16,768 bytes in 7 blocks
==11457==   total heap usage: 107 allocs, 100 frees, 18,330 bytes allocated
==11457== 
==11457== 32 bytes in 1 blocks are still reachable in loss record 1 of 7
==11457==    at 0x484B133: calloc (vg_replace_malloc.c:1675)
==11457==    by 0x48D2871: g_malloc0 (gmem.c:133)
==11457==    by 0x48B2A5D: g_hash_table_setup_storage.lto_priv.0 (ghash.c:536)
==11457==    by 0x48B2AF2: g_hash_table_new_full (ghash.c:1028)
==11457==    by 0x488E172: UnknownInlinedFun (gquark.c:64)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:376)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E172: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 32 bytes in 1 blocks are still reachable in loss record 2 of 7
==11457==    at 0x484B133: calloc (vg_replace_malloc.c:1675)
==11457==    by 0x48D2871: g_malloc0 (gmem.c:133)
==11457==    by 0x48B2A5D: g_hash_table_setup_storage.lto_priv.0 (ghash.c:536)
==11457==    by 0x48B2AF2: g_hash_table_new_full (ghash.c:1028)
==11457==    by 0x488E1A2: UnknownInlinedFun (gerror.c:50)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:377)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E1A2: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 64 bytes in 1 blocks are still reachable in loss record 3 of 7
==11457==    at 0x484B347: realloc (vg_replace_malloc.c:1800)
==11457==    by 0x48D292A: g_realloc (gmem.c:171)
==11457==    by 0x48B2A48: UnknownInlinedFun (ghash.c:324)
==11457==    by 0x48B2A48: g_hash_table_setup_storage.lto_priv.0 (ghash.c:534)
==11457==    by 0x48B2AF2: g_hash_table_new_full (ghash.c:1028)
==11457==    by 0x488E172: UnknownInlinedFun (gquark.c:64)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:376)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E172: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 64 bytes in 1 blocks are still reachable in loss record 4 of 7
==11457==    at 0x484B347: realloc (vg_replace_malloc.c:1800)
==11457==    by 0x48D292A: g_realloc (gmem.c:171)
==11457==    by 0x48B2A48: UnknownInlinedFun (ghash.c:324)
==11457==    by 0x48B2A48: g_hash_table_setup_storage.lto_priv.0 (ghash.c:534)
==11457==    by 0x48B2AF2: g_hash_table_new_full (ghash.c:1028)
==11457==    by 0x488E1A2: UnknownInlinedFun (gerror.c:50)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:377)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E1A2: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 96 bytes in 1 blocks are still reachable in loss record 5 of 7
==11457==    at 0x4843866: malloc (vg_replace_malloc.c:446)
==11457==    by 0x48D1879: g_malloc (gmem.c:100)
==11457==    by 0x48EAE14: g_slice_alloc (gslice.c:193)
==11457==    by 0x48B2ABA: g_hash_table_new_full (ghash.c:1016)
==11457==    by 0x488E172: UnknownInlinedFun (gquark.c:64)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:376)
==11457==    by 0x488E172: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E172: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 96 bytes in 1 blocks are still reachable in loss record 6 of 7
==11457==    at 0x4843866: malloc (vg_replace_malloc.c:446)
==11457==    by 0x48D1879: g_malloc (gmem.c:100)
==11457==    by 0x48EAE14: g_slice_alloc (gslice.c:193)
==11457==    by 0x48B2ABA: g_hash_table_new_full (ghash.c:1016)
==11457==    by 0x488E1A2: UnknownInlinedFun (gerror.c:50)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:377)
==11457==    by 0x488E1A2: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E1A2: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== 16,384 bytes in 1 blocks are still reachable in loss record 7 of 7
==11457==    at 0x4843866: malloc (vg_replace_malloc.c:446)
==11457==    by 0x48D1879: g_malloc (gmem.c:100)
==11457==    by 0x488E183: UnknownInlinedFun (gquark.c:65)
==11457==    by 0x488E183: UnknownInlinedFun (glib-init.c:376)
==11457==    by 0x488E183: UnknownInlinedFun (glib-init.c:365)
==11457==    by 0x488E183: glib_init_ctor (glib-init.c:518)
==11457==    by 0x4005436: call_init (dl-init.c:74)
==11457==    by 0x4005436: call_init (dl-init.c:26)
==11457==    by 0x400552C: _dl_init (dl-init.c:121)
==11457==    by 0x401D09F: ??? (in /usr/lib64/ld-linux-x86-64.so.2)
==11457== 
==11457== LEAK SUMMARY:
==11457==    definitely lost: 0 bytes in 0 blocks
==11457==    indirectly lost: 0 bytes in 0 blocks
==11457==      possibly lost: 0 bytes in 0 blocks
==11457==    still reachable: 16,768 bytes in 7 blocks
==11457==         suppressed: 0 bytes in 0 blocks
==11457== 
==11457== For lists of detected and suppressed errors, rerun with: -s
==11457== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Asumimos que:

Origen de los errores:
El stack trace indica que estas estan sucediendo en funciones de GLIB, como; g_malloc, g_hash_table_new_full... 

still reachable:
Esto en valgrind significa que se pidio memoria durante la ejecucion, pero que no se libero antes del exit. Sin embargo no son necesariamente leaks porque el programa aun tiene punteros apuntando hacia estas y aun pueden ser liberadas, muchas veces algo intencional de librerias como GLIB que mantienen estados globales.
