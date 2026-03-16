-- =============================================================
-- CppAtlas Content Patch: Foundations Pack 01
-- Date: 2026-03-16
-- Topics: basics, variables, types, operators, control-flow
-- Sources:
--   https://www.learncpp.com/cpp-tutorial/comments/
--   https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/
--   https://www.learncpp.com/cpp-tutorial/keywords-and-naming-identifiers/
--   https://www.learncpp.com/cpp-tutorial/introduction-to-expressions/
--   https://www.learncpp.com/cpp-tutorial/arithmetic-operators/
--   https://www.learncpp.com/cpp-tutorial/increment-decrement-operators-and-side-effects/
--   https://www.learncpp.com/cpp-tutorial/the-conditional-operator/
--   https://www.learncpp.com/cpp-tutorial/logical-operators/
--   https://www.learncpp.com/cpp-tutorial/introduction-to-fundamental-data-types/
--   https://www.learncpp.com/cpp-tutorial/void/
--   https://www.learncpp.com/cpp-tutorial/signed-integers/
--   https://www.learncpp.com/cpp-tutorial/chars/
--   https://www.learncpp.com/cpp-tutorial/floating-point-numbers/
--   https://www.learncpp.com/cpp-tutorial/introduction-to-type-conversion-and-static_cast/
--   https://www.learncpp.com/cpp-tutorial/control-flow-introduction/
--   https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/
--   https://www.learncpp.com/cpp-tutorial/switch-statement-basics/
--   https://www.learncpp.com/cpp-tutorial/for-statements/
--   https://www.learncpp.com/cpp-tutorial/break-and-continue/
--   https://www.studyplan.dev/intro-to-programming/creating-variables
--   https://www.studyplan.dev/intro-to-programming/numbers
--   https://www.studyplan.dev/intro-to-programming/types-literals
--   https://www.studyplan.dev/pro-cpp/variables-and-types
-- =============================================================

-- -----------------------------------------------------------------
-- Ensure required tags exist
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO tags (name, color) VALUES
    ('basics', '#89D185'),
    ('variables', '#89D185'),
    ('types', '#4EC9B0'),
    ('operators', '#DCDCAA'),
    ('control-flow', '#569CD6'),
    ('best-practices', '#89D185');

-- -----------------------------------------------------------------
-- Create NEW quizzes with system-style naming (no pack prefix)
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version)
VALUES
    ('C++ Fundamentals Drill', 'Identifiers, expressions, literals, comments, and basic I/O fundamentals', 1, 1, 'standard', 1, 0, 1),
    ('Control Flow Essentials', 'Practical if/switch/loops and flow-control reasoning tasks', 4, 2, 'standard', 1, 0, 1);

-- -----------------------------------------------------------------
-- 50 NEW questions
-- Mix of:
--  - existing quizzes: 1..4 (Basics / Variables&Types / Operators / Control Flow)
--  - newly created quizzes: C++ Fundamentals Drill / Control Flow Essentials
-- Types: mcq, true_false, code_output, multi_select, fill_blank
-- order_index range in this patch: 101..150
-- -----------------------------------------------------------------

-- Q101
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=1), 1, 'mcq',
 'Which statement best describes what a C++ expression is?',
 NULL,
 'An expression is a combination of literals/variables/operators/functions that can be evaluated to produce a value.',
 1, 10, 101, 1,
 'Think “produces a value”.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-expressions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=101), 'A construct that evaluates to a value', 1, 1),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=101), 'A preprocessor command', 0, 2),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=101), 'A linker-only directive', 0, 3),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=101), 'A file include path', 0, 4);

-- Q102
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=1), 1, 'true_false',
 'Single-line comments in C++ begin with //.',
 'Correct. C++ single-line comments use // and continue until end-of-line.',
 1, 10, 102, 1,
 'Recall basic syntax.',
 'https://www.learncpp.com/cpp-tutorial/comments/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=102), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=102), 'False', 0, 2);

