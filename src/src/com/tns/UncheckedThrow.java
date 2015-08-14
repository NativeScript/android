package com.tns;

class UncheckedThrow
{
	@SuppressWarnings("unchecked")
    private static <T extends Throwable> void throwException(Throwable exception, Object dummy) throws T
    {
        throw (T) exception;
    }

    public static void Throw(Throwable exception)
    {
    	UncheckedThrow.<RuntimeException>throwException(exception, null);
    }
}