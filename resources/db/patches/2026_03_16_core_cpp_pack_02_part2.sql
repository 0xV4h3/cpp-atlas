-- =============================================================
-- CppAtlas Content Patch: Core C++ Pack 02 (Part 2)
-- Date: 2026-03-16
-- Questions: 50 (order_index 1301..1350)
-- Focus: functions, arrays, pointers, references, strings
--
-- IMPORTANT:
-- - No BEGIN/COMMIT (ContentPatchService manages transaction)
-- - Standardized INSERT INTO questions (12 columns)
-- - SQLite-safe quoting
-- - Enriched coverage for Arrays & Strings and Pointers & References
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('functions', '#DCDCAA'),
('arrays', '#CE9178'),
('pointers', '#F14C4C'),
('references', '#F14C4C'),
('strings', '#CE9178'),
('best-practices', '#89D185');

-- New focused quizzes for enrichment
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Arrays & Strings Deep Dive Quiz', 'std::string APIs, C-style arrays, decay, indexing, capacity, appending/inserting', 6, 3, 'standard', 1, 0, 1),
('Pointers & References Intensive Quiz', 'Pointer arithmetic, const references, pass-by-reference patterns', 7, 3, 'standard', 1, 0, 1),
('Function Parameters & References Quiz', 'Argument passing styles and reference semantics in functions', 5, 2, 'standard', 1, 0, 1),
('C-Style Arrays Practice Quiz', 'Array layout, decay behavior, pointer subscripting, boundaries', 6, 2, 'standard', 1, 0, 1),
('std::string Operations Workshop Quiz', 'length/capacity/access/assignment/swap/append/insert patterns', 6, 3, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- 1301..1310 Arrays & Strings Deep Dive Quiz
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'What does std::string::size() return?',
 NULL,
 'Number of characters currently stored in the string.',
 1, 10, 1301, 1,
 'size and length are equivalent for std::string.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-length-and-capacity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1301), 'Current number of characters', 1, 1),
((SELECT id FROM questions WHERE order_index=1301), 'Allocated capacity in bytes only', 0, 2),
((SELECT id FROM questions WHERE order_index=1301), 'Index of last character', 0, 3),
((SELECT id FROM questions WHERE order_index=1301), 'Null terminator index only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'std::string::capacity() is always equal to std::string::size().',
 NULL,
 'False. capacity is allocated storage; it can exceed size.',
 2, 10, 1302, 1,
 'Capacity often grows geometrically.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-length-and-capacity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1302), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=1302), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::string s{"cat"};\nstd::cout << s[0] << s[2];',
 'Index 0 is c, index 2 is t -> ct.',
 1, 10, 1303, 1,
 'Zero-based indexing.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1303), 'ct', 1, 1),
((SELECT id FROM questions WHERE order_index=1303), 'ca', 0, 2),
((SELECT id FROM questions WHERE order_index=1303), 'at', 0, 3),
((SELECT id FROM questions WHERE order_index=1303), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which operation appends text to end of std::string?',
 NULL,
 'operator+= and append() add content at the end.',
 1, 10, 1304, 1,
 'End-growth operation.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-appending/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1304), 's += "text"', 1, 1),
((SELECT id FROM questions WHERE order_index=1304), 's.insert(0, "text")', 0, 2),
((SELECT id FROM questions WHERE order_index=1304), 's.clear("text")', 0, 3),
((SELECT id FROM questions WHERE order_index=1304), 's.pop_back("text")', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 6, 'multi_select',
 'Select true statements about C-style array decay.',
 NULL,
 'Arrays often decay to pointer to first element in expressions/function arguments.',
 3, 15, 1305, 1,
 'Decay changes type context.',
 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1305), 'Array may decay to pointer to first element', 1, 1),
((SELECT id FROM questions WHERE order_index=1305), 'Decay often happens when passing to function', 1, 2),
((SELECT id FROM questions WHERE order_index=1305), 'Array keeps full size info automatically after decay', 0, 3),
((SELECT id FROM questions WHERE order_index=1305), 'Decay can affect sizeof expectations', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank',
 'Method that adds substring at specific position is string::_____.',
 NULL,
 'insert.',
 2, 10, 1306, 1,
 'Position-based addition.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-inserting/');

INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1306), 'insert', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'What does std::string::c_str() provide?',
 NULL,
 'Pointer to null-terminated C-style string view of string data.',
 2, 10, 1307, 1,
 'Interoperability with C APIs.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1307), 'const char* null-terminated representation', 1, 1),
((SELECT id FROM questions WHERE order_index=1307), 'Mutable char* always safe to modify', 0, 2),
((SELECT id FROM questions WHERE order_index=1307), 'Number of words in string', 0, 3),
((SELECT id FROM questions WHERE order_index=1307), 'Iterator to end', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 6, 'true_false',
 'For int a[4], valid indices are 0 through 3.',
 NULL,
 'True.',
 1, 10, 1308, 1,
 'Index upper bound is size-1.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1308), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1308), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which operation swaps contents of two std::string objects efficiently?',
 NULL,
 'std::string::swap (or std::swap) exchanges internal states.',
 2, 10, 1309, 1,
 'Swap avoids manual copying patterns.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1309), 's1.swap(s2)', 1, 1),
((SELECT id FROM questions WHERE order_index=1309), 's1 = s2 = ""', 0, 2),
((SELECT id FROM questions WHERE order_index=1309), 's1.append(s2) and clear s2 always', 0, 3),
((SELECT id FROM questions WHERE order_index=1309), 'reinterpret_cast swap', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Arrays & Strings Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::string s{"ab"};\ns.insert(1, "Z");\nstd::cout << s;',
 'Inserting Z at index 1 yields aZb.',
 2, 10, 1310, 1,
 'insert(pos, str) semantics.',
 'https://www.learncpp.com/cpp-tutorial/stdstring-inserting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1310), 'aZb', 1, 1),
((SELECT id FROM questions WHERE order_index=1310), 'abZ', 0, 2),
((SELECT id FROM questions WHERE order_index=1310), 'Zab', 0, 3),
((SELECT id FROM questions WHERE order_index=1310), 'Compile error', 0, 4);

-- 1311..1320 Pointers & References Intensive Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'What does unary * operator do on a pointer?',
 NULL,
 'Dereferences pointer to access pointed object.',
 1, 10, 1311, 1,
 'Read/write pointed value.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1311), 'Access value at pointed address', 1, 1),
((SELECT id FROM questions WHERE order_index=1311), 'Take address of variable', 0, 2),
((SELECT id FROM questions WHERE order_index=1311), 'Allocate new memory', 0, 3),
((SELECT id FROM questions WHERE order_index=1311), 'Create reference alias', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false',
 'An lvalue reference must be initialized when declared.', NULL, 'True.', 1, 10, 1312, 1, 'Reference binding required.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1312), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1312), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output',
 'What is printed?',
 'int a[3]{10,20,30};\nint* p = a;\nstd::cout << *(p+1);',
 'Pointer arithmetic p+1 points to second element => 20.',
 3, 12, 1313, 1,
 'Array decays to pointer to first element.',
 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1313), '20', 1, 1),
((SELECT id FROM questions WHERE order_index=1313), '10', 0, 2),
((SELECT id FROM questions WHERE order_index=1313), '30', 0, 3),
((SELECT id FROM questions WHERE order_index=1313), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'Why pass large object by const lvalue reference?', NULL, 'Avoid copy cost while preventing modification.', 2, 10, 1314, 1, 'Performance + const safety.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1314), 'Avoid copies and keep object read-only', 1, 1),
((SELECT id FROM questions WHERE order_index=1314), 'Force object move semantics always', 0, 2),
((SELECT id FROM questions WHERE order_index=1314), 'Allow nullptr checks', 0, 3),
((SELECT id FROM questions WHERE order_index=1314), 'Replace all templates', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'multi_select',
 'Select true statements about references.',
 NULL,
 'References alias existing objects and cannot be reseated after binding.',
 2, 15, 1315, 1,
 'Alias semantics.',
 'https://www.studyplan.dev/intro-to-programming/references');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1315), 'Reference is an alias to object', 1, 1),
((SELECT id FROM questions WHERE order_index=1315), 'Reference must be initialized', 1, 2),
((SELECT id FROM questions WHERE order_index=1315), 'Reference can be reseated later', 0, 3),
((SELECT id FROM questions WHERE order_index=1315), 'const reference can bind to temporary', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'fill_blank',
 'Reference that cannot modify referenced object is ____ lvalue reference.',
 NULL,
 'const.',
 2, 10, 1316, 1,
 'const T&.',
 'https://www.learncpp.com/cpp-tutorial/lvalue-references-to-const/');

INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1316), 'const', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 8, 'mcq',
 'Pointer subscripting p[i] is equivalent to which expression?', NULL, 'p[i] is defined as *(p + i).', 3, 12, 1317, 1, 'Array-pointer equivalence.', 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1317), '*(p + i)', 1, 1),
