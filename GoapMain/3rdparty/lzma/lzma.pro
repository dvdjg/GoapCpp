#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T10:15:23
#
#-------------------------------------------------

TARGET = lzma
QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
CONFIG -= rtti

!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

INCLUDEPATH += com common api lz rangecoder lzma check simple delta # api/lzma
DEFINES += HAVE_INTTYPES_H HAVE_STDINT_H HAVE_LIMITS_H HAVE_STDBOOL_H HAVE_STRING_H HAVE_MEMORY_H HAVE_CLOCK_GETTIME \
    HAVE_CHECK_CRC32 HAVE_CHECK_CRC64 HAVE_CHECK_SHA256 \
    HAVE_DECODERS HAVE_DECODER_LZMA1 HAVE_DECODER_LZMA2 HAVE_DECODER_X86 HAVE_DECODER_DELTA \
    HAVE_ENCODERS HAVE_ENCODER_LZMA1 HAVE_ENCODER_LZMA2 HAVE_ENCODER_X86 HAVE_ENCODER_DELTA \
    HAVE_IMMINTRIN_H HAVE_MF_HC3 HAVE_MF_HC4 HAVE_MF_BT2 HAVE_MF_BT3 HAVE_MF_BT4 \
    HAVE_FUTIMES HAVE_FUTIMESAT HAVE_UTIMES ASSUME_RAM=1 PACKAGE_NAME=\\\"CustomLZMA\\\" PACKAGE_BUGREPORT=\\\"CustomLZMABugReport\\\" PACKAGE_URL=\\\"file://CustomLZMA\\\"


win32 {

DEFINES += _WIN32_WINNT=0x0501
DEFINES += _SCL_SECURE_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE _AFX_SECURE_NO_WARNINGS _ATL_SECURE_NO_WARNINGS
# _MBCS
#DEFINES -= UNICODE _UNICODE
DEFINES += MYTHREAD_VISTA
} else {
DEFINES += MYTHREAD_POSIX
}

HEADERS += \
    lzma/fastpos.h \
    lzma/lzma_common.h \
    lzma/lzma_decoder.h \
    lzma/lzma_encoder_private.h \
    lzma/lzma_encoder.h \
    lzma/lzma2_decoder.h \
    lzma/lzma2_encoder.h \
    api/lzma/base.h \
    api/lzma/bcj.h \
    api/lzma/block.h \
    api/lzma/check.h \
    api/lzma/container.h \
    api/lzma/delta.h \
    api/lzma/filter.h \
    api/lzma/hardware.h \
    api/lzma/index_hash.h \
    api/lzma/index.h \
    api/lzma/lzma12.h \
    api/lzma/stream_flags.h \
    api/lzma/version.h \
    api/lzma/vli.h \
    common/alone_decoder.h \
    common/block_buffer_encoder.h \
    common/block_decoder.h \
    common/block_encoder.h \
    common/common.h \
    common/easy_preset.h \
    common/filter_common.h \
    common/filter_decoder.h \
    common/filter_encoder.h \
    common/index_encoder.h \
    common/index.h \
    common/memcmplen.h \
    common/outqueue.h \
    common/stream_decoder.h \
    common/stream_flags_common.h \
    com/mythread.h \
    com/sysdefs.h \
    com/tuklib_common.h \
    com/tuklib_config.h \
    com/tuklib_cpucores.h \
    com/tuklib_exit.h \
    com/tuklib_gettext.h \
    com/tuklib_integer.h \
    com/tuklib_mbstr.h \
    com/tuklib_open_stdxxx.h \
    com/tuklib_physmem.h \
    com/tuklib_progname.h \
    xz/args.h \
    xz/coder.h \
    xz/file_io.h \
    xz/hardware.h \
    xz/list.h \
    xz/main.h \
    xz/message.h \
    xz/mytime.h \
    xz/options.h \
    xz/private.h \
    xz/signals.h \
    xz/suffix.h \
    xz/util.h \
    lz/lz_decoder.h \
    lz/lz_encoder_hash_table.h \
    lz/lz_encoder_hash.h \
    lz/lz_encoder.h \
    rangecoder/price.h \
    rangecoder/range_common.h \
    rangecoder/range_decoder.h \
    rangecoder/range_encoder.h \
    check/check.h \
    check/crc_macros.h \
    check/crc32_table_be.h \
    check/crc32_table_le.h \
    check/crc64_table_be.h \
    check/crc64_table_le.h \
    simple/simple_coder.h \
    simple/simple_decoder.h \
    simple/simple_encoder.h \
    simple/simple_private.h \
    delta/delta_common.h \
    delta/delta_decoder.h \
    delta/delta_encoder.h \
    delta/delta_private.h

