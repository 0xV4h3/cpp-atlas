#ifndef QUIZDATABASE_H
#define QUIZDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

/**
 * @brief Singleton manager for the CppAtlas SQLite quiz database.
 *
 * Responsibilities:
 *  - Open/create the database at QStandardPaths::AppDataLocation/cppatlas.db
 *  - Apply schema migrations from the embedded :/db/schema.sql resource
 *  - Seed initial content from :/db/seed_data.sql on first run
 *  - Provide the connection name used by all other repository classes
 *
 * Usage:
 * @code
 *   if (!QuizDatabase::instance().initialize()) {
 *       qCritical() << "DB init failed";
 *   }
 *   // Everywhere else use QSqlQuery with QuizDatabase::connectionName()
 * @endcode
 */
class QuizDatabase : public QObject
{
    Q_OBJECT

public:
    static QuizDatabase& instance();

    /**
     * @brief Initialize the database: open, migrate schema, seed if empty.
     * @return true on success
     */
    bool initialize();

    /**
     * @brief Close the database connection cleanly.
     */
    void shutdown();

    /**
     * @brief Returns true if the DB connection is open and valid.
     */
    bool isOpen() const;

    /**
     * @brief The Qt connection name used for QSqlDatabase::database(name).
     */
    static constexpr const char* CONNECTION_NAME = "CppAtlasQuizDB";

    /**
     * @brief Last error from the most recent DB operation.
     */
    QSqlError lastError() const;

    /**
     * @brief Returns the filesystem path of the database file.
     */
    QString databasePath() const;

private:
    explicit QuizDatabase(QObject* parent = nullptr);
    ~QuizDatabase() override = default;
    QuizDatabase(const QuizDatabase&) = delete;
    QuizDatabase& operator=(const QuizDatabase&) = delete;

    bool openDatabase();
    bool applySchema();
    bool runSqlFile(const QString& resourcePath);
    bool needsSeed() const;
    bool applySeed();
    int  currentSchemaVersion() const;

    QString     m_dbPath;
    QSqlError   m_lastError;
};

#endif // QUIZDATABASE_H