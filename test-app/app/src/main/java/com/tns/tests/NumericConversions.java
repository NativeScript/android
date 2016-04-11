package com.tns.tests;

public class NumericConversions {
	
	//long
	public long hasToReturnLong(int number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	public long hasToReturnLong(long number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	//int
	public int hasToReturnInt(long number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	public int hasToReturnInt(int number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	//double
	public double hasToReturnDouble(float number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	public double hasToReturnDouble(double number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	//float
	public float hasToReturnFloat(float number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	public float hasToReturnFloat(double number) {
		throw new UnsupportedOperationException("you have to override this method");
	}
	
	//call long
	public static long callHasToReturnLong(NumericConversions nc, long value)
	{
		long ret = nc.hasToReturnLong(value);
		return ret;
	}
	
	public static long callHasToReturnLong(NumericConversions nc, int value)
	{
		long ret = nc.hasToReturnLong(value);
		return ret;
	}
	
	//call int
	public static int callHasToReturnInt(NumericConversions nc, long value)
	{
		int ret = nc.hasToReturnInt(value);
		return ret;
	}
	
	//should blow up
	public static int callHasToReturnInt(NumericConversions nc, int value)
	{
		int ret = nc.hasToReturnInt(value);
		return ret;
	}
	
	//call double
	public static double callHasToReturnDouble(NumericConversions nc, double value)
	{
		double ret = nc.hasToReturnDouble(value);
		return ret;
	}
	
	public static double callHasToReturnDouble(NumericConversions nc, float value)
	{
		double ret = nc.hasToReturnDouble(value);
		return ret;
	}
	
	//call float
	//should blow up
	public static float callHasToReturnFloat(NumericConversions nc, double value)
	{
		float ret = nc.hasToReturnFloat(value);
		return ret;
	}
	
	public static float callHasToReturnFloat(NumericConversions nc, float value)
	{
		float ret = nc.hasToReturnFloat(value);
		return ret;
	}
}
