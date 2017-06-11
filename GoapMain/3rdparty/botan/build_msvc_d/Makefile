# Compiler Options
CXX            = cl /MTd /bigobj
LINKER         = link

CXXFLAGS       = /EHs /GR /Zi /FS
WARN_FLAGS     = /W4 /wd4250 /wd4251 /wd4275
SO_OBJ_FLAGS   = 

LIB_LINK_CMD   = $(LINKER) /DLL /DEBUG
CLI_LINK_CMD   = $(LINKER) /DEBUG
TEST_LINK_CMD  = $(LINKER) /DEBUG

LIB_LINKS_TO   = advapi32.lib user32.lib
CLI_LINKS_TO   = $(LIB_LINKS_TO)
TEST_LINKS_TO  = $(LIB_LINKS_TO)

LIB_FLAGS      = $(SO_OBJ_FLAGS) $(CXXFLAGS) $(WARN_FLAGS)
CLI_FLAGS      = $(CXXFLAGS) $(WARN_FLAGS)
TEST_FLAGS     = $(CXXFLAGS) $(WARN_FLAGS)

SCRIPTS_DIR    = ..\src\scripts
INSTALLED_LIB_DIR = c:\Botan/lib

CLI_POST_LINK_CMD  = 
TEST_POST_LINK_CMD = 


### Aliases for Common Programs
AR            = lib
COPY          = copy
CD            = @cd
ECHO          = @echo
INSTALL       = copy
INSTALL_CMD   = copy
MKDIR         = @md
MKDIR_INSTALL = @md
RM            = @del /Q
RM_R          = $(RM) /S
RMDIR         = @rmdir

# Executable targets
CLI           = .\botan-cli.exe
TEST          = .\botan-test.exe

# Library targets
#
# LIB_FILENAME is always the .lib file, that is either a static lib or a
# by-product of the DLL creation used to link the DLL into applications
LIB_BASENAME  = botand
LIB_FILENAME  = .\$(LIB_BASENAME).lib

!If "$(SO_OBJ_FLAGS)" == ""
# static lib
LIBRARIES     = $(LIB_FILENAME)
!Else
# shared lib
SO_FILENAME   = .\$(LIB_BASENAME).dll
LIBRARIES     = $(SO_FILENAME)
!Endif


# File Lists
LIBOBJS       = build\obj\lib\botan_all.obj \
                build\obj\lib\botan_all_aesni.obj \
                build\obj\lib\botan_all_avx2.obj \
                build\obj\lib\botan_all_rdrand.obj \
                build\obj\lib\botan_all_ssse3.obj

CLIOBJS       = build\obj\cli\asn1.obj \
                build\obj\cli\cc_enc.obj \
                build\obj\cli\compress.obj \
                build\obj\cli\encryption.obj \
                build\obj\cli\main.obj \
                build\obj\cli\math.obj \
                build\obj\cli\pubkey.obj \
                build\obj\cli\speed.obj \
                build\obj\cli\timing_tests.obj \
                build\obj\cli\tls_client.obj \
                build\obj\cli\tls_proxy.obj \
                build\obj\cli\tls_server.obj \
                build\obj\cli\tls_utils.obj \
                build\obj\cli\utils.obj \
                build\obj\cli\x509.obj

