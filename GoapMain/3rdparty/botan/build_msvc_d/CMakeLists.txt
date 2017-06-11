cmake_minimum_required(VERSION 2.8.0)
project(botan)

if(POLICY CMP0042)
cmake_policy(SET CMP0042 NEW)
endif()

set(BOTAN_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/botan_all.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/botan_all_aesni.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/botan_all_avx2.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/botan_all_rdrand.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/botan_all_ssse3.cpp"
)

set(BOTAN_CLI
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/asn1.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/cc_enc.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/compress.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/encryption.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/math.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/pubkey.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/speed.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/timing_tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/tls_client.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/tls_proxy.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/tls_server.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/tls_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/cli/x509.cpp"
)

set(BOTAN_TESTS
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_aead.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_asn1.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_bigint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_block.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_c25519.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_certstor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_compression.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_cryptobox.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_dh.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_dl_group.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_dlies.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_dsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ecc_pointmul.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ecdh.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ecdsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ecgdsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ecies.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_eckcdsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ed25519.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_elg.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_entropy.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ffi.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_filters.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_fpe.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_fuzzer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_gf2m.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_gost_3410.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_hash.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_kdf.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_keywrap.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_mac.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_mceliece.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_modes.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_mp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_name_constraint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_newhope.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ocb.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_ocsp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_octetstring.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_os_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_otp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_package_transform.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pad.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_passhash.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pbkdf.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pk_pad.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pkcs11.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pkcs11_high_level.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pkcs11_low_level.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_pubkey.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_rfc6979.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_rng.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_rsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_simd.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_srp6.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_stream.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_tls_messages.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_tpm.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_tss.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_workfactor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_x509_dn.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_x509_path.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/test_xmss.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/tests.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_ecc.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_ecdh.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_ecdsa.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_tls.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_tls_policy.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../src/tests/unit_x509.cpp"
)


option(ENABLED_OPTIONAL_WARINIGS "If enabled more strict warinig policy will be used" OFF)
option(ENABLED_LTO "If enabled link time optimization will be used" OFF)

set(COMPILER_FEATURES_RELEASE /EHs /GR /O2 /Oi /MT /bigobj)
set(COMPILER_FEATURES_DEBUG /EHs /GR /Zi /FS /MTd /bigobj)
set(COMPILER_FEATURES $<$<NOT:$<CONFIG:DEBUG>>:${COMPILER_FEATURES_RELEASE}>  $<$<CONFIG:DEBUG>:${COMPILER_FEATURES_DEBUG}>)
set(SHARED_FEATURES )
set(STATIC_FEATURES -DBOTAN_DLL=)
set(COMPILER_WARNINGS /W4 /wd4250 /wd4251 /wd4275)
set(COMPILER_INCLUDE_DIRS build/include build/include/external)
if(ENABLED_LTO)
    set(COMPILER_FEATURES ${COMPILER_FEATURES} -lto)
endif()
if(ENABLED_OPTIONAL_WARINIGS)
    set(COMPILER_OPTIONAL_WARNINGS -Wsign-promo -Wctor-dtor-privacy -Wdeprecated -Winit-self -Wnon-virtual-dtor -Wunused-macros -Wold-style-cast -Wuninitialized)
endif()

add_library(${PROJECT_NAME} STATIC ${BOTAN_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC  )
target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILER_WARNINGS} ${COMPILER_FEATURES} ${COMPILER_OPTIONAL_WARNINGS} PRIVATE ${STATIC_FEATURES})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPILER_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-static)

add_library(${PROJECT_NAME}_shared SHARED ${BOTAN_SOURCES})
target_link_libraries(${PROJECT_NAME}_shared PUBLIC  )
target_compile_options(${PROJECT_NAME}_shared PUBLIC ${COMPILER_WARNINGS} ${COMPILER_FEATURES} ${COMPILER_OPTIONAL_WARNINGS} PRIVATE ${SHARED_FEATURES})
target_include_directories(${PROJECT_NAME}_shared PUBLIC ${COMPILER_INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_cli ${BOTAN_CLI})
target_link_libraries(${PROJECT_NAME}_cli PRIVATE ${PROJECT_NAME}_shared  )
set_target_properties(${PROJECT_NAME}_cli PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-cli)

add_executable(${PROJECT_NAME}_tests ${BOTAN_TESTS})
target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}_shared  )
set_target_properties(${PROJECT_NAME}_tests PROPERTIES OUTPUT_NAME botan-test)

set(CONFIGURATION_FILES configure.py .gitignore .astylerc authors.txt news.rst readme.rst)
file(GLOB_RECURSE DOCUMENTATION_FILES doc/* )
file(GLOB_RECURSE HEADER_FILES src/*.h )
file(GLOB_RECURSE INFO_FILES src/lib/*info.txt )
add_custom_target(CONFIGURATION_DUMMY SOURCES ${CONFIGURATION_FILES} ${DOCUMENTATION_FILES} ${INFO_FILES} ${HEADER_FILES})
