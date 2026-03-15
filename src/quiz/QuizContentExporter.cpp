#include "quiz/QuizContentExporter.h"
#include "quiz/QuizDatabase.h"

#include <QDateTime>
#include <QFile>
#include <QMetaType>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>
#include <QVariant>
#include <QDebug>

bool QuizContentExporter::exportToFile(const QString& filePath,
                                        int*    outRowsTotal,
                                        QString* error)
{
    auto fail = [&](const QString& msg) -> bool {
        qWarning() << "[QuizContentExporter]" << msg;
        if (error) *error = msg;
        return false;
    };

    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return fail(QString("Cannot open output file: %1").arg(filePath));

    QTextStream out(&f);

    out << "-- CppAtlas quiz content export\n";
    out << "-- Generated: "
        << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n\n";

    QSqlDatabase db = QSqlDatabase::database(QuizDatabase::CONNECTION_NAME);

    struct TableSpec { QString name; QString orderBy; };
    const QList<TableSpec> tables = {
        { "topics",        "ORDER BY id" },
        { "tags",          "ORDER BY id" },
        { "quizzes",       "ORDER BY id" },
        { "questions",     "ORDER BY id" },
        { "options",       "ORDER BY id" },
        { "question_tags", "ORDER BY question_id, tag_id" },
        { "quiz_tags",     "ORDER BY quiz_id, tag_id" },
    };

    int totalRows = 0;

    for (const TableSpec& spec : tables) {
        out << "-- Table: " << spec.name << "\n";

        QSqlQuery q(db);
        if (!q.exec("SELECT * FROM " + spec.name + " " + spec.orderBy)) {
            out.flush();
            f.close();
            return fail(QString("Failed to query table '%1': %2")
                            .arg(spec.name, q.lastError().text()));
        }

        const QSqlRecord rec = q.record();
        const int fieldCount = rec.count();
        int tableRows = 0;

        while (q.next()) {
            out << "INSERT INTO " << spec.name << " (";
            for (int i = 0; i < fieldCount; ++i) {
                if (i > 0) out << ", ";
                out << rec.fieldName(i);
            }
            out << ") VALUES (";
            for (int i = 0; i < fieldCount; ++i) {
                if (i > 0) out << ", ";
                const QVariant val = q.value(i);
                if (val.isNull()) {
                    out << "NULL";
                } else {
                    const QMetaType::Type mt =
                        static_cast<QMetaType::Type>(val.typeId());
                    if (mt == QMetaType::Int || mt == QMetaType::LongLong
                            || mt == QMetaType::UInt || mt == QMetaType::ULongLong) {
                        out << val.toLongLong();
                    } else {
                        out << "'" << val.toString().replace("'", "''") << "'";
                    }
                }
            }
            out << ");\n";
            ++tableRows;
        }
        out << "\n";
        totalRows += tableRows;
        qDebug() << "[QuizContentExporter]" << spec.name << ":" << tableRows << "row(s)";
    }

    out.flush();
    f.close();

    if (outRowsTotal) *outRowsTotal = totalRows;
    if (error) error->clear();
    return true;
}
