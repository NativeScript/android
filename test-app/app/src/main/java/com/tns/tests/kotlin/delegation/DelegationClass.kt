package com.tns.tests.kotlin.delegation

class DelegationClass: StringGetter by StringGetterImpl()