-- =============================================================
-- CppAtlas Content Patch: STL & Modern C++ Pack 04 (Part 2)
-- Date: 2026-03-16
-- Questions: 50 (order_index 651..700)
-- Topics: stl, algorithms, lambdas, move-semantics, cpp11/cpp14/cpp17/cpp20
--
-- IMPORTANT:
-- - No BEGIN/COMMIT (managed by ContentPatchService)
-- - Standardized INSERT INTO questions (12 columns)
-- - SQLite-safe escaping
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

INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Associative Containers Deep Dive Quiz', 'Ordered/unordered associative containers and complexity intuition', 10, 3, 'standard', 1, 0, 1),
('Algorithmic Thinking with STL Quiz', 'Searching, counting, sorting, min/max patterns with iterators', 10, 3, 'standard', 1, 0, 1),
('Move Semantics & Value Categories Quiz', 'Rvalues, moves, ownership transfer, and modern idioms', 12, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 10: STL Quiz (10 questions, 651..660)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which container keeps keys unique but does not guarantee order?', NULL, 'std::unordered_set keeps unique keys with hash-based buckets.', 2, 10, 651, 1, 'Hash-based uniqueness.', 'https://cppreference.com/w/cpp/container/unordered_set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=651), 'std::unordered_set', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=651), 'std::set', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=651), 'std::vector', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=651), 'std::list', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'true_false', 'std::multimap allows duplicate keys.', NULL, 'True.', 2, 10, 652, 1, 'multi* containers allow duplicates.', 'https://cppreference.com/w/cpp/container/multimap.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=652), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=652), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'code_output', 'What is printed?', 'std::unordered_set<int> s{1,2,2,3};\nstd::cout << s.size();', 'Duplicate key 2 is stored once, total size 3.', 2, 10, 653, 1, 'Set uniqueness.', 'https://cppreference.com/w/cpp/container/unordered_set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=653), '3', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=653), '4', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=653), '2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=653), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which adaptor typically provides max element at top by default?', NULL, 'std::priority_queue defaults to max-heap behavior.', 2, 10, 654, 1, 'Default comparator.', 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=654), 'std::priority_queue', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=654), 'std::queue', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=654), 'std::stack', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=654), 'std::array', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'multi_select', 'Select ordered associative containers.', NULL, 'map, multimap, set, multiset are ordered.', 2, 15, 655, 1, 'Tree-based containers.', 'https://www.learncpp.com/cpp-tutorial/stl-containers-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=655), 'std::map', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=655), 'std::set', 1, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=655), 'std::unordered_map', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=655), 'std::multiset', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'fill_blank', 'Container adaptor with LIFO behavior is std::_____.', NULL, 'stack.', 1, 10, 656, 1, 'Last-in first-out.', 'https://www.studyplan.dev/pro-cpp/stacks');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=656), 'stack', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which container has stable iterators for insert/erase in most cases and non-contiguous storage?', NULL, 'std::list is node-based and non-contiguous.', 3, 12, 657, 1, 'Doubly-linked list.', 'https://cppreference.com/w/cpp/container/list.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=657), 'std::list', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=657), 'std::vector', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=657), 'std::array', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=657), 'std::priority_queue', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'true_false', 'std::vector::capacity() is always equal to std::vector::size().', NULL, 'False; capacity may exceed size.', 1, 10, 658, 1, 'Reserved storage vs used elements.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=658), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=658), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which associative container keeps equivalent keys contiguous by order and supports duplicates?', NULL, 'std::multimap supports duplicate ordered keys.', 3, 12, 659, 1, 'Duplicate ordered key-value.', 'https://cppreference.com/w/cpp/container/multimap.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=659), 'std::multimap', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=659), 'std::map', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=659), 'std::unordered_set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=659), 'std::deque', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'code_output', 'What is printed?', 'std::stack<int> st; st.push(3); st.push(7); std::cout << st.top();', 'Top is most recently pushed: 7.', 1, 10, 660, 1, 'LIFO top.', 'https://www.studyplan.dev/pro-cpp/stacks');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=660), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=660), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=660), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=660), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 12: Modern C++ Quiz (10 questions, 661..670)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'Which statement about moved-from objects is correct?', NULL, 'Moved-from objects remain valid but state is unspecified unless documented otherwise.', 3, 12, 661, 1, 'Post-move object rules.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=661), 'Valid but unspecified state', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=661), 'Always null and unusable', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=661), 'Always identical to pre-move state', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=661), 'Destroyed immediately', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'true_false', 'Move assignment should generally guard against self-assignment in robust implementations.', NULL, 'True in many manual-resource classes; depends on implementation style.', 3, 12, 662, 1, 'Defensive rule in custom resource types.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=662), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=662), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'code_output', 'What is printed?', 'std::variant<int,std::string> v{42};\nstd::cout << std::get<int>(v);', 'Variant currently holds int 42.', 2, 10, 663, 1, 'Type-safe union access.', 'https://www.studyplan.dev/pro-cpp/variants');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=663), '42', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=663), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=663), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=663), 'string', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'Which standard first introduced move semantics?', NULL, 'C++11 introduced move semantics and rvalue references.', 2, 10, 664, 1, 'Big modern milestone.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c11/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=664), 'C++11', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=664), 'C++14', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=664), 'C++17', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=664), 'C++20', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'multi_select', 'Select C++20-related features/concepts from list.', NULL, 'Ranges and concepts are associated with C++20.', 3, 15, 665, 1, 'Feature timeline.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=665), 'Ranges', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=665), 'Concepts', 1, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=665), 'Move semantics introduced here', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=665), 'Structured bindings introduced here', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'fill_blank', 'std::_____ can hold a value or no value (empty state).', NULL, 'optional.', 1, 10, 666, 1, 'Maybe-value type.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=666), 'optional', 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=666), 'std::optional', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'What does std::any primarily provide?', NULL, 'Type-erased storage for single value of arbitrary type.', 3, 12, 667, 1, 'Dynamic type-erased holder.', 'https://www.studyplan.dev/pro-cpp/any');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=667), 'Type-erased value container', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=667), 'Compile-time tuple decomposition', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=667), 'Associative map', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=667), 'Mutex wrapper', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'true_false', 'Rvalue references can bind to temporaries.', NULL, 'True.', 2, 10, 668, 1, 'Temporary binding.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=668), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=668), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'code_output', 'What is printed?', 'std::tuple<int,int> t{3,4};\nstd::cout << std::get<0>(t)+std::get<1>(t);', 'Tuple values sum to 7.', 2, 10, 669, 1, 'Tuple element access.', 'https://www.studyplan.dev/pro-cpp/tuple');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=669), '7', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=669), '34', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=669), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=669), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'Best-practice around std::move in return statements is to ____.', NULL, 'Use intentionally; avoid unnecessary moves that may block NRVO/copy elision in some contexts.', 3, 12, 670, 1, 'Measure and understand context.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=670), 'Use deliberately, not mechanically everywhere', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=670), 'Always wrap every return variable in std::move', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=670), 'Never use std::move at all', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=670), 'Use only in C++98', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Associative Containers Deep Dive Quiz (10 questions, 671..680)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container permits duplicate keys and uses hashing?', NULL, 'std::unordered_multimap.', 3, 12, 671, 1, 'Hash + duplicates.', 'https://cppreference.com/w/cpp/container/unordered_multimap.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=671), 'std::unordered_multimap', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=671), 'std::unordered_map', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=671), 'std::map', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=671), 'std::array', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::map lookup is generally logarithmic complexity.', NULL, 'True.', 2, 10, 672, 1, 'Tree-based map.', 'https://cppreference.com/w/cpp/container/map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=672), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=672), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::map<int,int> m{{2,20},{1,10}};\nstd::cout << m.begin()->first;', 'map iterates in key order; first key is 1.', 2, 10, 673, 1, 'Ordered iteration.', 'https://cppreference.com/w/cpp/container/map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=673), '1', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=673), '2', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=673), '10', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=673), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container supports duplicate equivalent keys in ordered form?', NULL, 'std::multiset.', 2, 10, 674, 1, 'Ordered duplicates.', 'https://cppreference.com/w/cpp/container/multiset.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=674), 'std::multiset', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=674), 'std::set', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=674), 'std::unordered_set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=674), 'std::deque', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select unordered associative containers.', NULL, 'unordered_map, unordered_multimap, unordered_set, unordered_multiset.', 2, 15, 675, 1, 'Hash family.', 'https://cppreference.com/w/cpp/container/unordered_map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=675), 'std::unordered_map', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=675), 'std::unordered_multiset', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=675), 'std::multimap', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=675), 'std::unordered_set', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'Ordered associative container storing unique key-value pairs is std::_____.', NULL, 'map.', 1, 10, 676, 1, 'Unique ordered key-value.', 'https://cppreference.com/w/cpp/container/map.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=676), 'map', 1),
((SELECT id FROM questions WHERE order_index=676), 'std::map', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which statement about std::unordered_map average complexity is typically expected?', NULL, 'Average O(1) lookup/insert/erase under good hashing assumptions.', 3, 12, 677, 1, 'Average-case hash complexity.', 'https://cppreference.com/w/cpp/container/unordered_map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=677), 'Average constant-time lookup', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=677), 'Always logarithmic lookup', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=677), 'Always linear lookup', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=677), 'Compile-time lookup only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::set and std::map are typically implemented with tree-based structures preserving order.', NULL, 'True.', 2, 10, 678, 1, 'Ordered node-based structures.', 'https://cppreference.com/w/cpp/container/set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=678), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=678), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container supports one key mapped to potentially multiple values in ordered mode?', NULL, 'std::multimap.', 2, 10, 679, 1, 'Duplicate key-value associations.', 'https://cppreference.com/w/cpp/container/multimap.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=679), 'std::multimap', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=679), 'std::map', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=679), 'std::unordered_set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=679), 'std::priority_queue', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::unordered_map<int,int> m; m[1]=10; m[1]=20; std::cout<<m[1];', 'Same key overwritten: value becomes 20.', 2, 10, 680, 1, 'Unique keys in map.', 'https://cppreference.com/w/cpp/container/unordered_map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=680), '20', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=680), '10', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=680), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Associative Containers Deep Dive Quiz' ORDER BY id DESC LIMIT 1) AND order_index=680), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Algorithmic Thinking with STL Quiz (10 questions, 681..690)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm sorts a range in ascending order by default?', NULL, 'std::sort.', 1, 10, 681, 1, 'Common sorting algorithm.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=681), 'std::sort', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=681), 'std::count', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=681), 'std::any_of', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=681), 'std::tuple', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::binary_search requires a sorted range.', NULL, 'True.', 2, 10, 682, 1, 'Precondition.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=682), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=682), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::vector<int> v{3,1,2}; std::sort(v.begin(), v.end()); std::cout<<v[0]<<v[1]<<v[2];', 'After sort: 1 2 3.', 2, 10, 683, 1, 'Sorted ascending.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=683), '123', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=683), '312', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=683), '321', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=683), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm counts elements matching predicate?', NULL, 'std::count_if.', 2, 10, 684, 1, 'Predicate-based counting.', 'https://www.studyplan.dev/pro-cpp/counting-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=684), 'std::count_if', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=684), 'std::find', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=684), 'std::remove_if', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=684), 'std::merge', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select algorithms related to min/max queries.', NULL, 'min_element, max_element, minmax_element.', 2, 15, 685, 1, 'Extremum algorithms.', 'https://www.studyplan.dev/pro-cpp/minimum-maximum-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=685), 'std::min_element', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=685), 'std::max_element', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=685), 'std::minmax_element', 1, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=685), 'std::count', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'Algorithm returning true if any element matches predicate is std::_____.', NULL, 'any_of.', 2, 10, 686, 1, 'all_of/any_of/none_of trio.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=686), 'any_of', 1),
((SELECT id FROM questions WHERE order_index=686), 'std::any_of', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which iterator category does std::sort require?', NULL, 'Random-access iterators.', 3, 12, 687, 1, 'Not all containers qualify.', 'https://www.learncpp.com/cpp-tutorial/stl-iterators-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=687), 'Random-access iterators', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=687), 'Input iterators only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=687), 'Output iterators only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=687), 'No iterators needed', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'Lambda capture [=] captures local variables by value by default.', NULL, 'True.', 2, 10, 688, 1, 'Capture defaults.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=688), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=688), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm family is focused on locating elements?', NULL, 'find/find_if/search and related.', 2, 10, 689, 1, 'Search intent.', 'https://www.studyplan.dev/pro-cpp/search-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=689), 'Search/find algorithms', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=689), 'Allocation algorithms', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=689), 'Thread scheduling algorithms', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=689), 'I/O formatting algorithms', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'int factor=2; auto mul=[factor](int x){return x*factor;}; std::cout<<mul(5);', 'factor captured by value (2), result 10.', 2, 10, 690, 1, 'Value capture semantics.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=690), '10', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=690), '5', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=690), '2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithmic Thinking with STL Quiz' ORDER BY id DESC LIMIT 1) AND order_index=690), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Move Semantics & Value Categories Quiz (10 questions, 691..700)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which category is a named variable expression usually?', NULL, 'Named variable expressions are lvalues.', 2, 10, 691, 1, 'Value category basics.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=691), 'lvalue', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=691), 'rvalue always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=691), 'xvalue only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=691), 'prvalue only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::move(x) changes x''s type permanently.', NULL, 'False. It is a cast expression result; x''s declared type remains unchanged.', 3, 12, 692, 1, 'Expression category vs declared type.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=692), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=692), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::string a{"hi"}; std::string b{std::move(a)}; std::cout<<b;', 'Moved-to object b contains transferred payload "hi".', 2, 10, 693, 1, 'Observe destination value.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=693), 'hi', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=693), 'empty always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=693), 'compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=693), 'null', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'When implementing move assignment manually, a key concern is ____.', NULL, 'Releasing current resources safely and leaving source valid.', 4, 15, 694, 1, 'Resource ownership discipline.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=694), 'Safe resource transfer and valid states', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=694), 'Disabling all destructors', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=694), 'Converting class to template automatically', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=694), 'Avoiding noexcept forever', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select', 'Select valid move-semantics principles.', NULL, 'Move operations transfer resources, moved-from object remains valid, and noexcept often helps containers.', 4, 15, 695, 1, 'Core semantics.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=695), 'Moved-from object should remain valid', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=695), 'Move can avoid deep copy in many cases', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=695), 'std::move alone performs resource transfer by itself', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=695), 'noexcept move may improve container behavior', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'An expression that refers to a persistent object identity is generally an ____.', NULL, 'lvalue.', 2, 10, 696, 1, 'Value categories.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=696), 'lvalue', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which standard introduced lambdas and move semantics?', NULL, 'C++11.', 2, 10, 697, 1, 'Foundational modern release.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c11/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=697), 'C++11', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=697), 'C++14', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=697), 'C++17', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=697), 'C++20', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'An object can be moved from multiple times as long as code handles valid-but-unspecified state correctly.', NULL, 'True in principle; semantics depend on type and invariants.', 4, 15, 698, 1, 'Post-move discipline.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=698), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=698), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'auto l = [](int a,int b){ return a>b ? a : b; }; std::cout<<l(2,9);', 'Lambda returns max: 9.', 1, 10, 699, 1, 'Simple lambda expression.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=699), '9', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=699), '2', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=699), '29', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=699), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Best-practice for modern C++ evolution in codebase is to ____.', NULL, 'Adopt features intentionally with tests and clear style guidelines.', 3, 12, 700, 1, 'Incremental modernization.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=700), 'Introduce features incrementally with tests', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=700), 'Rewrite everything at once without validation', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=700), 'Ignore moved-from object semantics', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1) AND order_index=700), 'Avoid documentation for new idioms', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 651 AND 660 THEN 'stl'
            WHEN q.order_index BETWEEN 661 AND 670 THEN 'move-semantics'
            WHEN q.order_index BETWEEN 671 AND 680 THEN 'stl'
            WHEN q.order_index BETWEEN 681 AND 690 THEN 'algorithms'
            WHEN q.order_index BETWEEN 691 AND 700 THEN 'move-semantics'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 651 AND 700;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=661), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=664), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=665), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=668), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=670), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=632), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=633), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=636), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=638), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=640), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=688), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=699), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=700), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=685), (SELECT id FROM tags WHERE name='stl')),
((SELECT id FROM questions WHERE order_index=689), (SELECT id FROM tags WHERE name='stl'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 651 AND 700;