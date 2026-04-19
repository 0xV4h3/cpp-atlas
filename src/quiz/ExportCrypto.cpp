#include "quiz/ExportCrypto.h"

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Salt generation
// ─────────────────────────────────────────────────────────────────────────────

QByteArray ExportCrypto::generateSalt()
{
    QByteArray salt(16, '\0');
    QRandomGenerator* rng = QRandomGenerator::global();
    for (int i = 0; i < 16; ++i)
        salt[i] = static_cast<char>(rng->bounded(256));
    return salt;
}

// ─────────────────────────────────────────────────────────────────────────────
// Key derivation: SHA-256( APP_SECRET || salt )
// ─────────────────────────────────────────────────────────────────────────────

QByteArray ExportCrypto::deriveKey(const QByteArray& salt)
{
    QCryptographicHash h(QCryptographicHash::Sha256);
    h.addData(QByteArray(APP_SECRET));
    h.addData(salt);
    return h.result();  // 32 bytes
}

// ─────────────────────────────────────────────────────────────────────────────
// XOR-stream cipher
//
// Keystream is built from 32-byte blocks:
//   block_i = SHA-256( key || i_as_4_bytes )
// This avoids repeating the keystream for data longer than 32 bytes.
// ─────────────────────────────────────────────────────────────────────────────

QByteArray ExportCrypto::xorStream(const QByteArray& data, const QByteArray& key)
{
    QByteArray result(data.size(), '\0');
    int pos = 0;
    quint32 counter = 0;

    while (pos < data.size()) {
        // Generate one 32-byte keystream block
        QCryptographicHash h(QCryptographicHash::Sha256);
        h.addData(key);
        // Append counter as 4 big-endian bytes for domain separation
        char counterBytes[4];
        counterBytes[0] = static_cast<char>((counter >> 24) & 0xFF);
        counterBytes[1] = static_cast<char>((counter >> 16) & 0xFF);
        counterBytes[2] = static_cast<char>((counter >>  8) & 0xFF);
        counterBytes[3] = static_cast<char>( counter        & 0xFF);
        h.addData(counterBytes, 4);
        const QByteArray block = h.result();  // 32 bytes

        const int blockUsed = qMin(block.size(), data.size() - pos);
        for (int i = 0; i < blockUsed; ++i)
            result[pos + i] = static_cast<char>(data[pos + i] ^ block[i]);

        pos += blockUsed;
        ++counter;
    }

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

QString ExportCrypto::encrypt(const QString& plaintext)
{
    const QByteArray plain = plaintext.toUtf8();
    const QByteArray salt  = generateSalt();          // 16 bytes, random
    const QByteArray key   = deriveKey(salt);          // 32 bytes
    const QByteArray cipher = xorStream(plain, key);  // same size as input

    // Format: salt (16 bytes) || ciphertext — then Base64-encode the whole thing
    QByteArray payload;
    payload.reserve(16 + cipher.size());
    payload.append(salt);
    payload.append(cipher);

    return QString::fromLatin1(payload.toBase64());
}

QString ExportCrypto::decrypt(const QString& base64Cipher, bool* ok)
{
    const QByteArray raw = QByteArray::fromBase64(base64Cipher.toLatin1());

    // Minimum: 16 bytes salt + at least 1 byte ciphertext
    if (raw.size() < 17) {
        qWarning() << "[ExportCrypto] decrypt: payload too short ("
                   << raw.size() << "bytes)";
        if (ok) *ok = false;
        return {};
    }

    const QByteArray salt   = raw.left(16);
    const QByteArray cipher = raw.mid(16);
    const QByteArray key    = deriveKey(salt);
    const QByteArray plain  = xorStream(cipher, key);

    // Validate: result must be valid UTF-8
    const QString result = QString::fromUtf8(plain);
    if (result.isEmpty() && !plain.isEmpty()) {
        qWarning() << "[ExportCrypto] decrypt: result is not valid UTF-8";
        if (ok) *ok = false;
        return {};
    }

    if (ok) *ok = true;
    return result;
}

bool ExportCrypto::looksEncrypted(const QString& value)
{
    // A valid encrypted blob is non-empty Base64 with at least ceil((16+1)*4/3)=24 chars.
    if (value.size() < 24) return false;
    const QByteArray raw = QByteArray::fromBase64(value.toLatin1());
    return raw.size() >= 17;
}
