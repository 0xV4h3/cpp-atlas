-- =============================================================
-- CppAtlas Content Patch: Core C++ Pack 02 (Part 3)
-- Date: 2026-03-16
-- Questions: 50 (order_index 1351..1400)
-- Focus: arrays, pointers, references, strings (+ functions)
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('functions', '#DCDCAA'),
('arrays', '#CE9178'),
('pointers', '#F14C4C'),
('references', '#F14C4C'),
('strings', '#CE9178'),
('best-practices', '#89D185');

INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Arrays Edge Cases Quiz', 'Array bounds, decay, multidimensional indexing and pointer-equivalent access', 6, 3, 'standard', 1, 0, 1),
('Pointers Semantics Quiz', 'Pointer arithmetic, dereference safety, aliasing and parameter passing', 7, 3, 'standard', 1, 0, 1),
('References Mastery Quiz', 'lvalue refs, const refs, binding rules, and API design', 7, 3, 'standard', 1, 0, 1),
('std::string Advanced Operations Quiz', 'capacity, assignment, append/insert/access, c_str interop', 10, 3, 'standard', 1, 0, 1),
('Function Interface Design Quiz', 'parameter styles, const-correctness, and readability/performance tradeoffs', 5, 3, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- 1351..1360 Arrays Edge Cases Quiz
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq',
 'For int arr[6], what is arr + 1 in most expression contexts?',
 NULL,
 'It points to the second element (pointer to arr[1]) after decay.',
 2, 10, 1351, 1,
 'Array-to-pointer decay.',
 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1351), 'Pointer to second element', 1, 1),
((SELECT id FROM questions WHERE order_index=1351), 'Pointer to last element', 0, 2),
((SELECT id FROM questions WHERE order_index=1351), 'Array size value', 0, 3),
((SELECT id FROM questions WHERE order_index=1351), 'Compile error always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'true_false',
 'sizeof(arr) (where arr is C-style array in same scope) gives total byte size of full array.',
 NULL,
 'True.',
 2, 10, 1352, 1,
 'Different from decayed pointer size.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1352), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1352), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'code_output',
 'What is printed?',
 'int a[5]{0,1,2,3,4};\nstd::cout << *(a + 4);',
 'a+4 points to fifth element with value 4.',
 2, 10, 1353, 1,
 'Pointer arithmetic in arrays.',
 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1353), '4', 1, 1),
((SELECT id FROM questions WHERE order_index=1353), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=1353), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1353), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq',
 'For int m[2][3], what is m[1][2]?',
 NULL,
 'Second row, third column element (0-based indexing).',
 2, 10, 1354, 1,
 'rows then cols indexing.',
 'https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1354), 'Element in row 2, column 3 (0-based 1,2)', 1, 1),
((SELECT id FROM questions WHERE order_index=1354), 'Element in row 1, column 2 (1-based)', 0, 2),
((SELECT id FROM questions WHERE order_index=1354), 'Always out-of-bounds', 0, 3),
((SELECT id FROM questions WHERE order_index=1354), 'Array pointer address only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'multi_select',
 'Select true statements about C-style arrays.',
 NULL,
 'C-style arrays are fixed-size and bounds are not runtime-checked by operator[].',
 2, 15, 1355, 1,
 'Fixed-size and manual safety.',
 'https://www.studyplan.dev/pro-cpp/c-style-arrays');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1355), 'Size is fixed at declaration', 1, 1),
((SELECT id FROM questions WHERE order_index=1355), 'Out-of-bounds indexing is undefined behavior', 1, 2),
((SELECT id FROM questions WHERE order_index=1355), 'operator[] always checks bounds at runtime', 0, 3),
((SELECT id FROM questions WHERE order_index=1355), 'Array may decay to pointer in many contexts', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'fill_blank',
 'When passed to function as parameter, C-style array often decays to ____.',
 NULL,
 'pointer.',
 2, 10, 1356, 1,
 'Loss of full type info.',
 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq',
 'Expression arr[i] is equivalent to which pointer form?',
 NULL,
 'By definition arr[i] == *(arr + i).',
 2, 10, 1357, 1,
 'Subscript desugaring.',
 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1357), '*(arr + i)', 1, 1),
