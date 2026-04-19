#include "quiz/QuizExporter.h"
#include "quiz/ExportCrypto.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

// ─────────────────────────────────────────────────────────────────────────────
// Export
// ─────────────────────────────────────────────────────────────────────────────

bool QuizExporter::exportToFile(const QList<QuestionDTO>& questions,
                                const QString& title,
                                const QString& description,
                                const QString& filePath)
{
    if (questions.isEmpty()) {
        qWarning() << "[QuizExporter] exportToFile: question list is empty";
        return false;
    }

    QJsonObject root;
    root["cppatlas_export"] = FORMAT_VERSION;
    root["title"]           = title;
    root["description"]     = description;
    root["exported_at"]     = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    root["question_count"]  = questions.size();
    // Mark that answers in this file are encrypted so importFromFile knows
    // to decrypt them.
    root["answers_encrypted"] = true;

    QJsonArray questionsArr;
    for (const auto& q : questions)
        questionsArr.append(questionToJson(q));
    root["questions"] = questionsArr;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "[QuizExporter] Cannot open file for writing:" << filePath;
        return false;
    }

    const QByteArray json = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (file.write(json) != json.size()) {
        qWarning() << "[QuizExporter] Write error:" << file.errorString();
        return false;
    }

    qDebug() << "[QuizExporter] Exported" << questions.size()
             << "questions to" << filePath << "(answers encrypted)";
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Import
// ─────────────────────────────────────────────────────────────────────────────

QList<QuestionDTO> QuizExporter::importFromFile(const QString& filePath,
                                                QString* error)
{
    auto fail = [&](const QString& msg) -> QList<QuestionDTO> {
        qWarning() << "[QuizExporter]" << msg;
        if (error) *error = msg;
        return {};
    };

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return fail(QString("Cannot open file: %1").arg(filePath));

    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError)
        return fail(QString("JSON parse error: %1").arg(parseErr.errorString()));

    if (!doc.isObject())
        return fail("File is not a valid CppAtlas export (root is not an object)");

    const QJsonObject root = doc.object();

    const int version = root.value("cppatlas_export").toInt(0);
    if (version < 1 || version > FORMAT_VERSION)
        return fail(QString("Unsupported export version: %1").arg(version));

    const QJsonArray questionsArr = root.value("questions").toArray();
    if (questionsArr.isEmpty())
        return fail("No questions found in file");

    // Detect whether this file has encrypted answers (v1 files without the
    // flag are treated as plaintext for backwards compatibility).
    const bool answersEncrypted = root.value("answers_encrypted").toBool(false);

    QList<QuestionDTO> result;
    result.reserve(questionsArr.size());
    for (const auto& val : questionsArr) {
        if (!val.isObject()) continue;
        result << questionFromJson(val.toObject(), answersEncrypted);
    }

    if (error) error->clear();
    qDebug() << "[QuizExporter] Imported" << result.size()
             << "questions from" << filePath
             << (answersEncrypted ? "(answers decrypted)" : "(plaintext)");
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// Serialisation helpers
// ─────────────────────────────────────────────────────────────────────────────

QJsonObject QuizExporter::questionToJson(const QuestionDTO& q)
{
    QJsonObject obj;
    obj["id"]         = q.id;
    obj["type"]       = q.type;
    obj["content"]    = q.content;       // question text — NOT secret, keep plain
    obj["difficulty"] = q.difficulty;
    obj["points"]     = q.points;

    if (!q.codeSnippet.isEmpty()) obj["code_snippet"] = q.codeSnippet;
    // explanation — shown to students after answering, keep plain
    if (!q.explanation.isEmpty()) obj["explanation"]  = q.explanation;
    if (!q.hint.isEmpty())        obj["hint"]         = q.hint;
    if (!q.refUrl.isEmpty())      obj["ref_url"]      = q.refUrl;

    if (!q.tags.isEmpty()) {
        QJsonArray tagsArr;
        for (const auto& t : q.tags) tagsArr.append(t);
        obj["tags"] = tagsArr;
    }

    // ── Encrypt answer-bearing data ──────────────────────────────────────────
    // For MCQ / multi_select / true_false / code_output: the is_correct flag
    // on each option is the answer.  We encrypt it so a student cannot open
    // the JSON and grep for "true".
    //
    // For fill_blank: accepted answers are stored separately in
    // fill_blank_answers; options are not used.  The accepted answers list
    // comes from QuestionDTO::acceptedAnswers.
    obj["options"] = optionsToJson(q.options);

    if (!q.acceptedAnswers.isEmpty()) {
        // Serialise the accepted answers list as a JSON array string, then
        // encrypt the whole thing as a single blob.
        QJsonArray ansArr;
        for (const QString& a : q.acceptedAnswers) ansArr.append(a);
        const QString plainJson =
            QJsonDocument(ansArr).toJson(QJsonDocument::Compact);
        obj["accepted_answers_enc"] = ExportCrypto::encrypt(plainJson);
        // Do NOT write a plain "accepted_answers" key.
    }

    return obj;
}

