-- =============================================================
-- CppAtlas Content Patch: Memory & Concurrency Pack 05 (Part 1)
-- Date: 2026-03-16
-- Questions: 55 (order_index 901..955)
--
-- Rules applied:
-- - No BEGIN/COMMIT (transaction handled by ContentPatchService)
-- - Standardized INSERT INTO questions (12 columns)
-- - SQLite-safe escaping
-- - Add EXACTLY 6 questions to existing "Memory Management Quiz" (quiz_id=11)
-- - Remaining 49 questions only in NEW quizzes
-- =============================================================

INSERT OR IGNORE INTO tags (name, color) VALUES
('memory', '#F14C4C'),
('smart-pointers', '#F14C4C'),
('concurrency', '#8BE9FD'),
('best-practices', '#89D185');

-- NEW quizzes for this part
INSERT OR IGNORE INTO quizzes (title, description, topic_id, difficulty, type, is_active, created_by, version) VALUES
('Smart Pointer Mastery Quiz', 'unique_ptr/shared_ptr/weak_ptr ownership and lifetime patterns', 11, 3, 'standard', 1, 0, 1),
('Threading Fundamentals Quiz', 'thread/jthread basics, IDs, sleep/yield and lifecycle safety', 13, 3, 'standard', 1, 0, 1),
('Mutexes & Locks Practice Quiz', 'mutex variants, lock wrappers and deadlock-avoidance patterns', 13, 4, 'standard', 1, 0, 1),
('Atomics & Synchronization Quiz', 'atomics, memory synchronization primitives, futures/promises/async', 13, 4, 'standard', 1, 0, 1),
('Async Patterns & Safety Quiz', 'condition variables, semaphores, task orchestration best practices', 13, 4, 'standard', 1, 0, 1);

-- -------------------------------------------------------------
-- Existing Quiz 11: Memory Management Quiz
-- EXACTLY 6 questions (901..906)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'mcq',
 'What does std::unique_ptr represent?',
 NULL,
 'std::unique_ptr represents exclusive ownership of a dynamically allocated object.',
 2, 10, 901, 1,
 'One owner only.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=901), 'Exclusive ownership smart pointer', 1, 1),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=901), 'Reference-counted shared owner', 0, 2),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=901), 'Raw pointer wrapper without deletion', 0, 3),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=901), 'Thread synchronization object', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'true_false',
 'std::shared_ptr uses reference counting for shared ownership.',
 NULL,
 'True. The managed object is destroyed when the last owning shared_ptr goes away.',
 2, 10, 902, 1,
 'Control block + ref count.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=902), 'True', 1, 1),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=902), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'code_output',
 'What is printed?',
 'auto p = std::make_unique<int>(42);\nstd::cout << *p;',
 'Dereferencing unique_ptr gives access to managed object value 42.',
 2, 10, 903, 1,
 'Operator* on smart pointer.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=903), '42', 1, 1),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=903), '0', 0, 2),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=903), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=903), 'nullptr', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'mcq',
 'Why is std::weak_ptr used with std::shared_ptr graphs?',
 NULL,
 'weak_ptr breaks ownership cycles and prevents memory leaks from circular references.',
 3, 12, 904, 1,
 'Non-owning observer.',
 'https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=904), 'To avoid circular ownership leaks', 1, 1),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=904), 'To make shared_ptr thread-unsafe', 0, 2),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=904), 'To disable deletion forever', 0, 3),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=904), 'To replace unique_ptr in all cases', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'multi_select',
 'Select valid memory-management best practices.',
 NULL,
 'Use RAII/smart pointers, clear ownership, and avoid manual new/delete when not needed.',
 3, 15, 905, 1,
 'Ownership clarity.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=905), 'Prefer RAII and smart pointers', 1, 1),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=905), 'Define ownership semantics explicitly', 1, 2),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=905), 'Use raw new/delete everywhere by default', 0, 3),