((SELECT id FROM questions WHERE order_index=1357), '&(arr + i)', 0, 2),
((SELECT id FROM questions WHERE order_index=1357), 'arr + i + *', 0, 3),
((SELECT id FROM questions WHERE order_index=1357), '*(i - arr)', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'true_false',
 'Using std::array can preserve size information in type unlike raw pointer decay patterns.',
 NULL,
 'True.',
 2, 10, 1358, 1,
 'Type-encoded extent.',
 'https://www.studyplan.dev/pro-cpp/c-style-arrays');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1358), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1358), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq',
 'Which is safest educational guideline for array iteration?',
 NULL,
 'Use known bounds and avoid assumptions beyond valid index range.',
 2, 10, 1359, 1,
 'Boundary discipline.',
 'https://www.studyplan.dev/pro-cpp/c-style-arrays');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1359), 'Iterate strictly within valid bounds', 1, 1),
((SELECT id FROM questions WHERE order_index=1359), 'Read arr[size] to check terminator always', 0, 2),
((SELECT id FROM questions WHERE order_index=1359), 'Assume UB is harmless', 0, 3),
((SELECT id FROM questions WHERE order_index=1359), 'Skip bounds reasoning', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays Edge Cases Quiz' ORDER BY id DESC LIMIT 1), 6, 'code_output',
 'What is printed?',
 'int a[2][2]{{1,2},{3,4}};\nstd::cout << a[0][1] + a[1][0];',
 '2 + 3 = 5.',
 2, 10, 1360, 1,
 '2D index mapping.',
 'https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1360), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=1360), '3', 0, 2),
((SELECT id FROM questions WHERE order_index=1360), '4', 0, 3),
((SELECT id FROM questions WHERE order_index=1360), 'Compile error', 0, 4);

-- 1361..1370 Pointers Semantics Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'What does pointer arithmetic p + 1 mean for int* p?',
 NULL,
 'Advances pointer by one int element (sizeof(int) bytes).',
 2, 10, 1361, 1,
 'Element-wise stepping.',
 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1361), 'Move to next int element', 1, 1),
