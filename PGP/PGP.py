from Crypto.PublicKey import ECC, RSA
from Crypto.Signature import DSS
from Crypto.Hash import SHA256
from Crypto.Cipher import DES, PKCS1_OAEP
from Crypto.Util.Padding import pad, unpad
import Crypto.Random
import zlib

# return: (private_key, public_key)
def generate_ECC_key():
    private_key = ECC.generate(curve='P-256')
    return private_key, private_key.public_key()


# return: (private_key, public_key)
def generate_RSA_key():
    private_key = RSA.generate(1024)
    return private_key, private_key.publickey()


def generate_DES_key():
    key = Crypto.Random.get_random_bytes(8)
    return key


def sign(data, key):
    h = SHA256.new(data)
    signer = DSS.new(key, 'fips-186-3')
    signature = signer.sign(h)
    return signature


# return True if data is authentic
def verify(data, signature, key):
    h = SHA256.new(data)
    verifier = DSS.new(key, 'fips-186-3')
    try:
        verifier.verify(h, signature)
        return True
    except ValueError:
        return False


def DES_encrypt(data, key):
    cipher = DES.new(key, DES.MODE_ECB)
    cipher_text = cipher.encrypt(pad(data, 32))
    return cipher_text


def DES_decrypt(data, key):
    cipher = DES.new(key, DES.MODE_ECB)
    data_dec = cipher.decrypt(data)
    plain_text = unpad(data_dec, 32)
    return plain_text


def RSA_encrypt(data, key):
    cipher = PKCS1_OAEP.new(key)
    cipher_text = cipher.encrypt(data)
    return cipher_text


def RSA_decrypt(data, key):
    cipher = PKCS1_OAEP.new(key)
    plain_text = cipher.decrypt(data)
    return plain_text


def Zip(data):
    return zlib.compress(data,7)


def UnZip(data):
    return zlib.decompress(data)


def PGP_send(data, alice_pri_key, bob_pub_key):
    data += sign(data, alice_pri_key)
    des_key = generate_DES_key()
    data = DES_encrypt(data, des_key)
    des_key_enc = RSA_encrypt(des_key, bob_pub_key)  # 加密后的DES密钥
    data += des_key_enc
    data = Zip(data)
    return data


def PGP_recieve(data, alice_pub_key, bob_pri_key):
    data = UnZip(data)
    data, des_key_enc = data[:-128], data[-128:]
    des_key = RSA_decrypt(des_key_enc, bob_pri_key)
    data = DES_decrypt(data, des_key)
    data, signature = data[:-64], data[-64:]
    assert verify(data, signature, alice_pub_key)
    return data


if __name__ == '__main__':
    alice_pri_key, alice_pub_key = generate_ECC_key()
    bob_pri_key, bob_pub_key = generate_RSA_key()
    data = b'Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test !!!!!'
    data = PGP_send(data, alice_pri_key, bob_pub_key)
    data = PGP_recieve(data, alice_pub_key, bob_pri_key)
    print(data)
