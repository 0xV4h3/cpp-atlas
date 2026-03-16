-- Corrective patch: fix fill_blank questions that have sentence-style
-- correct options instead of short keyword tokens.
-- The canonical answer for "What keyword exits the nearest loop immediately?"
-- is `break`, not a long explanation sentence.

-- Update the correct option for fill_blank question at order_index 1041
-- to use the short canonical token.
UPDATE options
SET content = 'break'
WHERE question_id = (
    SELECT id FROM questions
    WHERE order_index = 1041
      AND type = 'fill_blank'
    LIMIT 1
)
AND is_correct = 1;

-- Insert into fill_blank_answers as explicit answer key (preferred path)
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index)
SELECT id, 'break', 0
FROM questions
WHERE order_index = 1041
  AND type = 'fill_blank'
LIMIT 1;