((SELECT id FROM questions WHERE order_index=1317), '&(p + i)', 0, 2),
((SELECT id FROM questions WHERE order_index=1317), '*(i + p + 1 always)', 0, 3),
((SELECT id FROM questions WHERE order_index=1317), 'p + i + *', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'true_false',
 'Passing by non-const lvalue reference allows function to modify caller variable.', NULL, 'True.', 2, 10, 1318, 1, 'Mutating reference parameter.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1318), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1318), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 9, 'mcq',
 'Why is const lvalue reference parameter widely used?', NULL, 'Efficient pass-by-reference with read-only guarantee.', 2, 10, 1319, 1, 'Common API parameter type.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1319), 'Efficient and prevents mutation', 1, 1),
((SELECT id FROM questions WHERE order_index=1319), 'Forces copy every call', 0, 2),
((SELECT id FROM questions WHERE order_index=1319), 'Only for integers', 0, 3),
((SELECT id FROM questions WHERE order_index=1319), 'Disables function overloading', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Pointers & References Intensive Quiz' ORDER BY id DESC LIMIT 1), 8, 'code_output',
 'What is printed?',
 'int x=5; int* p=&x; *p=9; std::cout<<x;',
 'Dereferencing assignment modifies x through pointer.', 1, 10, 1320, 1, 'Indirect write.', 'https://www.studyplan.dev/intro-to-programming/pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1320), '9', 1, 1),
((SELECT id FROM questions WHERE order_index=1320), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=1320), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=1320), 'Compile error', 0, 4);

