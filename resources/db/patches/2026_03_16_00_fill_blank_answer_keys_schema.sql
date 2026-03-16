-- Patch 00: Add explicit accepted-answer table for fill_blank questions.
-- Numbered prefix ensures this schema patch always runs first.
-- The options table continues to work as a fallback for backward compatibility.

CREATE TABLE IF NOT EXISTS fill_blank_answers (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    answer      TEXT    NOT NULL,
    is_active   INTEGER DEFAULT 1,
    order_index INTEGER DEFAULT 0
);

CREATE INDEX IF NOT EXISTS idx_fill_blank_answers_qid
    ON fill_blank_answers(question_id);
