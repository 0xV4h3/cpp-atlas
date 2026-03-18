-- =============================================================
-- CppAtlas Content Patch: STL & Modern C++ Pack 04 (Part 1)
-- Date: 2026-03-16
-- Questions: 50 (order_index 601..650)
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
('STL Containers in Practice Quiz', 'Core STL containers, iterator categories, and complexity intuition', 10, 2, 'standard', 1, 0, 1),
('Algorithms & Lambdas Practice Quiz', 'Algorithm usage with predicates, captures, and functional style patterns', 10, 3, 'standard', 1, 0, 1),
('Modern C++ Features Quiz', 'C++11/14/17/20 features with move semantics and vocabulary types', 12, 3, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 10 (STL Quiz): 10 questions (601..610)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which container offers contiguous storage and fast random access?', NULL, 'std::vector stores elements contiguously and provides O(1) random access.', 1, 10, 601, 1, 'Think dynamic array.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=601), 'std::vector', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=601), 'std::list', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=601), 'std::forward_list', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=601), 'std::map', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'true_false', 'std::array has fixed size known at compile time.', NULL, 'True.', 1, 10, 602, 1, 'Static-size container.', 'https://cppreference.com/w/cpp/container/array.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=602), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=602), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'code_output', 'What is printed?', 'std::vector<int> v{1,2,3};\nstd::cout << v[2];', 'Index 2 is third element: 3.', 1, 10, 603, 1, 'Zero-based indexing.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=603), '3', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=603), '2', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=603), '1', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=603), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which associative container is ordered by key?', NULL, 'std::map is ordered (typically tree-based).', 2, 10, 604, 1, 'Ordered vs unordered maps.', 'https://cppreference.com/w/cpp/container/map.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=604), 'std::map', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=604), 'std::unordered_map', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=604), 'std::vector', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=604), 'std::deque', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'multi_select', 'Select containers that are sequence containers.', NULL, 'vector, deque, list, forward_list, array are sequence containers.', 2, 15, 605, 1, 'Container category.', 'https://www.learncpp.com/cpp-tutorial/stl-containers-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=605), 'std::vector', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=605), 'std::deque', 1, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=605), 'std::set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=605), 'std::list', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'fill_blank', 'std::_____ provides O(1) average key lookup using hashing.', NULL, 'unordered_map.', 2, 10, 606, 1, 'Hash-based associative container.', 'https://cppreference.com/w/cpp/container/unordered_map.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=606), 'unordered_map', 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=606), 'unordered map', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'Which container is optimized for frequent insertion/removal at both ends?', NULL, 'std::deque supports efficient push/pop at both front and back.', 2, 10, 607, 1, 'Double-ended queue.', 'https://cppreference.com/w/cpp/container/deque.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=607), 'std::deque', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=607), 'std::array', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=607), 'std::unordered_set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=607), 'std::multimap', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'true_false', 'std::list provides random-access iterators.', NULL, 'False. std::list provides bidirectional iterators.', 2, 10, 608, 1, 'Iterator category differs.', 'https://cppreference.com/w/cpp/container/list.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=608), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=608), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'mcq', 'What does container adaptor std::priority_queue expose?', NULL, 'It exposes highest-priority element at top (max-heap by default).', 2, 10, 609, 1, 'Top element semantics.', 'https://www.studyplan.dev/pro-cpp/priority-queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=609), 'Fast access to highest-priority element', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=609), 'Sorted traversal iterator interface', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=609), 'Key-value storage by hash', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=609), 'Random element erase by iterator', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(10, 10, 'code_output', 'What is printed?', 'std::deque<int> d{4,5,6};\nstd::cout << d.front() << d.back();', 'front() is 4, back() is 6 => output 46.', 1, 10, 610, 1, 'Front/back access.', 'https://cppreference.com/w/cpp/container/deque.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=610), '46', 1, 1),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=610), '45', 0, 2),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=610), '56', 0, 3),
((SELECT id FROM questions WHERE quiz_id=10 AND order_index=610), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Existing Quiz 12: Modern C++ Quiz (10 questions, 611..620)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'What does std::move do?', NULL, 'std::move casts to rvalue reference; it does not move by itself.', 2, 10, 611, 1, 'Cast, not transfer.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=611), 'Casts to rvalue reference', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=611), 'Physically copies memory to heap', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=611), 'Always destroys source object', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=611), 'Allocates smart pointer', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'true_false', 'Rvalue references are declared using && syntax.', NULL, 'True.', 2, 10, 612, 1, 'Reference category.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=612), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=612), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'code_output', 'What is printed?', 'std::string s{"abc"};\nstd::string t{std::move(s)};\nstd::cout << t;', 'Moved-to string t contains original value "abc".', 2, 10, 613, 1, 'Moved-from state valid but unspecified.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=613), 'abc', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=613), 'empty always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=613), 'compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=613), 'null', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'Which C++ standard introduced structured bindings?', NULL, 'C++17.', 2, 10, 614, 1, 'auto [a,b] syntax.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c17/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=614), 'C++17', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=614), 'C++11', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=614), 'C++14', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=614), 'C++20', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'multi_select', 'Select vocabulary types introduced/expanded in modern C++.', NULL, 'optional, variant, any are modern vocabulary types.', 3, 15, 615, 1, 'Value representation helpers.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=615), 'std::optional', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=615), 'std::variant', 1, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=615), 'std::any', 1, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=615), 'std::fstream_only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'fill_blank', 'Move constructor parameter type is usually ClassName&& (an ____ reference).', NULL, 'rvalue.', 2, 10, 616, 1, 'Category of temporary resources.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=616), 'rvalue', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'What is one reason move semantics can improve performance?', NULL, 'It may transfer ownership of resources instead of deep copying them.', 3, 12, 617, 1, 'Resource transfer vs copy.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=617), 'Avoiding expensive deep copies in some cases', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=617), 'Disabling destructors', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=617), 'Forcing stack allocation only', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=617), 'Replacing all templates', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'true_false', 'C++20 introduced ranges support in the standard library.', NULL, 'True.', 2, 10, 618, 1, 'Ranges era.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=618), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=618), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'code_output', 'What is printed?', 'auto [x,y] = std::pair{2,4};\nstd::cout << x+y;', 'Structured bindings unpack pair; sum is 6.', 2, 10, 619, 1, 'Pair decomposition.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c17/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=619), '6', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=619), '24', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=619), '2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=619), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(12, 12, 'mcq', 'Best practice after moving from an object is to assume it is ____.', NULL, 'Valid but unspecified state; reassign or destroy safely.', 3, 12, 620, 1, 'Post-move rule.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=620), 'valid but unspecified', 1, 1),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=620), 'always empty', 0, 2),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=620), 'always null pointer', 0, 3),
((SELECT id FROM questions WHERE quiz_id=12 AND order_index=620), 'immediately destroyed', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: STL Containers in Practice Quiz (10 questions, 621..630)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container is singly-linked?', NULL, 'std::forward_list is singly-linked.', 2, 10, 621, 1, 'One-direction links.', 'https://cppreference.com/w/cpp/container/forward_list.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=621), 'std::forward_list', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=621), 'std::list', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=621), 'std::vector', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=621), 'std::deque', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::set stores unique keys in sorted order.', NULL, 'True.', 2, 10, 622, 1, 'Ordered unique keys.', 'https://cppreference.com/w/cpp/container/set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=622), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=622), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::set<int> s{5,2,5,1};\nstd::cout << s.size();', 'Duplicates are removed in set -> size 3.', 2, 10, 623, 1, 'Uniqueness property.', 'https://cppreference.com/w/cpp/container/set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=623), '3', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=623), '4', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=623), '2', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=623), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container allows duplicate keys while remaining ordered?', NULL, 'std::multiset allows duplicates in sorted order.', 2, 10, 624, 1, 'Duplicate ordered keys.', 'https://cppreference.com/w/cpp/container/multiset.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=624), 'std::multiset', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=624), 'std::set', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=624), 'std::unordered_set', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=624), 'std::array', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select container adaptors in STL.', NULL, 'stack, queue, priority_queue are container adaptors.', 2, 15, 625, 1, 'Adaptor category.', 'https://www.learncpp.com/cpp-tutorial/the-standard-library/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=625), 'std::stack', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=625), 'std::queue', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=625), 'std::priority_queue', 1, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=625), 'std::vector', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'Container adaptor with FIFO semantics is std::_____.', NULL, 'queue.', 1, 10, 626, 1, 'First-in, first-out.', 'https://www.studyplan.dev/pro-cpp/queue');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=626), 'queue', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which associative container is hash-based and allows duplicate keys?', NULL, 'std::unordered_multimap.', 3, 12, 627, 1, 'Hash + duplicate keys.', 'https://cppreference.com/w/cpp/container/unordered_multimap.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=627), 'std::unordered_multimap', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=627), 'std::map', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=627), 'std::multiset', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=627), 'std::array', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::unordered_set preserves insertion order.', NULL, 'False. Order is unspecified bucket/hash dependent.', 2, 10, 628, 1, 'Not insertion-ordered.', 'https://cppreference.com/w/cpp/container/unordered_set.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=628), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=628), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container is best fit for LIFO adaptor behavior?', NULL, 'std::stack models LIFO.', 1, 10, 629, 1, 'Last in, first out.', 'https://www.studyplan.dev/pro-cpp/stacks');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=629), 'std::stack', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=629), 'std::queue', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=629), 'std::priority_queue', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=629), 'std::multimap', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::queue<int> q; q.push(1); q.push(2); std::cout<<q.front();', 'queue front is first inserted: 1.', 1, 10, 630, 1, 'FIFO front element.', 'https://www.studyplan.dev/pro-cpp/queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=630), '1', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=630), '2', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=630), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='STL Containers in Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=630), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Algorithms & Lambdas Practice Quiz (10 questions, 631..640)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'What does std::find_if return?', NULL, 'Iterator to first matching element or end iterator if none.', 2, 10, 631, 1, 'Predicate-based search.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=631), 'Iterator to first element satisfying predicate', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=631), 'Count of all matching elements', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=631), 'Boolean always', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=631), 'Sorted range', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'A lambda can capture outer scope variables.', NULL, 'True.', 1, 10, 632, 1, 'Capture list.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=632), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=632), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::vector<int> v{1,2,3};\nint s=0; std::for_each(v.begin(), v.end(), [&](int x){ s += x; });\nstd::cout<<s;', 'Lambda captures s by reference, sum is 6.', 2, 10, 633, 1, 'Capture by reference [&].', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=633), '6', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=633), '3', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=633), '0', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=633), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm is typically used for sorted-range binary search checks?', NULL, 'std::binary_search.', 2, 10, 634, 1, 'Requires sorted range.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=634), 'std::binary_search', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=634), 'std::accumulate', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=634), 'std::count', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=634), 'std::partition', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select counting-related STL algorithms.', NULL, 'count and count_if are counting algorithms.', 2, 15, 635, 1, 'Count with/without predicate.', 'https://www.studyplan.dev/pro-cpp/counting-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=635), 'std::count', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=635), 'std::count_if', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=635), 'std::find_if', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=635), 'std::sort', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'Lambda capture list syntax uses square ____.', NULL, 'brackets.', 1, 10, 636, 1, '[] around captures.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=636), 'brackets', 1),
((SELECT id FROM questions WHERE order_index=636), '[]', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'What does std::minmax_element return?', NULL, 'Pair of iterators to min and max elements.', 3, 12, 637, 1, 'Two iterators result.', 'https://www.studyplan.dev/pro-cpp/minimum-maximum-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=637), 'Pair of iterators to minimum and maximum', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=637), 'Single iterator to middle element', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=637), 'Count of min and max occurrences', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=637), 'Sorted copy of range', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'Capturing by value copies the captured variable state at lambda creation.', NULL, 'True.', 2, 10, 638, 1, 'Value capture snapshot.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=638), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=638), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm checks whether all elements satisfy a predicate?', NULL, 'std::all_of.', 2, 10, 639, 1, 'all_of/any_of/none_of family.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=639), 'std::all_of', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=639), 'std::find', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=639), 'std::remove', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=639), 'std::stable_sort', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'auto f = [](int x){ return x*3; };\nstd::cout << f(4);', 'Lambda returns 12.', 1, 10, 640, 1, 'Anonymous callable.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=640), '12', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=640), '8', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=640), '4', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Algorithms & Lambdas Practice Quiz' ORDER BY id DESC LIMIT 1) AND order_index=640), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- New Quiz: Modern C++ Features Quiz (10 questions, 641..650)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which standard introduced generic lambdas with auto parameters?', NULL, 'C++14 introduced generic lambdas.', 3, 12, 641, 1, 'auto in lambda params.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c14/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=641), 'C++14', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=641), 'C++11', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=641), 'C++17', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=641), 'C++20', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::optional can represent an absent value without sentinel hacks.', NULL, 'True.', 2, 10, 642, 1, 'Maybe-value wrapper.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=642), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=642), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::optional<int> v{5};\nstd::cout << v.value();', 'optional contains 5, value() returns 5.', 2, 10, 643, 1, 'Check presence semantics.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=643), '5', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=643), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=643), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=643), 'nullopt', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'std::variant primarily models what?', NULL, 'A type-safe union among listed alternative types.', 3, 12, 644, 1, 'One-of-many typed value.', 'https://www.studyplan.dev/pro-cpp/variants');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=644), 'Type-safe tagged union of alternatives', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=644), 'Dynamic polymorphism via vtable', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=644), 'Ordered map container', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=644), 'Thread synchronization primitive', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select', 'Select C++17 features from this list.', NULL, 'C++17 includes structured bindings, std::optional, std::variant, if constexpr.', 3, 15, 645, 1, 'Language/library timeline.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c17/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=645), 'Structured bindings', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=645), 'std::optional', 1, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=645), 'Concepts', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=645), 'if constexpr', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'Move assignment operator typically has signature Type& operator=(Type&&) ____.', NULL, 'noexcept is often recommended when possible.', 4, 15, 646, 1, 'Strong move semantics practice.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=646), 'noexcept', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'std::any is most useful when you need ____.', NULL, 'Type-erased value storage for arbitrary copyable types.', 3, 12, 647, 1, 'Type erasure container.', 'https://www.studyplan.dev/pro-cpp/any');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=647), 'Type-erased storage of arbitrary value types', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=647), 'Compile-time variant visitation only', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=647), 'Hash map lookups', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=647), 'Guaranteed no allocation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::move should be used blindly on every variable before return.', NULL, 'False. Overuse can inhibit optimizations and harm clarity; use intentionally.', 3, 12, 648, 1, 'Intentional moves only.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=648), 'True', 0, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=648), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'auto t = std::tuple{1,2,3};\nstd::cout << std::get<1>(t);', 'Tuple index 1 is second element: 2.', 2, 10, 649, 1, 'Zero-based tuple indexing.', 'https://www.studyplan.dev/pro-cpp/tuple');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=649), '2', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=649), '1', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=649), '3', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=649), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'A good modern C++ practice around moves is to ____.', NULL, 'Move when ownership transfer is intended and semantics remain clear.', 3, 12, 650, 1, 'Express ownership intent clearly.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=650), 'Use moves intentionally for ownership transfer', 1, 1),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=650), 'Call std::move on every variable always', 0, 2),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=650), 'Ignore moved-from state rules', 0, 3),
((SELECT id FROM questions WHERE quiz_id=(SELECT id FROM quizzes WHERE title='Modern C++ Features Quiz' ORDER BY id DESC LIMIT 1) AND order_index=650), 'Avoid documenting ownership semantics', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 601 AND 610 THEN 'stl'
            WHEN q.order_index BETWEEN 611 AND 620 THEN 'move-semantics'
            WHEN q.order_index BETWEEN 621 AND 630 THEN 'stl'
            WHEN q.order_index BETWEEN 631 AND 640 THEN 'algorithms'
            WHEN q.order_index BETWEEN 641 AND 650 THEN 'cpp17'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 601 AND 650;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=611), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=612), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=614), (SELECT id FROM tags WHERE name='cpp17')),
((SELECT id FROM questions WHERE order_index=618), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=632), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=633), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=636), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=638), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=640), (SELECT id FROM tags WHERE name='lambdas')),
((SELECT id FROM questions WHERE order_index=641), (SELECT id FROM tags WHERE name='cpp14')),
((SELECT id FROM questions WHERE order_index=645), (SELECT id FROM tags WHERE name='cpp17')),
((SELECT id FROM questions WHERE order_index=646), (SELECT id FROM tags WHERE name='move-semantics')),
((SELECT id FROM questions WHERE order_index=650), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 601 AND 650;