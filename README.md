# MINILIBS

This project is a collection of small libs for useful data structures in C.
It currently contains :
    - dynamic arrays : arrays that grow
    - disjointed set union : sets and connection matrices, useful for graphs and other nerdy things
    - intrusive singly linked list : good old list with the head in the node, space efficient
    - intrusive doubly linked lists : good old list with the head in the node, less space efficient but a good compromise between usability and memory usage.
    - round buffers : a way to handle buffers that loop over the buffer and overwrite the oldest position when full. (Think of a buffer as a circle)
    - sparse sets: constant ids, data at the index, great for caching. Deletion is O(N), but in the worst case you can keep O(1) deletion with some extra logic; for now we don't care. (Useful when loads of assets are present and reference each other but still need fast iteration with random deletions possibly happening in the middle of the data. Think of a vtable for ids: ids are mapped to slots that contain data. Good compromise between lists and arrays.)

# TODOs
## da
    - execute todos in the da/dynamic_array.h header
    - doxygen docs
    - unit tests
## dsu
    - doxygen docs
    - unit tests
## ill
    - execute todos in the ill/ill.h header
    - rename ill/ill to ill/intrusive_singly_linked.h
    - unit tests
## idl
    - implement interface based on ill module
    - rename idl/idl.h to idl/intrusive_doubly_linked.h
    - doxygen docs
    - unit tests
## rb
    - doxygen docs
    - unit tests
## ss
    - doxygen docs
    - unit tests
    - delete old stuff and re-implement properly
    - re-watch the video[https://www.youtube.com/watch?v=TfhrF7VrzWQ] explaining sparse-sets
## Build system
    make the following rules:
        - unit_test : run unit tests
        - doxygen : generate doxygen docs in ./documentation folder
        - static analysis : run static analysis with clang-analyzer
        - valgrind : run unit test with valgrind ( flags : --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --track-origins=yes --read-var-info=yes --show-reachable=yes --undef-value-errors=yes -q)
        - single_file : create a single file for each module that contain that module's header and source code in a define in the style of ifdef XXX_IMPLEMENTATION [...] endif, with xxx being the name of the module.
    add a substitution function for all rules since most module are compiled using the same build structure.