((SELECT id FROM questions WHERE quiz_id=11 AND order_index=905), 'Avoid circular shared_ptr ownership', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
(11, 11, 'fill_blank',
 'A pointer that observes shared ownership without increasing reference count is std::_____.',
 NULL,
 'weak_ptr.',
 2, 10, 906, 1,
 'Observer pointer.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');

-- -------------------------------------------------------------
-- NEW Quiz: Smart Pointer Mastery Quiz (10 questions, 907..916)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which factory is preferred for creating std::shared_ptr?',
 NULL,
 'std::make_shared is typically preferred for exception safety/perf (single allocation benefits in many implementations).',
 2, 10, 907, 1,
 'Factory helpers.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=907), 'std::make_shared<T>(...)', 1, 1),
((SELECT id FROM questions WHERE order_index=907), 'new T + manual delete', 0, 2),
((SELECT id FROM questions WHERE order_index=907), 'std::malloc', 0, 3),
((SELECT id FROM questions WHERE order_index=907), 'std::lock_guard', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'std::unique_ptr can be copied freely.',
 NULL,
 'False. unique_ptr is movable, not copyable.',
 2, 10, 908, 1,
 'Move-only type.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=908), 'True', 0, 1),
((SELECT id FROM questions WHERE order_index=908), 'False', 1, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_shared<int>(7);\nauto q = p;\nstd::cout << p.use_count();',
 'Two shared_ptr owners -> use_count usually 2 here.',
 3, 12, 909, 1,
 'Reference count grows with copies.',
 'https://www.studyplan.dev/pro-cpp/shared-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=909), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=909), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=909), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=909), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'What does weak_ptr::lock() do?',
 NULL,
 'It attempts to create shared_ptr if managed object still exists; otherwise empty shared_ptr.',
 3, 12, 910, 1,
 'Promote observer to owner temporarily.',
 'https://www.studyplan.dev/pro-cpp/weak-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=910), 'Returns shared_ptr if object still alive', 1, 1),
((SELECT id FROM questions WHERE order_index=910), 'Deletes object immediately', 0, 2),
((SELECT id FROM questions WHERE order_index=910), 'Resets all owners', 0, 3),
((SELECT id FROM questions WHERE order_index=910), 'Creates unique_ptr', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'multi_select',
 'Select valid statements about smart pointers.',
 NULL,
 'unique_ptr is move-only; shared_ptr is shared ownership; weak_ptr is non-owning observer.',
 2, 15, 911, 1,
 'Ownership taxonomy.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=911), 'unique_ptr has exclusive ownership', 1, 1),
((SELECT id FROM questions WHERE order_index=911), 'shared_ptr is reference-counted', 1, 2),
((SELECT id FROM questions WHERE order_index=911), 'weak_ptr increments shared ownership count', 0, 3),
((SELECT id FROM questions WHERE order_index=911), 'weak_ptr helps break cycles', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'fill_blank',
 'std::shared_ptr and std::weak_ptr coordinate through a shared control ____.',
 NULL,
 'block.',
 3, 12, 912, 1,
 'Metadata for counts.',
 'https://www.learncpp.com/cpp-tutorial/stdshared_ptr/');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Which smart pointer is generally best for tree parent back-references to avoid cycles?',
 NULL,
 'weak_ptr for back references.',
 3, 12, 913, 1,
 'Cycle prevention.',
 'https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=913), 'std::weak_ptr', 1, 1),
((SELECT id FROM questions WHERE order_index=913), 'std::shared_ptr', 0, 2),
((SELECT id FROM questions WHERE order_index=913), 'std::unique_ptr', 0, 3),
((SELECT id FROM questions WHERE order_index=913), 'raw pointer only always', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'true_false',
 'Calling reset() on unique_ptr releases ownership of the managed object.',
 NULL,
 'True (destroying managed object if non-null).',
 2, 10, 914, 1,
 'Ownership reset semantics.',
 'https://www.learncpp.com/cpp-tutorial/stdunique_ptr/');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=914), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=914), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'code_output',
 'What is printed?',
 'auto p = std::make_unique<int>(5);\nauto q = std::move(p);\nstd::cout << (p ? 1 : 0) << (q ? 1 : 0);',
 'After move, p is empty (0) and q owns object (1) => 01.',
 3, 12, 915, 1,
 'Moved-from unique_ptr empties.',
 'https://www.studyplan.dev/pro-cpp/move-semantics');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=915), '01', 1, 1),
