project('stlc', 'c')

tinycthread_include_dir = include_directories('STL_CThread/source')

# Test cases for List/Vector
test_list_executable = executable('test_list', 'tests/test_list.c')

test('Generic List Test - Add Item', test_list_executable, args: ['test_additem'], suite: 'core')
test('Generic List Test - Index Of 1', test_list_executable, args: ['test_indexof1'], suite: 'core')
test('Generic List Test - Index Of 2', test_list_executable, args: ['test_indexof2'], suite: 'core')
test('Generic List Test - Index Of 3', test_list_executable, args: ['test_indexof3'], suite: 'core')
test('Generic List Test - Get Item', test_list_executable, args: ['test_get'], suite: 'core')
test('Generic List Test - Get Range of Items', test_list_executable, args: ['test_getrange'], suite: 'core')
test('Generic List Test - Add Range of Items', test_list_executable, args: ['test_addrange'], suite: 'core')
test('Generic List Test - Add Range of Items At Index', test_list_executable, args: ['test_addrange_at'], suite: 'core')
test('Generic List Test - Remove Item', test_list_executable, args: ['test_remove'], suite: 'core')
test('Generic List Test - Remove Range of Items', test_list_executable, args: ['test_removerange'], suite: 'core')
test('Generic List Test - Remove Specified Item', test_list_executable, args: ['test_removeitem'], suite: 'core')
test('Generic List Test - Struct List', test_list_executable, args: ['test_structlist'], suite: 'core')
test('Generic List Test - Incorrect Data Validation', test_list_executable, args: ['test_incorrectdata'], suite: 'core')
test('Generic List Test - Reallocation Validation', test_list_executable, args: ['test_reallocation'], suite: 'core')

# Test cases for Concurrent Queue
test_concurrent_queue_executable = executable('test_concurrent_queue', 'tests/test_concurrent_queue.c', 'STL_CThread/source/tinycthread.c', include_directories: tinycthread_include_dir)

test('Generic Concurrent Queue Test - Enqueue and Dequeue', test_concurrent_queue_executable, args: ['test_enqueue_and_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 1', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 2', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 3', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 4', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 5', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 6', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 7', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Enqueue - Run 8', test_concurrent_queue_executable, args: ['test_multithread_enqueue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 1', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 2', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 3', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 4', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 5', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 6', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 7', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')
test('Generic Concurrent Queue Test - Multithread Dequeue - Run 8', test_concurrent_queue_executable, args: ['test_multithread_dequeue'], suite: 'core')

# Test cases for Concurrent Stack
test_concurrent_stack_executable = executable('test_concurrent_stack', 'tests/test_concurrent_stack.c', 'STL_CThread/source/tinycthread.c', include_directories: tinycthread_include_dir)

test('Generic Concurrent Stack Test - Push and Pop', test_concurrent_stack_executable, args: ['test_push_and_pop'], suite: 'core')
