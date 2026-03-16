-- Corrective patch: clamp out-of-range difficulty values to the valid domain [1..4].
-- Any quiz or question with difficulty > 4 is normalized to 4 (Expert).

UPDATE quizzes
SET difficulty = 4
WHERE difficulty > 4;

UPDATE questions
SET difficulty = 4
WHERE difficulty > 4;