QuestionDTO QuizExporter::questionFromJson(const QJsonObject& obj,
                                           bool answersEncrypted)
{
    QuestionDTO q;
    q.id          = obj.value("id").toInt(-1);
    q.type        = obj.value("type").toString("mcq");
    q.content     = obj.value("content").toString();
    q.difficulty  = obj.value("difficulty").toInt(1);
    q.points      = obj.value("points").toInt(10);
    q.codeSnippet = obj.value("code_snippet").toString();
    q.explanation = obj.value("explanation").toString();
    q.hint        = obj.value("hint").toString();
    q.refUrl      = obj.value("ref_url").toString();

    const QJsonArray tagsArr = obj.value("tags").toArray();
    for (const auto& t : tagsArr)
        q.tags << t.toString();

    q.options = optionsFromJson(obj.value("options").toArray(), answersEncrypted);

    // Decrypt accepted answers (fill_blank)
    if (obj.contains("accepted_answers_enc")) {
        bool ok = false;
        const QString decrypted =
            ExportCrypto::decrypt(obj.value("accepted_answers_enc").toString(), &ok);
        if (ok) {
            const QJsonArray ansArr =
                QJsonDocument::fromJson(decrypted.toUtf8()).array();
            for (const auto& a : ansArr)
                q.acceptedAnswers << a.toString();
        } else {
            qWarning() << "[QuizExporter] Failed to decrypt accepted_answers for question"
                       << q.id;
        }
    } else if (obj.contains("accepted_answers")) {
        // Backwards compat: plaintext accepted_answers from old exports
        const QJsonArray ansArr = obj.value("accepted_answers").toArray();
        for (const auto& a : ansArr)
            q.acceptedAnswers << a.toString();
    }

    return q;
}

// ─────────────────────────────────────────────────────────────────────────────
// Options serialisation
// ─────────────────────────────────────────────────────────────────────────────

QJsonArray QuizExporter::optionsToJson(const QList<OptionDTO>& options)
{
    QJsonArray arr;
    for (const auto& opt : options) {
        QJsonObject o;
        o["content"] = opt.content;
        if (!opt.codeSnippet.isEmpty()) o["code_snippet"] = opt.codeSnippet;

        // Encrypt the is_correct flag: we serialise it as "1" or "0" and
        // encrypt that tiny string.  A student cannot grep for true/false.
        const QString correctStr = opt.isCorrect ? QStringLiteral("1")
                                                 : QStringLiteral("0");
        o["c"] = ExportCrypto::encrypt(correctStr);  // short key name also
        // avoids grep for "correct"
        arr.append(o);
    }
    return arr;
}

QList<OptionDTO> QuizExporter::optionsFromJson(const QJsonArray& arr,
                                               bool answersEncrypted)
{
    QList<OptionDTO> opts;
    int order = 0;
    for (const auto& val : arr) {
        if (!val.isObject()) continue;
        const QJsonObject o = val.toObject();
        OptionDTO opt;
        opt.content     = o.value("content").toString();
        opt.codeSnippet = o.value("code_snippet").toString();
        opt.orderIndex  = order++;

        if (answersEncrypted && o.contains("c")) {
            // Decrypt the correctness flag
            bool ok = false;
            const QString plain =
                ExportCrypto::decrypt(o.value("c").toString(), &ok);
            opt.isCorrect = ok && (plain == QStringLiteral("1"));
        } else if (o.contains("is_correct")) {
            // Backwards compat: plaintext from old exports or non-encrypted files
            opt.isCorrect = o.value("is_correct").toBool(false);
        } else {
            opt.isCorrect = false;
        }

        opts << opt;
    }
    return opts;
}

// ─────────────────────────────────────────────────────────────────────────────
// Utility
// ─────────────────────────────────────────────────────────────────────────────

QString QuizExporter::ensureJsonExtension(const QString& filePath)
{
    if (filePath.endsWith(".json", Qt::CaseInsensitive))
        return filePath;
    return filePath + ".json";
}

bool QuizExporter::readHeader(const QString& filePath,
                              QString& outTitle,
                              QString& outDescription)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;
    const QJsonObject root = doc.object();
    outTitle       = root.value("title").toString();
    outDescription = root.value("description").toString();
    return true;
}