((SELECT id FROM questions WHERE order_index=1361), 'Increment pointed value', 0, 2),
((SELECT id FROM questions WHERE order_index=1361), 'Allocate new int', 0, 3),
((SELECT id FROM questions WHERE order_index=1361), 'Reset pointer to null', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false',
 'Dereferencing a null pointer is undefined behavior.',
 NULL,
 'True.',
 1, 10, 1362, 1,
 'Must validate pointer before dereference.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1362), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1362), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output',
 'What is printed?',
 'int x{7};\nint* p{&x};\nstd::cout << *p;',
 'Dereference gives value stored in x -> 7.',
 1, 10, 1363, 1,
 'Basic dereference.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1363), '7', 1, 1),
((SELECT id FROM questions WHERE order_index=1363), 'Address text', 0, 2),
((SELECT id FROM questions WHERE order_index=1363), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1363), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'Which declaration means pointer to const int?', NULL, 'const int* p.', 2, 10, 1364, 1, 'const applies to pointee here.', 'https://www.learncpp.com/cpp-tutorial/pointers-and-const/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1364), 'const int* p', 1, 1),
((SELECT id FROM questions WHERE order_index=1364), 'int* const p (pointee const)', 0, 2),
((SELECT id FROM questions WHERE order_index=1364), 'int const* const p (only this valid form)', 0, 3),
((SELECT id FROM questions WHERE order_index=1364), 'int&* p', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'multi_select',
 'Select true statements about pointers.', NULL, 'Pointers store addresses, can be null, and need valid target for dereference.', 2, 15, 1365, 1, 'Address semantics.', 'https://www.studyplan.dev/intro-to-programming/pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1365), 'Pointer may hold address of object', 1, 1),
((SELECT id FROM questions WHERE order_index=1365), 'Pointer can be nullptr', 1, 2),
((SELECT id FROM questions WHERE order_index=1365), 'All pointers are always valid to dereference', 0, 3),
((SELECT id FROM questions WHERE order_index=1365), 'Pointer arithmetic depends on pointed type', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'fill_blank',
 'Modern null pointer literal in C++ is _____.',
 NULL,
 'nullptr.',
 1, 10, 1366, 1,
 'Avoid old NULL integer macro style.',
 'https://www.learncpp.com/cpp-tutorial/null-pointers/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'Which operation is equivalent to p[i] for pointer p?', NULL, 'Equivalent to *(p + i).', 2, 10, 1367, 1, 'Subscript desugaring.', 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1367), '*(p + i)', 1, 1),
((SELECT id FROM questions WHERE order_index=1367), '&(p + i)', 0, 2),
((SELECT id FROM questions WHERE order_index=1367), '*p + i always', 0, 3),
((SELECT id FROM questions WHERE order_index=1367), 'p - i dereference', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'true_false',
 'Pointer to local variable becomes dangling after variable goes out of scope.',
 NULL,
 'True.',
 3, 12, 1368, 1,
 'Lifetime mismatch.',
 'https://www.studyplan.dev/intro-to-programming/dangling-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1368), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1368), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'Best practice after delete p in legacy raw-pointer code?', NULL, 'Set pointer to nullptr to reduce accidental use-after-free risk.', 2, 10, 1369, 1, 'Defensive pointer reset.', 'https://www.studyplan.dev/intro-to-programming/dangling-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1369), 'Assign p = nullptr', 1, 1),
((SELECT id FROM questions WHERE order_index=1369), 'Increment pointer', 0, 2),
((SELECT id FROM questions WHERE order_index=1369), 'Keep stale pointer for debug', 0, 3),
((SELECT id FROM questions WHERE order_index=1369), 'Cast pointer to int', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers Semantics Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output',
 'What is printed?',
 'int arr[3]{9,8,7};\nint* p{arr};\nstd::cout << p[0] << p[2];',
 'p[0]=9 and p[2]=7 -> 97.',
 2, 10, 1370, 1,
 'Pointer indexing view.',
 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1370), '97', 1, 1),
((SELECT id FROM questions WHERE order_index=1370), '98', 0, 2),
((SELECT id FROM questions WHERE order_index=1370), '87', 0, 3),
((SELECT id FROM questions WHERE order_index=1370), 'Compile error', 0, 4);

-- 1371..1380 References Mastery Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'What is an lvalue reference?', NULL, 'Alias to an existing named/modifiable object (subject to constness).', 1, 10, 1371, 1, 'Reference alias concept.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1371), 'Alias to existing object', 1, 1),
((SELECT id FROM questions WHERE order_index=1371), 'Nullable pointer type', 0, 2),
((SELECT id FROM questions WHERE order_index=1371), 'Temporary object copy', 0, 3),
((SELECT id FROM questions WHERE order_index=1371), 'Namespace shortcut', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false',
 'const lvalue reference can bind to a temporary value.', NULL, 'True.', 2, 10, 1372, 1, 'Important binding rule.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references-to-const/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1372), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1372), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output',
 'What is printed?',
 'int x{3};\nint& r{x};\nr = 11;\nstd::cout << x;',
 'Reference r aliases x; assignment updates x to 11.',
 1, 10, 1373, 1,
 'Alias mutation.',
 'https://www.studyplan.dev/intro-to-programming/references');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1373), '11', 1, 1),
((SELECT id FROM questions WHERE order_index=1373), '3', 0, 2),
((SELECT id FROM questions WHERE order_index=1373), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1373), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'Why use pass-by-const-reference for std::string parameter?', NULL, 'Avoid copies while preserving read-only contract.', 2, 10, 1374, 1, 'Performance + const safety.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1374), 'No copy + cannot modify argument', 1, 1),
((SELECT id FROM questions WHERE order_index=1374), 'Forces caller mutation', 0, 2),
((SELECT id FROM questions WHERE order_index=1374), 'Converts to C-array automatically', 0, 3),
((SELECT id FROM questions WHERE order_index=1374), 'Only works for char', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select',
 'Select true statements about references in function APIs.', NULL, 'References can express intent (in/out), const refs for input, non-const refs for output/inout.', 3, 15, 1375, 1, 'API intent signaling.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1375), 'const T& is common input parameter style', 1, 1),
((SELECT id FROM questions WHERE order_index=1375), 'T& may allow caller-visible mutation', 1, 2),
((SELECT id FROM questions WHERE order_index=1375), 'Reference can be left uninitialized', 0, 3),
((SELECT id FROM questions WHERE order_index=1375), 'Reference reseating after init is not allowed', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank',
 'Reference declaration uses symbol ____ after type.', NULL, '&.', 1, 10, 1376, 1, 'int& ref syntax.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'Which binding is valid?', NULL, 'const int& can bind to temporary like 42.', 2, 10, 1377, 1, 'Temporary binding rule.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references-to-const/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1377), 'const int& r = 42;', 1, 1),
((SELECT id FROM questions WHERE order_index=1377), 'int& r = 42;', 0, 2),
((SELECT id FROM questions WHERE order_index=1377), 'int& r;', 0, 3),
((SELECT id FROM questions WHERE order_index=1377), 'int&&& r = x;', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false',
 'References are generally safer for mandatory non-null aliases than raw pointers.', NULL, 'True in many API contexts where null is not meaningful.', 3, 12, 1378, 1, 'Non-null alias semantics.', 'https://www.studyplan.dev/intro-to-programming/references');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1378), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1378), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'When should non-const lvalue reference parameter be avoided?', NULL, 'When function should not mutate caller state (prefer const ref/value).', 2, 10, 1379, 1, 'Minimize unexpected side effects.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1379), 'When mutation is not intended', 1, 1),
