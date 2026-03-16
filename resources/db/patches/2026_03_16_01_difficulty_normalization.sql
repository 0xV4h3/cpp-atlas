-- Patch 01: Normalize difficulty values to 1-4 range across all entities.
-- This ensures data consistency before CHECK constraints are enforced in schema v4.

-- Normalize quizzes difficulty
UPDATE quizzes
SET difficulty = 4
WHERE difficulty > 4;

UPDATE quizzes
SET difficulty = 1
WHERE difficulty < 1;

-- Normalize questions difficulty
UPDATE questions
SET difficulty = 4
WHERE difficulty > 4;

UPDATE questions
SET difficulty = 1
WHERE difficulty < 1;

-- Normalize topics difficulty
UPDATE topics
SET difficulty = 4
WHERE difficulty > 4;

UPDATE topics
SET difficulty = 1
WHERE difficulty < 1;