TESTOBJS      = build\obj\test\main.obj \
                build\obj\test\test_aead.obj \
                build\obj\test\test_asn1.obj \
                build\obj\test\test_bigint.obj \
                build\obj\test\test_block.obj \
                build\obj\test\test_c25519.obj \
                build\obj\test\test_certstor.obj \
                build\obj\test\test_compression.obj \
                build\obj\test\test_cryptobox.obj \
                build\obj\test\test_dh.obj \
                build\obj\test\test_dl_group.obj \
                build\obj\test\test_dlies.obj \
                build\obj\test\test_dsa.obj \
                build\obj\test\test_ecc_pointmul.obj \
                build\obj\test\test_ecdh.obj \
                build\obj\test\test_ecdsa.obj \
                build\obj\test\test_ecgdsa.obj \
                build\obj\test\test_ecies.obj \
                build\obj\test\test_eckcdsa.obj \
                build\obj\test\test_ed25519.obj \
                build\obj\test\test_elg.obj \
                build\obj\test\test_entropy.obj \
                build\obj\test\test_ffi.obj \
                build\obj\test\test_filters.obj \
                build\obj\test\test_fpe.obj \
                build\obj\test\test_fuzzer.obj \
                build\obj\test\test_gf2m.obj \
                build\obj\test\test_gost_3410.obj \
                build\obj\test\test_hash.obj \
                build\obj\test\test_kdf.obj \
                build\obj\test\test_keywrap.obj \
                build\obj\test\test_mac.obj \
                build\obj\test\test_mceliece.obj \
                build\obj\test\test_modes.obj \
                build\obj\test\test_mp.obj \
                build\obj\test\test_name_constraint.obj \
                build\obj\test\test_newhope.obj \
                build\obj\test\test_ocb.obj \
                build\obj\test\test_ocsp.obj \
                build\obj\test\test_octetstring.obj \
                build\obj\test\test_os_utils.obj \
                build\obj\test\test_otp.obj \
                build\obj\test\test_package_transform.obj \
                build\obj\test\test_pad.obj \
                build\obj\test\test_passhash.obj \
                build\obj\test\test_pbkdf.obj \
                build\obj\test\test_pk_pad.obj \
                build\obj\test\test_pkcs11.obj \
                build\obj\test\test_pkcs11_high_level.obj \
                build\obj\test\test_pkcs11_low_level.obj \
                build\obj\test\test_pubkey.obj \
                build\obj\test\test_rfc6979.obj \
                build\obj\test\test_rng.obj \
                build\obj\test\test_rsa.obj \
                build\obj\test\test_simd.obj \
                build\obj\test\test_srp6.obj \
                build\obj\test\test_stream.obj \
                build\obj\test\test_tls_messages.obj \
                build\obj\test\test_tpm.obj \
                build\obj\test\test_tss.obj \
                build\obj\test\test_utils.obj \
                build\obj\test\test_workfactor.obj \
                build\obj\test\test_x509_dn.obj \
                build\obj\test\test_x509_path.obj \
                build\obj\test\test_xmss.obj \
                build\obj\test\tests.obj \
                build\obj\test\unit_ecc.obj \
                build\obj\test\unit_ecdh.obj \
                build\obj\test\unit_ecdsa.obj \
                build\obj\test\unit_tls.obj \
                build\obj\test\unit_tls_policy.obj \
                build\obj\test\unit_x509.obj

# First make target. Will be used by default
all: $(CLI) $(TEST)