((SELECT id FROM questions WHERE order_index=1379), 'When needing in-place modification', 0, 2),
((SELECT id FROM questions WHERE order_index=1379), 'When returning bool', 0, 3),
((SELECT id FROM questions WHERE order_index=1379), 'Always avoid all references', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='References Mastery Quiz' ORDER BY id DESC LIMIT 1), 9, 'code_output',
 'What is printed?',
 'void twice(int& v){ v *= 2; }\nint n=6; twice(n); std::cout<<n;',
 'Function modifies n through reference -> 12.',
 2, 10, 1380, 1,
 'Reference pass-through mutation.',
 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1380), '12', 1, 1),
((SELECT id FROM questions WHERE order_index=1380), '6', 0, 2),
((SELECT id FROM questions WHERE order_index=1380), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=1380), 'Compile error', 0, 4);

-- 1381..1390 std::string Advanced Operations Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'What is common reason to call string::reserve?', NULL, 'To reduce reallocations when expected growth is known.', 3, 12, 1381, 1, 'Capacity planning.', 'https://www.learncpp.com/cpp-tutorial/stdstring-length-and-capacity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1381), 'Minimize reallocations for expected growth', 1, 1),
((SELECT id FROM questions WHERE order_index=1381), 'Set size immediately to N always', 0, 2),
((SELECT id FROM questions WHERE order_index=1381), 'Convert to C array', 0, 3),
((SELECT id FROM questions WHERE order_index=1381), 'Sort characters', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'operator[] on std::string performs bounds checking and throws on invalid index.',
 NULL,
 'False; at() is bounds-checked accessor.',
 2, 10, 1382, 1,
 'Distinguish [] vs at().',
 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1382), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1382), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::string s{"xy"};\ns += "z";\nstd::cout << s.length();',
 'String becomes xyz, length is 3.', 1, 10, 1383, 1, 'Append then query length.', 'https://www.learncpp.com/cpp-tutorial/stdstring-appending/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1383), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1383), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=1383), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=1383), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which method replaces existing string contents?', NULL, 'assign() replaces with new content.', 2, 10, 1384, 1, 'Replacement operation.', 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1384), 'assign()', 1, 1),
