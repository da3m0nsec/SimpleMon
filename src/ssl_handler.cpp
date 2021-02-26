#include <SimpleMon/ssl_handler.h>

std::vector<uint8_t> encrypt (const uint8_t* in, size_t length, std::string path_to_key) {

	std::string plaintext("Your great-grandfather gave this watch to your granddad for good luck. Unfortunately, Dane's luck wasn't as good as his old man's.");
	std::vector<uint8_t> pt(plaintext.data(),plaintext.data()+plaintext.length());
	std::unique_ptr<Botan::RandomNumberGenerator> rng(new Botan::AutoSeeded_RNG);

	//load keypair
	Botan::SecureVector<uint8_t> keyBytes = open_key(path_to_key);
	Botan::DataSource_Memory source (keyBytes);
	std::unique_ptr<Botan::Private_Key> kp(Botan::PKCS8::load_key(source,*rng.get()));

	//encrypt with pk
	Botan::PK_Encryptor_EME enc(*kp,*rng.get(), "EME1(SHA-256)");
	std::vector<uint8_t> ct = enc.encrypt(pt,*rng.get());

	//decrypt with sk
	Botan::PK_Decryptor_EME dec(*kp,*rng.get(), "EME1(SHA-256)");
	std::cout << std::endl << "enc: " << Botan::hex_encode(ct) << std::endl << "dec: "<< Botan::hex_encode(dec.decrypt(ct));
}

bool verify (const uint8_t* in, size_t length, std::string pathToKey) {
	Botan::AutoSeeded_RNG rng;
	// Generate ECDSA keypair
	Botan::ECDSA_PrivateKey key(rng, Botan::EC_Group("secp521r1"));

	std::string text("This is a tasty burger!");
	std::vector<uint8_t> data(text.data(),text.data()+text.length());
	// sign data
	Botan::PK_Signer signer(key, rng, "EMSA1(SHA-256)");
	signer.update(data);
	std::vector<uint8_t> signature = signer.signature(rng);
	std::cout << "Signature:" << std::endl << Botan::hex_encode(signature);
	// verify signature
	Botan::PK_Verifier verifier(key, "EMSA1(SHA-256)");
	verifier.update(data);
	std::cout << std::endl << "is " << (verifier.check_signature(signature)? "valid" : "invalid");
	return 0;
}

Botan::SecureVector<uint8_t> open_key (std::string path_to_key) {

}