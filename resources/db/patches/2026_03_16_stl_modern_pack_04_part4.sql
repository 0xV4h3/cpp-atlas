-- =============================================================
-- CppAtlas Content Patch: STL & Modern C++ Pack 04 (Part 4)
-- Date: 2026-03-16
-- Questions: 50 (order_index 751..800)
-- Rule: ONLY NEW quizzes for stl_modern_pack_04
--
-- NOTE:
-- - Explicitly adds questions to existing "Move Semantics Deep Practice Quiz"
--   because user reported it had 0 questions.
-- - No BEGIN/COMMIT (handled by ContentPatchService).
-- - Standardized INSERT INTO questions (12 columns).
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('stl', '#50FA7B'),
('algorithms', '#50FA7B'),
('lambdas', '#FF79C6'),
('move-semantics', '#FFB86C'),
('cpp11', '#FFB86C'),
('cpp14', '#FFB86C'),
('cpp17', '#FFB86C'),
('cpp20', '#FFB86C'),
('best-practices', '#89D185');

-- New quizzes for part4 + keep using already created Move Semantics Deep Practice Quiz
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Ranges & Modern Algorithms Quiz', 'C++20 ranges mindset and modern algorithm usage patterns', 12, 4, 'standard', 1, 0, 1),
('Container Adaptors & Utilities Quiz', 'queue/stack/priority_queue and practical utility usage', 10, 3, 'standard', 1, 0, 1),
('Lambda Patterns & Captures Quiz', 'Capture strategies and lambda composition in realistic scenarios', 12, 3, 'standard', 1, 0, 1),
('STL Performance & Best Practices Quiz', 'Tradeoffs, complexity, memory behavior, and safe STL idioms', 10, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- A) Move Semantics Deep Practice Quiz (10 questions, 751..760)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'What is the primary purpose of a move constructor?',
 NULL,
 'It transfers ownership/resources from a temporary or expiring object instead of deep-copying.',
 3, 12, 751, 1, 'Resource transfer, not duplication.',
 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=751), 'Transfer resources from source object', 1, 1),
((SELECT id FROM questions WHERE order_index=751), 'Destroy source immediately', 0, 2),
((SELECT id FROM questions WHERE order_index=751), 'Force heap allocation', 0, 3),
((SELECT id FROM questions WHERE order_index=751), 'Disable copy constructor', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false',
 'std::move itself performs data movement.',
 NULL,
 'False. std::move is a cast to an rvalue expression; actual move occurs in move ctor/assignment.',
 3, 12, 752, 1, 'Cast vs operation.',
 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=752), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=752), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'std::string a{"hello"};\nstd::string b{std::move(a)};\nstd::cout << b.size();',
 'b receives moved content, so size is 5.',
 2, 10, 753, 1, 'Inspect moved-to object.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=753), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=753), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=753), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=753), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'What is generally true about moved-from objects?',
 NULL,
 'They must remain valid (destructible/assignable), but their state is unspecified unless documented.',
 3, 12, 754, 1, 'Validity != original value.',
 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=754), 'Valid but unspecified state', 1, 1),
((SELECT id FROM questions WHERE order_index=754), 'Always empty string', 0, 2),
((SELECT id FROM questions WHERE order_index=754), 'Immediately destroyed by language', 0, 3),
((SELECT id FROM questions WHERE order_index=754), 'Always UB to use', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select',
 'Select valid move-semantics practices.',
 NULL,
 'Implement noexcept move where possible, preserve invariants, and avoid unnecessary std::move.',
 4, 15, 755, 1, 'Design + exception guarantees.',
 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=755), 'Keep moved-from object valid', 1, 1),