((SELECT id FROM questions WHERE order_index=1384), 'append() only', 0, 2),
((SELECT id FROM questions WHERE order_index=1384), 'capacity()', 0, 3),
((SELECT id FROM questions WHERE order_index=1384), 'c_str()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select',
 'Select true statements about std::string APIs.', NULL, 'size/length report chars, append adds end text, insert adds at position, swap exchanges contents.', 2, 15, 1385, 1, 'Core mutating/query APIs.', 'https://www.studyplan.dev/pro-cpp/manipulating-std-strings');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1385), 'size() returns character count', 1, 1),
((SELECT id FROM questions WHERE order_index=1385), 'insert(pos, str) can add at chosen position', 1, 2),
((SELECT id FROM questions WHERE order_index=1385), 'swap exchanges contents between strings', 1, 3),
((SELECT id FROM questions WHERE order_index=1385), 'c_str() always returns mutable buffer', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank',
 'Function that returns pointer suitable for C APIs is string::_____.',
 NULL,
 'c_str.',
 2, 10, 1386, 1,
 'Interop accessor.', 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which operation most directly inserts at middle of string?', NULL, 'insert(index, text).', 2, 10, 1387, 1, 'Position-targeted mutation.', 'https://www.learncpp.com/cpp-tutorial/stdstring-inserting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1387), 'insert(pos, text)', 1, 1),
((SELECT id FROM questions WHERE order_index=1387), 'append(text)', 0, 2),
((SELECT id FROM questions WHERE order_index=1387), 'clear()', 0, 3),
((SELECT id FROM questions WHERE order_index=1387), 'front()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'swap between two strings can be cheaper than manual temp-copy-heavy replacements in many cases.',
 NULL,
 'True.',
 3, 12, 1388, 1,
 'Swap typically exchanges internals efficiently.', 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1388), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1388), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Best practice for string API heavy code readability?', NULL, 'Use descriptive names and clear mutation points rather than chained obscure edits.', 2, 10, 1389, 1, 'Clarity over clever one-liners.', 'https://www.studyplan.dev/pro-cpp/std-string');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1389), 'Prefer readable, explicit string transformations', 1, 1),
((SELECT id FROM questions WHERE order_index=1389), 'Always compress logic into one line', 0, 2),
((SELECT id FROM questions WHERE order_index=1389), 'Avoid naming intermediate states', 0, 3),
((SELECT id FROM questions WHERE order_index=1389), 'Ignore boundary/index checks', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Advanced Operations Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::string s{"abcd"};\nstd::cout << s.at(2);',
 'Index 2 is character c.', 1, 10, 1390, 1, 'Bounds-checked accessor.', 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1390), 'c', 1, 1),
((SELECT id FROM questions WHERE order_index=1390), 'b', 0, 2),
((SELECT id FROM questions WHERE order_index=1390), 'd', 0, 3),
((SELECT id FROM questions WHERE order_index=1390), 'Compile error', 0, 4);