-- Q103
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=1), 1, 'mcq',
 'What is printed by this code?',
 'std::cout << "A" << "B";',
 'Stream insertion writes both string literals sequentially to stdout.',
 1, 10, 103, 1,
 'No implicit spaces are inserted.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=103), 'AB', 1, 1),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=103), 'A B', 0, 2),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=103), 'A\nB', 0, 3),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=103), 'Compile error', 0, 4);

-- Q104
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'mcq',
 'Which is a valid identifier name in C++?',
 'Identifiers cannot start with a digit and cannot be reserved keywords.',
 1, 10, 104, 1,
 'Starts with letter or underscore.',
 'https://www.learncpp.com/cpp-tutorial/keywords-and-naming-identifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=104), 'studentCount', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=104), '2students', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=104), 'class', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=104), 'int', 0, 4);

-- Q105
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'code_output',
 'What is the output?',
 'int x{3};\nstd::cout << x + 4;',
 'x+4 evaluates to 7.',
 1, 10, 105, 1,
 'Simple integer arithmetic.',
 'https://www.studyplan.dev/intro-to-programming/numbers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=105), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=105), '34', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=105), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=105), 'Compile error', 0, 4);

-- Q106
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'multi_select',
 'Select all valid C++ fundamental integer types.',
 'short, int, long, and long long are standard signed integer types (and unsigned variants exist).',
 2, 15, 106, 1,
 'Think core fundamental integers.',
 'https://www.learncpp.com/cpp-tutorial/signed-integers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=106), 'int', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=106), 'short', 1, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=106), 'decimal', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=106), 'long long', 1, 4);

-- Q107
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'true_false',
 'The C++ standard guarantees that int is exactly 32 bits.',
 'False. int is guaranteed to be at least 16 bits.',
 2, 10, 107, 1,
 '“At least” vs “exactly”.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-fundamental-data-types/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=107), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=107), 'False', 1, 2);

-- Q108
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'mcq',
 'What is the value of c?',
 'char c = ''A'';',
 'Character literals use single quotes. c stores the character A.',
 1, 10, 108, 1,
 'Single quotes denote char literal.',
 'https://www.learncpp.com/cpp-tutorial/chars/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=108), 'Character A', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=108), 'String "A"', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=108), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=108), 'ASCII code only, not char', 0, 4);

-- Q109
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'fill_blank',
 'The keyword used for explicit compile-time checked conversion is ____.',
 'Use static_cast for explicit, checked-at-compile conversion syntax.',
 2, 10, 109, 1,
 'Syntax: static_cast<T>(value).',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-type-conversion-and-static_cast/');

-- Q110
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'code_output',
 'What is the output?',
 'int x{10};\nx += 5;\nstd::cout << x;',
 'Compound assignment adds and stores the result.',
 1, 10, 110, 1,
 'x becomes x+5.',
 'https://www.learncpp.com/cpp-tutorial/arithmetic-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=110), '15', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=110), '10', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=110), '5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=110), '105', 0, 4);

-- Q111
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'mcq',
 'What is the value of 17 % 5?',
 NULL,
 'Modulo returns remainder of integer division.',
 1, 10, 111, 1,
 '17 = 3*5 + r',
 'https://www.learncpp.com/cpp-tutorial/arithmetic-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=111), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=111), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=111), '5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=111), '1', 0, 4);

-- Q112
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'true_false',
 'The expression ++x increments x before the value is used.',
 'Prefix increment increments then yields incremented value.',
 2, 10, 112, 1,
 'Prefix vs postfix.',
 'https://www.learncpp.com/cpp-tutorial/increment-decrement-operators-and-side-effects/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=112), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=112), 'False', 0, 2);

-- Q113
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'mcq',
 'What does the conditional operator return here?',
 'int a=3,b=9;\nint m = (a>b) ? a : b;',
 'Since a>b is false, expression yields b.',
 2, 10, 113, 1,
 'false branch selected.',
 'https://www.learncpp.com/cpp-tutorial/the-conditional-operator/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=113), '9', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=113), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=113), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=113), 'Compile error', 0, 4);

