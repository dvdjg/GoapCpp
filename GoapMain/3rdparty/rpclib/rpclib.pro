QT       -= core gui

TARGET = rpc
TEMPLATE = lib
CONFIG += staticlib


!include(../../common.pri) {
    error(Could not find the common.pri file!)
}

!include(rpclib.pri) {
    error(Could not find the rpclib.pri file!)
}

#unix:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_gcc
#win32-msvc*:INCLUDEPATH += $$top_srcdir/3rdparty/botan/build_msvc

#LIBS += -lbotan$$SUFFIX -llzma

SOURCES += \ 
    lib/rpc/client.cc \
    lib/rpc/dispatcher.cc \
    lib/rpc/rpc_error.cc \
    lib/rpc/server.cc \
    lib/rpc/this_handler.cc \
    lib/rpc/this_server.cc \
    lib/rpc/this_session.cc \
    lib/rpc/detail/client_error.cc \
    lib/rpc/detail/response.cc \
    lib/rpc/detail/server_session.cc \
    include/rpc/dispatcher.inl \
    lib/rpc/nonstd/optional.cc

HEADERS += \ 
    include/rpc/client.h \
    include/rpc/compatibility.h \
    include/rpc/config.h \
    include/rpc/dispatcher.h \
    include/rpc/rpc_error.h \
    include/rpc/server.h \
    include/rpc/this_handler.h \
    include/rpc/this_server.h \
    include/rpc/this_session.h \
    include/rpc/version.h \
    include/rpc/msgpack.hpp \
    include/rpc/msgpack/cpp_config.hpp \
    include/rpc/msgpack/fbuffer.hpp \
    include/rpc/msgpack/iterator.hpp \
    include/rpc/msgpack/meta.hpp \
    include/rpc/msgpack/object_fwd.hpp \
    include/rpc/msgpack/object.hpp \
    include/rpc/msgpack/pack.hpp \
    include/rpc/msgpack/preprocessor.hpp \
    include/rpc/msgpack/sbuffer.hpp \
    include/rpc/msgpack/type.hpp \
    include/rpc/msgpack/unpack.hpp \
    include/rpc/msgpack/version.hpp \
    include/rpc/msgpack/versioning.hpp \
    include/rpc/msgpack/vrefbuffer.hpp \
    include/rpc/msgpack/zbuffer.hpp \
    include/rpc/msgpack/zone.hpp \
    include/rpc/msgpack/fbuffer.h \
    include/rpc/msgpack/gcc_atomic.h \
    include/rpc/msgpack/object.h \
    include/rpc/msgpack/pack_define.h \
    include/rpc/msgpack/pack_template.h \
    include/rpc/msgpack/pack.h \
    include/rpc/msgpack/predef.h \
    include/rpc/msgpack/sbuffer.h \
    include/rpc/msgpack/sysdep.h \
    include/rpc/msgpack/unpack_define.h \
    include/rpc/msgpack/unpack_template.h \
    include/rpc/msgpack/unpack.h \
    include/rpc/msgpack/util.h \
    include/rpc/msgpack/version_master.h \
    include/rpc/msgpack/version.h \
    include/rpc/msgpack/vrefbuffer.h \
    include/rpc/msgpack/zbuffer.h \
    include/rpc/msgpack/zone.h \
    include/rpc/msgpack/cpp_config_decl.hpp \
    include/rpc/msgpack/fbuffer_decl.hpp \
    include/rpc/msgpack/gcc_atomic.hpp \
    include/rpc/msgpack/iterator_decl.hpp \
    include/rpc/msgpack/meta_decl.hpp \
    include/rpc/msgpack/object_decl.hpp \
    include/rpc/msgpack/object_fwd_decl.hpp \
    include/rpc/msgpack/pack_decl.hpp \
    include/rpc/msgpack/parse_return.hpp \
    include/rpc/msgpack/sbuffer_decl.hpp \
    include/rpc/msgpack/unpack_decl.hpp \
    include/rpc/msgpack/unpack_exception.hpp \
    include/rpc/msgpack/vrefbuffer_decl.hpp \
    include/rpc/msgpack/zbuffer_decl.hpp \
    include/rpc/msgpack/zone_decl.hpp \
    include/rpc/msgpack/adaptor/adaptor_base_decl.hpp \
    include/rpc/msgpack/adaptor/adaptor_base.hpp \
    include/rpc/msgpack/adaptor/array_ref_decl.hpp \
    include/rpc/msgpack/adaptor/array_ref.hpp \
    include/rpc/msgpack/adaptor/bool.hpp \
    include/rpc/msgpack/adaptor/carray.hpp \
    include/rpc/msgpack/adaptor/char_ptr.hpp \
    include/rpc/msgpack/adaptor/check_container_size_decl.hpp \
    include/rpc/msgpack/adaptor/check_container_size.hpp \
    include/rpc/msgpack/adaptor/define_decl.hpp \
    include/rpc/msgpack/adaptor/define.hpp \
    include/rpc/msgpack/adaptor/deque.hpp \
    include/rpc/msgpack/adaptor/ext_decl.hpp \
    include/rpc/msgpack/adaptor/ext.hpp \
    include/rpc/msgpack/adaptor/fixint_decl.hpp \
    include/rpc/msgpack/adaptor/fixint.hpp \
    include/rpc/msgpack/adaptor/float.hpp \
    include/rpc/msgpack/adaptor/int_decl.hpp \
    include/rpc/msgpack/adaptor/int.hpp \
    include/rpc/msgpack/adaptor/list.hpp \
    include/rpc/msgpack/adaptor/map_decl.hpp \
    include/rpc/msgpack/adaptor/map.hpp \
    include/rpc/msgpack/adaptor/msgpack_tuple_decl.hpp \
    include/rpc/msgpack/adaptor/msgpack_tuple.hpp \
    include/rpc/msgpack/adaptor/nil_decl.hpp \
    include/rpc/msgpack/adaptor/nil.hpp \
    include/rpc/msgpack/adaptor/pair.hpp \
    include/rpc/msgpack/adaptor/raw_decl.hpp \
    include/rpc/msgpack/adaptor/raw.hpp \
    include/rpc/msgpack/adaptor/set.hpp \
    include/rpc/msgpack/adaptor/size_equal_only_decl.hpp \
    include/rpc/msgpack/adaptor/size_equal_only.hpp \
    include/rpc/msgpack/adaptor/string.hpp \
    include/rpc/msgpack/adaptor/v4raw_decl.hpp \
    include/rpc/msgpack/adaptor/v4raw.hpp \
    include/rpc/msgpack/adaptor/vector_bool.hpp \
    include/rpc/msgpack/adaptor/vector_char.hpp \
    include/rpc/msgpack/adaptor/vector_unsigned_char.hpp \
    include/rpc/msgpack/adaptor/vector.hpp \
    include/rpc/msgpack/adaptor/cpp17/optional.hpp \
    include/rpc/msgpack/adaptor/cpp17/string_view.hpp \
    include/rpc/msgpack/adaptor/cpp11/array_char.hpp \
    include/rpc/msgpack/adaptor/cpp11/array_unsigned_char.hpp \
    include/rpc/msgpack/adaptor/cpp11/array.hpp \
    include/rpc/msgpack/adaptor/cpp11/forward_list.hpp \
    include/rpc/msgpack/adaptor/cpp11/reference_wrapper.hpp \
    include/rpc/msgpack/adaptor/cpp11/shared_ptr.hpp \
    include/rpc/msgpack/adaptor/cpp11/tuple.hpp \
    include/rpc/msgpack/adaptor/cpp11/unique_ptr.hpp \
    include/rpc/msgpack/adaptor/cpp11/unordered_map.hpp \
    include/rpc/msgpack/adaptor/cpp11/unordered_set.hpp \
    include/rpc/msgpack/adaptor/boost/fusion.hpp \
    include/rpc/msgpack/adaptor/boost/msgpack_variant_decl.hpp \
    include/rpc/msgpack/adaptor/boost/msgpack_variant.hpp \
    include/rpc/msgpack/adaptor/boost/optional.hpp \
    include/rpc/msgpack/adaptor/boost/string_ref.hpp \
    include/rpc/msgpack/adaptor/boost/string_view.hpp \
    include/rpc/msgpack/adaptor/tr1/unordered_map.hpp \
    include/rpc/msgpack/adaptor/tr1/unordered_set.hpp \
    include/rpc/msgpack/predef/architecture/x86/32.h \
    include/rpc/msgpack/predef/architecture/x86/64.h \
    include/rpc/msgpack/preprocessor/arithmetic.hpp \
    include/rpc/msgpack/preprocessor/array.hpp \
    include/rpc/msgpack/preprocessor/assert_msg.hpp \
    include/rpc/msgpack/preprocessor/cat.hpp \
    include/rpc/msgpack/preprocessor/comma_if.hpp \
    include/rpc/msgpack/preprocessor/comma.hpp \
    include/rpc/msgpack/preprocessor/comparison.hpp \
    include/rpc/msgpack/preprocessor/control.hpp \
    include/rpc/msgpack/preprocessor/debug.hpp \
    include/rpc/msgpack/preprocessor/dec.hpp \
    include/rpc/msgpack/preprocessor/empty.hpp \
    include/rpc/msgpack/preprocessor/enum_params_with_a_default.hpp \
    include/rpc/msgpack/preprocessor/enum_params_with_defaults.hpp \
    include/rpc/msgpack/preprocessor/enum_params.hpp \
    include/rpc/msgpack/preprocessor/enum_shifted_params.hpp \
    include/rpc/msgpack/preprocessor/enum_shifted.hpp \
    include/rpc/msgpack/preprocessor/enum.hpp \
    include/rpc/msgpack/preprocessor/expand.hpp \
    include/rpc/msgpack/preprocessor/expr_if.hpp \
    include/rpc/msgpack/preprocessor/facilities.hpp \
    include/rpc/msgpack/preprocessor/for.hpp \
    include/rpc/msgpack/preprocessor/identity.hpp \
    include/rpc/msgpack/preprocessor/if.hpp \
    include/rpc/msgpack/preprocessor/inc.hpp \
    include/rpc/msgpack/preprocessor/iterate.hpp \
    include/rpc/msgpack/preprocessor/iteration.hpp \
    include/rpc/msgpack/preprocessor/library.hpp \
    include/rpc/msgpack/preprocessor/limits.hpp \
    include/rpc/msgpack/preprocessor/list.hpp \
    include/rpc/msgpack/preprocessor/logical.hpp \
    include/rpc/msgpack/preprocessor/max.hpp \
    include/rpc/msgpack/preprocessor/min.hpp \
    include/rpc/msgpack/preprocessor/punctuation.hpp \
    include/rpc/msgpack/preprocessor/repeat_2nd.hpp \
    include/rpc/msgpack/preprocessor/repeat_3rd.hpp \
    include/rpc/msgpack/preprocessor/repeat_from_to_2nd.hpp \
    include/rpc/msgpack/preprocessor/repeat_from_to_3rd.hpp \
    include/rpc/msgpack/preprocessor/repeat_from_to.hpp \
    include/rpc/msgpack/preprocessor/repeat.hpp \
    include/rpc/msgpack/preprocessor/repetition.hpp \
    include/rpc/msgpack/preprocessor/selection.hpp \
    include/rpc/msgpack/preprocessor/seq.hpp \
    include/rpc/msgpack/preprocessor/slot.hpp \
    include/rpc/msgpack/preprocessor/stringize.hpp \
    include/rpc/msgpack/preprocessor/tuple.hpp \
    include/rpc/msgpack/preprocessor/variadic.hpp \
    include/rpc/msgpack/preprocessor/while.hpp \
    include/rpc/msgpack/preprocessor/wstringize.hpp \
    include/rpc/msgpack/v2/cpp_config_decl.hpp \
    include/rpc/msgpack/v2/create_object_visitor.hpp \
    include/rpc/msgpack/v2/fbuffer_decl.hpp \
    include/rpc/msgpack/v2/iterator_decl.hpp \
    include/rpc/msgpack/v2/meta_decl.hpp \
    include/rpc/msgpack/v2/null_visitor.hpp \
    include/rpc/msgpack/v2/object_decl.hpp \
    include/rpc/msgpack/v2/object_fwd_decl.hpp \
    include/rpc/msgpack/v2/object_fwd.hpp \
    include/rpc/msgpack/v2/object.hpp \
    include/rpc/msgpack/v2/pack_decl.hpp \
    include/rpc/msgpack/v2/parse_return.hpp \
    include/rpc/msgpack/v2/parse.hpp \
    include/rpc/msgpack/v2/sbuffer_decl.hpp \
    include/rpc/msgpack/v2/unpack_decl.hpp \
    include/rpc/msgpack/v2/unpack.hpp \
    include/rpc/msgpack/v2/vrefbuffer_decl.hpp \
    include/rpc/msgpack/v2/x3_parse.hpp \
    include/rpc/msgpack/v2/x3_unpack.hpp \
    include/rpc/msgpack/v2/zbuffer_decl.hpp \
    include/rpc/msgpack/v2/zone_decl.hpp \
    include/rpc/msgpack/v2/adaptor/adaptor_base_decl.hpp \
    include/rpc/msgpack/v2/adaptor/adaptor_base.hpp \
    include/rpc/msgpack/v2/adaptor/array_ref_decl.hpp \
    include/rpc/msgpack/v2/adaptor/check_container_size_decl.hpp \
    include/rpc/msgpack/v2/adaptor/define_decl.hpp \
    include/rpc/msgpack/v2/adaptor/ext_decl.hpp \
    include/rpc/msgpack/v2/adaptor/fixint_decl.hpp \
    include/rpc/msgpack/v2/adaptor/int_decl.hpp \
    include/rpc/msgpack/v2/adaptor/map_decl.hpp \
    include/rpc/msgpack/v2/adaptor/msgpack_tuple_decl.hpp \
    include/rpc/msgpack/v2/adaptor/nil_decl.hpp \
    include/rpc/msgpack/v2/adaptor/raw_decl.hpp \
    include/rpc/msgpack/v2/adaptor/size_equal_only_decl.hpp \
    include/rpc/msgpack/v2/adaptor/v4raw_decl.hpp \
    include/rpc/msgpack/v2/detail/cpp03_zone_decl.hpp \
    include/rpc/msgpack/v2/detail/cpp11_zone_decl.hpp \
    include/rpc/msgpack/preprocessor/arithmetic/detail/div_base.hpp \
    include/rpc/msgpack/v2/cpp_config_decl.hpp \
    include/rpc/msgpack/v2/create_object_visitor.hpp \
    include/rpc/msgpack/v2/fbuffer_decl.hpp \
    include/rpc/msgpack/v2/iterator_decl.hpp \
    include/rpc/msgpack/v2/meta_decl.hpp \
    include/rpc/msgpack/v2/null_visitor.hpp \
    include/rpc/msgpack/v2/object_decl.hpp \
    include/rpc/msgpack/v2/object_fwd_decl.hpp \
    include/rpc/msgpack/v2/object_fwd.hpp \
    include/rpc/msgpack/v2/object.hpp \
    include/rpc/msgpack/v2/pack_decl.hpp \
    include/rpc/msgpack/v2/parse_return.hpp \
    include/rpc/msgpack/v2/parse.hpp \
    include/rpc/msgpack/v2/sbuffer_decl.hpp \
    include/rpc/msgpack/v2/unpack_decl.hpp \
    include/rpc/msgpack/v2/unpack.hpp \
    include/rpc/msgpack/v2/vrefbuffer_decl.hpp \
    include/rpc/msgpack/v2/x3_parse.hpp \
    include/rpc/msgpack/v2/x3_unpack.hpp \
    include/rpc/msgpack/v2/zbuffer_decl.hpp \
    include/rpc/msgpack/v2/zone_decl.hpp \
    include/rpc/msgpack/detail/cpp03_zone.hpp \
    include/rpc/msgpack/detail/cpp11_zone.hpp \
    include/rpc/detail/all.h \
    include/rpc/detail/any.h \
    include/rpc/detail/async_writer.h \
    include/rpc/detail/bool.h \
    include/rpc/detail/call.h \
    include/rpc/detail/client_error.h \
    include/rpc/detail/constant.h \
    include/rpc/detail/dev_utils.h \
    include/rpc/detail/func_tools.h \
    include/rpc/detail/func_traits.h \
    include/rpc/detail/if.h \
    include/rpc/detail/invoke.h \
    include/rpc/detail/is_specialization_of.h \
    include/rpc/detail/log.h \
    include/rpc/detail/make_unique.h \
    include/rpc/detail/not.h \
    include/rpc/detail/pimpl.h \
    include/rpc/detail/response.h \
    include/rpc/detail/server_session.h \
    include/rpc/detail/thread_group.h \
    include/rpc/detail/util.h \
    include/rpc/msgpack/v1/cpp_config.hpp \
    include/rpc/msgpack/v1/cpp_config_decl.hpp \
    include/rpc/msgpack/v1/fbuffer.hpp \
    include/rpc/msgpack/v1/fbuffer_decl.hpp \
    include/rpc/msgpack/v1/iterator.hpp \
    include/rpc/msgpack/v1/iterator_decl.hpp \
    include/rpc/msgpack/v1/meta.hpp \
    include/rpc/msgpack/v1/meta_decl.hpp \
    include/rpc/msgpack/v1/object.hpp \
    include/rpc/msgpack/v1/object_decl.hpp \
    include/rpc/msgpack/v1/object_fwd.hpp \
    include/rpc/msgpack/v1/object_fwd_decl.hpp \
    include/rpc/msgpack/v1/pack.hpp \
    include/rpc/msgpack/v1/pack_decl.hpp \
    include/rpc/msgpack/v1/parse_return.hpp \
    include/rpc/msgpack/v1/preprocessor.hpp \
    include/rpc/msgpack/v1/sbuffer.hpp \
    include/rpc/msgpack/v1/sbuffer_decl.hpp \
    include/rpc/msgpack/v1/unpack.hpp \
    include/rpc/msgpack/v1/unpack_decl.hpp \
    include/rpc/msgpack/v1/unpack_exception.hpp \
    include/rpc/msgpack/v1/version.hpp \
    include/rpc/msgpack/v1/versioning.hpp \
    include/rpc/msgpack/v1/vrefbuffer.hpp \
    include/rpc/msgpack/v1/vrefbuffer_decl.hpp \
    include/rpc/msgpack/v1/zbuffer.hpp \
    include/rpc/msgpack/v1/zbuffer_decl.hpp \
    include/rpc/msgpack/v1/zone.hpp \
    include/rpc/msgpack/v1/zone_decl.hpp \
    include/rpc/msgpack/v1/adaptor/adaptor_base.hpp \
    include/rpc/msgpack/v1/adaptor/adaptor_base_decl.hpp \
    include/rpc/msgpack/v1/adaptor/array_ref.hpp \
    include/rpc/msgpack/v1/adaptor/array_ref_decl.hpp \
    include/rpc/msgpack/v1/adaptor/bool.hpp \
    include/rpc/msgpack/v1/adaptor/carray.hpp \
    include/rpc/msgpack/v1/adaptor/char_ptr.hpp \
    include/rpc/msgpack/v1/adaptor/check_container_size.hpp \
    include/rpc/msgpack/v1/adaptor/check_container_size_decl.hpp \
    include/rpc/msgpack/v1/adaptor/define.hpp \
    include/rpc/msgpack/v1/adaptor/define_decl.hpp \
    include/rpc/msgpack/v1/adaptor/deque.hpp \
    include/rpc/msgpack/v1/adaptor/ext.hpp \
    include/rpc/msgpack/v1/adaptor/ext_decl.hpp \
    include/rpc/msgpack/v1/adaptor/fixint.hpp \
    include/rpc/msgpack/v1/adaptor/fixint_decl.hpp \
    include/rpc/msgpack/v1/adaptor/float.hpp \
    include/rpc/msgpack/v1/adaptor/int.hpp \
    include/rpc/msgpack/v1/adaptor/int_decl.hpp \
    include/rpc/msgpack/v1/adaptor/list.hpp \
    include/rpc/msgpack/v1/adaptor/map.hpp \
    include/rpc/msgpack/v1/adaptor/map_decl.hpp \
    include/rpc/msgpack/v1/adaptor/msgpack_tuple.hpp \
    include/rpc/msgpack/v1/adaptor/msgpack_tuple_decl.hpp \
    include/rpc/msgpack/v1/adaptor/nil.hpp \
    include/rpc/msgpack/v1/adaptor/nil_decl.hpp \
    include/rpc/msgpack/v1/adaptor/pair.hpp \
    include/rpc/msgpack/v1/adaptor/raw.hpp \
    include/rpc/msgpack/v1/adaptor/raw_decl.hpp \
    include/rpc/msgpack/v1/adaptor/set.hpp \
    include/rpc/msgpack/v1/adaptor/size_equal_only.hpp \
    include/rpc/msgpack/v1/adaptor/size_equal_only_decl.hpp \
    include/rpc/msgpack/v1/adaptor/string.hpp \
    include/rpc/msgpack/v1/adaptor/v4raw.hpp \
    include/rpc/msgpack/v1/adaptor/v4raw_decl.hpp \
    include/rpc/msgpack/v1/adaptor/vector.hpp \
    include/rpc/msgpack/v1/adaptor/vector_bool.hpp \
    include/rpc/msgpack/v1/adaptor/vector_char.hpp \
    include/rpc/msgpack/v1/adaptor/vector_unsigned_char.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/array.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/array_char.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/array_unsigned_char.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/forward_list.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/reference_wrapper.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/shared_ptr.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/tuple.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/unique_ptr.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/unordered_map.hpp \
    include/rpc/msgpack/v1/adaptor/cpp11/unordered_set.hpp \
    include/rpc/msgpack/v1/adaptor/boost/fusion.hpp \
    include/rpc/msgpack/v1/adaptor/boost/msgpack_variant.hpp \
    include/rpc/msgpack/v1/adaptor/boost/msgpack_variant_decl.hpp \
    include/rpc/msgpack/v1/adaptor/boost/optional.hpp \
    include/rpc/msgpack/v1/adaptor/boost/string_ref.hpp \
    include/rpc/msgpack/v1/adaptor/boost/string_view.hpp \
    include/rpc/msgpack/v1/adaptor/cpp17/optional.hpp \
    include/rpc/msgpack/v1/adaptor/cpp17/string_view.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_define_array_decl.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_define_map_decl.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_msgpack_tuple_decl.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_define_array.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_define_array_decl.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_define_map.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_define_map_decl.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_msgpack_tuple.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp11_msgpack_tuple_decl.hpp \
    include/rpc/msgpack/v1/adaptor/tr1/unordered_map.hpp \
    include/rpc/msgpack/v1/adaptor/tr1/unordered_set.hpp \
    include/rpc/msgpack/v1/detail/cpp03_zone_decl.hpp \
    include/rpc/msgpack/v1/detail/cpp11_zone_decl.hpp \
    include/rpc/msgpack/v2/adaptor/boost/msgpack_variant_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp03_define_array_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp03_define_map_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp03_msgpack_tuple_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp11_define_array_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp11_define_map_decl.hpp \
    include/rpc/msgpack/v2/adaptor/detail/cpp11_msgpack_tuple_decl.hpp \
    include/rpc/msgpack/adaptor/detail/cpp03_define_array.hpp \
    include/rpc/msgpack/adaptor/detail/cpp03_define_map.hpp \
    include/rpc/msgpack/adaptor/detail/cpp03_msgpack_tuple.hpp \
    include/rpc/msgpack/adaptor/detail/cpp11_define_array.hpp \
    include/rpc/msgpack/adaptor/detail/cpp11_define_map.hpp \
    include/rpc/msgpack/adaptor/detail/cpp11_msgpack_tuple.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_define_array.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_define_map.hpp \
    include/rpc/msgpack/v1/adaptor/detail/cpp03_msgpack_tuple.hpp \
    include/rpc/msgpack/v1/detail/cpp03_zone.hpp \
    include/rpc/msgpack/v1/detail/cpp11_zone.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    rpclib.pri