-- 1321..1330 Function Parameters & References Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'What is pass-by-value?', NULL, 'Function gets a copy of argument value.', 1, 10, 1321, 1, 'Independent local parameter copy.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1321), 'Function receives argument copy', 1, 1),
((SELECT id FROM questions WHERE order_index=1321), 'Function receives ownership pointer', 0, 2),
((SELECT id FROM questions WHERE order_index=1321), 'Function always mutates caller data', 0, 3),
((SELECT id FROM questions WHERE order_index=1321), 'Function receives namespace', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'true_false',
 'Pass-by-lvalue-reference can avoid copy and allow mutation.', NULL, 'True.', 2, 10, 1322, 1, 'Reference parameter.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1322), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1322), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?',
 'void inc(int& v){ ++v; }\nint x=4; inc(x); std::cout<<x;', 'Reference parameter modifies caller x to 5.', 2, 10, 1323, 1, 'Non-const ref mutation.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1323), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=1323), '4', 0, 2),
((SELECT id FROM questions WHERE order_index=1323), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=1323), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'When is pass-by-const-reference often preferred?', NULL, 'For large objects needing read-only access without copy.', 2, 10, 1324, 1, 'Large immutable argument.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1324), 'Large read-only parameters', 1, 1),
((SELECT id FROM questions WHERE order_index=1324), 'Need to mutate caller object', 0, 2),
((SELECT id FROM questions WHERE order_index=1324), 'For raw array size deduction', 0, 3),
((SELECT id FROM questions WHERE order_index=1324), 'Only for char types', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'multi_select',
 'Select advantages of const lvalue reference parameters.', NULL, 'Avoid copies, allow binding to temporaries, and enforce non-modifying intent.', 3, 15, 1325, 1, 'const T& advantages.', 'https://www.learncpp.com/cpp-tutorial/lvalue-references-to-const/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1325), 'Avoids expensive copies', 1, 1),
((SELECT id FROM questions WHERE order_index=1325), 'Can bind to temporaries', 1, 2),
((SELECT id FROM questions WHERE order_index=1325), 'Guarantees parameter mutation', 0, 3),
((SELECT id FROM questions WHERE order_index=1325), 'Documents read-only intent', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'fill_blank',
 'A reference parameter that should not modify argument is usually qualified with ____.', NULL, 'const.', 1, 10, 1326, 1, 'const T& parameter style.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');

INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1326), 'const', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'What does this function return? int sum(int a,int b){ return a+b; } called with (2,5)', NULL, 'Returns 7.', 1, 10, 1327, 1, 'Simple value return.', 'https://www.studyplan.dev/intro-to-programming/functions');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1327), '7', 1, 1),
((SELECT id FROM questions WHERE order_index=1327), '25', 0, 2),
((SELECT id FROM questions WHERE order_index=1327), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=1327), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'true_false',
 'Passing by value isolates function modifications from caller variable.', NULL, 'True.', 1, 10, 1328, 1, 'Local copy semantics.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1328), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1328), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'mcq',
 'Which parameter style best communicates "input-only large object"?', NULL, 'const T&.', 2, 10, 1329, 1, 'Read-only + no copy.', 'https://www.learncpp.com/cpp-tutorial/pass-by-const-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1329), 'const T&', 1, 1),