-- Q114
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'multi_select',
 'Select all logical operators in C++.',
 'The core logical operators are &&, ||, and !.',
 1, 15, 114, 1,
 'Three common logical operators.',
 'https://www.learncpp.com/cpp-tutorial/logical-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=114), '&&', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=114), '||', 1, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=114), '::', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=114), '!', 1, 4);

-- Q115
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'mcq',
 'Which statement about if blocks is correct?',
 'A block is a sequence of statements enclosed in braces; if controls execution based on condition.',
 1, 10, 115, 1,
 'Braces form a block.',
 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=115), 'if executes block only when condition is true', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=115), 'if always executes both branches', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=115), 'if cannot use boolean expressions', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=115), 'if is only for integer constants', 0, 4);

-- Q116
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'code_output',
 'What is printed?',
 'int x{2};\nif (x==2) std::cout << "yes"; else std::cout << "no";',
 'Condition is true, so yes is printed.',
 1, 10, 116, 1,
 'x equals 2.',
 'https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=116), 'yes', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=116), 'no', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=116), 'yesno', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=116), 'Compile error', 0, 4);

-- Q117
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'true_false',
 'switch case labels must be compile-time constants.',
 'Correct. Case labels are integral constant expressions.',
 2, 10, 117, 1,
 'Runtime variable case labels are not allowed.',
 'https://www.learncpp.com/cpp-tutorial/switch-statement-basics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=117), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=117), 'False', 0, 2);

-- Q118
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'fill_blank',
 'The statement used to exit the nearest loop immediately is ____.',
 'break exits the innermost loop or switch.',
 1, 10, 118, 1,
 'Not continue.',
 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');

-- Q119
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'mcq',
 'How many iterations does this loop run?',
 'for(int i=0;i<4;++i){}',
 'i takes 0,1,2,3 -> 4 iterations.',
 1, 10, 119, 1,
 'Upper bound is exclusive.',
 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=119), '4', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=119), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=119), '5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=119), '0', 0, 4);

-- Q120
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 1, 'mcq',
 'Which option is a keyword in C++?',
 'Keywords are reserved and cannot be used as identifiers.',
 1, 10, 120, 1,
 'Think language-reserved words.',
 'https://www.learncpp.com/cpp-tutorial/keywords-and-naming-identifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=120), 'while', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=120), 'countItems', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=120), 'playerScore', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=120), '_value2', 0, 4);

-- Q121
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 2, 'code_output',
 'What is printed?',
 'int apples{5};\nint oranges{7};\nstd::cout << apples + oranges;',
 'Simple integer addition.',
 1, 10, 121, 1,
 '5+7',
 'https://www.studyplan.dev/intro-to-programming/creating-variables');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=121), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=121), '57', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=121), '2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=121), 'Compile error', 0, 4);

-- Q122
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 3, 'mcq',
 'Which literal is a floating-point literal?',
 'A literal with decimal point (or exponent) is floating-point by default (double).',
 1, 10, 122, 1,
 'Contains decimal point.',
 'https://www.studyplan.dev/intro-to-programming/types-literals');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=122), '3.14', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=122), '42', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=122), '''x''', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=122), '"hello"', 0, 4);

-- Q123
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 3, 'true_false',
 'void means “no value” when used as a function return type.',
 'Correct. A void function does not return a value.',
 1, 10, 123, 1,
 'Void return type.',
 'https://www.learncpp.com/cpp-tutorial/void/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=123), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=123), 'False', 0, 2);

-- Q124
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 4, 'multi_select',
 'Select all arithmetic operators.',
 'Arithmetic operators include + - * / % for integers (modulo).',
 1, 15, 124, 1,
 'Basic math operators.',
 'https://www.learncpp.com/cpp-tutorial/arithmetic-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=124), '+', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=124), '*', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=124), '&&', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=124), '/', 1, 4);

-- Q125
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?',
 'int x{1};\nif (x) std::cout << "T"; else std::cout << "F";',
 'Non-zero integer in condition converts to true.',
 2, 10, 125, 1,
 '0=false, non-zero=true.',
 'https://www.learncpp.com/cpp-tutorial/control-flow-introduction/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=125), 'T', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=125), 'F', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=125), 'TF', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=125), 'Compile error', 0, 4);

