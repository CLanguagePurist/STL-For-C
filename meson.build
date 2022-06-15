project('stlc', 'c')

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
test_concurrent_queue_executable = executable('test_concurrent_queue', 'tests/test_concurrent_queue.c')

test('Generic Concurrent Queue Test - Enqueue and Dequeue', test_concurrent_queue_executable, args: ['test_enqueue_and_dequeue'], suite: 'core')