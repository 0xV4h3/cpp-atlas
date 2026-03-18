-- =============================================================
-- CppAtlas Content Patch: Memory & Concurrency Pack 05 (Part 2)
-- Date: 2026-03-16
-- Questions: 50 (order_index 956..1005)
--
-- Rules:
-- - No BEGIN/COMMIT (ContentPatchService manages transactions)
-- - Standardized INSERT INTO questions (12 columns)
-- - SQLite-safe quoting
-- - For part2: ONLY NEW quizzes (no additional questions to old Memory Management Quiz)
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('memory', '#F14C4C'),
('smart-pointers', '#F14C4C'),
('concurrency', '#8BE9FD'),
('best-practices', '#89D185');

-- NEW quizzes for part2
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Memory Model Essentials Quiz', 'Stack/heap, manual management pitfalls, copy/move ownership implications', 11, 3, 'standard', 1, 0, 1),
('Smart Pointer Patterns Quiz', 'Practical ownership graphs and weak/shared interactions', 11, 4, 'standard', 1, 0, 1),
('Thread Coordination Quiz', 'Condition variables, futures/promises, and async task orchestration', 13, 4, 'standard', 1, 0, 1),
('Locking Strategies Quiz', 'Mutex flavors, lock wrappers and deadlock-safe locking plans', 13, 4, 'standard', 1, 0, 1),
('Atomic Design Quiz', 'Atomic operations, atomic_ref usage and correctness boundaries', 13, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- A) Memory Model Essentials Quiz (956..965)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which memory region typically stores local automatic variables?',
 NULL,
 'Stack memory usually stores automatic local variables.',
 1, 10, 956, 1,
 'Function-scope locals.',
 'https://www.studyplan.dev/pro-cpp/stack-memory');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=956), 'Stack memory', 1, 1),
((SELECT id FROM questions WHERE order_index=956), 'Heap memory always', 0, 2),
((SELECT id FROM questions WHERE order_index=956), 'Text segment only', 0, 3),
((SELECT id FROM questions WHERE order_index=956), 'TLS always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'Memory allocated with new must eventually be released (or owned by RAII wrapper).',
 NULL,
 'True. Otherwise leak risk appears.',
 2, 10, 957, 1,
 'Ownership and release responsibility.',
 'https://www.studyplan.dev/intro-to-programming/manual-memory-management');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=957), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=957), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'int* p = new int(8);\nstd::cout << *p;\ndelete p;',
 'Dynamically allocated int stores value 8.',
 1, 10, 958, 1,
 'Dereference before delete.',
 'https://www.studyplan.dev/pro-cpp/dynamic-memory');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=958), '8', 1, 1),
((SELECT id FROM questions WHERE order_index=958), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=958), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=958), 'Undefined always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'What is a dangling pointer?',
 NULL,
 'A pointer that refers to memory no longer valid (freed/out-of-scope).',
 2, 10, 959, 1,
 'Lifetime mismatch.',
 'https://www.studyplan.dev/intro-to-programming/manual-memory-management');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=959), 'Pointer to invalid/deallocated object', 1, 1),
((SELECT id FROM questions WHERE order_index=959), 'Null pointer only', 0, 2),
((SELECT id FROM questions WHERE order_index=959), 'Pointer to const object', 0, 3),
((SELECT id FROM questions WHERE order_index=959), 'Any pointer on heap', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'multi_select',
 'Select valid memory safety practices.',
 NULL,
 'RAII, clear ownership, and nulling/rebinding after delete are standard safety habits.',
 3, 15, 960, 1,
 'Prevent leaks/UAF.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=960), 'Prefer RAII wrappers', 1, 1),
((SELECT id FROM questions WHERE order_index=960), 'Define ownership semantics', 1, 2),
((SELECT id FROM questions WHERE order_index=960), 'Ignore object lifetime boundaries', 0, 3),
((SELECT id FROM questions WHERE order_index=960), 'Avoid raw new/delete when unnecessary', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'fill_blank',
 'Manual memory release for new T uses keyword ____.',
 NULL,
 'delete.',
 1, 10, 961, 1,
 'Scalar delete syntax.',
 'https://www.studyplan.dev/intro-to-programming/manual-memory-management');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=961), 'delete', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which semantic operation often transfers resources without deep copy?',
 NULL,
 'Move semantics transfer resources from source to destination.',
 2, 10, 962, 1,
 'Copy vs move.',
 'https://www.studyplan.dev/pro-cpp/move-semantics');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=962), 'Move semantics', 1, 1),
