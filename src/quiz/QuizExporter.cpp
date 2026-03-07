#include "quiz/QuizExporter.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

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
             << "questions to" << filePath;
    return true;
}

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

    // Version check
    const int version = root.value("cppatlas_export").toInt(0);
    if (version < 1 || version > FORMAT_VERSION)
        return fail(QString("Unsupported export version: %1").arg(version));

    const QJsonArray questionsArr = root.value("questions").toArray();
    if (questionsArr.isEmpty())
        return fail("No questions found in file");

    QList<QuestionDTO> result;
    result.reserve(questionsArr.size());
    for (const auto& val : questionsArr) {
        if (!val.isObject()) continue;
        result << questionFromJson(val.toObject());
    }

    if (error) error->clear();
    qDebug() << "[QuizExporter] Imported" << result.size()
             << "questions from" << filePath;
    return result;
}

QJsonObject QuizExporter::questionToJson(const QuestionDTO& q)
{
    QJsonObject obj;
    obj["id"]          = q.id;        // original DB id for reference; -1 for transient
    obj["type"]        = q.type;
    obj["content"]     = q.content;
    obj["difficulty"]  = q.difficulty;
    obj["points"]      = q.points;

    if (!q.codeSnippet.isEmpty())  obj["code_snippet"]  = q.codeSnippet;
    if (!q.explanation.isEmpty())  obj["explanation"]   = q.explanation;
    if (!q.hint.isEmpty())         obj["hint"]          = q.hint;
    if (!q.refUrl.isEmpty())       obj["ref_url"]       = q.refUrl;

    if (!q.tags.isEmpty()) {
        QJsonArray tagsArr;
        for (const auto& t : q.tags) tagsArr.append(t);
        obj["tags"] = tagsArr;
    }

    obj["options"] = optionsToJson(q.options);
    return obj;
}

QuestionDTO QuizExporter::questionFromJson(const QJsonObject& obj)
{
    QuestionDTO q;
    q.id          = obj.value("id").toInt(-1);  // -1 = transient (not from DB)
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

    q.options = optionsFromJson(obj.value("options").toArray());
    return q;
}

QJsonArray QuizExporter::optionsToJson(const QList<OptionDTO>& options)
{
    QJsonArray arr;
    for (const auto& opt : options) {
        QJsonObject o;
        o["content"]    = opt.content;
        o["is_correct"] = opt.isCorrect;
        if (!opt.codeSnippet.isEmpty()) o["code_snippet"] = opt.codeSnippet;
        arr.append(o);
    }
    return arr;
}

QList<OptionDTO> QuizExporter::optionsFromJson(const QJsonArray& arr)
{
    QList<OptionDTO> opts;
    int order = 0;
    for (const auto& val : arr) {
        if (!val.isObject()) continue;
        const QJsonObject o = val.toObject();
        OptionDTO opt;
        opt.content     = o.value("content").toString();
        opt.isCorrect   = o.value("is_correct").toBool(false);
        opt.codeSnippet = o.value("code_snippet").toString();
        opt.orderIndex  = order++;
        opts << opt;
    }
    return opts;
}