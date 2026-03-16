-- Patch 02: Backfill fill_blank_answers from legacy correct options.
-- Migrates all fill_blank questions that have a correct option entry but no
-- explicit fill_blank_answers row, providing the standard fallback path while
-- transitioning to the explicit answer-key model.
--
-- This patch is idempotent: INSERT OR IGNORE prevents duplicate rows.

-- Safety guard (handles apply before schema patch in edge cases)
CREATE TABLE IF NOT EXISTS fill_blank_answers (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
    answer      TEXT    NOT NULL,
    is_active   INTEGER DEFAULT 1,
    order_index INTEGER DEFAULT 0
);
CREATE INDEX IF NOT EXISTS idx_fill_blank_answers_qid
    ON fill_blank_answers(question_id);

-- Migrate: for every fill_blank question that has is_correct=1 option(s)
-- and no existing fill_blank_answers row, insert from options.
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT
    o.question_id,
    o.content,
    ROW_NUMBER() OVER (PARTITION BY o.question_id ORDER BY o.order_index) - 1
FROM options o
JOIN questions q ON q.id = o.question_id
WHERE q.type     = 'fill_blank'
  AND o.is_correct = 1
  AND NOT EXISTS (
      SELECT 1 FROM fill_blank_answers f
      WHERE f.question_id = o.question_id
        AND lower(trim(f.answer)) = lower(trim(o.content))
  );
