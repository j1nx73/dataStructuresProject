cmake_minimum_required(VERSION 3.10)
project(BankingSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(banking_system
        main.cpp
        bank_system.cpp
        account.cpp
        transaction.cpp
        transaction_queue.cpp
)