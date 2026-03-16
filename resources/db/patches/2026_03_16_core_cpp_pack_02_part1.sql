-- =============================================================
-- CppAtlas Content Patch: Core C++ Pack 02
-- Date: 2026-03-16
-- Topics: functions, arrays, pointers, references, strings
--
-- IMPORTANT:
-- - No BEGIN/COMMIT here (transaction is managed by ContentPatchService).
-- - SQLite-safe quoting only (apostrophes escaped as '').
-- - Mix of existing and NEW quizzes, max ~10 questions per quiz.
-- - Question types: mcq, multi_select, true_false, code_output, fill_blank
--
-- Sources:
-- https://www.learncpp.com/cpp-tutorial/introduction-to-function-parameters-and-arguments/
-- https://www.learncpp.com/cpp-tutorial/why-functions-are-useful-and-how-to-use-them-effectively/
-- https://www.studyplan.dev/intro-to-programming/functions
-- https://www.studyplan.dev/intro-to-programming/debugging-functions
-- https://www.studyplan.dev/intro-to-programming/return-values
-- https://www.studyplan.dev/intro-to-programming/arguments
-- https://cppreference.com/w/cpp/language/functions.html
-- https://www.learncpp.com/cpp-tutorial/recursion/
-- https://www.learncpp.com/cpp-tutorial/function-pointers/
-- https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/
-- https://www.studyplan.dev/pro-cpp/c-style-arrays
-- https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/
-- https://www.learncpp.com/cpp-tutorial/return-by-reference-and-return-by-address/
-- https://www.learncpp.com/cpp-tutorial/pass-by-address/
-- https://www.learncpp.com/cpp-tutorial/null-pointers/
-- https://www.learncpp.com/cpp-tutorial/pointers-and-const/
-- https://www.learncpp.com/cpp-tutorial/type-deduction-with-pointers-references-and-const/
-- https://www.learncpp.com/cpp-tutorial/pass-by-address-part-2/
-- https://www.learncpp.com/cpp-tutorial/value-categories-lvalues-and-rvalues/
-- https://www.studyplan.dev/intro-to-programming/references
-- https://www.studyplan.dev/intro-to-programming/pointers
-- https://www.studyplan.dev/intro-to-programming/this
-- https://www.studyplan.dev/intro-to-programming/dangling-pointers
-- https://www.studyplan.dev/pro-cpp/c-strings
-- https://www.studyplan.dev/pro-cpp/std-string
-- https://www.studyplan.dev/pro-cpp/manipulating-std-strings
-- https://www.learncpp.com/cpp-tutorial/c-style-array-decay/
-- https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/
-- https://www.learncpp.com/cpp-tutorial/c-style-strings/
-- =============================================================

-- -----------------------------------------------------------------
-- Ensure required tags exist
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO tags (name, color) VALUES
    ('functions', '#DCDCAA'),
    ('arrays', '#CE9178'),
    ('pointers', '#F14C4C'),
    ('references', '#F14C4C'),
    ('strings', '#CE9178'),
    ('best-practices', '#89D185');

-- -----------------------------------------------------------------
-- Create NEW quizzes (system-style names)
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO quizzes
(title, description, topic_id, difficulty, type, is_active, created_by, version)
VALUES
('Function Design & Calls Quiz',
 'Function signatures, parameters, returns, recursion, and function pointers',
 5, 2, 'standard', 1, 0, 1),
('Pointers & References Deep Dive Quiz',
 'Address semantics, null checks, const correctness, and reference behavior',
 7, 3, 'standard', 1, 0, 1),
('Arrays & Strings Practice Quiz',
 'C-style arrays, decay, C-strings, std::string operations',
 6, 2, 'standard', 1, 0, 1);

