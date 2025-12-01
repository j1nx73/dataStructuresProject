cmake_minimum_required(VERSION 3.26)
project(bankingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
# List all source files for the executable
add_executable(bankingSystem
        src/main.cpp
        src/utils.cpp
        src/transaction_list.cpp
        include/account_bst.h
        src/account_bst.cpp
        include/pending_queue.h
        src/pending_queue.cpp
        include/bank_service.h
        src/bank_service.cpp
        include/ui.h
        src/ui.cpp
        include/auth.h
        src/auth.cpp
        include/persistence.h
        src/persistence.cpp
)
