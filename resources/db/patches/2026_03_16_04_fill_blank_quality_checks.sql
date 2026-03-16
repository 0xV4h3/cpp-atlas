-- Patch 04: fill_blank answer quality cleanup.
-- Removes duplicate answer keys per question after case-insensitive normalization.
-- Retains the row with the lowest id (earliest inserted) among duplicates.
-- This patch is idempotent.

DELETE FROM fill_blank_answers
WHERE id NOT IN (
    SELECT MIN(id)
    FROM fill_blank_answers
    GROUP BY question_id, lower(trim(answer))
);
