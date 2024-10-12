cmake_minimum_required(VERSION 3.26)
project(Aspfinal)

set(CMAKE_CXX_STANDARD 17)

add_executable(Aspfinal main.cpp
        Btree.cpp
        Btree.h
        DataBase.cpp
        DataBase.h
        DataCustomerAccount.cpp
        DataCustomerAccount.h
)
