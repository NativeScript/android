package com.tns.tests.kotlin.extensions

fun Any.extensionFunctionWithNoArgumentsAndNoReturnValue(){}

fun Any.extensionFunctionWithNoArgumentsAndStringReturnValue() = "some data"

fun Any.extensionFunctionWithOnePrimitiveArgumentAndNoReturnValue(arg: Int){}

fun Any.extensionFunctionWithOnePrimitiveArgumentAndStringReturnValue(arg: Int) = "some data $arg"

fun Any.extensionFunctionWithOneObjectTypeArgumentAndNoReturnValue(arg: Any){}

fun Any.extensionFunctionWithOneObjectTypeArgumentAndStringReturnValue(arg: Any) = "some data $arg"

fun Any.extensionFunctionWithMultipleObjectTypeArgumentsAndNoReturnValue(arg1: Any, arg2: Any){}

fun Any.extensionFunctionWithMultipleObjectTypeArgumentsAndStringReturnValue(arg1: Any, arg2: Any) = "some data $arg1 $arg2"