((SELECT id FROM questions WHERE order_index=962), 'Copy semantics always', 0, 2),
((SELECT id FROM questions WHERE order_index=962), 'Mutex locking', 0, 3),
((SELECT id FROM questions WHERE order_index=962), 'Thread detachment', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'Copy semantics for owning raw pointers can be dangerous without custom handling.',
 NULL,
 'True. Naive shallow copies can double-delete or leak.',
 3, 12, 963, 1,
 'Rule-of-five/RAII motivation.',
 'https://www.studyplan.dev/pro-cpp/copy-semantics');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=963), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=963), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Best approach for educational safety-first memory code?',
 NULL,
 'Prefer value semantics/RAII/smart pointers over manual raw ownership where possible.',
 2, 10, 964, 1,
 'Make lifetime explicit and safer.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=964), 'Use RAII and explicit ownership models', 1, 1),
((SELECT id FROM questions WHERE order_index=964), 'Use raw pointers everywhere', 0, 2),
((SELECT id FROM questions WHERE order_index=964), 'Hide ownership assumptions', 0, 3),
((SELECT id FROM questions WHERE order_index=964), 'Avoid cleanup paths', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Memory Model Essentials Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'int x = 5;\nint* p = &x;\nstd::cout << *p;',
 'Pointer to stack variable x prints 5.',
 1, 10, 965, 1,
 'Address-of local variable.',
 'https://www.studyplan.dev/pro-cpp/stack-memory');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=965), '5', 1, 1),
((SELECT id FROM questions WHERE order_index=965), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=965), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=965), 'nullptr', 0, 4);

-- -------------------------------------------------------------
-- B) Smart Pointer Patterns Quiz (966..975)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which pointer type is ideal for exclusive ownership transfer?',
 NULL,
 'unique_ptr is the standard move-only exclusive owner.',
 2, 10, 966, 1,
 'Move-only owner.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=966), 'std::unique_ptr', 1, 1),
((SELECT id FROM questions WHERE order_index=966), 'std::shared_ptr', 0, 2),
((SELECT id FROM questions WHERE order_index=966), 'std::weak_ptr', 0, 3),
((SELECT id FROM questions WHERE order_index=966), 'std::thread', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'weak_ptr contributes to shared_ptr use_count as strong owner.',
 NULL,
 'False; weak_ptr is non-owning.',
 2, 10, 967, 1,
 'Observer semantics.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=967), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=967), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_shared<int>(3);\nstd::weak_ptr<int> w = p;\nstd::cout << (w.expired() ? 1 : 0);',
 'While p owns object, weak_ptr is not expired => 0.',
 3, 12, 968, 1,
 'expired() reflects ownership existence.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=968), '0', 1, 1),
((SELECT id FROM questions WHERE order_index=968), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=968), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=968), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Circular ownership between two shared_ptr-linked objects causes what issue?',
 NULL,
 'Reference cycle leak: refcount never reaches zero.',
 3, 12, 969, 1,
 'Cycle leak pattern.',
 'https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=969), 'Memory leak via reference cycle', 1, 1),