-- 1391..1400 Function Interface Design Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'Which parameter style is usually best for small cheap-to-copy types like int?', NULL, 'Pass-by-value is simple and efficient for small trivially copyable types.', 1, 10, 1391, 1, 'Don''t overcomplicate with refs for tiny scalars.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1391), 'Pass by value', 1, 1),
((SELECT id FROM questions WHERE order_index=1391), 'Always const reference', 0, 2),
((SELECT id FROM questions WHERE order_index=1391), 'Always raw pointer', 0, 3),
((SELECT id FROM questions WHERE order_index=1391), 'Global variable only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'true_false',
 'Using const in function interfaces can communicate non-modifying intent.', NULL, 'True.', 1, 10, 1392, 1, 'Interface clarity.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1392), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1392), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?',
 'int add(int a,int b){ return a+b; }\nstd::cout << add(8,1);',
 'Returns and prints 9.', 1, 10, 1393, 1, 'Simple function call.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1393), '9', 1, 1),
((SELECT id FROM questions WHERE order_index=1393), '81', 0, 2),
((SELECT id FROM questions WHERE order_index=1393), '8', 0, 3),
((SELECT id FROM questions WHERE order_index=1393), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'When designing function interface, what is primary goal?', NULL, 'Clear intent, correctness, and maintainability with suitable performance tradeoffs.', 2, 10, 1394, 1, 'Readable contract first.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1394), 'Clear, correct, maintainable contract', 1, 1),
((SELECT id FROM questions WHERE order_index=1394), 'Maximize parameter count', 0, 2),
((SELECT id FROM questions WHERE order_index=1394), 'Hide all semantics', 0, 3),
((SELECT id FROM questions WHERE order_index=1394), 'Always mutate inputs', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'multi_select',
 'Select good function interface practices.', NULL, 'Use meaningful names, const correctness, and minimal side effects.', 2, 15, 1395, 1, 'API ergonomics.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1395), 'Choose descriptive parameter names', 1, 1),
((SELECT id FROM questions WHERE order_index=1395), 'Use const where mutation is not intended', 1, 2),
((SELECT id FROM questions WHERE order_index=1395), 'Introduce hidden side effects by default', 0, 3),
((SELECT id FROM questions WHERE order_index=1395), 'Keep function responsibilities focused', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'fill_blank',
 'If a function should modify caller variable directly, parameter style can be non-const lvalue ____.',
 NULL,
 'reference.',
 2, 10, 1396, 1,
 'T& parameter.',
 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'Why avoid unnecessary pass-by-reference for trivial scalar parameters?', NULL, 'Can reduce clarity with little/no performance benefit for small cheap types.', 2, 10, 1397, 1, 'Appropriate abstraction level.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1397), 'May hurt clarity without meaningful benefit', 1, 1),
((SELECT id FROM questions WHERE order_index=1397), 'Reference is always faster by language rule', 0, 2),
((SELECT id FROM questions WHERE order_index=1397), 'Value parameters cannot be optimized', 0, 3),
((SELECT id FROM questions WHERE order_index=1397), 'References are mandatory in C++', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'true_false',
 'Function interfaces should make ownership/mutation expectations explicit where relevant.', NULL, 'True.', 2, 10, 1398, 1, 'Contract clarity principle.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1398), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1398), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'Which is often clearer for optional mutation semantics?', NULL, 'Separate read-only and mutating overloads/functions can improve API clarity.', 3, 12, 1399, 1, 'Intent-specific interfaces.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1399), 'Explicitly distinct read-only vs mutating interfaces', 1, 1),
((SELECT id FROM questions WHERE order_index=1399), 'Always mutate silently', 0, 2),
((SELECT id FROM questions WHERE order_index=1399), 'Use global state instead of parameters', 0, 3),
((SELECT id FROM questions WHERE order_index=1399), 'Hide all parameter meanings', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Interface Design Quiz' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?',
 'void printLen(const std::string& s){ std::cout << s.size(); }\nprintLen(std::string{"abc"});',
 'const ref can bind to temporary; size is 3.', 2, 10, 1400, 1, 'Temporary binding with const ref.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references-to-const/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1400), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=1400), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=1400), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=1400), 'abc', 0, 4);

-- Primary question_tags mapping
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 1351 AND 1360 THEN 'arrays'
    WHEN q.order_index BETWEEN 1361 AND 1370 THEN 'pointers'
    WHEN q.order_index BETWEEN 1371 AND 1380 THEN 'references'
    WHEN q.order_index BETWEEN 1381 AND 1390 THEN 'strings'
    WHEN q.order_index BETWEEN 1391 AND 1400 THEN 'functions'
    ELSE 'best-practices'
END
WHERE q.order_index BETWEEN 1351 AND 1400;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=1355), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1358), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1365), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1368), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1374), (SELECT id FROM tags WHERE name='functions')),
((SELECT id FROM questions WHERE order_index=1375), (SELECT id FROM tags WHERE name='functions')),
((SELECT id FROM questions WHERE order_index=1385), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1389), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1395), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1398), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 1351 AND 1400;