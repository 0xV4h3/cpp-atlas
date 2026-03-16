-- Corrective patch: fix fill_blank questions that have sentence-style
-- correct options instead of short keyword tokens.
--
-- Safety guard: ensure the fill_blank_answers table exists before inserting.
-- This makes the patch safe to apply regardless of patch execution order.
CREATE TABLE IF NOT EXISTS fill_blank_answers (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    answer      TEXT    NOT NULL,
    is_active   INTEGER DEFAULT 1,
    order_index INTEGER DEFAULT 0
);
CREATE INDEX IF NOT EXISTS idx_fill_blank_answers_qid
    ON fill_blank_answers(question_id);

-- Target by stable question content match rather than fragile order_index.
-- Fixes: "Type wrapper for lock-free style scalar synchronization is std::_____."
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'atomic', 0
FROM questions
WHERE type = 'fill_blank'
  AND content LIKE '%lock-free style scalar synchronization%'
LIMIT 1;

-- Legacy option cleanup: shorten any sentence-style correct option for this question.
UPDATE options
SET content = 'atomic'
WHERE question_id = (
    SELECT id FROM questions
    WHERE type = 'fill_blank'
      AND content LIKE '%lock-free style scalar synchronization%'
    LIMIT 1
)
AND is_correct = 1
AND length(content) > 20;