((SELECT id FROM questions WHERE order_index=1329), 'T&', 0, 2),
((SELECT id FROM questions WHERE order_index=1329), 'T', 0, 3),
((SELECT id FROM questions WHERE order_index=1329), 'T* mutable', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Function Parameters & References Quiz' ORDER BY id DESC LIMIT 1), 5, 'code_output',
 'What is printed?', 'void setZero(int& v){ v=0; }\nint n=9; setZero(n); std::cout<<n;', 'Reference allows direct modification of caller variable.', 1, 10, 1330, 1, 'Mutation through ref param.', 'https://www.learncpp.com/cpp-tutorial/pass-by-lvalue-reference/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1330), '0', 1, 1),
((SELECT id FROM questions WHERE order_index=1330), '9', 0, 2),
((SELECT id FROM questions WHERE order_index=1330), '90', 0, 3),
((SELECT id FROM questions WHERE order_index=1330), 'Compile error', 0, 4);

-- 1331..1340 C-Style Arrays Practice Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq', 'For int a[5], what is last valid index?', NULL, 'Last valid index is 4.', 1, 10, 1331, 1, 'size-1 rule.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1331), '4', 1, 1),
((SELECT id FROM questions WHERE order_index=1331), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=1331), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=1331), '0', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'true_false', 'Array name often decays to pointer to first element in function arguments.', NULL, 'True.', 2, 10, 1332, 1, 'Common decay context.', 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1332), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1332), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'code_output', 'What is printed?', 'int a[4]{2,4,6,8};\nstd::cout << a[1] + a[3];', 'a[1]=4 and a[3]=8 => 12.', 1, 10, 1333, 1, 'Index access.', 'https://www.studyplan.dev/pro-cpp/c-style-arrays');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1333), '12', 1, 1),
((SELECT id FROM questions WHERE order_index=1333), '10', 0, 2),
((SELECT id FROM questions WHERE order_index=1333), '6', 0, 3),
((SELECT id FROM questions WHERE order_index=1333), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq', 'What does *(a+2) access for int a[4]?', NULL, 'Equivalent to a[2].', 2, 10, 1334, 1, 'Pointer arithmetic equivalence.', 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1334), 'Third element a[2]', 1, 1),
((SELECT id FROM questions WHERE order_index=1334), 'Second element a[1]', 0, 2),
((SELECT id FROM questions WHERE order_index=1334), 'First element a[0]', 0, 3),
((SELECT id FROM questions WHERE order_index=1334), 'Out-of-bounds always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'multi_select', 'Select array-related safety practices.', NULL, 'Check bounds, avoid invalid indexing, and keep size context explicit.', 3, 15, 1335, 1, 'Prevent UB.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1335), 'Respect valid index range', 1, 1),
((SELECT id FROM questions WHERE order_index=1335), 'Track array size carefully', 1, 2),
((SELECT id FROM questions WHERE order_index=1335), 'Assume decay preserves size metadata', 0, 3),
((SELECT id FROM questions WHERE order_index=1335), 'Prefer safer abstractions where practical', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'fill_blank', 'Expression p[i] is equivalent to *(p + ____).', NULL, 'i.', 2, 10, 1336, 1, 'Subscript equivalence.', 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');

INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1336), 'i', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq', 'What is common challenge after array decay in function parameter?', NULL, 'Array size information is not automatically preserved via raw pointer parameter.', 3, 12, 1337, 1, 'Need explicit size parameter.', 'https://www.learncpp.com/cpp-tutorial/c-style-array-decay/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1337), 'Losing automatic size information', 1, 1),
((SELECT id FROM questions WHERE order_index=1337), 'Elements become immutable', 0, 2),
((SELECT id FROM questions WHERE order_index=1337), 'Array becomes sorted', 0, 3),
((SELECT id FROM questions WHERE order_index=1337), 'Memory doubles', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'true_false', 'Out-of-bounds access on C-style arrays is undefined behavior.', NULL, 'True.', 2, 10, 1338, 1, 'Critical safety rule.', 'https://www.studyplan.dev/pro-cpp/c-style-arrays');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1338), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1338), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'mcq', 'For int a[2][3], how many total int elements?', NULL, '2*3 = 6 total elements.', 2, 10, 1339, 1, 'rows*cols.', 'https://www.learncpp.com/cpp-tutorial/multidimensional-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1339), '6', 1, 1),
((SELECT id FROM questions WHERE order_index=1339), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=1339), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=1339), '2', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='C-Style Arrays Practice Quiz' ORDER BY id DESC LIMIT 1), 6, 'code_output', 'What is printed?', 'int a[3]{1,3,5}; int* p=a; std::cout << p[2];', 'p[2] accesses third element 5.', 2, 10, 1340, 1, 'Pointer subscripting.', 'https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-subscripting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1340), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=1340), '3', 0, 2),
((SELECT id FROM questions WHERE order_index=1340), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=1340), 'Compile error', 0, 4);