-- Q126
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'fill_blank',
 'The operator that computes remainder is ____.',
 'Use modulo operator % for integer remainder.',
 1, 10, 126, 1,
 'Arithmetic remainder operator.',
 'https://www.learncpp.com/cpp-tutorial/arithmetic-operators/');

-- Q127
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'mcq',
 'Which declaration uses list-initialization?',
 'Brace initialization uses {}.',
 1, 10, 127, 1,
 'Curly braces.',
 'https://www.studyplan.dev/pro-cpp/variables-and-types');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=127), 'int x{42};', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=127), 'int x = ;', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=127), 'int x( ;', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=127), 'x int = 42;', 0, 4);

-- Q128
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'code_output',
 'What is the output?',
 'bool a{true}; bool b{false};\nstd::cout << (a && b);',
 'true && false is false; ostream prints 0/1 by default.',
 2, 10, 128, 1,
 'Logical AND needs both true.',
 'https://www.learncpp.com/cpp-tutorial/logical-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=128), '0', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=128), '1', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=128), 'false (text)', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=128), 'Compile error', 0, 4);

-- Q129
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'mcq',
 'What does continue do inside a loop?',
 'continue skips the remainder of current iteration and proceeds with next iteration.',
 2, 10, 129, 1,
 'Not full loop exit.',
 'https://www.learncpp.com/cpp-tutorial/break-and-continue/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=129), 'Skips to next iteration', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=129), 'Exits loop entirely', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=129), 'Restarts program', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=129), 'Breaks out of function', 0, 4);

-- Q130
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=1), 1, 'multi_select',
 'Select all good identifier naming practices.',
 'Meaningful names and consistency improve readability.',
 2, 15, 130, 1,
 'Think readability and intent.',
 'https://www.learncpp.com/cpp-tutorial/keywords-and-naming-identifiers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=130), 'Use meaningful names', 1, 1),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=130), 'Use random one-letter names everywhere', 0, 2),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=130), 'Keep style consistent', 1, 3),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=130), 'Use reserved keywords as variables', 0, 4);

-- Q131
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'mcq',
 'Which type is commonly used for decimal values?',
 'double is the usual default floating-point type.',
 1, 10, 131, 1,
 'Floating-point.',
 'https://www.learncpp.com/cpp-tutorial/floating-point-numbers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=131), 'double', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=131), 'int', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=131), 'char', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=131), 'void', 0, 4);

-- Q132
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'code_output',
 'What is printed?',
 'double x{2.5};\nstd::cout << static_cast<int>(x);',
 'static_cast<int> truncates fractional part.',
 2, 10, 132, 1,
 'Fraction is discarded.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-type-conversion-and-static_cast/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=132), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=132), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=132), '2.5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=132), 'Compile error', 0, 4);

-- Q133
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'true_false',
 'The operator precedence of * is higher than +.',
 'Correct. Multiplication binds tighter than addition.',
 2, 10, 133, 1,
 'Arithmetic precedence.',
 'https://www.learncpp.com/cpp-tutorial/arithmetic-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=133), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=133), 'False', 0, 2);

-- Q134
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'mcq',
 'Which operator is logical NOT?',
 'Logical NOT is unary !.',
 1, 10, 134, 1,
 'Unary logical inversion.',
 'https://www.learncpp.com/cpp-tutorial/logical-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=134), '!', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=134), '~', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=134), '!=', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=134), '&', 0, 4);

-- Q135
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'code_output',
 'What is printed?',
 'for (int i=0;i<3;++i) std::cout << i;',
 'Loop prints 0 then 1 then 2.',
 1, 10, 135, 1,
 'Starts at 0, stops before 3.',
 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=135), '012', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=135), '123', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=135), '01', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=135), 'Compile error', 0, 4);