((SELECT id FROM questions WHERE order_index=755), 'Mark move operations noexcept when appropriate', 1, 2),
((SELECT id FROM questions WHERE order_index=755), 'Call std::move on everything blindly', 0, 3),
((SELECT id FROM questions WHERE order_index=755), 'Preserve class invariants after move', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank',
 'Rvalue references use the token ____ in declarations.',
 NULL,
 'Double ampersand: &&.',
 2, 10, 756, 1, 'Syntax detail.',
 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=756), '&&', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Why can noexcept on move operations matter for containers?',
 NULL,
 'Containers may prefer move during reallocation when it is noexcept for strong exception safety.',
 4, 15, 757, 1, 'Container strategy choice.',
 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=757), 'Can enable safer/faster move paths in containers', 1, 1),
((SELECT id FROM questions WHERE order_index=757), 'Disables move entirely', 0, 2),
((SELECT id FROM questions WHERE order_index=757), 'Only affects debug prints', 0, 3),
((SELECT id FROM questions WHERE order_index=757), 'Makes copy constructor deleted automatically', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false',
 'A named variable expression is an lvalue even if its type is rvalue-reference.',
 NULL,
 'True: value category of named variable expression is lvalue.',
 4, 15, 758, 1, 'Named-variable rule.',
 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=758), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=758), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'std::vector<std::string> v;\nstd::string s{"x"};\nv.push_back(std::move(s));\nstd::cout << v.size();',
 'One element inserted, size=1.',
 2, 10, 759, 1, 'Container receives moved value.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=759), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=759), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=759), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=759), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics Deep Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Best practice when designing move-enabled classes?',
 NULL,
 'Define clear ownership semantics and test copy/move behavior explicitly.',
 3, 12, 760, 1, 'Semantics before optimization.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=760), 'Document ownership + test moves/copies', 1, 1),
((SELECT id FROM questions WHERE order_index=760), 'Rely on undefined moved-from states', 0, 2),
((SELECT id FROM questions WHERE order_index=760), 'Avoid invariants in move ops', 0, 3),
((SELECT id FROM questions WHERE order_index=760), 'Force std::move in all returns', 0, 4);

-- -------------------------------------------------------------
-- B) Ranges & Modern Algorithms Quiz (10 questions, 761..770)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Ranges in C++20 primarily aim to improve what?',
 NULL,
 'Composability/readability of algorithm pipelines over views and ranges.',
 3, 12, 761, 1, 'Pipeline style processing.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=761), 'Composable, readable algorithm pipelines', 1, 1),
((SELECT id FROM questions WHERE order_index=761), 'Replace all containers', 0, 2),
((SELECT id FROM questions WHERE order_index=761), 'Disable iterators', 0, 3),
((SELECT id FROM questions WHERE order_index=761), 'Runtime reflection only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false',
 'C++20 concepts can improve template diagnostics and constraints clarity.',
 NULL,
 'True.',
 3, 12, 762, 1, 'Constraint expression clarity.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=762), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=762), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'std::vector<int> v{1,2,3,4};\nstd::cout << std::count_if(v.begin(), v.end(), [](int x){return x>2;});',
 'Elements >2 are 3 and 4 => count 2.',
 2, 10, 763, 1, 'Predicate count.',
 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=763), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=763), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=763), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=763), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which algorithm is suitable to locate first occurrence of a subrange in a range?',
 NULL,
 'std::search.',
 3, 12, 764, 1, 'Subsequence search.',
 'https://www.studyplan.dev/pro-cpp/search-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=764), 'std::search', 1, 1),
((SELECT id FROM questions WHERE order_index=764), 'std::count', 0, 2),
((SELECT id FROM questions WHERE order_index=764), 'std::shuffle', 0, 3),
((SELECT id FROM questions WHERE order_index=764), 'std::merge', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select',
 'Select useful algorithm categories for interview-style STL tasks.',
 NULL,
 'Searching, counting, sorting, min/max are frequent.',
 2, 15, 765, 1, 'Core categories.',
 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=765), 'Searching algorithms', 1, 1),
((SELECT id FROM questions WHERE order_index=765), 'Counting algorithms', 1, 2),
((SELECT id FROM questions WHERE order_index=765), 'Min/max algorithms', 1, 3),
((SELECT id FROM questions WHERE order_index=765), 'Only I/O streaming', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank',
 'C++20 introduced _____ to express template requirements declaratively.',
 NULL,
 'concepts.',
 3, 12, 766, 1, 'Constraint mechanism.',
 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=766), 'concepts', 1),