-- 1341..1350 std::string Operations Workshop Quiz
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which method checks whether string has zero length?', NULL, 'empty() returns true when size()==0.', 1, 10, 1341, 1, 'Boolean emptiness check.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1341), 'empty()', 1, 1),
((SELECT id FROM questions WHERE order_index=1341), 'erase()', 0, 2),
((SELECT id FROM questions WHERE order_index=1341), 'front()', 0, 3),
((SELECT id FROM questions WHERE order_index=1341), 'at_end()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::string::length() and std::string::size() are equivalent in behavior.', NULL, 'True.', 1, 10, 1342, 1, 'Two names, same meaning.', 'https://www.learncpp.com/cpp-tutorial/stdstring-length-and-capacity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1342), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1342), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::string s{"ab"};\ns.append("cd");\nstd::cout << s;', 'append adds at end => abcd.', 1, 10, 1343, 1, 'append operation.', 'https://www.learncpp.com/cpp-tutorial/stdstring-appending/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1343), 'abcd', 1, 1),
((SELECT id FROM questions WHERE order_index=1343), 'ab', 0, 2),
((SELECT id FROM questions WHERE order_index=1343), 'cdab', 0, 3),
((SELECT id FROM questions WHERE order_index=1343), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'What does string::at(index) provide vs operator[]?', NULL, 'at() performs bounds checking and may throw on invalid index.', 3, 12, 1344, 1, 'Safer indexed access.', 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1344), 'Bounds-checked character access', 1, 1),
((SELECT id FROM questions WHERE order_index=1344), 'Always faster and unchecked only', 0, 2),
((SELECT id FROM questions WHERE order_index=1344), 'Returns C-string pointer', 0, 3),
((SELECT id FROM questions WHERE order_index=1344), 'Changes string capacity only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select valid std::string mutation operations.', NULL, 'append, insert, assign, swap, erase are mutation operations.', 2, 15, 1345, 1, 'String editing API.', 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1345), 'append', 1, 1),
((SELECT id FROM questions WHERE order_index=1345), 'insert', 1, 2),
((SELECT id FROM questions WHERE order_index=1345), 'swap', 1, 3),
((SELECT id FROM questions WHERE order_index=1345), 'capacity (query only)', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'Method returning pointer to null-terminated char array view is ____().', NULL, 'c_str.', 2, 10, 1346, 1, 'C interop method.', 'https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-c-style-arrays/');

INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1346), 'c_str', 1),
((SELECT id FROM questions WHERE order_index=1346), 'c_str()', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'What does reserve(n) primarily affect?', NULL, 'It adjusts capacity reservation, not logical size directly.', 3, 12, 1347, 1, 'Capacity planning.', 'https://www.learncpp.com/cpp-tutorial/stdstring-length-and-capacity/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1347), 'Capacity reservation', 1, 1),
((SELECT id FROM questions WHERE order_index=1347), 'Immediately sets size to n', 0, 2),
((SELECT id FROM questions WHERE order_index=1347), 'Clears string always', 0, 3),
((SELECT id FROM questions WHERE order_index=1347), 'Converts to char[]', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::string supports assignment from another std::string.', NULL, 'True.', 1, 10, 1348, 1, 'Copy assignment available.', 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1348), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1348), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which call inserts text at beginning of string s?', NULL, 's.insert(0, "...") inserts at index 0.', 2, 10, 1349, 1, 'Insert at position 0.', 'https://www.learncpp.com/cpp-tutorial/stdstring-inserting/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1349), 's.insert(0, "x")', 1, 1),
((SELECT id FROM questions WHERE order_index=1349), 's.append("x")', 0, 2),
((SELECT id FROM questions WHERE order_index=1349), 's.swap("x")', 0, 3),
((SELECT id FROM questions WHERE order_index=1349), 's.capacity("x")', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='std::string Operations Workshop Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::string s{"hello"};\ns.assign("hi");\nstd::cout<<s;', 'assign replaces contents with hi.', 1, 10, 1350, 1, 'Replacement assignment.', 'https://www.learncpp.com/cpp-tutorial/stdstring-assignment-and-swapping/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1350), 'hi', 1, 1),
((SELECT id FROM questions WHERE order_index=1350), 'hellohi', 0, 2),
((SELECT id FROM questions WHERE order_index=1350), 'hello', 0, 3),
((SELECT id FROM questions WHERE order_index=1350), 'Compile error', 0, 4);

-- Primary tag mapping
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id, t.id
FROM questions q
JOIN tags t ON t.name = CASE
    WHEN q.order_index BETWEEN 1301 AND 1310 THEN 'strings'
    WHEN q.order_index BETWEEN 1311 AND 1320 THEN 'pointers'
    WHEN q.order_index BETWEEN 1321 AND 1330 THEN 'functions'
    WHEN q.order_index BETWEEN 1331 AND 1340 THEN 'arrays'
    WHEN q.order_index BETWEEN 1341 AND 1350 THEN 'strings'
    ELSE 'best-practices'
END
WHERE q.order_index BETWEEN 1301 AND 1350;

-- Secondary tags for richer coverage
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=1305), (SELECT id FROM tags WHERE name='arrays')),
((SELECT id FROM questions WHERE order_index=1306), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1314), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=1315), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=1316), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=1317), (SELECT id FROM tags WHERE name='arrays')),
((SELECT id FROM questions WHERE order_index=1324), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=1329), (SELECT id FROM tags WHERE name='references')),
((SELECT id FROM questions WHERE order_index=1335), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1344), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1347), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 1301 AND 1350;