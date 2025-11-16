cmake_minimum_required(VERSION 4.0)
project(bankingSystem)

set(CMAKE_CXX_STANDARD 20)

add_executable(bankingSystem main.cpp
        Account.cpp
        Account.h
        Transactions.cpp
        Transactions.h
        BST.cpp
        BST.h
        Queue.cpp
        Queue.h
        PendingTransaction.cpp
        PendingTransaction.h
        Bank.cpp
        Bank.h
        Utils.h)