-- Q136
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'multi_select',
 'Select all loop constructs in C++.',
 'C++ has for, while, and do-while loops.',
 1, 15, 136, 1,
 'Core loop trio.',
 'https://www.learncpp.com/cpp-tutorial/control-flow-introduction/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=136), 'for', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=136), 'while', 1, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=136), 'do-while', 1, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=136), 'foreach keyword', 0, 4);

-- Q137
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'What happens if a switch case has no break and another case follows?',
 'Execution may fall through into next case.',
 2, 10, 137, 1,
 'Classic switch pitfall.',
 'https://www.learncpp.com/cpp-tutorial/switch-statement-basics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=137), 'Control may continue into next case', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=137), 'Program always crashes', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=137), 'switch becomes invalid syntax', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=137), 'Loop exits', 0, 4);

-- Q138
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'fill_blank',
 'A named storage location in a program is called a ____.',
 'That is the basic definition of a variable.',
 1, 10, 138, 1,
 'Core concept in programming.',
 'https://www.studyplan.dev/intro-to-programming/creating-variables');

-- Q139
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'mcq',
 'Which of these is a character literal?',
 'Character literals use single quotes.',
 1, 10, 139, 1,
 'Single quotes.',
 'https://www.learncpp.com/cpp-tutorial/chars/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=139), '''Z''', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=139), '"Z"', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=139), 'Z', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=139), 'string(Z)', 0, 4);

-- Q140
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'code_output',
 'What is printed?',
 'int x{1};\nstd::cout << x++ << x;',
 'Postfix yields old value then increments: prints 1 then 2.',
 3, 12, 140, 1,
 'Postfix side effect after value use.',
 'https://www.learncpp.com/cpp-tutorial/increment-decrement-operators-and-side-effects/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=140), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=140), '11', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=140), '22', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=140), '21', 0, 4);

-- Q141
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'mcq',
 'Which expression uses the ternary conditional operator?',
 'Ternary form is condition ? expr1 : expr2.',
 1, 10, 141, 1,
 'Look for ? and : pair.',
 'https://www.learncpp.com/cpp-tutorial/the-conditional-operator/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=141), '(a>b) ? a : b', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=141), 'a && b', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=141), 'a += b', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=141), 'if (a>b) a', 0, 4);

-- Q142
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'true_false',
 'A do-while loop checks its condition after executing the loop body.',
 'Correct; therefore body executes at least once.',
 1, 10, 142, 1,
 'Post-condition loop.',
 'https://www.learncpp.com/cpp-tutorial/control-flow-introduction/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=142), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=142), 'False', 0, 2);

-- Q143
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1), 1, 'mcq',
 'Why are comments useful in educational code?',
 'Comments communicate intent and improve maintainability when used clearly.',
 1, 10, 143, 1,
 'Think readability.',
 'https://www.learncpp.com/cpp-tutorial/comments/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=143), 'They explain intent for readers', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=143), 'They speed up runtime performance directly', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=143), 'They replace variable names', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1) AND order_index=143), 'They are required for compilation', 0, 4);

-- Q144
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'code_output',
 'What is printed?',
 'char c{''A''};\nstd::cout << c;',
 'Output stream prints character A.',
 1, 10, 144, 1,
 'char streamed as character.',
 'https://www.learncpp.com/cpp-tutorial/chars/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=144), 'A', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=144), '65', 0, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=144), 'a', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=144), 'Compile error', 0, 4);

-- Q145
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 3, 'multi_select',
 'Select all floating-point related standard types.',
 'float, double, and long double are floating-point types.',
 2, 15, 145, 1,
 'Three FP types.',
 'https://www.learncpp.com/cpp-tutorial/floating-point-numbers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=145), 'float', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=145), 'double', 1, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=145), 'long double', 1, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=145), 'int16', 0, 4);

-- Q146
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=3), 4, 'mcq',
 'What is the result type of logical operators like && and ||?',
 'Logical operators yield bool.',
 2, 10, 146, 1,
 'Boolean context.',
 'https://www.learncpp.com/cpp-tutorial/logical-operators/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=146), 'bool', 1, 1),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=146), 'int', 0, 2),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=146), 'char', 0, 3),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=146), 'void', 0, 4);

-- Q147
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=4), 5, 'mcq',
 'Which statement about switch is true?',
 'switch is often convenient for multiple discrete integral cases.',
 2, 10, 147, 1,
 'Discrete branches.',
 'https://www.learncpp.com/cpp-tutorial/switch-statement-basics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=147), 'switch can branch on integral expressions', 1, 1),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=147), 'switch works only with floating-point values', 0, 2),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=147), 'switch cannot have default', 0, 3),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=147), 'switch always requires braces inside each case', 0, 4);

-- Q148
INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?',
 'int sum{0};\nfor(int i=1;i<=3;++i) sum+=i;\nstd::cout << sum;',
 'Sum of 1+2+3 is 6.',
 2, 10, 148, 1,
 'Arithmetic progression.',
 'https://www.learncpp.com/cpp-tutorial/for-statements/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=148), '6', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=148), '5', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=148), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1) AND order_index=148), 'Compile error', 0, 4);

-- Q149
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=1), 1, 'fill_blank',
 'The stream used for standard output in C++ is ____.',
 'std::cout is the standard output stream.',
 1, 10, 149, 1,
 'Namespace + stream name.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-iostream-cout-cin-and-endl/');

-- Q150
INSERT INTO questions
(quiz_id, topic_id, type, content, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE id=2), 2, 'multi_select',
 'Select all good practices when introducing variables.',
 'Initialize variables and use clear names to reduce bugs.',
 2, 15, 150, 1,
 'Think maintainability.',
 'https://www.studyplan.dev/intro-to-programming/creating-variables');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=150), 'Initialize variables before use', 1, 1),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=150), 'Use descriptive names', 1, 2),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=150), 'Rely on uninitialized values for speed', 0, 3),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=150), 'Keep scope as narrow as practical', 1, 4);

-- -----------------------------------------------------------------
-- question_tags for all new questions in this patch
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN quizzes z ON z.id = q.quiz_id
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 101 AND 103 THEN 'basics'
    WHEN q.order_index BETWEEN 104 AND 109 THEN 'types'
    WHEN q.order_index BETWEEN 110 AND 114 THEN 'operators'
    WHEN q.order_index BETWEEN 115 AND 119 THEN 'control-flow'
    WHEN q.order_index = 120 THEN 'basics'
    WHEN q.order_index IN (121,127,138,150) THEN 'variables'
    WHEN q.order_index IN (122,123,131,132,139,144,145) THEN 'types'
    WHEN q.order_index IN (124,126,128,133,134,140,141,146) THEN 'operators'
    WHEN q.order_index IN (125,129,135,136,137,142,147,148) THEN 'control-flow'
    WHEN q.order_index IN (130,143,149) THEN 'best-practices'
    ELSE 'basics'
END
WHERE q.order_index BETWEEN 101 AND 150
  AND (
      z.id IN (1,2,3,4)
      OR z.title IN ('C++ Fundamentals Drill','Control Flow Essentials')
  );

-- additional secondary tags for selected questions
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=104), (SELECT id FROM tags WHERE name='variables')),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=107), (SELECT id FROM tags WHERE name='variables')),
((SELECT id FROM questions WHERE quiz_id=3 AND order_index=114), (SELECT id FROM tags WHERE name='control-flow')),
((SELECT id FROM questions WHERE quiz_id=4 AND order_index=136), (SELECT id FROM tags WHERE name='operators')),
((SELECT id FROM questions WHERE quiz_id=1 AND order_index=130), (SELECT id FROM tags WHERE name='variables')),
((SELECT id FROM questions WHERE quiz_id=2 AND order_index=150), (SELECT id FROM tags WHERE name='best-practices'));

-- -----------------------------------------------------------------
-- derive quiz_tags for affected quizzes
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.quiz_id IN (
    1,2,3,4,
    (SELECT id FROM quizzes WHERE title='C++ Fundamentals Drill' ORDER BY id DESC LIMIT 1),
    (SELECT id FROM quizzes WHERE title='Control Flow Essentials' ORDER BY id DESC LIMIT 1)
);