((SELECT id FROM questions WHERE order_index=915), '10', 0, 2),
((SELECT id FROM questions WHERE order_index=915), '11', 0, 3),
((SELECT id FROM questions WHERE order_index=915), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Smart Pointer Mastery Quiz' ORDER BY id DESC LIMIT 1), 11, 'mcq',
 'Best practice when exposing ownership from API?',
 NULL,
 'Express ownership explicitly via unique_ptr/shared_ptr/value/reference as appropriate.',
 3, 12, 916, 1,
 'Ownership in type system.',
 'https://www.studyplan.dev/pro-cpp/smart-pointers');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=916), 'Encode ownership intent in parameter/return types', 1, 1),
((SELECT id FROM questions WHERE order_index=916), 'Use raw pointers for everything', 0, 2),
((SELECT id FROM questions WHERE order_index=916), 'Hide ownership semantics', 0, 3),
((SELECT id FROM questions WHERE order_index=916), 'Never document lifetime', 0, 4);

-- -------------------------------------------------------------
-- NEW Quiz: Threading Fundamentals Quiz (10 questions, 917..926)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does std::thread::join() do?',
 NULL,
 'Blocks until thread finishes execution, synchronizing with thread completion.',
 2, 10, 917, 1,
 'Wait for completion.',
 'https://cppreference.com/w/cpp/thread/thread.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=917), 'Waits for thread to finish', 1, 1),
((SELECT id FROM questions WHERE order_index=917), 'Starts thread execution', 0, 2),
((SELECT id FROM questions WHERE order_index=917), 'Kills thread immediately', 0, 3),
((SELECT id FROM questions WHERE order_index=917), 'Locks mutex automatically', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Destroying a joinable std::thread without join/detach leads to std::terminate.',
 NULL,
 'True.',
 3, 12, 918, 1,
 'Thread lifecycle rule.',
 'https://cppreference.com/w/cpp/thread/thread.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=918), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=918), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::thread::id id = std::this_thread::get_id();\nstd::cout << (id == std::thread::id{} ? 0 : 1);',
 'Current thread id is valid/non-default, so expression typically prints 1.',
 3, 12, 919, 1,
 'Valid thread id check.',
 'https://cppreference.com/w/cpp/thread/get_id.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=919), '1', 1, 1),
((SELECT id FROM questions WHERE order_index=919), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=919), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=919), 'undefined', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is std::jthread notable for (vs std::thread)?',
 NULL,
 'It supports cooperative stop token and joins on destruction semantics (RAII-friendly thread management).',
 3, 12, 920, 1,
 'Safer thread RAII.',
 'https://cppreference.com/w/cpp/thread/jthread.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=920), 'RAII-style joining + stop support', 1, 1),