((SELECT id FROM questions WHERE order_index=766), 'concept', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'What does std::any_of return?',
 NULL,
 'True if at least one element satisfies predicate.',
 1, 10, 767, 1, 'Existential check.',
 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=767), 'bool indicating whether any element matches', 1, 1),
((SELECT id FROM questions WHERE order_index=767), 'Iterator to first non-match', 0, 2),
((SELECT id FROM questions WHERE order_index=767), 'Count of all matches', 0, 3),
((SELECT id FROM questions WHERE order_index=767), 'Sorted copy', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'std::sort is stable by default.',
 NULL,
 'False; std::sort is not stable (use std::stable_sort for stability).',
 2, 10, 768, 1, 'Sort stability nuance.',
 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=768), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=768), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which algorithm family is ideal for threshold/filter checks without materializing new containers?',
 NULL,
 'Predicate algorithms (all_of/any_of/none_of/find_if) can evaluate conditions directly on ranges.',
 3, 12, 769, 1, 'Predicate-centric workflows.',
 'https://www.studyplan.dev/pro-cpp/search-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=769), 'Predicate/search algorithms', 1, 1),
((SELECT id FROM questions WHERE order_index=769), 'Container adaptors only', 0, 2),
((SELECT id FROM questions WHERE order_index=769), 'Tuple utilities only', 0, 3),
((SELECT id FROM questions WHERE order_index=769), 'Type traits only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Ranges & Modern Algorithms Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'std::vector<int> v{5,1,7};\nauto it = std::max_element(v.begin(), v.end());\nstd::cout << *it;',
 'Maximum element is 7.',
 2, 10, 770, 1, 'max_element result iterator.',
 'https://www.studyplan.dev/pro-cpp/minimum-maximum-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=770), '7', 1, 1),
((SELECT id FROM questions WHERE order_index=770), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=770), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=770), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- C) Container Adaptors & Utilities Quiz (10 questions, 771..780)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which adaptor provides highest-priority element access by default?',
 NULL,
 'std::priority_queue.',
 2, 10, 771, 1, 'Heap-backed adaptor.',
 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=771), 'std::priority_queue', 1, 1),
((SELECT id FROM questions WHERE order_index=771), 'std::queue', 0, 2),
((SELECT id FROM questions WHERE order_index=771), 'std::stack', 0, 3),
((SELECT id FROM questions WHERE order_index=771), 'std::multimap', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'std::queue supports random access to middle elements.',
 NULL,
 'False; queue exposes front/back operations, not random access.',
 2, 10, 772, 1, 'Restricted interface by design.',
 'https://www.studyplan.dev/pro-cpp/queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=772), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=772), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::queue<int> q; q.push(4); q.push(9); q.pop(); std::cout << q.front();',
 'After popping 4, front is 9.',
 2, 10, 773, 1, 'FIFO progression.',
 'https://www.studyplan.dev/pro-cpp/queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=773), '9', 1, 1),
((SELECT id FROM questions WHERE order_index=773), '4', 0, 2),
((SELECT id FROM questions WHERE order_index=773), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=773), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which adaptor operation reads top element of stack without removing it?',
 NULL,
 'top().',
 1, 10, 774, 1, 'Peek operation.',
 'https://www.studyplan.dev/pro-cpp/stacks');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=774), 'top()', 1, 1),