SOURCES += \
    lzma/fastpos_table.c \
    lzma/fastpos_tablegen.c \
    lzma/lzma_decoder.c \
    lzma/lzma_encoder_optimum_fast.c \
    lzma/lzma_encoder_optimum_normal.c \
    lzma/lzma_encoder_presets.c \
    lzma/lzma_encoder.c \
    lzma/lzma2_decoder.c \
    lzma/lzma2_encoder.c \
    common/alone_decoder.c \
    common/alone_encoder.c \
    common/auto_decoder.c \
    common/block_buffer_decoder.c \
    common/block_buffer_encoder.c \
    common/block_decoder.c \
    common/block_encoder.c \
    common/block_header_decoder.c \
    common/block_header_encoder.c \
    common/block_util.c \
    common/common.c \
    common/easy_buffer_encoder.c \
    common/easy_decoder_memusage.c \
    common/easy_encoder_memusage.c \
    common/easy_encoder.c \
    common/easy_preset.c \
    common/filter_buffer_decoder.c \
    common/filter_buffer_encoder.c \
    common/filter_common.c \
    common/filter_decoder.c \
    common/filter_encoder.c \
    common/filter_flags_decoder.c \
    common/filter_flags_encoder.c \
    common/hardware_cputhreads.c \
    common/hardware_physmem.c \
    common/index_decoder.c \
    common/index_encoder.c \
    common/index_hash.c \
    common/index.c \
    common/outqueue.c \
    common/stream_buffer_decoder.c \
    common/stream_buffer_encoder.c \
    common/stream_decoder.c \
    common/stream_encoder_mt.c \
    common/stream_encoder.c \
    common/stream_flags_common.c \
    common/stream_flags_decoder.c \
    common/stream_flags_encoder.c \
    common/vli_decoder.c \
    common/vli_encoder.c \
    common/vli_size.c \
    com/tuklib_cpucores.c \
    com/tuklib_exit.c \
    com/tuklib_mbstr_fw.c \
    com/tuklib_mbstr_width.c \
    com/tuklib_open_stdxxx.c \
    com/tuklib_physmem.c \
    com/tuklib_progname.c \
    xz/args.c \
    xz/coder.c \
    xz/file_io.c \
    xz/hardware.c \
    xz/list.c \
    xz/main.c \
    xz/message.c \
    xz/mytime.c \
    xz/options.c \
    xz/signals.c \
    xz/suffix.c \
    xz/util.c \
    xz/xzdec.c \
    lz/lz_decoder.c \
    lz/lz_encoder_mf.c \
    lz/lz_encoder.c \
    rangecoder/price_table.c \
    rangecoder/price_tablegen.c \
    check/check.c \
    check/crc32_fast.c \
    check/crc32_table.c \
    check/crc32_tablegen.c \
    check/crc64_fast.c \
    check/crc64_table.c \
    check/crc64_tablegen.c \
    check/sha256.c \
    simple/armthumb.c \
    simple/powerpc.c \
    simple/simple_coder.c \
    simple/simple_decoder.c \
    simple/simple_encoder.c \
    simple/x86.c \
    delta/delta_common.c \
    delta/delta_decoder.c \
    delta/delta_encoder.c

unix {
    target.path = /usr/lib
    INSTALLS += target
}
