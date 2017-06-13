#include <gmock/gmock.h>
#include "tests.h"
#include <botan_all.h>
#include <iostream>

using namespace std;
using namespace Botan_Tests;

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    const bool log_success = false; // = flag_set("log-success");
    const bool run_online_tests = false; // = flag_set("run-online-tests");
    const bool run_long_tests = false; // = flag_set("run-long-tests");
    const std::string data_dir; // = get_arg_or("data-dir", "src/tests/data");
    const std::string pkcs11_lib; // = get_arg("pkcs11-lib");
    const std::string provider; // = get_arg("provider");
    const std::string drbg_seed; // = get_arg("drbg-seed");

    Botan_Tests::Provider_Filter pf;
    if (!provider.empty())
    {
        cout << " provider:" << provider;
        pf.set(provider);
    }
#if defined(BOTAN_HAS_OPENSSL)
    if (provider.empty() || provider == "openssl")
    {
        ERR_load_crypto_strings();
    }
#endif
    std::unique_ptr<Botan::RandomNumberGenerator> rng;

#if defined(BOTAN_HAS_HMAC_DRBG) && defined(BOTAN_HAS_SHA2_64)
    std::vector<uint8_t> seed = Botan::hex_decode(drbg_seed);
    if (seed.empty())
    {
        const uint64_t ts = Botan_Tests::Test::timestamp();
        seed.resize(8);
        Botan::store_be(ts, seed.data());
    }

    cout << " rng:HMAC_DRBG with seed '" << Botan::hex_encode(seed) << "'";

    // Expand out the seed to 512 bits to make the DRBG happy
    std::unique_ptr<Botan::HashFunction> sha512(Botan::HashFunction::create("SHA-512"));
    sha512->update(seed);
    seed.resize(sha512->output_length());
    sha512->final(seed.data());

    std::unique_ptr<Botan::HMAC_DRBG> drbg(new Botan::HMAC_DRBG("SHA-384"));
    drbg->initialize_with(seed.data(), seed.size());
    rng.reset(new Botan::Serialized_RNG(drbg.release()));

#else

    if (drbg_seed != "")
    {
        throw Botan_Tests::Test_Error("HMAC_DRBG disabled in build, cannot specify DRBG seed");
    }

#if defined(BOTAN_HAS_SYSTEM_RNG)
    output() << " rng:system";
    rng.reset(new Botan::System_RNG);
#elif defined(BOTAN_HAS_AUTO_SEEDING_RNG)
    output() << " rng:autoseeded";
    rng.reset(new Botan::Serialized_RNG(new Botan::AutoSeeded_RNG));
#else
    // last ditch fallback for RNG-less build
    class Bogus_Fallback_RNG : public Botan::RandomNumberGenerator
    {
    public:
        std::string name() const override
        {
            return "Bogus_Fallback_RNG";
        }

        void clear() override
        {
            /* ignored */
        }

        void randomize(uint8_t out[], size_t len) override
        {
            for (size_t i = 0; i != len; ++i)
            {
                out[i] = std::rand();
            }
        }

        bool is_seeded() const override
        {
            return true;
        }

        void add_entropy(const uint8_t[], size_t) override
        {
            /* ignored */
        }
    };

    rng.reset(new Bogus_Fallback_RNG);
#endif

#endif
    Botan_Tests::Test::setup_tests(log_success, run_online_tests, run_long_tests,
                                   data_dir, pkcs11_lib, pf, rng.get());
    return RUN_ALL_TESTS();
}