((SELECT id FROM questions WHERE order_index=774), 'front()', 0, 2),
((SELECT id FROM questions WHERE order_index=774), 'peek_back()', 0, 3),
((SELECT id FROM questions WHERE order_index=774), 'get()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select',
 'Select valid adaptor/container pairings commonly used internally.',
 NULL,
 'queue/deque, stack/deque or vector, priority_queue/vector are common defaults/uses.',
 3, 15, 775, 1, 'Underlying container choice.',
 'https://www.learncpp.com/cpp-tutorial/the-standard-library/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=775), 'std::queue with std::deque', 1, 1),
((SELECT id FROM questions WHERE order_index=775), 'std::stack with std::deque', 1, 2),
((SELECT id FROM questions WHERE order_index=775), 'std::priority_queue with std::vector', 1, 3),
((SELECT id FROM questions WHERE order_index=775), 'std::stack with std::map only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank',
 'std::priority_queue::top() returns the element with highest ____.',
 NULL,
 'priority.',
 1, 10, 776, 1, 'Name says it.', 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=776), 'priority', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which statement about std::deque is correct?',
 NULL,
 'Deque supports efficient push/pop at both ends and random access.',
 2, 10, 777, 1, 'Double-ended behavior.', 'https://www.studyplan.dev/pro-cpp/deque');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=777), 'Efficient insert/remove at both ends', 1, 1),
((SELECT id FROM questions WHERE order_index=777), 'Strict contiguous storage like vector always', 0, 2),
((SELECT id FROM questions WHERE order_index=777), 'No random access operator[]', 0, 3),
((SELECT id FROM questions WHERE order_index=777), 'Only single-ended operations', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'std::stack follows FIFO behavior.',
 NULL,
 'False. Stack is LIFO.',
 1, 10, 778, 1,
 'Last in, first out.', 'https://www.studyplan.dev/pro-cpp/stacks');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=778), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=778), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which method removes top element of std::priority_queue?',
 NULL,
 'pop().',
 1, 10, 779, 1,
 'Classic container pop.', 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=779), 'pop()', 1, 1),
((SELECT id FROM questions WHERE order_index=779), 'erase_top()', 0, 2),
((SELECT id FROM questions WHERE order_index=779), 'remove()', 0, 3),
((SELECT id FROM questions WHERE order_index=779), 'clear_top()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Container Adaptors & Utilities Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::priority_queue<int> pq; pq.push(1); pq.push(5); pq.push(3); std::cout<<pq.top();',
 'Default priority_queue gives largest element first -> 5.',
 2, 10, 780, 1,
 'Max-heap default.', 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=780), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=780), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=780), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=780), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- D) Lambda Patterns & Captures Quiz (10 questions, 781..790)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Capture list [this] in member context captures what?',
 NULL,
 'Captures this pointer (object context).',
 3, 12, 781, 1,
 'Member lambda pattern.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=781), 'this pointer/object context', 1, 1),
