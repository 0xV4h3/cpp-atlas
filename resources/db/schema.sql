CREATE TABLE IF NOT EXISTS schema_version (
    version     INTEGER PRIMARY KEY,
    applied_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
    description TEXT
);

CREATE TABLE IF NOT EXISTS users (
    id            INTEGER  PRIMARY KEY AUTOINCREMENT,
    username      TEXT     NOT NULL UNIQUE COLLATE NOCASE,
    display_name  TEXT     NOT NULL,
    password_hash TEXT     NOT NULL,
    salt          TEXT     NOT NULL,
    avatar_color  TEXT     DEFAULT '#007ACC',
    avatar_path   TEXT     DEFAULT '',
    created_at    DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_login    DATETIME,
    is_admin      INTEGER  DEFAULT 0
);

CREATE TABLE IF NOT EXISTS topics (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    slug        TEXT     NOT NULL UNIQUE,
    title       TEXT     NOT NULL,
    description TEXT,
    parent_id   INTEGER  REFERENCES topics(id) ON DELETE SET NULL,
    level       INTEGER  DEFAULT 0,
    difficulty  INTEGER  DEFAULT 1,
    order_index INTEGER  DEFAULT 0,
    icon        TEXT     DEFAULT '?',
    ref_url     TEXT,
    ref_url2    TEXT
);

CREATE TABLE IF NOT EXISTS tags (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    name  TEXT    NOT NULL UNIQUE COLLATE NOCASE,
    color TEXT    DEFAULT '#569CD6'
);

CREATE TABLE IF NOT EXISTS quizzes (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    title       TEXT     NOT NULL,
    description TEXT,
    topic_id    INTEGER  REFERENCES topics(id) ON DELETE SET NULL,
    difficulty  INTEGER  DEFAULT 1,
    time_limit  INTEGER  DEFAULT 0,
    is_timed    INTEGER  DEFAULT 0,
    type        TEXT     DEFAULT 'standard',
    is_active   INTEGER  DEFAULT 1,
    created_by  INTEGER  DEFAULT 0,
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
    version     INTEGER  DEFAULT 1
);

CREATE TABLE IF NOT EXISTS quiz_tags (
    quiz_id  INTEGER NOT NULL REFERENCES quizzes(id)  ON DELETE CASCADE,
    tag_id   INTEGER NOT NULL REFERENCES tags(id)     ON DELETE CASCADE,
    PRIMARY KEY (quiz_id, tag_id)
);

CREATE TABLE IF NOT EXISTS questions (
    id           INTEGER  PRIMARY KEY AUTOINCREMENT,
    quiz_id      INTEGER  REFERENCES quizzes(id) ON DELETE CASCADE,
    topic_id     INTEGER  REFERENCES topics(id)  ON DELETE SET NULL,
    type         TEXT     NOT NULL DEFAULT 'mcq',
    content      TEXT     NOT NULL,
    code_snippet TEXT,
    explanation  TEXT,
    difficulty   INTEGER  DEFAULT 1,
    time_limit   INTEGER  DEFAULT 0,
    points       INTEGER  DEFAULT 10,
    order_index  INTEGER  DEFAULT 0,
    is_active    INTEGER  DEFAULT 1,
    hint         TEXT,
    ref_url      TEXT,
    created_at   DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS question_tags (
    question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    tag_id      INTEGER NOT NULL REFERENCES tags(id)      ON DELETE CASCADE,
    PRIMARY KEY (question_id, tag_id)
);

CREATE TABLE IF NOT EXISTS options (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    question_id  INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    content      TEXT    NOT NULL,
    code_snippet TEXT,
    is_correct   INTEGER DEFAULT 0,
    order_index  INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS quiz_sessions (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER  NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    quiz_id     INTEGER  REFERENCES quizzes(id) ON DELETE SET NULL,
    started_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
    finished_at DATETIME,
    score       INTEGER  DEFAULT 0,
    max_score   INTEGER  DEFAULT 0,
    time_spent  INTEGER  DEFAULT 0,
    mode        TEXT     DEFAULT 'practice',
    is_complete INTEGER  DEFAULT 0
);

CREATE TABLE IF NOT EXISTS question_attempts (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    session_id  INTEGER  NOT NULL REFERENCES quiz_sessions(id) ON DELETE CASCADE,
    question_id INTEGER  REFERENCES questions(id) ON DELETE SET NULL,
    user_answer TEXT,
    is_correct  INTEGER  DEFAULT 0,
    time_spent  INTEGER  DEFAULT 0,
    hint_used   INTEGER  DEFAULT 0,
    answered_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS user_topic_stats (
    user_id         INTEGER NOT NULL REFERENCES users(id)  ON DELETE CASCADE,
    topic_id        INTEGER NOT NULL REFERENCES topics(id) ON DELETE CASCADE,
    attempts        INTEGER DEFAULT 0,
    correct         INTEGER DEFAULT 0,
    last_attempt_at DATETIME,
    mastery_level   REAL    DEFAULT 0.0,
    PRIMARY KEY (user_id, topic_id)
);

CREATE TABLE IF NOT EXISTS custom_tests (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER  NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    title       TEXT     NOT NULL,
    description TEXT,
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at  DATETIME
);

CREATE TABLE IF NOT EXISTS custom_test_questions (
    test_id     INTEGER NOT NULL REFERENCES custom_tests(id)  ON DELETE CASCADE,
    question_id INTEGER NOT NULL REFERENCES questions(id)     ON DELETE CASCADE,
    order_index INTEGER DEFAULT 0,
    PRIMARY KEY (test_id, question_id)
);

CREATE TABLE IF NOT EXISTS recommendations (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER  NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    topic_id    INTEGER  REFERENCES topics(id) ON DELETE CASCADE,
    reason      TEXT,
    ref_url     TEXT,
    is_read     INTEGER  DEFAULT 0,
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_sessions_user    ON quiz_sessions(user_id);
CREATE INDEX IF NOT EXISTS idx_attempts_session ON question_attempts(session_id);
CREATE INDEX IF NOT EXISTS idx_stats_user       ON user_topic_stats(user_id);
CREATE INDEX IF NOT EXISTS idx_questions_quiz   ON questions(quiz_id);
CREATE INDEX IF NOT EXISTS idx_questions_topic  ON questions(topic_id);
CREATE INDEX IF NOT EXISTS idx_topics_parent    ON topics(parent_id);
CREATE INDEX IF NOT EXISTS idx_topics_slug      ON topics(slug);

INSERT OR IGNORE INTO schema_version (version, description)
VALUES (1, 'Initial schema');