-- =============================================================
-- Existing Quiz 5: Functions Quiz (8 questions)
-- order_index: 201..208
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'mcq',
 'What is a function parameter in C++?',
 NULL,
 'A parameter is a named variable in a function declaration/definition that receives an argument value.',
 1, 10, 201, 1,
 'Defined in function signature.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-function-parameters-and-arguments/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=201), 'A variable declared in a function signature', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=201), 'A variable declared only in main()', 0, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=201), 'A compile flag', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=201), 'A namespace alias', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'true_false',
 'Arguments are the values passed at the call site to a function.',
 NULL,
 'True. Parameters belong to the function declaration/definition; arguments are supplied by the caller.',
 1, 10, 202, 1,
 'Call-site values.',
 'https://www.studyplan.dev/intro-to-programming/arguments');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=202), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=202), 'False', 0, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'code_output',
 'What is printed?',
 'int add(int a, int b){ return a+b; }\nstd::cout << add(4,3);',
 'The function returns 7.',
 1, 10, 203, 1,
 'Simple return value.',
 'https://www.studyplan.dev/intro-to-programming/return-values');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=203), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=203), '43', 0, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=203), '1', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=203), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'mcq',
 'Why are functions useful?',
 NULL,
 'Functions reduce duplication, improve readability, and support decomposition of logic.',
 1, 10, 204, 1,
 'Think maintainability.',
 'https://www.learncpp.com/cpp-tutorial/why-functions-are-useful-and-how-to-use-them-effectively/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=204), 'They help organize and reuse logic', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=204), 'They always make code faster', 0, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=204), 'They replace all loops', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=204), 'They disable debugging', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'multi_select',
 'Select all true statements about recursion.',
 NULL,
 'A recursive function calls itself (directly/indirectly) and must have a terminating base case.',
 2, 15, 205, 1,
 'Base case is essential.',
 'https://www.learncpp.com/cpp-tutorial/recursion/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=205), 'A base case prevents infinite recursion', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=205), 'Recursion requires at least one recursive path', 1, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=205), 'Recursion cannot use parameters', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=205), 'Recursive calls use additional stack frames', 1, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'fill_blank',
 'Complete: A function that returns no value has return type ____.',
 NULL,
 'Use void for functions that do not return a value.',
 1, 10, 206, 1,
 'No-value return type.',
 'https://cppreference.com/w/cpp/language/functions.html');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'mcq',
 'What is a function pointer?',
 NULL,
 'A function pointer stores the address of a function with a matching signature.',
 3, 12, 207, 1,
 'Pointer to callable function symbol.',
 'https://www.learncpp.com/cpp-tutorial/function-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=207), 'A pointer that can store address of a function', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=207), 'A function returning a pointer only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=207), 'A pointer that stores only integers', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=207), 'A reference alias', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(5, 5, 'code_output',
 'What is printed?',
 'int f(int x){ return x*x; }\nstd::cout << f(5);',
 'f(5) returns 25.',
 1, 10, 208, 1,
 'Square function.',
 'https://www.studyplan.dev/intro-to-programming/functions');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=208), '25', 1, 1),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=208), '10', 0, 2),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=208), '5', 0, 3),
((SELECT id FROM questions WHERE quiz_id=5 AND order_index=208), 'Compile error', 0, 4);