((SELECT id FROM questions WHERE order_index=781), 'All locals by value', 0, 2),
((SELECT id FROM questions WHERE order_index=781), 'Only globals', 0, 3),
((SELECT id FROM questions WHERE order_index=781), 'No capture allowed', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false',
 'A mutable lambda allows modifying copies of value-captured variables.',
 NULL,
 'True.',
 3, 12, 782, 1,
 'mutable keyword behavior.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=782), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=782), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'int x=5; auto f=[x]() mutable { x+=2; return x; }; std::cout<<f();',
 'Value-captured x copy becomes 7 inside mutable lambda.',
 3, 12, 783, 1,
 'Copy modified inside closure.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=783), '7', 1, 1),
((SELECT id FROM questions WHERE order_index=783), '5', 0, 2),
((SELECT id FROM questions WHERE order_index=783), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=783), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Why are lambdas convenient with STL algorithms?',
 NULL,
 'They provide inline callable behavior near algorithm invocation.',
 2, 10, 784, 1,
 'Locality of behavior.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=784), 'Inline, local predicate/transform logic', 1, 1),
((SELECT id FROM questions WHERE order_index=784), 'They replace all containers', 0, 2),
((SELECT id FROM questions WHERE order_index=784), 'They disable type checking', 0, 3),
((SELECT id FROM questions WHERE order_index=784), 'They only work in C++20', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select',
 'Select valid lambda capture forms.',
 NULL,
 '[=], [&], [x], [&,x], [this] are valid examples.',
 2, 15, 785, 1,
 'Capture syntax variants.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=785), '[=]', 1, 1),
((SELECT id FROM questions WHERE order_index=785), '[&]', 1, 2),
((SELECT id FROM questions WHERE order_index=785), '[x]', 1, 3),
((SELECT id FROM questions WHERE order_index=785), '[??]', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank',
 'Generic lambda parameters use keyword ____ in parameter list.',
 NULL,
 'auto.',
 2, 10, 786, 1,
 'C++14 generic lambdas.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c14/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=786), 'auto', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Which capture mode is safer when closure outlives local variables?',
 NULL,
 'Capturing by value is often safer than capturing dangling references.',
 3, 12, 787, 1,
 'Lifetime safety.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=787), 'Capture by value (case-dependent safer default)', 1, 1),
((SELECT id FROM questions WHERE order_index=787), 'Capture all by reference always', 0, 2),
((SELECT id FROM questions WHERE order_index=787), 'No captures with mutable references', 0, 3),
((SELECT id FROM questions WHERE order_index=787), 'Capturing globals by pointer only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false',
 'Lambda expression can be assigned to auto variable and invoked later.',
 NULL,
 'True.',
 1, 10, 788, 1,
 'Closure object storage.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=788), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=788), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq',
 'Best-practice when lambda body becomes large/complex?',
 NULL,
 'Extract into named function/function object for readability and reuse.',
 3, 12, 789, 1,
 'Balance brevity vs maintainability.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=789), 'Refactor into named callable when complexity grows', 1, 1),
((SELECT id FROM questions WHERE order_index=789), 'Add more nested lambdas only', 0, 2),
((SELECT id FROM questions WHERE order_index=789), 'Use macros for all captures', 0, 3),
((SELECT id FROM questions WHERE order_index=789), 'Disable tests', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambda Patterns & Captures Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output',
 'What is printed?',
 'std::vector<int> v{1,2,3};\nint c = std::count_if(v.begin(), v.end(), [](int x){ return x>=2; });\nstd::cout<<c;',
 'Elements >=2 are 2 and 3 => 2.',
 2, 10, 790, 1,
 'Predicate threshold.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=790), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=790), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=790), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=790), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- E) STL Performance & Best Practices Quiz (791..800)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'When frequent front insertions are required, which sequence container is often better than vector?',
 NULL,
 'deque/list can be better; deque commonly chosen for random access + both-end operations.',
 3, 12, 791, 1,
 'Operation profile driven choice.', 'https://www.studyplan.dev/pro-cpp/deque');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=791), 'std::deque', 1, 1),
((SELECT id FROM questions WHERE order_index=791), 'std::array', 0, 2),
((SELECT id FROM questions WHERE order_index=791), 'std::unordered_map', 0, 3),
((SELECT id FROM questions WHERE order_index=791), 'std::tuple', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'Choosing containers based on operation complexity and access patterns is a best practice.',
 NULL,
 'True.',
 2, 10, 792, 1,
 'Data-structure fit.', 'https://www.learncpp.com/cpp-tutorial/stl-containers-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=792), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=792), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::vector<int> v; v.reserve(5); std::cout << v.capacity();',
 'reserve(5) requests capacity at least 5 (commonly prints 5).',
 2, 10, 793, 1,
 'Capacity vs size.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=793), 'At least 5 (commonly 5)', 1, 1),
((SELECT id FROM questions WHERE order_index=793), '0 always', 0, 2),
((SELECT id FROM questions WHERE order_index=793), 'Exactly size()', 0, 3),
((SELECT id FROM questions WHERE order_index=793), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Why can pre-reserving vector capacity help performance?',
 NULL,
 'It can reduce reallocations and element moves/copies during growth.',
 3, 12, 794, 1,
 'Amortization control.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=794), 'Fewer reallocations during push_back growth', 1, 1),
((SELECT id FROM questions WHERE order_index=794), 'Forces stable ordering in unordered_map', 0, 2),
((SELECT id FROM questions WHERE order_index=794), 'Disables exceptions', 0, 3),
((SELECT id FROM questions WHERE order_index=794), 'Eliminates all allocations forever', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select',
 'Select performance-aware STL habits.',
 NULL,
 'Choose correct container, reserve when appropriate, and avoid needless copies.',
 3, 15, 795, 1,
 'Data + algorithm fit.', 'https://www.learncpp.com/cpp-tutorial/the-standard-library/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=795), 'Use reserve() when expected growth is known', 1, 1),
((SELECT id FROM questions WHERE order_index=795), 'Prefer algorithm+lambda over manual loops when clearer', 1, 2),
((SELECT id FROM questions WHERE order_index=795), 'Ignore complexity characteristics', 0, 3),
((SELECT id FROM questions WHERE order_index=795), 'Avoid unnecessary temporary copies', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank',
 'In big-O notation, average unordered_map lookup is commonly O(___).',
 NULL,
 '1.',
 2, 10, 796, 1,
 'Average-case hash table behavior.', 'https://cppreference.com/w/cpp/container/unordered_map.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=796), '1', 1),
((SELECT id FROM questions WHERE order_index=796), 'one', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'Which is generally more appropriate for ordered key traversal?',
 NULL,
 'std::map/std::set keep keys ordered.',
 2, 10, 797, 1,
 'Need sorted traversal.', 'https://cppreference.com/w/cpp/container/map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=797), 'std::map', 1, 1),
((SELECT id FROM questions WHERE order_index=797), 'std::unordered_map', 0, 2),
((SELECT id FROM questions WHERE order_index=797), 'std::queue', 0, 3),
((SELECT id FROM questions WHERE order_index=797), 'std::stack', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false',
 'Algorithm preconditions (e.g., sorted range) should be treated as part of API contract.',
 NULL,
 'True.',
 2, 10, 798, 1,
 'Contract-aware programming.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=798), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=798), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq',
 'A reasonable best-practice when uncertain between two containers is to ____.',
 NULL,
 'Prototype/measure with representative workload and validate complexity assumptions.',
 3, 12, 799, 1,
 'Evidence-driven selection.', 'https://www.learncpp.com/cpp-tutorial/the-standard-library/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=799), 'Benchmark/measure realistic workload', 1, 1),
((SELECT id FROM questions WHERE order_index=799), 'Choose randomly', 0, 2),
((SELECT id FROM questions WHERE order_index=799), 'Always pick vector regardless of workload', 0, 3),
((SELECT id FROM questions WHERE order_index=799), 'Ignore memory behavior', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Performance & Best Practices Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output',
 'What is printed?',
 'std::vector<int> v{1,2,3};\nstd::cout << v.size() << ":" << v.capacity();',
 'size is 3; capacity is implementation-defined but >=3.',
 3, 12, 800, 1,
 'Capacity may exceed size.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=800), '3:(>=3)', 1, 1),
((SELECT id FROM questions WHERE order_index=800), '3:0', 0, 2),
((SELECT id FROM questions WHERE order_index=800), '0:3', 0, 3),
((SELECT id FROM questions WHERE order_index=800), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping for 751..800
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 751 AND 760 THEN 'move-semantics'
            WHEN q.order_index BETWEEN 761 AND 770 THEN 'algorithms'
            WHEN q.order_index BETWEEN 771 AND 780 THEN 'stl'
            WHEN q.order_index BETWEEN 781 AND 790 THEN 'lambdas'
            WHEN q.order_index BETWEEN 791 AND 800 THEN 'best-practices'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 751 AND 800;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=751), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=752), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=757), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=761), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=762), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=766), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=772), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=783), (SELECT id FROM tags WHERE name='cpp14')),
((SELECT id FROM questions WHERE order_index=786), (SELECT id FROM tags WHERE name='cpp14')),
((SELECT id FROM questions WHERE order_index=795), (SELECT id FROM tags WHERE name='stl')),
((SELECT id FROM questions WHERE order_index=799), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 751 AND 800;