# Build Commands
build\obj\lib\botan_all.obj: botan_all.cpp
	$(CXX) $(LIB_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c botan_all.cpp /Fo$@

build\obj\lib\botan_all_aesni.obj: botan_all_aesni.cpp
	$(CXX)  $(LIB_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c botan_all_aesni.cpp /Fo$@

build\obj\lib\botan_all_avx2.obj: botan_all_avx2.cpp
	$(CXX)  $(LIB_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c botan_all_avx2.cpp /Fo$@

build\obj\lib\botan_all_rdrand.obj: botan_all_rdrand.cpp
	$(CXX)  $(LIB_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c botan_all_rdrand.cpp /Fo$@

build\obj\lib\botan_all_ssse3.obj: botan_all_ssse3.cpp
	$(CXX)  $(LIB_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c botan_all_ssse3.cpp /Fo$@


build\obj\cli\asn1.obj: ..\src\cli\asn1.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\asn1.cpp /Fo$@

build\obj\cli\cc_enc.obj: ..\src\cli\cc_enc.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\cc_enc.cpp /Fo$@

build\obj\cli\compress.obj: ..\src\cli\compress.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\compress.cpp /Fo$@

build\obj\cli\encryption.obj: ..\src\cli\encryption.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\encryption.cpp /Fo$@

build\obj\cli\main.obj: ..\src\cli\main.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\main.cpp /Fo$@

build\obj\cli\math.obj: ..\src\cli\math.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\math.cpp /Fo$@

build\obj\cli\pubkey.obj: ..\src\cli\pubkey.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\pubkey.cpp /Fo$@

build\obj\cli\speed.obj: ..\src\cli\speed.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\speed.cpp /Fo$@

build\obj\cli\timing_tests.obj: ..\src\cli\timing_tests.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\timing_tests.cpp /Fo$@

build\obj\cli\tls_client.obj: ..\src\cli\tls_client.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\tls_client.cpp /Fo$@

build\obj\cli\tls_proxy.obj: ..\src\cli\tls_proxy.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\tls_proxy.cpp /Fo$@

build\obj\cli\tls_server.obj: ..\src\cli\tls_server.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\tls_server.cpp /Fo$@

build\obj\cli\tls_utils.obj: ..\src\cli\tls_utils.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\tls_utils.cpp /Fo$@

build\obj\cli\utils.obj: ..\src\cli\utils.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\utils.cpp /Fo$@

build\obj\cli\x509.obj: ..\src\cli\x509.cpp
	$(CXX) $(CLI_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\cli\x509.cpp /Fo$@


build\obj\test\main.obj: ..\src\tests\main.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\main.cpp /Fo$@

build\obj\test\test_aead.obj: ..\src\tests\test_aead.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_aead.cpp /Fo$@

build\obj\test\test_asn1.obj: ..\src\tests\test_asn1.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_asn1.cpp /Fo$@

build\obj\test\test_bigint.obj: ..\src\tests\test_bigint.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_bigint.cpp /Fo$@

build\obj\test\test_block.obj: ..\src\tests\test_block.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_block.cpp /Fo$@

build\obj\test\test_c25519.obj: ..\src\tests\test_c25519.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_c25519.cpp /Fo$@

build\obj\test\test_certstor.obj: ..\src\tests\test_certstor.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_certstor.cpp /Fo$@

build\obj\test\test_compression.obj: ..\src\tests\test_compression.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_compression.cpp /Fo$@

build\obj\test\test_cryptobox.obj: ..\src\tests\test_cryptobox.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_cryptobox.cpp /Fo$@

build\obj\test\test_dh.obj: ..\src\tests\test_dh.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_dh.cpp /Fo$@

build\obj\test\test_dl_group.obj: ..\src\tests\test_dl_group.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_dl_group.cpp /Fo$@

build\obj\test\test_dlies.obj: ..\src\tests\test_dlies.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_dlies.cpp /Fo$@

build\obj\test\test_dsa.obj: ..\src\tests\test_dsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_dsa.cpp /Fo$@

build\obj\test\test_ecc_pointmul.obj: ..\src\tests\test_ecc_pointmul.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ecc_pointmul.cpp /Fo$@

build\obj\test\test_ecdh.obj: ..\src\tests\test_ecdh.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ecdh.cpp /Fo$@

build\obj\test\test_ecdsa.obj: ..\src\tests\test_ecdsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ecdsa.cpp /Fo$@

build\obj\test\test_ecgdsa.obj: ..\src\tests\test_ecgdsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ecgdsa.cpp /Fo$@

build\obj\test\test_ecies.obj: ..\src\tests\test_ecies.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ecies.cpp /Fo$@

build\obj\test\test_eckcdsa.obj: ..\src\tests\test_eckcdsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_eckcdsa.cpp /Fo$@

build\obj\test\test_ed25519.obj: ..\src\tests\test_ed25519.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ed25519.cpp /Fo$@

build\obj\test\test_elg.obj: ..\src\tests\test_elg.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_elg.cpp /Fo$@

build\obj\test\test_entropy.obj: ..\src\tests\test_entropy.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_entropy.cpp /Fo$@

build\obj\test\test_ffi.obj: ..\src\tests\test_ffi.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ffi.cpp /Fo$@

build\obj\test\test_filters.obj: ..\src\tests\test_filters.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_filters.cpp /Fo$@

build\obj\test\test_fpe.obj: ..\src\tests\test_fpe.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_fpe.cpp /Fo$@

build\obj\test\test_fuzzer.obj: ..\src\tests\test_fuzzer.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_fuzzer.cpp /Fo$@

build\obj\test\test_gf2m.obj: ..\src\tests\test_gf2m.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_gf2m.cpp /Fo$@

build\obj\test\test_gost_3410.obj: ..\src\tests\test_gost_3410.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_gost_3410.cpp /Fo$@

build\obj\test\test_hash.obj: ..\src\tests\test_hash.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_hash.cpp /Fo$@

build\obj\test\test_kdf.obj: ..\src\tests\test_kdf.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_kdf.cpp /Fo$@

build\obj\test\test_keywrap.obj: ..\src\tests\test_keywrap.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_keywrap.cpp /Fo$@

build\obj\test\test_mac.obj: ..\src\tests\test_mac.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_mac.cpp /Fo$@

build\obj\test\test_mceliece.obj: ..\src\tests\test_mceliece.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_mceliece.cpp /Fo$@

build\obj\test\test_modes.obj: ..\src\tests\test_modes.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_modes.cpp /Fo$@

build\obj\test\test_mp.obj: ..\src\tests\test_mp.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_mp.cpp /Fo$@

build\obj\test\test_name_constraint.obj: ..\src\tests\test_name_constraint.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_name_constraint.cpp /Fo$@

build\obj\test\test_newhope.obj: ..\src\tests\test_newhope.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_newhope.cpp /Fo$@

build\obj\test\test_ocb.obj: ..\src\tests\test_ocb.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ocb.cpp /Fo$@

build\obj\test\test_ocsp.obj: ..\src\tests\test_ocsp.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_ocsp.cpp /Fo$@

build\obj\test\test_octetstring.obj: ..\src\tests\test_octetstring.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_octetstring.cpp /Fo$@

build\obj\test\test_os_utils.obj: ..\src\tests\test_os_utils.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_os_utils.cpp /Fo$@

build\obj\test\test_otp.obj: ..\src\tests\test_otp.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_otp.cpp /Fo$@

build\obj\test\test_package_transform.obj: ..\src\tests\test_package_transform.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_package_transform.cpp /Fo$@

build\obj\test\test_pad.obj: ..\src\tests\test_pad.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pad.cpp /Fo$@

build\obj\test\test_passhash.obj: ..\src\tests\test_passhash.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_passhash.cpp /Fo$@

build\obj\test\test_pbkdf.obj: ..\src\tests\test_pbkdf.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pbkdf.cpp /Fo$@

build\obj\test\test_pk_pad.obj: ..\src\tests\test_pk_pad.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pk_pad.cpp /Fo$@

build\obj\test\test_pkcs11.obj: ..\src\tests\test_pkcs11.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pkcs11.cpp /Fo$@

build\obj\test\test_pkcs11_high_level.obj: ..\src\tests\test_pkcs11_high_level.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pkcs11_high_level.cpp /Fo$@

build\obj\test\test_pkcs11_low_level.obj: ..\src\tests\test_pkcs11_low_level.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pkcs11_low_level.cpp /Fo$@

build\obj\test\test_pubkey.obj: ..\src\tests\test_pubkey.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_pubkey.cpp /Fo$@

build\obj\test\test_rfc6979.obj: ..\src\tests\test_rfc6979.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_rfc6979.cpp /Fo$@

build\obj\test\test_rng.obj: ..\src\tests\test_rng.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_rng.cpp /Fo$@

build\obj\test\test_rsa.obj: ..\src\tests\test_rsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_rsa.cpp /Fo$@

build\obj\test\test_simd.obj: ..\src\tests\test_simd.cpp
	$(CXX)  $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_simd.cpp /Fo$@

build\obj\test\test_srp6.obj: ..\src\tests\test_srp6.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_srp6.cpp /Fo$@

build\obj\test\test_stream.obj: ..\src\tests\test_stream.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_stream.cpp /Fo$@

build\obj\test\test_tls_messages.obj: ..\src\tests\test_tls_messages.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_tls_messages.cpp /Fo$@

build\obj\test\test_tpm.obj: ..\src\tests\test_tpm.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_tpm.cpp /Fo$@

build\obj\test\test_tss.obj: ..\src\tests\test_tss.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_tss.cpp /Fo$@

build\obj\test\test_utils.obj: ..\src\tests\test_utils.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_utils.cpp /Fo$@

build\obj\test\test_workfactor.obj: ..\src\tests\test_workfactor.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_workfactor.cpp /Fo$@

build\obj\test\test_x509_dn.obj: ..\src\tests\test_x509_dn.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_x509_dn.cpp /Fo$@

build\obj\test\test_x509_path.obj: ..\src\tests\test_x509_path.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_x509_path.cpp /Fo$@

build\obj\test\test_xmss.obj: ..\src\tests\test_xmss.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\test_xmss.cpp /Fo$@

build\obj\test\tests.obj: ..\src\tests\tests.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\tests.cpp /Fo$@

build\obj\test\unit_ecc.obj: ..\src\tests\unit_ecc.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_ecc.cpp /Fo$@

build\obj\test\unit_ecdh.obj: ..\src\tests\unit_ecdh.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_ecdh.cpp /Fo$@

build\obj\test\unit_ecdsa.obj: ..\src\tests\unit_ecdsa.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_ecdsa.cpp /Fo$@

build\obj\test\unit_tls.obj: ..\src\tests\unit_tls.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_tls.cpp /Fo$@

build\obj\test\unit_tls_policy.obj: ..\src\tests\unit_tls_policy.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_tls_policy.cpp /Fo$@

build\obj\test\unit_x509.obj: ..\src\tests\unit_x509.cpp
	$(CXX) $(TEST_FLAGS) /Ibuild\include /Ibuild\include\external /nologo /c ..\src\tests\unit_x509.cpp /Fo$@


libs: $(LIBRARIES)

# Link Commands
$(CLI): $(LIBRARIES) $(CLIOBJS)
	$(CLI_LINK_CMD) /OUT:$@ $(CLIOBJS) $(LIB_FILENAME) $(CLI_LINKS_TO)
	$(CLI_POST_LINK_CMD)

$(TEST): $(LIBRARIES) $(TESTOBJS)
	$(TEST_LINK_CMD) /OUT:$@ $(TESTOBJS) $(LIB_FILENAME) $(TEST_LINKS_TO)
	$(TEST_POST_LINK_CMD)

!If "$(SO_OBJ_FLAGS)" == ""
# static lib
$(LIB_FILENAME): $(LIBOBJS)
	$(AR) /OUT:$@ $(LIBOBJS)
!Else
# shared lib
# Creates the DLL $(SO_FILENAME) and the .lib $(LIB_FILENAME)
$(SO_FILENAME): $(LIBOBJS)
	$(LIB_LINK_CMD) /OUT:$@ $(LIBOBJS) $(LIB_LINKS_TO)
!Endif

# Fake Targets

SPHINX_CONFIG = ..\src\build-data\sphinx
SPHINX_OPTS = -b html

docs:
	$(COPY) ..\doc\manual\*.rst build\docs\manual

clean:
	-$(RM) build\obj\lib\*
	-$(RM) build\obj\cli\*
	-$(RM) build\obj\test\*
	-$(RM) .\*.manifest
	-$(RM) .\*.exp
	-$(RM) .\*.dll
	-$(RM) $(LIBRARIES) $(CLI) $(TEST)

distclean: clean
	$(RM_R) build
	$(RMDIR) build\include\botan\internal
	$(RMDIR) build\include\botan
	$(RMDIR) build\include
	$(RMDIR) build\lib build\tests
	$(RMDIR) build
	$(RM) Makefile $(LIB_BASENAME).* $(CLI).*

install: $(CLI) docs
	$(SCRIPTS_DIR)\install.py --destdir=c:\Botan --build-dir="build" --bindir=bin --libdir=lib --docdir=docs --includedir=include