((SELECT id FROM questions WHERE order_index=920), 'No ability to run callable', 0, 2),
((SELECT id FROM questions WHERE order_index=920), 'Always detached behavior', 0, 3),
((SELECT id FROM questions WHERE order_index=920), 'Replaces mutexes', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select valid thread utility functions.',
 NULL,
 'yield, sleep_for, sleep_until, get_id are thread utility APIs.',
 2, 15, 921, 1,
 'Basic thread helpers.',
 'https://cppreference.com/w/cpp/thread/yield.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=921), 'std::this_thread::yield()', 1, 1),
((SELECT id FROM questions WHERE order_index=921), 'std::this_thread::sleep_for(...)', 1, 2),
((SELECT id FROM questions WHERE order_index=921), 'std::this_thread::sleep_until(...)', 1, 3),
((SELECT id FROM questions WHERE order_index=921), 'std::thread::force_pause()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Calling std::this_thread::_____() hints scheduler to run another thread.',
 NULL,
 'yield.',
 2, 10, 922, 1,
 'Scheduler hint API.',
 'https://cppreference.com/w/cpp/thread/yield.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'When should detach() be used cautiously?',
 NULL,
 'Detached threads outlive handle and require careful lifetime/resource coordination.',
 3, 12, 923, 1,
 'Lost join handle.',
 'https://cppreference.com/w/cpp/thread/thread.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=923), 'When lifetime synchronization is hard', 1, 1),
((SELECT id FROM questions WHERE order_index=923), 'Always, as default pattern', 0, 2),
((SELECT id FROM questions WHERE order_index=923), 'Only for single-threaded code', 0, 3),
((SELECT id FROM questions WHERE order_index=923), 'To replace atomics', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::this_thread::sleep_for can be used to pause current thread for a duration.',
 NULL,
 'True.',
 1, 10, 924, 1,
 'Duration-based sleep.',
 'https://cppreference.com/w/cpp/thread/sleep_for.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=924), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=924), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which call blocks current thread until specified time point?',
 NULL,
 'sleep_until.',
 2, 10, 925, 1,
 'Time-point sleep.',
 'https://cppreference.com/w/cpp/thread/sleep_until.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=925), 'std::this_thread::sleep_until(...)', 1, 1),
((SELECT id FROM questions WHERE order_index=925), 'std::this_thread::yield()', 0, 2),
((SELECT id FROM questions WHERE order_index=925), 'std::thread::joinable()', 0, 3),
((SELECT id FROM questions WHERE order_index=925), 'std::promise::set_value()', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Threading Fundamentals Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::cout << std::this_thread::get_id();',
 'A valid textual representation of current thread id is printed.',
 2, 10, 926, 1,
 'ID is implementation-defined text.',
 'https://cppreference.com/w/cpp/thread/get_id.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=926), 'A non-empty thread id representation', 1, 1),
((SELECT id FROM questions WHERE order_index=926), 'Always 0', 0, 2),
((SELECT id FROM questions WHERE order_index=926), 'Always empty', 0, 3),
((SELECT id FROM questions WHERE order_index=926), 'Compile error always', 0, 4);

-- -------------------------------------------------------------
-- NEW Quiz: Mutexes & Locks Practice Quiz (10 questions, 927..936)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is std::lock_guard primarily for?',
 NULL,
 'RAII mutex ownership in scope to ensure unlock on exit.',
 2, 10, 927, 1,
 'Scope-based locking.',
 'https://cppreference.com/w/cpp/thread/lock_guard.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=927), 'Automatic scoped lock/unlock (RAII)', 1, 1),
((SELECT id FROM questions WHERE order_index=927), 'Timed waits on condition variable', 0, 2),
((SELECT id FROM questions WHERE order_index=927), 'Reference counting memory', 0, 3),
((SELECT id FROM questions WHERE order_index=927), 'Thread creation helper', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::unique_lock is generally more flexible than std::lock_guard.',
 NULL,
 'True; unique_lock supports deferred locking, unlock/relock, timed locking with timed mutexes, etc.',
 3, 12, 928, 1,
 'Flexibility vs simplicity.',
 'https://cppreference.com/w/cpp/thread/unique_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=928), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=928), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::mutex m;\n{ std::lock_guard<std::mutex> g(m); std::cout << "L"; }\nstd::cout << "U";',
 'Lock held in inner scope, then released; output LU.',
 2, 10, 929, 1,
 'RAII scope boundaries.',
 'https://cppreference.com/w/cpp/thread/lock_guard.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=929), 'LU', 1, 1),
((SELECT id FROM questions WHERE order_index=929), 'UL', 0, 2),
((SELECT id FROM questions WHERE order_index=929), 'L', 0, 3),
((SELECT id FROM questions WHERE order_index=929), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which mutex type supports shared (reader) locks?',
 NULL,
 'shared_mutex / shared_timed_mutex support shared ownership semantics for readers.',
 4, 15, 930, 1,
 'Reader-writer patterns.',
 'https://cppreference.com/w/cpp/thread/shared_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=930), 'std::shared_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=930), 'std::mutex', 0, 2),
((SELECT id FROM questions WHERE order_index=930), 'std::recursive_mutex', 0, 3),
((SELECT id FROM questions WHERE order_index=930), 'std::timed_mutex', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select lock wrapper classes from standard library.',
 NULL,
 'lock_guard, unique_lock, scoped_lock, shared_lock.',
 3, 15, 931, 1,
 'Locking wrappers.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=931), 'std::lock_guard', 1, 1),
((SELECT id FROM questions WHERE order_index=931), 'std::unique_lock', 1, 2),
((SELECT id FROM questions WHERE order_index=931), 'std::scoped_lock', 1, 3),
((SELECT id FROM questions WHERE order_index=931), 'std::allocator_lock', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'Mutex type allowing same thread to lock multiple times is std::_____mutex.',
 NULL,
 'recursive_.',
 3, 12, 932, 1,
 'Recursive lock semantics.',
 'https://cppreference.com/w/cpp/thread/recursive_mutex.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which utility helps lock multiple mutexes safely to reduce deadlock risk?',
 NULL,
 'std::scoped_lock can lock multiple mutexes using deadlock-avoidance strategy.',
 4, 15, 933, 1,
 'Multi-mutex locking.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=933), 'std::scoped_lock', 1, 1),
((SELECT id FROM questions WHERE order_index=933), 'std::atomic', 0, 2),
((SELECT id FROM questions WHERE order_index=933), 'std::promise', 0, 3),
((SELECT id FROM questions WHERE order_index=933), 'std::jthread', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::shared_lock is commonly used with shared_mutex for read-only critical sections.',
 NULL,
 'True.',
 3, 12, 934, 1,
 'Reader lock wrapper.',
 'https://cppreference.com/w/cpp/thread/shared_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=934), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=934), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is std::try_lock used for?',
 NULL,
 'Attempts to lock multiple mutexes without deadlock by try-locking pattern.',
 4, 15, 935, 1,
 'Non-blocking lock attempt semantics.',
 'https://cppreference.com/w/cpp/thread/try_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=935), 'Try-locking multiple mutexes', 1, 1),
((SELECT id FROM questions WHERE order_index=935), 'Blocking sleep primitive', 0, 2),
((SELECT id FROM questions WHERE order_index=935), 'Atomic compare-exchange wrapper', 0, 3),
((SELECT id FROM questions WHERE order_index=935), 'Condition variable wait helper only', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Mutexes & Locks Practice Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::mutex m; std::unique_lock<std::mutex> lk(m, std::defer_lock); std::cout << lk.owns_lock();',
 'Deferred lock means lock not acquired initially => owns_lock false (0).',
 3, 12, 936, 1,
 'defer_lock tag behavior.',
 'https://cppreference.com/w/cpp/thread/lock_tag_t.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=936), '0', 1, 1),
((SELECT id FROM questions WHERE order_index=936), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=936), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=936), 'undefined', 0, 4);

-- -------------------------------------------------------------
-- NEW Quiz: Atomics & Synchronization Quiz (10 questions, 937..946)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does std::atomic<T> provide?',
 NULL,
 'Atomic operations on T to avoid data races for that object (subject to correct usage).',
 3, 12, 937, 1,
 'Race-free primitive operations.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=937), 'Atomic operations for shared variable access', 1, 1),
((SELECT id FROM questions WHERE order_index=937), 'Automatic mutex around all code', 0, 2),
((SELECT id FROM questions WHERE order_index=937), 'Guaranteed lock-free for all types', 0, 3),
((SELECT id FROM questions WHERE order_index=937), 'Replacement for all synchronization', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Atomic increment on std::atomic<int> can be done with ++ operator.',
 NULL,
 'True.',
 2, 10, 938, 1,
 'Operator overload support.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=938), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=938), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::atomic<int> c{0};\n++c; ++c;\nstd::cout << c.load();',
 'Atomic counter increments to 2.',
 2, 10, 939, 1,
 'load() reads current value.',
 'https://cppreference.com/w/cpp/atomic/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=939), '2', 1, 1),
((SELECT id FROM questions WHERE order_index=939), '1', 0, 2),
((SELECT id FROM questions WHERE order_index=939), '0', 0, 3),
((SELECT id FROM questions WHERE order_index=939), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is std::atomic_ref used for?',
 NULL,
 'Provides atomic operations on an existing object via reference-like wrapper.',
 4, 15, 940, 1,
 'Atomic view over existing storage.',
 'https://cppreference.com/w/cpp/atomic/atomic_ref.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=940), 'Atomic operations on existing object storage', 1, 1),
((SELECT id FROM questions WHERE order_index=940), 'Thread creation API', 0, 2),
((SELECT id FROM questions WHERE order_index=940), 'Mutex ownership wrapper', 0, 3),
((SELECT id FROM questions WHERE order_index=940), 'Future result container', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select asynchronous facilities from standard library.',
 NULL,
 'future, promise, async are async task/result primitives.',
 3, 15, 941, 1,
 'Task/result APIs.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=941), 'std::future', 1, 1),
((SELECT id FROM questions WHERE order_index=941), 'std::promise', 1, 2),
((SELECT id FROM questions WHERE order_index=941), 'std::async', 1, 3),
((SELECT id FROM questions WHERE order_index=941), 'std::lock_guard', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'std::promise communicates a result to associated std::_____.',
 NULL,
 'future.',
 2, 10, 942, 1,
 'Producer-consumer result channel.',
 'https://cppreference.com/w/cpp/thread/promise.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does std::future::get() do?',
 NULL,
 'Retrieves the result (blocking if needed) and consumes shared state value.',
 3, 12, 943, 1,
 'Result retrieval semantics.',
 'https://cppreference.com/w/cpp/thread/future.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=943), 'Waits (if needed) then returns async result', 1, 1),
((SELECT id FROM questions WHERE order_index=943), 'Starts new thread always', 0, 2),
((SELECT id FROM questions WHERE order_index=943), 'Locks mutex forever', 0, 3),
((SELECT id FROM questions WHERE order_index=943), 'Creates semaphore', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'std::async may run function asynchronously depending on launch policy.',
 NULL,
 'True.',
 2, 10, 944, 1,
 'Launch policy nuance.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=944), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=944), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'When is mutex still needed even if atomics exist?',
 NULL,
 'For protecting compound invariants/critical sections spanning multiple variables.',
 4, 15, 945, 1,
 'Multi-variable consistency.',
 'https://cppreference.com/w/cpp/thread/mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=945), 'When protecting multi-step shared invariants', 1, 1),
((SELECT id FROM questions WHERE order_index=945), 'Never; atomics always replace mutexes', 0, 2),
((SELECT id FROM questions WHERE order_index=945), 'Only for single integer increment', 0, 3),
((SELECT id FROM questions WHERE order_index=945), 'Only in debug builds', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Atomics & Synchronization Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'auto fut = std::async(std::launch::deferred, []{ return 3; });\nstd::cout << fut.get();',
 'Deferred async executes when get() called; returns 3.',
 3, 12, 946, 1,
 'Deferred policy behavior.',
 'https://cppreference.com/w/cpp/thread/async.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=946), '3', 1, 1),
((SELECT id FROM questions WHERE order_index=946), '0', 0, 2),
((SELECT id FROM questions WHERE order_index=946), 'Compile error', 0, 3),
((SELECT id FROM questions WHERE order_index=946), 'undefined', 0, 4);

-- -------------------------------------------------------------
-- NEW Quiz: Async Patterns & Safety Quiz (10 questions, 947..955)
-- (9 questions to make total 55 in this part)
-- -------------------------------------------------------------
INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What is condition_variable typically used for?',
 NULL,
 'Thread coordination by waiting for condition state changes under mutex protection.',
 3, 12, 947, 1,
 'Wait-notify pattern.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=947), 'Wait/notify synchronization on shared state', 1, 1),
((SELECT id FROM questions WHERE order_index=947), 'Atomic increment primitive', 0, 2),
((SELECT id FROM questions WHERE order_index=947), 'Container for thread IDs', 0, 3),
((SELECT id FROM questions WHERE order_index=947), 'Smart pointer replacement', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Condition variable waits should usually re-check predicate in a loop.',
 NULL,
 'True, to handle spurious wakeups and missed states correctly.',
 4, 15, 948, 1,
 'Predicate loop pattern.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=948), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=948), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'code_output',
 'What is printed?',
 'std::counting_semaphore<3> sem(1);\nstd::cout << "S"; sem.acquire(); sem.release(); std::cout << "E";',
 'Acquire/release pair succeeds with available permit; output SE.',
 4, 15, 949, 1,
 'Semaphore permit flow.',
 'https://cppreference.com/w/cpp/thread/counting_semaphore.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=949), 'SE', 1, 1),
((SELECT id FROM questions WHERE order_index=949), 'ES', 0, 2),
((SELECT id FROM questions WHERE order_index=949), 'S only', 0, 3),
((SELECT id FROM questions WHERE order_index=949), 'Compile error', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'What does std::scoped_lock help with for multiple mutexes?',
 NULL,
 'Acquires multiple mutexes with deadlock-avoidance algorithm.',
 4, 15, 950, 1,
 'Multiple-lock safety.',
 'https://cppreference.com/w/cpp/thread/scoped_lock.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=950), 'Safer multi-mutex locking', 1, 1),
((SELECT id FROM questions WHERE order_index=950), 'Async result retrieval', 0, 2),
((SELECT id FROM questions WHERE order_index=950), 'Atomic compare-exchange', 0, 3),
((SELECT id FROM questions WHERE order_index=950), 'Thread detachment', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'multi_select',
 'Select robust concurrency safety practices.',
 NULL,
 'Minimize shared mutable state, use RAII locks, define ownership/lifetime clearly, test race-prone paths.',
 4, 15, 951, 1,
 'Design for correctness first.',
 'https://cppreference.com/w/cpp/thread/mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=951), 'Prefer immutable data where possible', 1, 1),
((SELECT id FROM questions WHERE order_index=951), 'Use RAII lock wrappers', 1, 2),
((SELECT id FROM questions WHERE order_index=951), 'Share raw mutable globals everywhere', 0, 3),
((SELECT id FROM questions WHERE order_index=951), 'Document thread ownership/lifetime', 1, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'fill_blank',
 'std::future pairs naturally with std::_____ as producer.',
 NULL,
 'promise.',
 2, 10, 952, 1,
 'Producer-consumer result channel.',
 'https://cppreference.com/w/cpp/thread/promise.html');

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Which mutex allows timed lock attempts?',
 NULL,
 'timed_mutex and recursive_timed_mutex support timed locking operations.',
 3, 12, 953, 1,
 'Time-bounded locking.',
 'https://cppreference.com/w/cpp/thread/timed_mutex.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=953), 'std::timed_mutex', 1, 1),
((SELECT id FROM questions WHERE order_index=953), 'std::mutex only', 0, 2),
((SELECT id FROM questions WHERE order_index=953), 'std::shared_lock', 0, 3),
((SELECT id FROM questions WHERE order_index=953), 'std::future', 0, 4);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'true_false',
 'Data race freedom can be achieved through correct mutex locking or proper atomic synchronization.',
 NULL,
 'True.',
 3, 12, 954, 1,
 'Synchronization correctness.',
 'https://cppreference.com/w/cpp/atomic.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=954), 'True', 1, 1),
((SELECT id FROM questions WHERE order_index=954), 'False', 0, 2);

INSERT INTO questions (quiz_id, topic_id, type, content, code_snippet, explanation, difficulty, points, order_index, is_active, hint, ref_url) VALUES
((SELECT id FROM quizzes WHERE title='Async Patterns & Safety Quiz' ORDER BY id DESC LIMIT 1), 13, 'mcq',
 'Best practice for concurrency in educational codebases?',
 NULL,
 'Favor simple, explicit synchronization patterns and deterministic testable designs.',
 3, 12, 955, 1,
 'Teachable correctness > clever tricks.',
 'https://cppreference.com/w/cpp/thread/condition_variable.html');
INSERT INTO options (question_id, content, is_correct, order_index) VALUES
((SELECT id FROM questions WHERE order_index=955), 'Keep synchronization explicit and testable', 1, 1),
((SELECT id FROM questions WHERE order_index=955), 'Prefer hidden global locks', 0, 2),
((SELECT id FROM questions WHERE order_index=955), 'Ignore lifetimes of worker threads', 0, 3),
((SELECT id FROM questions WHERE order_index=955), 'Avoid documenting thread contracts', 0, 4);

-- -------------------------------------------------------------
-- Primary question_tags mapping (901..955)
-- -------------------------------------------------------------
INSERT OR IGNORE INTO question_tags (question_id, tag_id)
SELECT q.id,
       (SELECT id FROM tags WHERE name = CASE
            WHEN q.order_index BETWEEN 901 AND 916 THEN 'smart-pointers'
            WHEN q.order_index BETWEEN 917 AND 936 THEN 'concurrency'
            WHEN q.order_index BETWEEN 937 AND 955 THEN 'concurrency'
            ELSE 'best-practices'
       END)
FROM questions q
WHERE q.order_index BETWEEN 901 AND 955;

-- Secondary tags
INSERT OR IGNORE INTO question_tags (question_id, tag_id) VALUES
((SELECT id FROM questions WHERE order_index=905), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=916), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=933), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=941), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=951), (SELECT id FROM tags WHERE name='best-practices')),
((SELECT id FROM questions WHERE order_index=955), (SELECT id FROM tags WHERE name='best-practices'));

-- Derive quiz_tags for affected quizzes
INSERT OR IGNORE INTO quiz_tags (quiz_id, tag_id)
SELECT DISTINCT q.quiz_id, qt.tag_id
FROM questions q
JOIN question_tags qt ON qt.question_id = q.id
WHERE q.order_index BETWEEN 901 AND 955;