((SELECT id FROM questions WHERE order_index=969), 'Compile-time error always', 0, 2),
((SELECT id FROM questions WHERE order_index=969), 'Automatic cycle collection by shared_ptr', 0, 3),
((SELECT id FROM questions WHERE order_index=969), 'Deadlock in mutex only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'multi_select',
 'Select good shared_ptr usage patterns.',
 NULL,
 'Use shared ownership when truly needed, avoid cycles, and pass by const ref when ownership not needed.',
 3, 15, 970, 1,
 'Ownership intent discipline.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=970), 'Use shared_ptr only for genuine shared ownership', 1, 1),
((SELECT id FROM questions WHERE order_index=970), 'Break cycles with weak_ptr', 1, 2),
((SELECT id FROM questions WHERE order_index=970), 'Use shared_ptr for every object by default', 0, 3),
((SELECT id FROM questions WHERE order_index=970), 'Prefer make_shared where appropriate', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'fill_blank',
 'Function returning exclusive owner should typically return std::_____.',
 NULL,
 'unique_ptr.',
 2, 10, 971, 1,
 'Factory ownership transfer.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=971), 'unique_ptr', 1),
((SELECT id FROM questions WHERE order_index=971), 'std::unique_ptr', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'What does use_count() on shared_ptr represent?',
 NULL,
 'Number of strong shared owners.',
 2, 10, 972, 1,
 'Strong ref count only.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=972), 'Number of owning shared_ptr instances', 1, 1),
((SELECT id FROM questions WHERE order_index=972), 'Number of weak_ptr observers', 0, 2),
((SELECT id FROM questions WHERE order_index=972), 'Total number of threads', 0, 3),
((SELECT id FROM questions WHERE order_index=972), 'Bytes allocated', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'Passing shared_ptr by value increments ownership count.',
 NULL,
 'True.',
 2, 10, 973, 1,
 'Copying shared owner.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=973), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=973), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which method on unique_ptr releases ownership and returns raw pointer?',
 NULL,
 'release().',
 3, 12, 974, 1,
 'Manual transfer edge-case.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=974), 'release()', 1, 1),
((SELECT id FROM questions WHERE order_index=974), 'reset()', 0, 2),
((SELECT id FROM questions WHERE order_index=974), 'lock()', 0, 3),
((SELECT id FROM questions WHERE order_index=974), 'detach()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Patterns Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_shared<int>(1);\nauto q = p;\nq.reset();\nstd::cout << p.use_count();',
 'After q reset, only p owns => use_count 1.',
 2, 10, 975, 1,
 'Owner count after reset.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=975), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=975), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=975), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=975), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- C) Thread Coordination Quiz (976..985)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does std::condition_variable::wait require along with predicate pattern?',
 NULL,
 'Typically a std::unique_lock<std::mutex> and predicate re-check.',
 3, 12, 976, 1,
 'Wait with lock + predicate.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=976), 'std::unique_lock<std::mutex>', 1, 1),
((SELECT id FROM questions WHERE order_index=976), 'std::atomic_ref<int>', 0, 2),
((SELECT id FROM questions WHERE order_index=976), 'std::lock_guard only (for wait call)', 0, 3),
((SELECT id FROM questions WHERE order_index=976), 'No lock needed', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::promise can set either value or exception for a future.',
 NULL,
 'True.',
 3, 12, 977, 1,
 'Result channel supports exceptions.',
 'https://cppreference.com/w/cpp/thread/promise.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=977), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=977), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'auto fut = std::async(std::launch::async, []{ return 9; });\nstd::cout << fut.get();',
 'Asynchronous task returns 9.',
 2, 10, 978, 1,
 'future get result.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=978), '9', 1, 1),
((SELECT id FROM questions WHERE order_index=978), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=978), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=978), 'undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is a key difference between async launch::deferred and launch::async?',
 NULL,
 'deferred runs on get/wait in calling thread context; async can run concurrently.',
 4, 15, 979, 1,
 'Launch policy semantics.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=979), 'deferred may execute lazily on get()', 1, 1),
((SELECT id FROM questions WHERE order_index=979), 'deferred always creates new OS thread immediately', 0, 2),
((SELECT id FROM questions WHERE order_index=979), 'async cannot return future', 0, 3),
((SELECT id FROM questions WHERE order_index=979), 'Both are identical in all implementations', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select valid producer-consumer coordination tools.',
 NULL,
 'condition_variable, semaphore, future/promise are common coordination primitives.',
 4, 15, 980, 1,
 'Coordination primitives.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=980), 'std::condition_variable', 1, 1),
((SELECT id FROM questions WHERE order_index=980), 'std::counting_semaphore', 1, 2),
((SELECT id FROM questions WHERE order_index=980), 'std::future/std::promise', 1, 3),
((SELECT id FROM questions WHERE order_index=980), 'std::array', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'std::future::wait() blocks until shared state becomes ____.',
 NULL,
 'ready.',
 2, 10, 981, 1,
 'State readiness.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=981), 'ready', 1);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Why is predicate form of condition_variable wait preferred?',
 NULL,
 'It handles spurious wakeups and re-checks condition atomically under lock discipline.',
 4, 15, 982, 1,
 'Always guard with condition check.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=982), 'Prevents logic errors from spurious wakeups', 1, 1),
((SELECT id FROM questions WHERE order_index=982), 'Eliminates need for mutex completely', 0, 2),
((SELECT id FROM questions WHERE order_index=982), 'Forces lock-free execution', 0, 3),
((SELECT id FROM questions WHERE order_index=982), 'Creates thread automatically', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'A future can be used to retrieve async exception thrown in task.',
 NULL,
 'True.',
 3, 12, 983, 1,
 'Exception propagation channel.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=983), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=983), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which primitive directly represents a limited number of permits/resources?',
 NULL,
 'counting_semaphore.',
 3, 12, 984, 1,
 'Permit counting model.',
 'https://cppreference.com/w/cpp/thread/counting_semaphore.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=984), 'std::counting_semaphore', 1, 1),
((SELECT id FROM questions WHERE order_index=984), 'std::recursive_mutex', 0, 2),
((SELECT id FROM questions WHERE order_index=984), 'std::promise', 0, 3),
((SELECT id FROM questions WHERE order_index=984), 'std::shared_ptr', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Thread Coordination Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::promise<int> p; auto f = p.get_future(); p.set_value(4); std::cout << f.get();',
 'Promise sets value consumed via future => 4.',
 2, 10, 985, 1,
 'Promise/future handoff.',
 'https://cppreference.com/w/cpp/thread/promise.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=985), '4', 1, 1),
((SELECT id FROM questions WHERE order_index=985), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=985), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=985), 'undefined', 0, 4);

-- -------------------------------------------------------------
-- D) Locking Strategies Quiz (986..995)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which mutex allows the same thread to lock multiple times?',
 NULL,
 'recursive_mutex.',
 3, 12, 986, 1,
 'Re-entrant locking semantics.',
 'https://cppreference.com/w/cpp/thread/recursive_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=986), 'std::recursive_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=986), 'std::mutex', 0, 2),
((SELECT id FROM questions WHERE order_index=986), 'std::shared_mutex', 0, 3),
((SELECT id FROM questions WHERE order_index=986), 'std::counting_semaphore', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::scoped_lock can lock multiple mutexes in one object.',
 NULL,
 'True.',
 3, 12, 987, 1,
 'Variadic lock wrapper.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=987), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=987), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::mutex m;\nstd::unique_lock<std::mutex> lk(m);\nstd::cout << lk.owns_lock();',
 'unique_lock owns lock after locking constructor => 1.',
 2, 10, 988, 1,
 'owns_lock query.',
 'https://cppreference.com/w/cpp/thread/unique_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=988), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=988), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=988), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=988), 'undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which lock wrapper supports deferred locking via std::defer_lock?',
 NULL,
 'unique_lock supports defer_lock.',
 3, 12, 989, 1,
 'Lock timing control.',
 'https://cppreference.com/w/cpp/thread/lock_tag_t.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=989), 'std::unique_lock', 1, 1),
((SELECT id FROM questions WHERE order_index=989), 'std::lock_guard', 0, 2),
((SELECT id FROM questions WHERE order_index=989), 'std::shared_lock only', 0, 3),
((SELECT id FROM questions WHERE order_index=989), 'std::jthread', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select timed mutex types.',
 NULL,
 'timed_mutex and recursive_timed_mutex provide timed lock APIs.',
 3, 15, 990, 1,
 'Timeout-capable mutexes.',
 'https://cppreference.com/w/cpp/thread/timed_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=990), 'std::timed_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=990), 'std::recursive_timed_mutex', 1, 2),
((SELECT id FROM questions WHERE order_index=990), 'std::mutex', 0, 3),
((SELECT id FROM questions WHERE order_index=990), 'std::condition_variable', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Read-mostly workload with many readers often uses std::shared_____.',
 NULL,
 'mutex.',
 3, 12, 991, 1,
 'Reader-writer lock.',
 'https://cppreference.com/w/cpp/thread/shared_mutex.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=991), 'mutex', 1),
((SELECT id FROM questions WHERE order_index=991), 'shared_mutex', 2),
((SELECT id FROM questions WHERE order_index=991), 'shared mutex', 3);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which lock type is commonly paired with shared_mutex for shared/read lock ownership?',
 NULL,
 'shared_lock.',
 3, 12, 992, 1,
 'Read lock wrapper.',
 'https://cppreference.com/w/cpp/thread/shared_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=992), 'std::shared_lock', 1, 1),
((SELECT id FROM questions WHERE order_index=992), 'std::lock_guard', 0, 2),
((SELECT id FROM questions WHERE order_index=992), 'std::scoped_lock only', 0, 3),
((SELECT id FROM questions WHERE order_index=992), 'std::promise', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Lock order consistency is a strategy to reduce deadlock risk.',
 NULL,
 'True.',
 4, 15, 993, 1,
 'Global lock ordering.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=993), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=993), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which primitive lets you attempt multiple locks and detect failure index?',
 NULL,
 'std::try_lock returns index of lock that failed, or -1 on success.',
 4, 15, 994, 1,
 'Multi-lock attempt function.',
 'https://cppreference.com/w/cpp/thread/try_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=994), 'std::try_lock', 1, 1),
((SELECT id FROM questions WHERE order_index=994), 'std::lock_guard', 0, 2),
((SELECT id FROM questions WHERE order_index=994), 'std::atomic_ref', 0, 3),
((SELECT id FROM questions WHERE order_index=994), 'std::future', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Locking Strategies Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::mutex m; std::lock_guard<std::mutex> g(m); std::cout << "ok";',
 'lock_guard acquires lock in scope and prints ok.',
 1, 10, 995, 1,
 'Simple RAII lock.',
 'https://cppreference.com/w/cpp/thread/lock_guard.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=995), 'ok', 1, 1),
((SELECT id FROM questions WHERE order_index=995), 'Compile error', 0, 2),
((SELECT id FROM questions WHERE order_index=995), 'deadlock always', 0, 3),
((SELECT id FROM questions WHERE order_index=995), '0', 0, 4);

-- -------------------------------------------------------------
-- E) Atomic Design Quiz (996..1005)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What problem does std::atomic primarily address?',
 NULL,
 'Data races on shared scalar/state updates when used correctly.',
 3, 12, 996, 1,
 'Race-safe operations.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=996), 'Data-race-safe atomic operations', 1, 1),
((SELECT id FROM questions WHERE order_index=996), 'Automatic deadlock detection', 0, 2),
((SELECT id FROM questions WHERE order_index=996), 'Thread scheduling fairness', 0, 3),
((SELECT id FROM questions WHERE order_index=996), 'Heap defragmentation', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Atomic operations can be lock-free for some types/platforms, but not guaranteed for all.',
 NULL,
 'True.',
 4, 15, 997, 1,
 'Platform/type dependent.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=997), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=997), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> a{1};\na.fetch_add(2);\nstd::cout << a.load();',
 '1 + 2 => 3.',
 2, 10, 998, 1,
 'fetch_add semantics.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=998), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=998), '2', 0, 2),
((SELECT id FROM questions WHERE order_index=998), '1', 0, 3),
((SELECT id FROM questions WHERE order_index=998), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'When is atomic_ref useful?',
 NULL,
 'When you need atomic operations on an existing suitably aligned object without changing its type declaration.',
 4, 15, 999, 1,
 'Atomic overlay on existing storage.',
 'https://cppreference.com/w/cpp/atomic/atomic_ref.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=999), 'Atomic operations on existing object storage', 1, 1),
((SELECT id FROM questions WHERE order_index=999), 'Replacing mutex hierarchy', 0, 2),
((SELECT id FROM questions WHERE order_index=999), 'Creating futures from promises', 0, 3),
((SELECT id FROM questions WHERE order_index=999), 'Thread joining', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select valid statements about atomics and mutexes.',
 NULL,
 'Atomics are good for simple shared state ops; mutexes needed for larger critical sections/invariants.',
 4, 15, 1000, 1,
 'Choose correct synchronization scope.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1000), 'Atomics can protect simple counters/flags', 1, 1),
((SELECT id FROM questions WHERE order_index=1000), 'Mutex often needed for compound invariants', 1, 2),
((SELECT id FROM questions WHERE order_index=1000), 'Atomics eliminate all need for locks always', 0, 3),
((SELECT id FROM questions WHERE order_index=1000), 'Synchronization design should match data access pattern', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Atomic compare-and-swap style API commonly uses name compare_exchange____.',
 NULL,
 'weak/strong.',
 4, 15, 1001, 1,
 'CAS operations.', 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT OR IGNORE INTO fill_blank_answers (question_id, answer, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1001), 'weak', 1),
((SELECT id FROM questions WHERE order_index=1001), 'strong', 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which tool best synchronizes one-time data handoff with result propagation?',
 NULL,
 'promise/future pair.',
 3, 12, 1002, 1,
 'Producer-consumer result channel.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1002), 'std::promise + std::future', 1, 1),
((SELECT id FROM questions WHERE order_index=1002), 'std::mutex + std::thread::id only', 0, 2),
((SELECT id FROM questions WHERE order_index=1002), 'std::weak_ptr + std::shared_ptr', 0, 3),
((SELECT id FROM questions WHERE order_index=1002), 'std::array + std::tuple', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'A data race is undefined behavior in C++.',
 NULL,
 'True.',
 3, 12, 1003, 1,
 'Critical concurrency rule.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1003), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=1003), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Best-practice for concurrency education content?',
 NULL,
 'Prefer simple patterns first, then layered advanced primitives with clear invariants/tests.',
 3, 12, 1004, 1,
 'Pedagogy + correctness.',
 'https://cppreference.com/w/cpp/thread/mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1004), 'Teach simple safe patterns before advanced optimizations', 1, 1),
((SELECT id FROM questions WHERE order_index=1004), 'Start with lock-free tricks only', 0, 2),
((SELECT id FROM questions WHERE order_index=1004), 'Ignore edge-case races', 0, 3),
((SELECT id FROM questions WHERE order_index=1004), 'Avoid deterministic tests', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomic Design Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> v{10};\nstd::cout << v.fetch_sub(3);',
 'fetch_sub returns previous value (10), then stores 7.',
 4, 15, 1005, 1,
 'Return-old-value semantics.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=1005), '10', 1, 1),
((SELECT id FROM questions WHERE order_index=1005), '7', 0, 2),
((SELECT id FROM questions WHERE order_index=1005), '3', 0, 3),
((SELECT id FROM questions WHERE order_index=1005), 'Compile error', 0, 4);

-- -------------------------------------------------------------
-- Primary tag mapping (956..1005)
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 956 AND 975 THEN 'smart-pointers'
            WHEN q.order_index BETWEEN 976 AND 1005 THEN 'concurrency'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 956 AND 1005;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=960), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=964), (SELECT id FROM tags WHERE name='memory')),
((SELECT id FROM questions WHERE order_index=970), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=973), (SELECT id FROM tags WHERE name='memory')),
((SELECT id FROM questions WHERE order_index=980), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=982), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=985), (SELECT id FROM tags WHERE name='concurrency')),
((SELECT id FROM questions WHERE order_index=990), (SELECT id FROM tags WHERE name='concurrency')),
((SELECT id FROM questions WHERE order_index=993), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1000), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=1004), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 956 AND 1005;