-- =============================================================
-- Existing Quiz 6: Arrays & Strings Quiz (8 questions)
-- order_index: 209..216
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 6, 'mcq',
 'In a C-style array of size 6, what is the last valid index?',
 NULL,
 'C-style arrays are zero-indexed, so last index is size-1.',
 1, 10, 209, 1,
 'Zero-based indexing.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=209), '5', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=209), '6', 0, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=209), '4', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=209), '0', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 10, 'true_false',
 'std::string::size() returns the number of characters in the string.',
 NULL,
 'True. size() and length() are equivalent for std::string.',
 1, 10, 210, 1,
 'size() == length()',
 'https://www.studyplan.dev/pro-cpp/std-string');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=210), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=210), 'False', 0, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 10, 'code_output',
 'What is printed?',
 'std::string s{"Hi"};\ns += "!";\nstd::cout << s;',
 'Appending ! gives Hi!.',
 1, 10, 211, 1,
 'operator+= on std::string.',
 'https://www.studyplan.dev/pro-cpp/manipulating-std-strings');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=211), 'Hi!', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=211), 'Hi', 0, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=211), 'H i !', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=211), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 7, 'mcq',
 'What is array decay?',
 NULL,
 'When passed to a function, a C-style array often decays to pointer to first element.',
 2, 10, 212, 1,
 'Common in function arguments.',
 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=212), 'Conversion of array to pointer in many contexts', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=212), 'Automatic sorting of array', 0, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=212), 'Memory leak in arrays', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=212), 'Array resizing at runtime', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 10, 'multi_select',
 'Select all true statements about C-style strings.',
 NULL,
 'C-style strings are char arrays terminated by a null character.',
 2, 15, 213, 1,
 'Null terminator matters.',
 'https://www.learncpp.com/cpp-tutorial/c-style-strings/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=213), 'They are arrays of char ending with ''\0''', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=213), 'Missing null terminator can cause issues', 1, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=213), 'They auto-track length like std::string', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=213), 'String literals are null-terminated', 1, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 7, 'fill_blank',
 'For int a[3][4], the number of columns is ____.',
 NULL,
 'In a[rows][cols], second dimension is columns: 4.',
 2, 10, 214, 1,
 'a[3][4] means 3 rows, 4 cols.',
 'https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 10, 'mcq',
 'Which method checks whether std::string is empty?',
 NULL,
 'Use empty() to test whether size is zero.',
 1, 10, 215, 1,
 'Boolean emptiness check.',
 'https://www.studyplan.dev/pro-cpp/std-string');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=215), 'empty()', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=215), 'clear()', 0, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=215), 'erase()', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=215), 'length(0)', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(6, 6, 'code_output',
 'What is printed?',
 'int a[3]{1,2,3};\nstd::cout << a[1];',
 'a[1] is the second element: 2.',
 1, 10, 216, 1,
 'Zero-based index.',
 'https://www.studyplan.dev/pro-cpp/c-style-arrays');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=216), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=216), '1', 0, 2),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=216), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=6 AND order_index=216), 'Compile error', 0, 4);

-- =============================================================
-- Existing Quiz 7: Pointers & References Quiz (8 questions)
-- order_index: 217..224
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'mcq',
 'What does the & operator produce when used as unary address-of?',
 NULL,
 'It yields the memory address of its operand.',
 2, 10, 217, 1,
 'Address-of operator.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=217), 'Address of the variable', 1, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=217), 'Value at pointer target', 0, 2),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=217), 'Bitwise AND always', 0, 3),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=217), 'A reference declaration only', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'true_false',
 'A null pointer points to a valid object.',
 NULL,
 'False. nullptr indicates pointer points to no object.',
 1, 10, 218, 1,
 'Null means no target object.',
 'https://www.learncpp.com/cpp-tutorial/null-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=218), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=218), 'False', 1, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'code_output',
 'What is printed?',
 'int x{42};\nint* p{&x};\nstd::cout << *p;',
 'Dereferencing p reads x, so output is 42.',
 2, 10, 219, 1,
 'Pointer dereference.',
 'https://www.studyplan.dev/intro-to-programming/pointers');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=219), '42', 1, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=219), 'Address text', 0, 2),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=219), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=219), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 9, 'mcq',
 'What is a reference in C++?',
 NULL,
 'A reference is an alias to an existing object.',
 2, 10, 220, 1,
 'Alias, not rebindable object.',
 'https://www.studyplan.dev/intro-to-programming/references');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=220), 'An alias to an existing object', 1, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=220), 'A nullable pointer', 0, 2),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=220), 'A new independent object copy', 0, 3),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=220), 'A preprocessor macro', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 9, 'true_false',
 'A reference can be reseated to refer to another object after initialization.',
 NULL,
 'False. References cannot be reseated after binding.',
 2, 10, 221, 1,
 'Unlike pointers.',
 'https://www.studyplan.dev/intro-to-programming/references');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=221), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=221), 'False', 1, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'multi_select',
 'Select all valid pointer constness forms.',
 NULL,
 'int* const, const int*, and const int* const are all valid forms.',
 3, 15, 222, 1,
 'Read right-to-left.',
 'https://www.learncpp.com/cpp-tutorial/pointers-and-const/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=222), 'int* const p', 1, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=222), 'const int* p', 1, 2),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=222), 'const int* const p', 1, 3),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=222), 'int&* p', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'fill_blank',
 'Complete: A pointer that points nowhere should be initialized with ____.',
 NULL,
 'Use nullptr in modern C++.',
 1, 10, 223, 1,
 'Modern null pointer literal.',
 'https://www.learncpp.com/cpp-tutorial/null-pointers/');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
(7, 8, 'mcq',
 'What is a dangling pointer?',
 NULL,
 'A pointer that refers to memory that is no longer valid.',
 3, 12, 224, 1,
 'Lifetime mismatch issue.',
 'https://www.studyplan.dev/intro-to-programming/dangling-pointers');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=224), 'Pointer to invalid/deallocated object', 1, 1),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=224), 'Pointer set to nullptr', 0, 2),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=224), 'Pointer to const object', 0, 3),
((SELECT id FROM questions WHERE quiz_id=7 AND order_index=224), 'Pointer in global scope', 0, 4);

-- =============================================================
-- New Quiz: Function Design & Calls Quiz (8 questions)
-- order_index: 225..232
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'mcq',
 'Which is generally better for a reusable utility function?',
 NULL,
 'A function with clear name, parameters, and single responsibility is typically more reusable.',
 2, 10, 225, 1,
 'Single responsibility.',
 'https://www.learncpp.com/cpp-tutorial/why-functions-are-useful-and-how-to-use-them-effectively/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=225), 'Small focused function with explicit parameters', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=225), 'One giant function for entire program', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=225), 'Function with hidden global side-effects only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=225), 'Unreachable function', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'code_output',
 'What is printed?',
 'int twice(int x){ return x*2; }\nstd::cout << twice(6);',
 'twice(6) is 12.',
 1, 10, 226, 1,
 'Multiply by two.',
 'https://www.studyplan.dev/intro-to-programming/functions');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=226), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=226), '6', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=226), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=226), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'true_false',
 'A function can return by reference, but you must ensure the referred object outlives the reference.',
 NULL,
 'True. Returning reference to local variable is invalid (dangling reference).',
 3, 12, 227, 1,
 'Lifetime safety.',
 'https://www.learncpp.com/cpp-tutorial/return-by-reference-and-return-by-address/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=227), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=227), 'False', 0, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'mcq',
 'Which return pattern is unsafe?',
 NULL,
 'Returning pointer/reference to local automatic object is unsafe.',
 3, 12, 228, 1,
 'Local lifetime ends at function return.',
 'https://www.learncpp.com/cpp-tutorial/return-by-reference-and-return-by-address/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=228), 'Returning address of a local variable', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=228), 'Returning an int by value', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=228), 'Returning std::string by value', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=228), 'Returning parameter by value', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'multi_select',
 'Select valid reasons to pass arguments by pointer/reference.',
 NULL,
 'Passing by pointer/reference can allow mutation and avoid copying large objects.',
 2, 15, 229, 1,
 'Performance and mutability.',
 'https://www.learncpp.com/cpp-tutorial/pass-by-address/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=229), 'Allow function to modify caller object', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=229), 'Avoid expensive copies', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=229), 'Guarantee no null checks are needed for pointers', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=229), 'Pointers are always safer than references', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'fill_blank',
 'Complete: A function that calls itself is called ____.',
 NULL,
 'Such function is recursive.',
 1, 10, 230, 1,
 'Self-call concept.',
 'https://www.learncpp.com/cpp-tutorial/recursion/');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'mcq',
 'Which type can store a pointer to int f(double)?',
 NULL,
 'Function pointer type must match signature and return type.',
 4, 15, 231, 1,
 'Read function pointer syntax carefully.',
 'https://www.learncpp.com/cpp-tutorial/function-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=231), 'int (*p)(double)', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=231), 'double (*p)(int)', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=231), 'int* p(double)', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=231), 'int p* (double)', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1),
 5, 'true_false',
 'Debugging function behavior often starts by verifying inputs and return values.',
 NULL,
 'True. Instrumenting inputs/outputs and expectations is a practical debugging strategy.',
 2, 10, 232, 1,
 'Check boundary and expected values.',
 'https://www.studyplan.dev/intro-to-programming/debugging-functions');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=232), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Function Design & Calls Quiz' ORDER BY id DESC LIMIT 1) AND order_index=232), 'False', 0, 2);

-- =============================================================
-- New Quiz: Pointers & References Deep Dive Quiz (9 questions)
-- order_index: 233..241
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'mcq',
 'What is the safest default initialization for a pointer when no target exists yet?',
 NULL,
 'Initialize with nullptr to avoid indeterminate pointer value.',
 1, 10, 233, 1,
 'Never leave pointer uninitialized.',
 'https://www.learncpp.com/cpp-tutorial/null-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=233), 'nullptr', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=233), '0xDEADBEEF', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=233), 'Uninitialized local pointer', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=233), 'Any random address', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'multi_select',
 'Select true statements about pass-by-address.',
 NULL,
 'Caller passes pointer; callee may mutate object and should validate null if required by contract.',
 2, 15, 234, 1,
 'Pointer parameter semantics.',
 'https://www.learncpp.com/cpp-tutorial/pass-by-address/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=234), 'Function can modify object via pointer', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=234), 'Pointer argument can be nullptr', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=234), 'Pointer arguments cannot be reassigned inside function', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=234), 'Address passing is always faster for tiny POD values', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 9, 'code_output',
 'What is printed?',
 'int x{10};\nint& r{x};\nr=25;\nstd::cout << x;',
 'Reference aliases x; assigning through r updates x.',
 2, 10, 235, 1,
 'Alias semantics.',
 'https://www.studyplan.dev/intro-to-programming/references');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=235), '25', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=235), '10', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=235), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=235), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'true_false',
 'Dereferencing nullptr is undefined behavior.',
 NULL,
 'True. You must ensure pointer points to valid object before dereferencing.',
 2, 10, 236, 1,
 'Validate pointer before *p.',
 'https://www.learncpp.com/cpp-tutorial/null-pointers/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=236), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=236), 'False', 0, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'mcq',
 'What does const int* p mean?',
 NULL,
 'Pointer to const int: value pointed-to is read-only through p; p itself may change target.',
 3, 12, 237, 1,
 'Const applies to pointee here.',
 'https://www.learncpp.com/cpp-tutorial/pointers-and-const/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=237), 'Pointer to const int', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=237), 'Const pointer to int', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=237), 'Const pointer to const int only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=237), 'Reference to int', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'fill_blank',
 'Complete: int* const p means p is a ____ pointer.',
 NULL,
 'Const pointer: address in p cannot change after initialization.',
 3, 12, 238, 1,
 'Const binds to pointer object.',
 'https://www.learncpp.com/cpp-tutorial/pointers-and-const/');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 9, 'mcq',
 'Which expression category can bind to non-const lvalue reference int&?',
 NULL,
 'Non-const lvalue references bind to modifiable lvalues.',
 3, 12, 239, 1,
 'lvalue vs rvalue.',
 'https://www.learncpp.com/cpp-tutorial/value-categories-lvalues-and-rvalues/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=239), 'Named int variable', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=239), 'Literal 42', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=239), 'Temporary from x+y', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=239), 'nullptr', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 8, 'multi_select',
 'Select all common causes of dangling pointers.',
 NULL,
 'Dangling pointers arise when object lifetime ends but pointer still references old address.',
 3, 15, 240, 1,
 'Lifetime termination cases.',
 'https://www.studyplan.dev/intro-to-programming/dangling-pointers');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=240), 'Deleting heap object but keeping old pointer', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=240), 'Returning address of local variable', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=240), 'Setting pointer to nullptr after delete', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=240), 'Object goes out of scope while pointer remains', 1, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1),
 9, 'true_false',
 'auto& can deduce a reference type while plain auto generally deduces by value.',
 NULL,
 'True. auto strips references unless explicitly requested via auto& / auto&&.',
 3, 12, 241, 1,
 'Type deduction nuances.',
 'https://www.learncpp.com/cpp-tutorial/type-deduction-with-pointers-references-and-const/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=241), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Pointers & References Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=241), 'False', 0, 2);

-- =============================================================
-- New Quiz: Arrays & Strings Practice Quiz (9 questions)
-- order_index: 242..250
-- =============================================================

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 6, 'mcq',
 'What is required for a C-style string to be valid as a C-string?',
 NULL,
 'It must be null-terminated.',
 2, 10, 242, 1,
 'Terminating byte.',
 'https://www.learncpp.com/cpp-tutorial/c-style-strings/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=242), 'A terminating ''\0'' character', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=242), 'A trailing newline', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=242), 'A semicolon at end', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=242), 'A UTF-8 BOM', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 10, 'code_output',
 'What is printed?',
 'std::string s{"abc"};\nstd::cout << s.size();',
 'abc has 3 characters.',
 1, 10, 243, 1,
 'Count chars.',
 'https://www.studyplan.dev/pro-cpp/std-string');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=243), '3', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=243), '4', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=243), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=243), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 6, 'true_false',
 'In most expressions, a C-style array can decay to pointer to its first element.',
 NULL,
 'True. This happens in many contexts such as function argument passing.',
 2, 10, 244, 1,
 'Decay behavior.',
 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=244), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=244), 'False', 0, 2);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 10, 'multi_select',
 'Select valid std::string operations.',
 NULL,
 'Common operations include concatenation, substr, and find.',
 2, 15, 245, 1,
 'Core std::string API.',
 'https://www.studyplan.dev/pro-cpp/manipulating-std-strings');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=245), 's += "x"', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=245), 's.find("ab")', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=245), 's.substr(1,2)', 1, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=245), 's.ptrdiff()', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 6, 'fill_blank',
 'Complete: In int a[2][3], there are ____ rows.',
 NULL,
 'First dimension gives row count: 2.',
 2, 10, 246, 1,
 'a[rows][cols].',
 'https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/');

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 10, 'mcq',
 'Which statement about std::string is correct?',
 NULL,
 'std::string manages memory automatically and tracks size.',
 1, 10, 247, 1,
 'Compared to raw C-string buffers.',
 'https://www.studyplan.dev/pro-cpp/std-string');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=247), 'It owns character storage and knows its size', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=247), 'It always stores text on stack only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=247), 'It cannot contain spaces', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=247), 'It requires manual null terminator management', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 10, 'code_output',
 'What is printed?',
 'std::string s{"cat"};\nstd::cout << s[1];',
 'Index 1 is ''a'' in "cat".',
 1, 10, 248, 1,
 'Zero-based string indexing.',
 'https://www.studyplan.dev/pro-cpp/manipulating-std-strings');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=248), 'a', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=248), 'c', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=248), 't', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=248), 'Compile error', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 6, 'mcq',
 'What is true about bounds for C-style arrays?',
 NULL,
 'No automatic bounds checking is performed by raw array indexing.',
 2, 10, 249, 1,
 'Out-of-bounds is dangerous.',
 'https://www.studyplan.dev/pro-cpp/c-style-arrays');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=249), 'Out-of-bounds access is undefined behavior', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=249), 'Compiler inserts mandatory runtime checks always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=249), 'Indexing starts from 1', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=249), 'Array size can grow dynamically automatically', 0, 4);

INSERT INTO questions
(quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url)
VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1),
 10, 'true_false',
 'C-style string literals are typically stored as null-terminated character arrays.',
 NULL,
 'True. String literals include a terminating null character.',
 2, 10, 250, 1,
 'Literal storage model.',
 'https://www.learncpp.com/cpp-tutorial/c-style-strings/');

INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=250), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Arrays & Strings Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=250), 'False', 0, 2);

-- -----------------------------------------------------------------
-- Primary question_tags mapping
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 201 AND 208 THEN 'functions'
    WHEN q.order_index BETWEEN 209 AND 216 THEN 'arrays'
    WHEN q.order_index BETWEEN 217 AND 224 THEN 'pointers'
    WHEN q.order_index BETWEEN 225 AND 232 THEN 'functions'
    WHEN q.order_index BETWEEN 233 AND 241 THEN 'pointers'
    WHEN q.order_index BETWEEN 242 AND 250 THEN 'strings'
    ELSE 'best-practices'
END
WHERE q.order_index BETWEEN 201 AND 250;

-- Secondary tags for better coverage
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=210), (SELECT id FROM tags WHERE name='strings')),
((SELECT id FROM questions WHERE order_index=211), (SELECT id FROM tags WHERE name='strings')),
((SELECT id FROM questions WHERE order_index=213), (SELECT id FROM tags WHERE name='strings')),
((SELECT id FROM questions WHERE order_index=220), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=221), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=235), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=239), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=247), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=249), (SELECT id FROM tags WHERE name='best-practices'));

-- -----------------------------------------------------------------
-- Derive quiz_tags for affected quizzes
-- -----------------------------------------------------------------
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 201 AND 250;