-- =============================================================
-- CppAtlas Content Patch: STL & Modern C++ Pack 04 (Part 3)
-- Date: 2026-03-16
-- Questions: 50 (order_index 701..750)
-- Rule: ONLY NEW quizzes (no inserts into existing quiz_id 10/12)
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

-- Five NEW quizzes (10 questions each)
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('STL Iterators & Containers Mastery Quiz', 'Iterator categories and practical STL container tradeoffs', 10, 3, 'standard', 1, 0, 1),
('Search & Counting Algorithms Quiz', 'find/search/count/binary search patterns and constraints', 10, 3, 'standard', 1, 0, 1),
('Lambdas in Real Workflows Quiz', 'Capture modes, predicates, and lambda-driven algorithm design', 12, 3, 'standard', 1, 0, 1),
('Move Semantics Deep Practice Quiz', 'rvalues, std::move, move ctor/assignment and post-move state', 12, 4, 'standard', 1, 0, 1),
('Modern C++ Vocabulary Types Quiz', 'optional/variant/any/tuple and modern feature selection', 12, 3, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Quiz A: STL Iterators & Containers Mastery Quiz (701..710)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container guarantees contiguous storage?', NULL, 'std::vector stores elements contiguously.', 1, 10, 701, 1, 'Dynamic array model.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=701), 'std::vector', 1, 1),
((SELECT id FROM questions WHERE order_index=701), 'std::list', 0, 2),
((SELECT id FROM questions WHERE order_index=701), 'std::forward_list', 0, 3),
((SELECT id FROM questions WHERE order_index=701), 'std::map', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::deque guarantees contiguous memory like std::vector.', NULL, 'False. deque is segmented, not fully contiguous.', 2, 10, 702, 1, 'Segmented storage.', 'https://cppreference.com/w/cpp/container/deque.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=702), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=702), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::array<int,3> a{4,5,6};\nstd::cout << a[0] << a[2];', 'Outputs first then last elements: 46.', 1, 10, 703, 1, 'Index 0 and 2.', 'https://cppreference.com/w/cpp/container/array.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=703), '46', 1, 1),
((SELECT id FROM questions WHERE order_index=703), '45', 0, 2),
((SELECT id FROM questions WHERE order_index=703), '56', 0, 3),
((SELECT id FROM questions WHERE order_index=703), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which iterator category does std::list provide?', NULL, 'std::list provides bidirectional iterators.', 2, 10, 704, 1, 'Not random access.', 'https://cppreference.com/w/cpp/container/list.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=704), 'Bidirectional iterator', 1, 1),
((SELECT id FROM questions WHERE order_index=704), 'Random-access iterator', 0, 2),
((SELECT id FROM questions WHERE order_index=704), 'Output iterator only', 0, 3),
((SELECT id FROM questions WHERE order_index=704), 'No iterators', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select containers that are sequence containers.', NULL, 'vector/deque/list/forward_list/array are sequence containers.', 2, 15, 705, 1, 'Category recognition.', 'https://www.learncpp.com/cpp-tutorial/stl-containers-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=705), 'std::vector', 1, 1),
((SELECT id FROM questions WHERE order_index=705), 'std::deque', 1, 2),
((SELECT id FROM questions WHERE order_index=705), 'std::set', 0, 3),
((SELECT id FROM questions WHERE order_index=705), 'std::forward_list', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'Iterator category required by std::sort is ____-access iterator.', NULL, 'random.', 2, 10, 706, 1, 'Sort constraints.', 'https://www.learncpp.com/cpp-tutorial/stl-iterators-overview/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=706), 'random', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container is singly-linked?', NULL, 'std::forward_list.', 2, 10, 707, 1, 'Forward links only.', 'https://cppreference.com/w/cpp/container/forward_list.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=707), 'std::forward_list', 1, 1),
((SELECT id FROM questions WHERE order_index=707), 'std::list', 0, 2),
((SELECT id FROM questions WHERE order_index=707), 'std::deque', 0, 3),
((SELECT id FROM questions WHERE order_index=707), 'std::map', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::array has fixed size and supports random access.', NULL, 'True.', 1, 10, 708, 1, 'Compile-time size.', 'https://cppreference.com/w/cpp/container/array.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=708), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=708), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which container adaptor provides FIFO behavior?', NULL, 'std::queue.', 1, 10, 709, 1, 'First in, first out.', 'https://www.studyplan.dev/pro-cpp/queue');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=709), 'std::queue', 1, 1),
((SELECT id FROM questions WHERE order_index=709), 'std::stack', 0, 2),
((SELECT id FROM questions WHERE order_index=709), 'std::priority_queue', 0, 3),
((SELECT id FROM questions WHERE order_index=709), 'std::set', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='STL Iterators & Containers Mastery Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::vector<int> v{9,8,7};\nstd::cout << v.front() << v.back();', 'front=9, back=7 => 97.', 1, 10, 710, 1, 'Front/back helpers.', 'https://cppreference.com/w/cpp/container/vector.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=710), '97', 1, 1),
((SELECT id FROM questions WHERE order_index=710), '98', 0, 2),
((SELECT id FROM questions WHERE order_index=710), '87', 0, 3),
((SELECT id FROM questions WHERE order_index=710), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Quiz B: Search & Counting Algorithms Quiz (711..720)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm finds first element matching predicate?', NULL, 'std::find_if.', 2, 10, 711, 1, 'Predicate search.', 'https://www.studyplan.dev/pro-cpp/search-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=711), 'std::find_if', 1, 1),
((SELECT id FROM questions WHERE order_index=711), 'std::count_if', 0, 2),
((SELECT id FROM questions WHERE order_index=711), 'std::all_of', 0, 3),
((SELECT id FROM questions WHERE order_index=711), 'std::min_element', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::count_if returns iterator to first match.', NULL, 'False, it returns count.', 1, 10, 712, 1, 'Count vs iterator.', 'https://www.studyplan.dev/pro-cpp/counting-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=712), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=712), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::vector<int> v{1,2,3,4};\nstd::cout << std::count_if(v.begin(), v.end(), [](int x){ return x%2==0; });', 'Even numbers are 2 and 4 => count 2.', 2, 10, 713, 1, 'Predicate counting.', 'https://www.studyplan.dev/pro-cpp/counting-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=713), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=713), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=713), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=713), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'What does std::binary_search return?', NULL, 'Boolean indicating whether value exists in sorted range.', 2, 10, 714, 1, 'Existence check only.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=714), 'bool', 1, 1),
((SELECT id FROM questions WHERE order_index=714), 'iterator always', 0, 2),
((SELECT id FROM questions WHERE order_index=714), 'index position', 0, 3),
((SELECT id FROM questions WHERE order_index=714), 'pair of iterators', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'multi_select', 'Select algorithms that produce iterators to extreme values.', NULL, 'min_element/max_element/minmax_element return iterators.', 2, 15, 715, 1, 'Extremum iterators.', 'https://www.studyplan.dev/pro-cpp/minimum-maximum-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=715), 'std::min_element', 1, 1),
((SELECT id FROM questions WHERE order_index=715), 'std::max_element', 1, 2),
((SELECT id FROM questions WHERE order_index=715), 'std::minmax_element', 1, 3),
((SELECT id FROM questions WHERE order_index=715), 'std::count', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'fill_blank', 'std::all_of returns true only if ____ elements satisfy predicate.', NULL, 'all.', 1, 10, 716, 1, 'Universal quantifier style.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=716), 'all', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm can be used to test if no elements satisfy condition?', NULL, 'std::none_of.', 2, 10, 717, 1, 'Companion of any_of/all_of.', 'https://www.learncpp.com/cpp-tutorial/stl-algorithms-overview/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=717), 'std::none_of', 1, 1),
((SELECT id FROM questions WHERE order_index=717), 'std::find', 0, 2),
((SELECT id FROM questions WHERE order_index=717), 'std::sort', 0, 3),
((SELECT id FROM questions WHERE order_index=717), 'std::unique', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'true_false', 'std::binary_search works correctly on unsorted ranges.', NULL, 'False; sorted precondition is required.', 2, 10, 718, 1, 'Preconditions matter.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=718), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=718), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'mcq', 'Which algorithm is a good first choice for linear search by value?', NULL, 'std::find performs linear search in range.', 1, 10, 719, 1, 'No predicate needed.', 'https://www.studyplan.dev/pro-cpp/search-algorithms');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=719), 'std::find', 1, 1),
((SELECT id FROM questions WHERE order_index=719), 'std::count', 0, 2),
((SELECT id FROM questions WHERE order_index=719), 'std::lower_bound', 0, 3),
((SELECT id FROM questions WHERE order_index=719), 'std::max', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Search & Counting Algorithms Quiz' ORDER BY id DESC LIMIT 1), 10, 'code_output', 'What is printed?', 'std::vector<int> v{2,4,6,8};\nstd::cout << std::binary_search(v.begin(), v.end(), 6);', 'Search finds 6 in sorted range, bool prints 1.', 2, 10, 720, 1, 'Boolean stream output.', 'https://www.studyplan.dev/pro-cpp/binary-search');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=720), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=720), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=720), '6', 0, 3),
((SELECT id FROM questions WHERE order_index=720), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Quiz C: Lambdas in Real Workflows Quiz (721..730)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'What is a lambda expression?', NULL, 'An anonymous function object that can capture context.', 1, 10, 721, 1, 'Inline callable.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=721), 'Anonymous function object', 1, 1),
((SELECT id FROM questions WHERE order_index=721), 'Template specialization syntax', 0, 2),
((SELECT id FROM questions WHERE order_index=721), 'Preprocessor macro', 0, 3),
((SELECT id FROM questions WHERE order_index=721), 'Namespace alias', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'Capture list [=] means capture local variables by value by default.', NULL, 'True.', 1, 10, 722, 1, 'Value default capture.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=722), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=722), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'int a=3; auto f=[a](int x){ return x+a; }; std::cout << f(4);', 'a captured by value as 3, result is 7.', 1, 10, 723, 1, 'Closure stores captured state.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=723), '7', 1, 1),
((SELECT id FROM questions WHERE order_index=723), '4', 0, 2),
((SELECT id FROM questions WHERE order_index=723), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=723), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which capture form commonly captures everything by reference?', NULL, '[&].', 1, 10, 724, 1, 'Reference default capture.', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=724), '[&]', 1, 1),
((SELECT id FROM questions WHERE order_index=724), '[=]', 0, 2),
((SELECT id FROM questions WHERE order_index=724), '[this]', 0, 3),
((SELECT id FROM questions WHERE order_index=724), '[]', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select', 'Select common lambda use-cases in STL context.', NULL, 'Predicates, custom comparators, transformations are common use-cases.', 2, 15, 725, 1, 'Algorithms + callables.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=725), 'Predicate for std::find_if', 1, 1),
((SELECT id FROM questions WHERE order_index=725), 'Comparator for std::sort', 1, 2),
((SELECT id FROM questions WHERE order_index=725), 'Compile-time include guard replacement', 0, 3),
((SELECT id FROM questions WHERE order_index=725), 'Inline transformation logic', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'Lambda introducer and capture list begins with square ____.', NULL, 'brackets.', 1, 10, 726, 1, '[] syntax.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=726), 'brackets', 1),
((SELECT id FROM questions WHERE order_index=726), '[]', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'What does empty capture list [] imply?', NULL, 'No local variables are captured.', 1, 10, 727, 1, 'Stateless lambda (unless globals/static used).', 'https://www.learncpp.com/cpp-tutorial/lambda-captures/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=727), 'No captures from local scope', 1, 1),
((SELECT id FROM questions WHERE order_index=727), 'Capture all by value', 0, 2),
((SELECT id FROM questions WHERE order_index=727), 'Capture this automatically', 0, 3),
((SELECT id FROM questions WHERE order_index=727), 'Lambda cannot be called', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'Generic lambdas (auto parameters) were introduced in C++14.', NULL, 'True.', 2, 10, 728, 1, 'Feature timeline.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c14/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=728), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=728), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Why prefer lambda over separate tiny named function in local algorithm call sometimes?', NULL, 'Improves locality/readability when logic is short and context-specific.', 2, 10, 729, 1, 'Local intent expression.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=729), 'Keeps short logic near call site', 1, 1),
((SELECT id FROM questions WHERE order_index=729), 'Guarantees faster runtime always', 0, 2),
((SELECT id FROM questions WHERE order_index=729), 'Disables captures entirely', 0, 3),
((SELECT id FROM questions WHERE order_index=729), 'Removes need for tests', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Lambdas in Real Workflows Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'auto cmp = [](int a,int b){ return a<b; };\nstd::cout << cmp(2,5);', '2<5 is true, bool prints 1 by default.', 1, 10, 730, 1, 'Comparator lambda.', 'https://www.studyplan.dev/pro-cpp/lambda');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=730), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=730), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=730), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=730), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Quiz D: Move Semantics & Value Categories Quiz (731..740)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which expression category often represents temporary values?', NULL, 'prvalues often represent pure rvalue temporaries.', 3, 12, 731, 1, 'Value category fundamentals.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=731), 'prvalue/rvalue category', 1, 1),
((SELECT id FROM questions WHERE order_index=731), 'lvalue only', 0, 2),
((SELECT id FROM questions WHERE order_index=731), 'constvalue', 0, 3),
((SELECT id FROM questions WHERE order_index=731), 'staticvalue', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::move can enable move constructor selection when available.', NULL, 'True.', 2, 10, 732, 1, 'Cast enabling overload choice.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=732), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=732), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::vector<std::string> v; std::string s{"x"}; v.push_back(std::move(s)); std::cout<<v.size();', 'One element pushed, size is 1.', 2, 10, 733, 1, 'Move into container.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-smart-pointers-move-semantics/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=733), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=733), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=733), '2', 0, 3),
((SELECT id FROM questions WHERE order_index=733), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'A robust move constructor should typically leave source object in ____.', NULL, 'A valid destructible state.', 3, 12, 734, 1, 'Post-move invariant.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=734), 'Valid state (often empty/neutral)', 1, 1),
((SELECT id FROM questions WHERE order_index=734), 'Destroyed state', 0, 2),
((SELECT id FROM questions WHERE order_index=734), 'Arbitrary UB state', 0, 3),
((SELECT id FROM questions WHERE order_index=734), 'Original state guaranteed', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select', 'Select valid statements about move semantics.', NULL, 'Move semantics can reduce copies; source remains valid; std::move is cast.', 3, 15, 735, 1, 'Core principles.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=735), 'std::move is an rvalue cast utility', 1, 1),
((SELECT id FROM questions WHERE order_index=735), 'Move can improve performance for expensive resources', 1, 2),
((SELECT id FROM questions WHERE order_index=735), 'Moved-from object must be immediately destroyed', 0, 3),
((SELECT id FROM questions WHERE order_index=735), 'Move operations should preserve class invariants', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'Named variable expression is typically an ____ value category.', NULL, 'lvalue.', 2, 10, 736, 1, 'Identity-bearing expression.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=736), 'lvalue', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which standard feature pair is strongly associated with C++11 modernization?', NULL, 'Lambdas and move semantics are hallmark C++11 features.', 2, 10, 737, 1, 'Modern baseline.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c11/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=737), 'Lambdas + move semantics', 1, 1),
((SELECT id FROM questions WHERE order_index=737), 'Ranges + concepts', 0, 2),
((SELECT id FROM questions WHERE order_index=737), 'Coroutines + modules', 0, 3),
((SELECT id FROM questions WHERE order_index=737), 'Only std::any', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'Rvalue references can bind to lvalues without std::move/cast.', NULL, 'False.', 3, 12, 738, 1, 'Binding rules.', 'https://www.learncpp.com/cpp-tutorial/rvalue-references/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=738), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=738), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'What helps containers prefer move over copy during reallocation?', NULL, 'noexcept move operations often allow stronger move path choices.', 4, 15, 739, 1, 'Exception guarantees interplay.', 'https://www.learncpp.com/cpp-tutorial/move-constructors-and-move-assignment/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=739), 'noexcept move constructor/assignment', 1, 1),
((SELECT id FROM questions WHERE order_index=739), 'Deleting copy constructor only', 0, 2),
((SELECT id FROM questions WHERE order_index=739), 'Using macros', 0, 3),
((SELECT id FROM questions WHERE order_index=739), 'Global variables', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Move Semantics & Value Categories Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::string a{"ab"}; auto b = std::move(a); std::cout << b.size();', 'Moved-to string b has size 2.', 2, 10, 740, 1, 'Observe destination object.', 'https://www.learncpp.com/cpp-tutorial/stdmove/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=740), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=740), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=740), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=740), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Quiz E: Modern C++ Vocabulary Types Quiz (741..750)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which type models an optional value?', NULL, 'std::optional.', 1, 10, 741, 1, 'Maybe-value semantics.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=741), 'std::optional', 1, 1),
((SELECT id FROM questions WHERE order_index=741), 'std::vector', 0, 2),
((SELECT id FROM questions WHERE order_index=741), 'std::queue', 0, 3),
((SELECT id FROM questions WHERE order_index=741), 'std::set', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::variant can hold one of several alternative types at a time.', NULL, 'True.', 2, 10, 742, 1, 'Tagged union concept.', 'https://www.studyplan.dev/pro-cpp/variants');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=742), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=742), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::variant<int,std::string> v{"ok"};\nstd::cout << std::get<std::string>(v);', 'Variant currently stores std::string "ok".', 2, 10, 743, 1, 'Type-safe access.', 'https://www.studyplan.dev/pro-cpp/variants');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=743), 'ok', 1, 1),
((SELECT id FROM questions WHERE order_index=743), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=743), 'int', 0, 3),
((SELECT id FROM questions WHERE order_index=743), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'std::any is useful when you need what capability?', NULL, 'Store values of arbitrary types with runtime type inspection/cast.', 3, 12, 744, 1, 'Type-erased holder.', 'https://www.studyplan.dev/pro-cpp/any');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=744), 'Store arbitrary typed values in one container/field', 1, 1),
((SELECT id FROM questions WHERE order_index=744), 'Compile-time variant visitation only', 0, 2),
((SELECT id FROM questions WHERE order_index=744), 'Automatic move assignment generation', 0, 3),
((SELECT id FROM questions WHERE order_index=744), 'Key ordering in maps', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'multi_select', 'Select tuple-related valid statements.', NULL, 'std::tuple stores heterogenous fixed-size collections; std::get accesses by index/type (where unique).', 2, 15, 745, 1, 'Heterogeneous product type.', 'https://www.studyplan.dev/pro-cpp/tuple');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=745), 'std::tuple can store different types together', 1, 1),
((SELECT id FROM questions WHERE order_index=745), 'std::get<0>(t) accesses first element', 1, 2),
((SELECT id FROM questions WHERE order_index=745), 'Tuple size is dynamic at runtime', 0, 3),
((SELECT id FROM questions WHERE order_index=745), 'Structured bindings can unpack tuple values', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'fill_blank', 'std::_____ provides type-safe union-style alternatives.', NULL, 'variant.', 2, 10, 746, 1, 'One-of-many types.', 'https://www.studyplan.dev/pro-cpp/variants');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=746), 'variant', 1),
((SELECT id FROM questions WHERE order_index=746), 'std::variant', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Which modern standard is associated with ranges and concepts?', NULL, 'C++20.', 2, 10, 747, 1, 'Feature timeline.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c20/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=747), 'C++20', 1, 1),
((SELECT id FROM questions WHERE order_index=747), 'C++11', 0, 2),
((SELECT id FROM questions WHERE order_index=747), 'C++14', 0, 3),
((SELECT id FROM questions WHERE order_index=747), 'C++17', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'true_false', 'std::optional<T> can be empty (no T value present).', NULL, 'True.', 1, 10, 748, 1, 'Disengaged state.', 'https://www.studyplan.dev/pro-cpp/optional');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=748), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=748), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'code_output', 'What is printed?', 'std::tuple<int,int,int> t{1,2,3};\nstd::cout << std::get<2>(t);', 'Index 2 is third element: 3.', 1, 10, 749, 1, 'Zero-based tuple index.', 'https://www.studyplan.dev/pro-cpp/tuple');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=749), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=749), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=749), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=749), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Modern C++ Vocabulary Types Quiz' ORDER BY id DESC LIMIT 1), 12, 'mcq', 'Good practice when adopting modern vocabulary types in legacy code?', NULL, 'Migrate incrementally with tests and clear semantic mapping.', 3, 12, 750, 1, 'Safe modernization.', 'https://www.learncpp.com/cpp-tutorial/introduction-to-c17/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=750), 'Incremental migration with tests', 1, 1),
((SELECT id FROM questions WHERE order_index=750), 'Replace everything in one commit without validation', 0, 2),
((SELECT id FROM questions WHERE order_index=750), 'Avoid documenting behavior changes', 0, 3),
((SELECT id FROM questions WHERE order_index=750), 'Ban optional/variant/any entirely', 0, 4);

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
            WHEN q.order_index BETWEEN 701 AND 710 THEN 'stl'
            WHEN q.order_index BETWEEN 711 AND 720 THEN 'algorithms'
            WHEN q.order_index BETWEEN 721 AND 730 THEN 'lambdas'
            WHEN q.order_index BETWEEN 731 AND 740 THEN 'move-semantics'
            WHEN q.order_index BETWEEN 741 AND 750 THEN 'cpp17'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 651 AND 750;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=664), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=665), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=669), (SELECT id FROM tags WHERE name='cpp17')),
((SELECT id FROM questions WHERE order_index=670), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=721), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=728), (SELECT id FROM tags WHERE name='cpp14')),
((SELECT id FROM questions WHERE order_index=730), (SELECT id FROM tags WHERE name='algorithms')),
((SELECT id FROM questions WHERE order_index=737), (SELECT id FROM tags WHERE name='cpp11')),
((SELECT id FROM questions WHERE order_index=747), (SELECT id FROM tags WHERE name='cpp20')),
((SELECT id FROM questions WHERE order_index=750), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 651 AND 750;